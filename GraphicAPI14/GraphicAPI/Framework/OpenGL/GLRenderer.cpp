////////////////////////////////////////////////////////////////////////////////////////////////
//	GLRenderer.cpp 
//	OpenGL Rendering core class functions definitions
//	Created: January 2006
//	
////////////////////////////////////////////////////////////////////////////////////////////////

#include <windows.h>
#include "../Types/types.h"
#include "../Templates/Array.h"
#include "../Math/MathLib.h"

#include "../Image/Image.h"
#include "../Image/ImageIO.h"
#include "../Image/ImgTypes.h"

#include "../3DObjects/3DObject.h"
#include "../Camera/Camera.h"
#include "../Renderer/Renderer.h"

#include <stdio.h>
#include <stdlib.h>

#include <gl/gl.h>
#include <gl/glu.h>
#include <gl/glut.h>

#include "Extensions/glext.h"
#include "Extensions/wglext.h"
#include "GLExtStruct.h"
#include "Extensions/GLExtensions.h"

#include "GLConvFunc.h"
#include "GLDevCaps.h"
#include "GLightMgr.h"
#include "GLMaterialMgr.h"
#include "GLTextureMgr.h"

#include "GLARBRenderElement.h"
#include "GLShaderObject.h"
#include "GLRenderer.h"


#pragma comment (lib,"opengl32.lib")
#pragma comment (lib, "glu32.lib")

CGLObj::CGLObj():CGraphicAPI()
{
	hWndRect=NULL;
	hDC=NULL;
	hRC=NULL;

	mWorldMat.Identity();

	SetTwoSideStencil = & CGLObj::SetTwoSideStencilNULL;
}

///////////////////////////////////////////////////////////////
//Class destructor
CGLObj::~CGLObj()
{
	if (hRC)											// Do We Have A Rendering Context?
	{
		if (!wglMakeCurrent(NULL,NULL))					// Are We Able To Release The DC And RC Contexts?
		{
			MessageBox(NULL,"Release Of DC And RC Failed.","SHUTDOWN ERROR",MB_OK | MB_ICONINFORMATION);
		}

		if (!wglDeleteContext(hRC))						// Are We Able To Delete The RC?
		{
			MessageBox(NULL,"Release Rendering Context Failed.","SHUTDOWN ERROR",MB_OK | MB_ICONINFORMATION);
		}
		hRC=NULL;										// Set RC To NULL
	}

	//if (hDC && !ReleaseDC(hWndRect,hDC))					// Are We Able To Release The DC
	{
		//MessageBox(NULL,"Release Device Context Failed.","SHUTDOWN ERROR",MB_OK | MB_ICONINFORMATION);
		hDC=NULL;										// Set DC To NULL
	}
	
	//Deleting All Render Elements From Array
	GLARBRenderElement *pRenderElement = NULL;
	RenderElementArray.SetPointerToStart();
	while(true)
	{
		pRenderElement = RenderElementArray.GetElementAndMovePointer();
		if(pRenderElement!=NULL)
			ReleaseRenderElement(pRenderElement);
		else
			break;
	}
	
	//Deleting Shader Objects
	sGLShaderObject *pShaderObj = NULL;
	ShaderObjectsArray.SetPointerToStart();
	while(true)
	{
		pShaderObj = ShaderObjectsArray.GetElementAndMovePointer();

		if(pShaderObj!=NULL)
			ReleaseShaderElement(pShaderObj);
		else
			break;
	}
	
}

/////////////////////////////////////////////////////////////////////
//Creating OpenGL Window
bool CGLObj::Create(HWND hWnd)
{
	this->hWndRect = hWnd;
	GLuint	PixelFormat;				// Holds The Results After Searching For A Match
	PIXELFORMATDESCRIPTOR pfd;			// Pixel Format Descriptor Necessary For OpenGL
	memset(&pfd,0,sizeof(PIXELFORMATDESCRIPTOR));

	pfd.nSize = sizeof(PIXELFORMATDESCRIPTOR);
	pfd.nVersion = 1;
	pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
	pfd.iPixelType = PFD_TYPE_RGBA;
	pfd.cColorBits = 32;
	pfd.cDepthBits = 24;
	pfd.cStencilBits = 8;
	pfd.iLayerType = PFD_MAIN_PLANE;
	
	if (!(hDC=GetDC(hWnd)))							// Did We Get A Device Context?
	{									
		MessageBox(NULL,"Can't Create A GL Device Context.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return FALSE;								// Return FALSE
	}
	
	if (!(PixelFormat=ChoosePixelFormat(hDC,&pfd)))	// Did Windows Find A Matching Pixel Format?
	{									
		MessageBox(NULL,"Can't Find A Suitable PixelFormat.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return FALSE;								// Return FALSE
	}

	if(!SetPixelFormat(hDC,PixelFormat,&pfd))		// Are We Able To Set The Pixel Format?
	{	
		MessageBox(NULL,"Can't Set The PixelFormat.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return FALSE;								// Return FALSE
	}

	if (!(hRC=wglCreateContext(hDC)))				// Are We Able To Get A Rendering Context?
	{	
		MessageBox(NULL,"Can't Create A GL Rendering Context.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return FALSE;								// Return FALSE
	}

	if(!wglMakeCurrent(hDC,hRC))					// Try To Activate The Rendering Context
	{	
		MessageBox(NULL,"Can't Activate The GL Rendering Context.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return FALSE;								// Return FALSE
	}

	InitGLExtensions(mGLExt,hDC);
	CGLDeviceCaps *pGLDevCaps = new CGLDeviceCaps(&mGLExt);
	pDevCaps = pGLDevCaps;
		
	return TRUE;
}

///////////////////////////////////////////////////////////////////////////////
// All Setup For OpenGL Goes Here
bool CGLObj::Init()
{
	
	if(!mGLExt.GL_ARB_transpose_matrix_flag)
		return false;

	if(!mGLExt.GL_ARB_vertex_buffer_object_flag)
		return false;

	if(!mGLExt.GL_EXT_texture3D_flag)
		return false;

	if(!mGLExt.GL_ARB_texture_cube_map_flag)
		return false;

	if(!mGLExt.GL_ARB_multitexture_flag)
		return false;

	if(!mGLExt.GL_EXT_framebuffer_object_flag)
		return false;

	if(mGLExt.GL_EXT_stencil_two_side_flag)
		SetTwoSideStencil = & CGLObj::SetTwoSideStencilEXT;
	else if(mGLExt.GL_ATI_separate_stencil_flag)
		SetTwoSideStencil = & CGLObj::SetTwoSideStencilATI; 

/*	
	if(!mGLExt.WGL_ARB_extensions_string_flag)
		return false;

	if(!mGLExt.WGL_ARB_pbuffer_flag)
		return false;

	if(!mGLExt.WGL_ARB_pixel_format_flag)
		return false;

	if(!mGLExt.WGL_ARB_render_texture_flag)
		return false;
*/
	glShadeModel(GL_SMOOTH);							// Enable Smooth Shading
	glClearColor(0.0f, 0.0f, 0.0f, 0.5f);				// Black Background
	glClearDepth(1.0f);									// Depth Buffer Setup
	glEnable(GL_DEPTH_TEST);							// Enables Depth Testing
	glDepthFunc(GL_LEQUAL);								// The Type Of Depth Testing To Do
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);	// Really Nice Perspective Calculations
	//Enabling Separate Specular Color Calcultion
	glLightModeli( GL_LIGHT_MODEL_COLOR_CONTROL, GL_SEPARATE_SPECULAR_COLOR );
	//glEnable(GL_LIGHTING);
	//glEnable(GL_LIGHT0);
	pTexMgr = new CGLTextureSystem(hWndRect,pDevCaps,&mGLExt);
	pMatMgr = new CGLMaterialSystem();
	pLightMgr = new CGLightSystem();

	return TRUE;										// Initialization Went OK
}

///////////////////////////////////////////////////////////////////////////////
//Drawing scene using OpenGL API
void CGLObj::Render()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	// Clear Screen And Depth Buffer
	float static angle = 0;
	static bool first = true;

	pTexMgr->BindRenderTarget(2);
	glClearColor(0,0,0,0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	pTexMgr->SetTexFilter(1,TEXF_LINEAR,TEXF_LINEAR,TEXF_LINEAR);
	pTexMgr->SetTexture(1,0);
	
	glPushMatrix();
	glTranslatef(0,2,0);
	angle+=0.1f;
	glRotatef(angle*2,0,0,1);
	if(angle>360)
		angle=0;

	glRotatef(90,1,0,0);
	glutSolidTeapot(1.2f);
	glPopMatrix();
	pTexMgr->DisableTextures();
	pTexMgr->UnBindRenderTarget();

	glClearColor(0.4,0.4,0.5,1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	pTexMgr->SetTexFilter(2,TEXF_LINEAR,TEXF_LINEAR,TEXF_LINEAR);
	pTexMgr->SetAnisotropicFilter(2,8);
	pTexMgr->SetTexture(2,0);
	glPushMatrix();
	glTranslatef(0,1,0);
	glRotatef(angle,0,0,1);
	glBegin(GL_QUADS);
		glTexCoord2f(0,0);
		glVertex3f(-1,0,-1);

		glTexCoord2f(1,0);
		glVertex3f(1,0,-1);	

		glTexCoord2f(1,1);
		glVertex3f(1,0,1);

		glTexCoord2f(0,1);
		glVertex3f(-1,0,1);		
	glEnd();
	glPopMatrix();
	pTexMgr->DisableTextures();

	/*if(first && angle>75)
	{
		this->GrabScreenShot();
		first = false;
	}*/

	SwapBuffers(hDC);
}

//////////////////////////////////////////////////////////////////////////////////
// Resize And Initialize
void CGLObj::Resize(int width, int height)
{
	if (height==0)										// Prevent A Divide By Zero By
		height=1;										// Making Height Equal One

	Width = width;
	Height = height;
	glViewport(0,0,width,height);						// Reset The Current Viewport

	glMatrixMode(GL_PROJECTION);						// Select The Projection Matrix
	glLoadIdentity();									// Reset The Projection Matrix

	// Calculate The Aspect Ratio Of The Window
	gluPerspective(mFOV,GLdouble(width)/GLdouble(height),mNear,mFar);
	// And Set Default View
	gluLookAt(0,0,0, 0,1,0, 0,0,1);

	glMatrixMode(GL_MODELVIEW);							// Select The Modelview Matrix
	glLoadIdentity();
	glRotated(90,1,0,0);	
	
	gluLookAt(mPos.x,mPos.y,mPos.z, mView.x,mView.y,mView.z, mUp.x,mUp.y,mUp.z);
}

//////////////////////////////////////////////////////////////////////////////////
// Updating And Setting Camera Options
void CGLObj::SetView(const CCamera &Cam)
{
	mPos  = Cam.Position();
	mView = Cam.ViewVec();
	mUp   = Cam.UpVec();

	mFOV  = Cam.FOV();
	mNear = Cam.Near();
	mFar  = Cam.Far();

	glLoadIdentity();									// Reset Modelview Matrix
	glRotated(90,1,0,0);								// Global Space With Z Up Vector

	gluLookAt(mPos.x,mPos.y,mPos.z, mView.x,mView.y,mView.z, mUp.x,mUp.y,mUp.z);
}

//////////////////////////////////////////////////////////////////////////////////
// Setting Camera Options
void CGLObj::SetView()
{
	glLoadIdentity();									// Reset Modelview Matrix
	glRotated(90,1,0,0);								// Global Space With Z Up Vector

	gluLookAt(mPos.x,mPos.y,mPos.z, mView.x,mView.y,mView.z, mUp.x,mUp.y,mUp.z);
}

//////////////////////////////////////////////////////////////////////////////////
// Setting Viewport
void CGLObj::SetViewport(int width,int height)
{
	Width = width;
	Height = height;
	glViewport(0,0,Width,Height);
}

//////////////////////////////////////////////////////////////////////////////////
// Setting Orthographic Projection
void CGLObj::SetOrthoProj(float l,float r,float b,float t,float zn,float zf)
{
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	
	//GLfloat matrix[16];
	glOrtho(l,r,b,t,zn,zf);
	//glGetFloatv(GL_PROJECTION_MATRIX,matrix);

	gluLookAt(0,0,0, 0,1,0, 0,0,1);
	//glGetFloatv(GL_PROJECTION_MATRIX,matrix);

	//glTranslatef(0,-2,0);
	//glGetFloatv(GL_PROJECTION_MATRIX,matrix);

	glMatrixMode(GL_MODELVIEW);
}

//////////////////////////////////////////////////////////////////////////////////
// Setting Orthographic Projection
void CGLObj::SetOrthoProj(float left, float right, float bottom, float top)
{
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	gluOrtho2D(left,right,bottom,top);
	gluLookAt(0,0,0, 0,1,0, 0,0,1);
	//glTranslatef(0,-2,0);
	
	glMatrixMode(GL_MODELVIEW);
}

//////////////////////////////////////////////////////////////////////////////////
// Picking Screen.
// Getting Point In Global Space Were User Picked Camera Near Clip Plane
Vector3D CGLObj::PickScreen(float MouseX,float MouseY)
{
	float x = MouseX;
	float y = MouseY;

	GLint viewport[4];
	GLdouble projection[16];
	GLdouble modelview[16];
  
	GLdouble px, py, pz;
	
	glGetIntegerv(GL_VIEWPORT, viewport);
	glGetDoublev(GL_PROJECTION_MATRIX, projection);
	glGetDoublev(GL_MODELVIEW_MATRIX, modelview);

	y = Height - y;

	gluUnProject(x, y, 0.0,
				modelview, projection, viewport,
				&px, &py, &pz);
	
	return Vector3D (float(px),float(py),float(pz));
}

//////////////////////////////////////////////////////////////////////////////////
// Clearing All Buffers
void CGLObj::Clear()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
}

//////////////////////////////////////////////////////////////////////////////////
// Clearing Only Chosen Buffers
void CGLObj::Clear(bool Color,bool Depth, bool Stencil)
{
	GLbitfield mask=0;

	if(Color)
		mask = mask | GL_COLOR_BUFFER_BIT;
	if(Depth)
		mask = mask | GL_DEPTH_BUFFER_BIT;
	if(Stencil)
		mask = mask | GL_STENCIL_BUFFER_BIT;
	
	glClear(mask);
}

//////////////////////////////////////////////////////////////////////////////////
// Setting Polygon Fill Mode
void CGLObj::SetFillMode(uint fillmode)
{
	GLenum Face = GL_FRONT;
	GLenum Fill;

	switch(fillmode)
	{
	case 0:
		Fill = GL_POINT;
		break;

	case 1:
		Fill = GL_LINE;
		break;

	case 2:
		Fill = GL_FILL;
		break;

	default:
		Fill = GL_FILL;
		break; 
	}
	glPolygonMode(Face,Fill);
}

//////////////////////////////////////////////////////////////////////////////////
// Setting Polygon Cull Mode
void CGLObj::SetCullMode(bool CullFace,bool CCW)
{
	(CCW) ? (glFrontFace(GL_CCW)) : (glFrontFace(GL_CW));
	(CullFace) ? (glEnable(GL_CULL_FACE)) : (glDisable(GL_CULL_FACE));
}

//////////////////////////////////////////////////////////////////////////////////
// Setting Backgrounf Color
void CGLObj::SetBgColor(ubyte R,ubyte G,ubyte B)
{
	glClearColor(float(R)/255.0f, float(G)/255.0f, float(B)/255.0f, 1.0f);
}

//////////////////////////////////////////////////////////////////////////////////
// Setting Shading Mode (Gouraud or Flat Shading)
void CGLObj::SetShadingMode(bool Smooth)
{
	if(Smooth)
		glShadeModel(GL_SMOOTH); //Gouraud shading
	else
		glShadeModel(GL_FLAT);
}

//////////////////////////////////////////////////////////////////////////////////
// Enabling/Disabling Shading And Specular Calculations
void CGLObj::LightingEnable(bool flag)
{
	(flag) ? (glEnable(GL_LIGHTING)) : (glDisable(GL_LIGHTING));
}

//////////////////////////////////////////////////////////////////////////////////
// Enabling/Disabling Normals Normalization. In Graphics Cards With T&L 
// Application Should Run Faster Rather Normalizing Normals On CPU
void CGLObj::SetNormalsNormalization(bool Normalize)
{
	(Normalize) ? (glEnable(GL_NORMALIZE)) : (glDisable(GL_NORMALIZE));
}

//////////////////////////////////////////////////////////////////////////////////
// Enabling/Disabling Colors Blending
void CGLObj::AlphaBlendEnable(bool flag)
{
	(flag)?(glEnable(GL_BLEND)):(glDisable(GL_BLEND));
}

//////////////////////////////////////////////////////////////////////////////////
// Setting Blending Mode
void CGLObj::SetBlendingMode(BLENDOP Source, BLENDOP Destination)
{
	glBlendFunc(GetGLBlendOp(Source),GetGLBlendOp(Destination));
}

//////////////////////////////////////////////////////////////////////////////////
// Enabling/Disabling Alpha Test
void CGLObj::AlphaTestEnable(bool flag)
{
	(flag) ? (glEnable(GL_ALPHA_TEST)) : (glDisable(GL_ALPHA_TEST));
}

//////////////////////////////////////////////////////////////////////////////////
// Setting Alpha Test Function
void CGLObj::AlphaFunc(FUNC func,float ref)
{
	glAlphaFunc(GetGLFunc(func),ref); 
}

//////////////////////////////////////////////////////////////////////////////////
//Depth Testing Setting Functions
//////////////////////////////////////////////////////////////////////////////////
// Enabling/Disabling Depth Test
void CGLObj::DepthTestEnable(bool flag)
{
	(flag) ? (glEnable(GL_DEPTH_TEST)) : (glDisable(GL_DEPTH_TEST));
}

//////////////////////////////////////////////////////////////////////////////////
// Enabling/Disabling Writing To Depth Buffer
void CGLObj::DepthWriteEnable(bool flag)
{
	(flag) ? (glDepthMask(GL_TRUE)) : (glDepthMask(GL_FALSE));
}
	
//////////////////////////////////////////////////////////////////////////////////
// Setting Depth Test Function
void CGLObj::DepthFunc(FUNC func)
{
	glDepthFunc(GetGLFunc(func));
}

//////////////////////////////////////////////////////////////////////////////////
// Enabling/Disabling Writing To Color Buffer
void CGLObj::ColorsWriteEnable(bool R,bool G,bool B,bool A)
{
	glColorMask(R,G,B,A);
}


//////////////////////////////////////////////////////////////////////////////////
// Stencil Testing Setting Functions
//////////////////////////////////////////////////////////////////////////////////
// A bit mask to enable and disable writing of individual bits in the stencil planes
void CGLObj::StencilWriteMask(uint mask)
{
	glStencilMask(mask);
}
//////////////////////////////////////////////////////////////////////////////////
// Enabling/Disabling Stencil Test
void CGLObj::StencilTestEnable(bool flag)
{
	(flag) ? (glEnable(GL_STENCIL_TEST)) : (glDisable(GL_STENCIL_TEST));
}
//////////////////////////////////////////////////////////////////////////////////
// Setting Stencil Test Function
void CGLObj::StencilFunc(FUNC func,uint ref,uint mask)
{
	if(ref>255)
		ref=255;
	if(mask>255)
		mask=255;

	glStencilFunc(GetGLFunc(func),ref,mask);
}
//////////////////////////////////////////////////////////////////////////////////
// Setting Stencil Test Operations
void CGLObj::StencilOp(OP fail, OP zfail, OP zpass)
{
	glStencilOp(GetGLOp(fail),GetGLOp(zfail),GetGLOp(zpass));
}
//////////////////////////////////////////////////////////////////////////////////
// Enabling/Disabling Two Side Stencil Test. Culling Mode Must Be Turned Off!!!
void  CGLObj::TwoSideStencilEnable(bool flag)
{
	(flag)?(glEnable(GL_STENCIL_TEST_TWO_SIDE_EXT)):(glDisable(GL_STENCIL_TEST_TWO_SIDE_EXT));
}

//////////////////////////////////////////////////////////////////////////////////
// Class Protected Method For Setting Two Side Stencil Depending On Extensions Supporting
// If GL_EXT_stencil_two_side Is Supported
void CGLObj::SetTwoSideStencilEXT(bool front,FUNC func,uint ref,uint mask,OP fail,OP zfail,OP zpass)
{
	if(front)
		glActiveStencilFaceEXT(GL_FRONT);
	else
		glActiveStencilFaceEXT(GL_BACK);

	glStencilFunc(GetGLFunc(func), ref, mask);
	glStencilOp(GetGLOp(fail),GetGLOp(zfail),GetGLOp(zpass));
}

// If GL_ATI_separate_stencil Is Supported
void CGLObj::SetTwoSideStencilATI(bool front,FUNC func,uint ref,uint mask,OP fail,OP zfail,OP zpass)
{
	if(front)
		glStencilOpSeparateATI(GL_FRONT,GetGLOp(fail),GetGLOp(zfail),GetGLOp(zpass));
	else
		glStencilOpSeparateATI(GL_BACK,GetGLOp(fail),GetGLOp(zfail),GetGLOp(zpass));

	glStencilFunc(GetGLFunc(func), ref, mask);
}

//////////////////////////////////////////////////////////////////////////////////
// Setting Stencil For Back Faces. In This Graphic API Front Face Has CCW Order
void CGLObj::SetStencilForBackFace(FUNC func,uint ref,uint mask,OP fail,OP zfail,OP zpass)
{
	//glActiveStencilFaceEXT(GL_BACK);
	//glStencilFunc(GetGLFunc(func), ref, mask);
	//glStencilOp(GetGLOp(fail),GetGLOp(zfail),GetGLOp(zpass));
	(this->*SetTwoSideStencil)(false,func,ref,mask,fail,zfail,zpass);
}
//////////////////////////////////////////////////////////////////////////////////
// Setting Stencil For Front Faces. In This Graphic API Front Face Has CCW Order
void CGLObj::SetStencilForFrontFace(FUNC func,uint ref,uint mask,OP fail,OP zfail,OP zpass)
{
	//glActiveStencilFaceEXT(GL_FRONT);
	//glStencilFunc(GetGLFunc(func), ref, mask);
	//glStencilOp(GetGLOp(fail),GetGLOp(zfail),GetGLOp(zpass));
	(this->*SetTwoSideStencil)(true,func,ref,mask,fail,zfail,zpass);
}

//////////////////////////////////////////////////////////////////////////////////
// Clip Planes Setting Functions
//////////////////////////////////////////////////////////////////////////////////
// Enabling/Disabling Clip Plane
void CGLObj::ClipPlaneEnable(uint Index, bool flag)
{
	if(Index >= pDevCaps->MaxClipPlanes())
		return ;

	(flag) ? (glEnable(GL_CLIP_PLANE0+Index)) : (glDisable(GL_CLIP_PLANE0+Index));
}

//////////////////////////////////////////////////////////////////////////////////
// Setting Specified Clip Plane Equation
bool CGLObj::SetClipPlane(uint Index, Vector4D &PlaneEq)
{
	if(Index >= pDevCaps->MaxClipPlanes())
		return false;

	GLdouble equation[4];
	equation[0] = GLdouble(PlaneEq.x);
	equation[1] = GLdouble(PlaneEq.y);
	equation[2] = GLdouble(PlaneEq.z);
	equation[3] = GLdouble(PlaneEq.w);

	glClipPlane(GL_CLIP_PLANE0 + Index, equation);
	
	return true;
}

//////////////////////////////////////////////////////////////////////////////////
// Enabling/Disabling Scissor Test
void CGLObj::ScissorTestEnable(bool flag)
{
	(flag) ? (glEnable(GL_SCISSOR_TEST)) : (glDisable(GL_SCISSOR_TEST));
}

//////////////////////////////////////////////////////////////////////////////////
// Setting Scissor Rectangle In Scissor Test
void CGLObj::SetScissorRect(uint x, uint y, uint w, uint h)
{
	glScissor(GLint(x),GLint(y),GLsizei (w), GLsizei(h));
}

//////////////////////////////////////////////////////////////////////////////////
// Setting Polygon Offset
// It Is Used To Solve Depth Test Artifacts
void CGLObj::SetPolygonOffset(bool flag, float bias, float slopeBias)
{
	if(flag)
	{
		glEnable(GL_POLYGON_OFFSET_FILL);
		glPolygonOffset(bias, slopeBias);
	}
	else
	{
		glDisable(GL_POLYGON_OFFSET_FILL);
		glPolygonOffset(0.0f, 0.0f);
	}
}

//////////////////////////////////////////////////////////////////////////////////
// Saving Screen Iamge On Disc
void CGLObj::GrabScreenShot(char* file_name)
{
	CImage *Img = new CImage();
	Img->Create(FORMAT_RGB8, Width, Height, 1, 1);

	glReadPixels(0, 0, Width, Height, GL_BGR_EXT, GL_UNSIGNED_BYTE, Img->getPixels());

	CBMP *pBMPImage = new CBMP();
	
	if(file_name)
	{
		pBMPImage->SaveImage(Img,file_name);
	}
	else
	{
		// Find the first unused filename and save to it
		char fileName[32]={0};
		FILE *file = NULL;
		for (int i = 0; i < 100; i++)
		{
			sprintf(fileName, "Screenshot%02d.bmp", i);
	
			if ((file=fopen(&fileName[0],"rb"))==NULL)
			{
				pBMPImage->SaveImage(Img,fileName);
				break;
			}
			else
				fclose(file);
		}
	}
	
	delete (Img);
	delete (pBMPImage);
}


//////////////////////////////////////////////////////////////////////////////////
// 3DObject Manipulating Methods
//////////////////////////////////////////////////////////////////////////////////
// Adding New 3DObject To Array And Creating Data Buffers
uint CGLObj::Add3DObjToBuffer(C3DObject* pObj3D, C3DObjectDesc* ObjDesc, bool dynamic)
{
	if(pObj3D == NULL)
		return 0;

	GLARBRenderElement RenderElement;
	memset(&RenderElement,0,sizeof(GLARBRenderElement));

	memcpy(&RenderElement.VertStruct,&pObj3D->GetVertexDescription(),sizeof(VertexDesc));
	RenderElement.IndexNum = pObj3D->GetIndexNum();
	RenderElement.VertexNum = pObj3D->GetVertexNum();
	RenderElement.TexNum = pObj3D->GetTexNum();
	RenderElement.dynamic = dynamic; //Do We Have To Create Dynamic Buffres 
	RenderElement.ModelTransformMatrix = pObj3D->GetModelMatrix();

	//if(ObjDesc!=NULL)
	//	RenderElement.ObjDesc.CopyData(ObjDesc);

	RenderElement.ObjDesc = new C3DObjectDesc();
	if(ObjDesc!=NULL)
		RenderElement.ObjDesc->CopyData(ObjDesc);
	

	//Copy Data Into Buffers
	if(RenderElement.VertStruct.VertDesc & VERTEX_POSITION_FLAG)
	{
		glGenBuffersARB(1, &RenderElement.VertexBufferID);
		glBindBufferARB(GL_ARRAY_BUFFER_ARB, RenderElement.VertexBufferID);
		glBufferDataARB(GL_ARRAY_BUFFER_ARB, RenderElement.VertexNum*sizeof(Vector3D), 
						pObj3D->GetVertexArray(), 
						dynamic ? GL_STREAM_DRAW_ARB : GL_STATIC_DRAW_ARB );
	}

	if(RenderElement.VertStruct.VertDesc & VERTEX_NORMAL_FLAG)
	{
		glGenBuffersARB(1, &RenderElement.NormalBufferID);
		glBindBufferARB(GL_ARRAY_BUFFER_ARB, RenderElement.NormalBufferID);
		glBufferDataARB(GL_ARRAY_BUFFER_ARB, RenderElement.VertexNum*sizeof(Vector3D),
						pObj3D->GetNormalArray(), 
						dynamic ? GL_STREAM_DRAW_ARB : GL_STATIC_DRAW_ARB );
	}
	
	if(RenderElement.VertStruct.VertDesc & VERTEX_PRIMARY_COLOR_FLAG)
	{
		glGenBuffersARB(1, &RenderElement.PrimaryColorBufferID);
		glBindBufferARB(GL_ARRAY_BUFFER_ARB, RenderElement.PrimaryColorBufferID);
		glBufferDataARB(GL_ARRAY_BUFFER_ARB, RenderElement.VertexNum*sizeof(ubyte)*4,
						pObj3D->GetPrimaryColorArray(), 
						dynamic ? GL_STREAM_DRAW_ARB : GL_STATIC_DRAW_ARB );
	}

	if(RenderElement.VertStruct.VertDesc & VERTEX_SECONDARY_COLOR_FLAG)
	{
		glGenBuffersARB(1, &RenderElement.SecondaryColorBufferID);
		glBindBufferARB(GL_ARRAY_BUFFER_ARB, RenderElement.SecondaryColorBufferID);
		glBufferDataARB(GL_ARRAY_BUFFER_ARB, RenderElement.VertexNum*sizeof(ubyte)*3,
						pObj3D->GetSecondaryColorArray(),
						dynamic ? GL_STREAM_DRAW_ARB : GL_STATIC_DRAW_ARB );
	}

	if(RenderElement.VertStruct.VertDesc & VERTEX_INDEXED_FLAG)
	{
		uint *ptr = RenderElement.IndexBuffer = new uint [RenderElement.IndexNum];
		memcpy(RenderElement.IndexBuffer,pObj3D->GetIndexArray(),
				sizeof(uint)*RenderElement.IndexNum);
	}
	else
		RenderElement.IndexBuffer = NULL;

	//Texture Coords 
	uint tn=0;
	//uint64 x=1;
	for(uint i=0; i<RenderElement.TexNum; i++)
	{
		glGenBuffersARB(1, &RenderElement.TexBuffersID[i]);
		glBindBufferARB(GL_ARRAY_BUFFER_ARB, RenderElement.TexBuffersID[i]);

		tn = pObj3D->GetTexCountCoord(i);
		if(!tn)
			continue;

		glBufferDataARB(GL_ARRAY_BUFFER_ARB, RenderElement.VertexNum*sizeof(float)*tn,
			pObj3D->GetTexCoordArray(i), 
			dynamic ? GL_STREAM_DRAW_ARB : GL_STATIC_DRAW_ARB );
	}

	return RenderElementArray.AddElement(RenderElement);
}

//////////////////////////////////////////////////////////////////////////////////
// Deleting 3DObject Data From Buffers Using Object Index
bool CGLObj::Delete3DObjFromBuffer(uint ObjIdx)
{
	GLARBRenderElement *RenderElement = RenderElementArray.GetElement(ObjIdx);

	if(!ReleaseRenderElement(RenderElement))
		return false;
	
	return RenderElementArray.DeleteElement(ObjIdx);
}

//////////////////////////////////////////////////////////////////////////////////
// Protected Method For Releasing All Data From Render Element
bool CGLObj::ReleaseRenderElement(GLARBRenderElement *RenderElement)
{
	if(RenderElement == NULL)
		return false;

	if(RenderElement->IndexBuffer != NULL)
		delete [] RenderElement->IndexBuffer;

	//Realesing Data From Buffers
	if(RenderElement->VertexBufferID!=0)
		glDeleteBuffersARB(1, &RenderElement->VertexBufferID);
	if(RenderElement->NormalBufferID!=0)
		glDeleteBuffersARB(1, &RenderElement->NormalBufferID);
	if(RenderElement->PrimaryColorBufferID!=0)
		glDeleteBuffersARB(1, &RenderElement->PrimaryColorBufferID);
	if(RenderElement->SecondaryColorBufferID!=0)
		glDeleteBuffersARB(1, &RenderElement->SecondaryColorBufferID);

	for(uint i=0; i<RenderElement->TexNum; i++)
		glDeleteBuffersARB(1, &RenderElement->TexBuffersID[i]);

	delete (RenderElement->ObjDesc);
	RenderElement->ObjDesc = NULL;

	return true;
}

//////////////////////////////////////////////////////////////////////////////////
// Updating Data In Buffers From 3DObject
bool CGLObj::UpdateBufferFrom3DObj(uint ObjIdx, C3DObject *pObj3D, uint64 flags)
{
	GLARBRenderElement *Elem = RenderElementArray.GetElement(ObjIdx);

	//If RenderElement Exists In Array
	if(Elem == NULL)
		return false;

	//Check If Vertex Description Are Equal
	VertexDesc objvertdesc = pObj3D->GetVertexDescription();
	if( (Elem->VertStruct.VertDesc != objvertdesc.VertDesc) ||
		(Elem->VertStruct.TexCount != objvertdesc.TexCount)	)
		return false;

	bool All=false;
	if(flags==0)
		All=true;

	bool dynamic = Elem->dynamic;

	Elem->ModelTransformMatrix = pObj3D->GetModelMatrix();

	GLvoid *DataPointer = NULL;
	//Updating Vertex Position
	if(Elem->VertStruct.VertDesc & VERTEX_POSITION_FLAG)
	{
		if(All || (flags & VERTEX_POSITION_FLAG))
		{
			glBindBufferARB(GL_ARRAY_BUFFER, Elem->VertexBufferID);
			glBufferDataARB(GL_ARRAY_BUFFER_ARB, Elem->VertexNum*sizeof(Vector3D), 
						NULL, dynamic ? GL_STREAM_DRAW_ARB : GL_STATIC_DRAW_ARB );
			DataPointer = (GLfloat *)glMapBufferARB(GL_ARRAY_BUFFER, GL_WRITE_ONLY);
			memcpy(DataPointer,pObj3D->GetVertexArray(),sizeof(Vector3D)*Elem->VertexNum);
			glUnmapBufferARB(GL_ARRAY_BUFFER);
		}
	}

	//Updating Normals Array
	if(Elem->VertStruct.VertDesc & VERTEX_NORMAL_FLAG)
	{
		if(All || (flags & VERTEX_NORMAL_FLAG))
		{
			glBindBufferARB(GL_ARRAY_BUFFER, Elem->NormalBufferID);
			glBufferDataARB(GL_ARRAY_BUFFER_ARB, Elem->VertexNum*sizeof(Vector3D), 
						NULL, dynamic ? GL_STREAM_DRAW_ARB : GL_STATIC_DRAW_ARB );
			DataPointer = (GLfloat *)glMapBufferARB(GL_ARRAY_BUFFER, GL_WRITE_ONLY);
			memcpy(DataPointer,pObj3D->GetNormalArray(),sizeof(Vector3D)*Elem->VertexNum);
			glUnmapBufferARB(GL_ARRAY_BUFFER);
		}
	}

	//Updating Primary Color Array
	if(Elem->VertStruct.VertDesc & VERTEX_PRIMARY_COLOR_FLAG)
	{
		if(All || (flags & VERTEX_PRIMARY_COLOR_FLAG))
		{
			glBindBufferARB(GL_ARRAY_BUFFER, Elem->PrimaryColorBufferID);
			glBufferDataARB(GL_ARRAY_BUFFER_ARB, Elem->VertexNum*sizeof(ubyte)*4, 
						NULL, dynamic ? GL_STREAM_DRAW_ARB : GL_STATIC_DRAW_ARB );
			DataPointer = (GLfloat *)glMapBufferARB(GL_ARRAY_BUFFER, GL_WRITE_ONLY);
			memcpy(DataPointer,pObj3D->GetPrimaryColorArray(),
									sizeof(ubyte)*Elem->VertexNum*4);
			glUnmapBufferARB(GL_ARRAY_BUFFER);
		}
	}

	//Updating Secondary Color Array
	if(Elem->VertStruct.VertDesc & VERTEX_SECONDARY_COLOR_FLAG)
	{
		if(All || (flags & VERTEX_SECONDARY_COLOR_FLAG))
		{
			glBindBufferARB(GL_ARRAY_BUFFER, Elem->SecondaryColorBufferID);
			glBufferDataARB(GL_ARRAY_BUFFER_ARB, Elem->VertexNum*sizeof(ubyte)*3, 
						NULL, dynamic ? GL_STREAM_DRAW_ARB : GL_STATIC_DRAW_ARB );
			DataPointer = (GLfloat *)glMapBufferARB(GL_ARRAY_BUFFER, GL_WRITE_ONLY);
			memcpy(DataPointer,pObj3D->GetSecondaryColorArray(),
									sizeof(ubyte)*Elem->VertexNum*3);
			glUnmapBufferARB(GL_ARRAY_BUFFER);
		}
	}

	//This Function Do Not Update Index Buffer
	//Vertex Weight Buffer Updating Is Not Yet Implemented

	uint tn=0;
	uint64 x = 1;
	//Updating Texture Coords
	for(uint i=0; i<Elem->TexNum; i++)
	{
		if(All || ((flags>>(i+19)) & x) )
		{
			tn = pObj3D->GetTexCountCoord(i);
			if(!tn)
				continue;

			glBindBufferARB(GL_ARRAY_BUFFER, Elem->TexBuffersID[i]);
			glBufferDataARB(GL_ARRAY_BUFFER_ARB, Elem->VertexNum*sizeof(float)*tn, 
						NULL, dynamic ? GL_STREAM_DRAW_ARB : GL_STATIC_DRAW_ARB );
			DataPointer = (GLfloat *)glMapBufferARB(GL_ARRAY_BUFFER, GL_WRITE_ONLY);
			memcpy(DataPointer,pObj3D->GetTexCoordArray(i),
									sizeof(float)*Elem->VertexNum*tn);
			glUnmapBufferARB(GL_ARRAY_BUFFER);
		}
	}

	return true;
}

//////////////////////////////////////////////////////////////////////////////////
// Updating Index Buffer
bool CGLObj::UpdateIndexBufferFrom3DObj(uint ObjIdx,C3DObject* pObj3D)
{
	GLARBRenderElement* Elem = RenderElementArray.GetElement(ObjIdx);

	if(Elem == NULL)
		return false;

	if(Elem->IndexBuffer == NULL)
		return false;

	uint IndexNum = pObj3D->GetIndexNum();
	if(IndexNum != Elem->IndexNum)
		return false;

	memcpy(Elem->IndexBuffer,pObj3D->GetIndexArray(),sizeof(uint)*IndexNum);

	return true;
}

/*
//////////////////////////////////////////////////////////////////////////////////
// Render One 3DObject 
bool CGLObj::Render(uint ObjIdx,uint64 flags)
{
	return Render(1,&ObjIdx,flags,NULL);
}

//////////////////////////////////////////////////////////////////////////////////
// Render One 3DObject With Another 3DObject Description
bool CGLObj::Render(uint ObjIdx, uint64 flags, C3DObjectDesc *pObjDesc)
{
	return Render(1, &ObjIdx, flags, &pObjDesc);
}

//////////////////////////////////////////////////////////////////////////////////
//Render Many 3DObjects From Index Array
bool CGLObj::Render(uint ObjNum, uint *ObjIdx,uint64 flags)
{
	return Render(ObjNum,ObjIdx,flags,NULL);
}
*/
//////////////////////////////////////////////////////////////////////////////////
// Redner Many 3DObjects From Index Array With Specified Descriptions
bool CGLObj::Render(uint ObjNum, uint *ObjIdx,uint64 flags, C3DObjectDesc **ppObjDesc)
{
	C3DObjectDesc *pObj3DDesc;
	if(!ppObjDesc)
		pObj3DDesc = NULL;
	else
		pObj3DDesc = ppObjDesc[0];

	if(ObjIdx==NULL)
		return false;

	for(uint i=0; i<ObjNum; i++)
	{
		GLARBRenderElement *Elem = RenderElementArray.GetElement(ObjIdx[i]);

		//If RenderElement Exists In Array
		if(Elem == NULL)
			continue;

		if(ppObjDesc)
			pObj3DDesc = ppObjDesc[i];
		else
			pObj3DDesc = Elem->ObjDesc;

		if(!SetBuffersData(Elem,flags,pObj3DDesc))
			continue;

		if(!SetShaderObject(Elem->ObjDesc->GetGPUProgramID(),Elem->ObjDesc->GetShaderDesc()))
			continue;

		//Pushing Current Modelview Matrix On Stack
		glPushMatrix();

		//Multiplying World Matrix With 3DObject Model Matrix And OpenGL ModelView Matrix
		glMultTransposeMatrixf(mWorldMat*Elem->ModelTransformMatrix);

		//Rendering Element
		if(Elem->VertStruct.VertDesc & VERTEX_INDEXED_FLAG)
			glDrawElements(GL_TRIANGLES,Elem->IndexNum,GL_UNSIGNED_INT,Elem->IndexBuffer);		
		else
			glDrawArrays(GL_TRIANGLES,0,Elem->VertexNum);

		//Turn Off Shader Object
		TurnOffShader();

		// Disable Pointers
		if(Elem->VertStruct.VertDesc & VERTEX_POSITION_FLAG)
			glDisableClientState( GL_VERTEX_ARRAY );// Disable Vertex Array
		if(Elem->VertStruct.VertDesc & VERTEX_NORMAL_FLAG)
			glDisableClientState( GL_NORMAL_ARRAY );// Disable Normal Array
		if(Elem->VertStruct.VertDesc & VERTEX_PRIMARY_COLOR_FLAG)
			glDisableClientState( GL_COLOR_ARRAY );// Disable Color Array
		if(Elem->VertStruct.VertDesc & VERTEX_SECONDARY_COLOR_FLAG)
			glDisableClientState( GL_SECONDARY_COLOR_ARRAY );// Disable Secondary Color Array

		uint end = 0;
		//(Elem->TexNum>=Elem->ObjDesc.GetTexUnitsNum()) ? (end=Elem->TexNum) : 
		//	(end=Elem->ObjDesc.GetTexUnitsNum()) ;
		end = Elem->TexNum;
		pTexMgr->DisableTextures();

		for(uint x=0; x<end; x++)
		{
			if( (Elem->VertStruct.VertDesc >> (x+19)) & 1 )
			{
				glClientActiveTextureARB(GL_TEXTURE0_ARB+x);
				glDisableClientState(GL_TEXTURE_COORD_ARRAY);
			}
		}
		//Taking Current ModelView Matrix From Stack
		glPopMatrix();
	}

	return true;
}

//////////////////////////////////////////////////////////////////////////////////
// Setting New ModelMatrix For Rendering Element
bool CGLObj::SetRenderElementModelMatrix(uint ObjIdx, Matrix4x4& M)
{
	GLARBRenderElement *Elem = RenderElementArray.GetElement(ObjIdx);

	if(Elem == NULL)
		return false;

	if(Matrix4x4() == M)
		Elem->ModelTransformMatrix.Identity();
	else
		Elem->ModelTransformMatrix = M;

	return true;
}

//////////////////////////////////////////////////////////////////////////////////
// Setting New Object Description For Rendering Element
bool CGLObj::SetRenderElement3DObjDesc(uint ObjIdx, C3DObjectDesc* ObjDesc)
{
	GLARBRenderElement *Elem = RenderElementArray.GetElement(ObjIdx);

	if(Elem == NULL)
		return false;

	if(ObjDesc == NULL)
		return false;
	Elem->ObjDesc->CopyData(ObjDesc);

	return true;
}


//////////////////////////////////////////////////////////////////////////////////
// Setting Buffers Data For Rendering
bool CGLObj::SetBuffersData(GLARBRenderElement *Elem,uint64 flags,C3DObjectDesc* pObjDesc)
{
	if(Elem==NULL)
		return false;

	bool All=false;
	if(flags==0)
		All=true;

	bool dynamic = Elem->dynamic;

	C3DObjectDesc *pObj3DDesc;
	if(!pObjDesc)
		pObj3DDesc = pObjDesc;
	else
		pObj3DDesc = Elem->ObjDesc;

	//Setting Vertex Buffer
	if(Elem->VertStruct.VertDesc & VERTEX_POSITION_FLAG)
	{
		if(All || (flags & VERTEX_POSITION_FLAG))
		{
			glEnableClientState( GL_VERTEX_ARRAY );// Enable Vertex Array
			glBindBufferARB( GL_ARRAY_BUFFER_ARB, Elem->VertexBufferID );
			// Set The Vertex Pointer To The Vertex Buffer
			glVertexPointer( 3, GL_FLOAT, 0, (char *) NULL );
		}
	}

	//Setting Normal Buffer
	if(Elem->VertStruct.VertDesc & VERTEX_NORMAL_FLAG)
	{
		if(All || (flags & VERTEX_NORMAL_FLAG))
		{
			glEnableClientState( GL_NORMAL_ARRAY );// Enable Normal Array
			glBindBufferARB( GL_ARRAY_BUFFER_ARB, Elem->NormalBufferID );
			// Set The Normal Pointer To The Normal Buffer
			glNormalPointer( GL_FLOAT, 0, (char *) NULL );
		}
	}

	//Setting Primary Color Buffer
	if(Elem->VertStruct.VertDesc & VERTEX_PRIMARY_COLOR_FLAG)
	{
		if(All || (flags & VERTEX_PRIMARY_COLOR_FLAG))
		{
			glEnableClientState( GL_COLOR_ARRAY );// Enable Color Array
			glBindBufferARB( GL_ARRAY_BUFFER_ARB, Elem->PrimaryColorBufferID );
			// Set The Color Pointer To The Color Buffer
			glColorPointer( 4, GL_UNSIGNED_BYTE, 0, (char *) NULL );
		}
	}

	//Setting Secondary Color Buffer
	if(Elem->VertStruct.VertDesc & VERTEX_SECONDARY_COLOR_FLAG)
	{
		if(All || (flags & VERTEX_SECONDARY_COLOR_FLAG))
		{
			glEnableClientState( GL_SECONDARY_COLOR_ARRAY );// Enable Secondary Color Array
			glBindBufferARB( GL_ARRAY_BUFFER_ARB, Elem->SecondaryColorBufferID );
			// Set The Secondary Color Pointer To The Secondary Color Buffer
			glSecondaryColorPointerEXT( 3, GL_UNSIGNED_BYTE, 0, (char *) NULL );
		}
	}

	//Setting Texture Coords And Texture Units 
	uint tn=0;
	uint end = 0;
	uint64 x=1;
	GLenum texunit = GL_TEXTURE0_ARB;
	
	if(Elem->TexNum >= pObj3DDesc->GetTexUnitsNum())
		end = Elem->TexNum;
	else
		end = pObj3DDesc->GetTexUnitsNum();

	for(uint i=0; i<end; i++)
	{
		if( (Elem->VertStruct.VertDesc>>(i+19)) & x )
		{
			if(All || (flags & (x<<(i+19))) )
			{
				tn = uint((Elem->VertStruct.TexCount >> i*2) & 3)+1;
				if(!tn)
					continue;
				//Setting Texture Coords
				glClientActiveTextureARB(GL_TEXTURE0_ARB+i);
				glEnableClientState(GL_TEXTURE_COORD_ARRAY);

				glBindBufferARB( GL_ARRAY_BUFFER_ARB, Elem->TexBuffersID[i] );
				// Set Texture Coords Pointer 
				glTexCoordPointer(tn, GL_FLOAT, 0, (char *) NULL);
			}
		}
		uint TexID = pObj3DDesc->GetTextureID(i);

		if(TexID)
		{
			if(All || (flags & (x<<(i+19))) )
			{
				TexAddrStruct TexAddr = pObj3DDesc->GetTextureAddress(i);
				pTexMgr->SetTexture(TexID,i);
				pTexMgr->SetTextureAddress(TexAddr.U, TexAddr.V, TexAddr.W);
			}
		}
	}
	
	return true;
}



//////////////////////////////////////////////////////////////////////////////////////////
// Shader Managment System
//////////////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////////////
// Creating Shader Object From Files 
uint CGLObj::CreateShaderFromFiles(char *VertexShader, char *FragmentShader, bool IsHighLevelShader)
{
	if(!VertexShader || !FragmentShader)
		return 0;

	FILE *file = NULL;
	long end;

	//Reading Vertex Shader Source From File
	if((file = fopen(VertexShader,"rb"))==NULL)
		return 0;

	fseek(file,0,SEEK_END);//Searching End Of The File
	end = ftell(file);//Getting File End Position (Size Of The File)
	fseek(file,0,SEEK_SET);//Searching Beginning Of The File

	char *VertexShaderSource = new char[end+1];//Creating Char Array To Read VS Source
	fread(VertexShaderSource,end,1,file);//Reading Vertex Shader Source From File
	VertexShaderSource[end]='\0';
	fclose(file);//Closing Vertex Shader File

	//Reading Fragment Shader Source From File
	if((file = fopen(FragmentShader,"rb"))==NULL)
		return 0;

	fseek(file,0,SEEK_END);//Searching End Of The File
	end = ftell(file);//Getting File End Position (Size Of The File)
	fseek(file,0,SEEK_SET);//Searching Beginning Of The File

	char *FragmentShaderSource = new char[end+1];//Creating Char Array To Read FS Source
	fread(FragmentShaderSource, end, 1, file);//Reading Vertex Shader Source From File
	FragmentShaderSource[end]='\0';
	fclose(file);//Closing Vertex Shader File

	//Creating Shader Object From Vertex And Fragment Shader Source Code
	uint ShaderObjIdx = this->CreateShader(VertexShaderSource,FragmentShaderSource);

	delete [] VertexShaderSource;
	delete [] FragmentShaderSource;
	
	return ShaderObjIdx;
}

//////////////////////////////////////////////////////////////////////////////////////////
// Creating Shader Object From Strings
// Strings Are Not Released In Function
uint CGLObj::CreateShader(char *strVertexShaderSource, char *strFragmentShaderSource, bool IsHighLevelShader)
{
	if(!strVertexShaderSource || !strFragmentShaderSource)
		return 0;

	sGLShaderObject ShaderObj;
	memset(&ShaderObj,0,sizeof(sGLShaderObject));

	//Create A Vertex Shader And A Fragment Shader Object
	ShaderObj.hVertrexShader = glCreateShaderObjectARB(GL_VERTEX_SHADER);
	ShaderObj.hFragmentShader = glCreateShaderObjectARB(GL_FRAGMENT_SHADER);

	const char *szVertShader = strVertexShaderSource;
	const char *szFragShader = strFragmentShaderSource;
	//Load Source Code Strings Into Shaders
	glShaderSourceARB(ShaderObj.hVertrexShader,1,&szVertShader,NULL);
	glShaderSourceARB(ShaderObj.hFragmentShader,1,&szFragShader,NULL);

	GLint vertCompiled,fragCompiled;
	//Compile Vertex Shader
	glCompileShaderARB(ShaderObj.hVertrexShader);
	glGetProgramivARB(ShaderObj.hVertrexShader, GL_COMPILE_STATUS, &vertCompiled);

	//Compile Fragment Shader
	glCompileShaderARB(ShaderObj.hFragmentShader);
	glGetProgramivARB(ShaderObj.hFragmentShader, GL_COMPILE_STATUS, &fragCompiled);

	//Check Status
	if(!vertCompiled || !fragCompiled)
		return 0;

	//Create A Program Object And Attach The Two Compiled Shaders
	ShaderObj.hProgramObject = glCreateProgramObjectARB();
	glAttachObjectARB(ShaderObj.hProgramObject, ShaderObj.hVertrexShader);
	glAttachObjectARB(ShaderObj.hProgramObject, ShaderObj.hFragmentShader);

	//Link The Program Object
	glLinkProgramARB(ShaderObj.hProgramObject);

	//Check Link Status
	GLint linked;
	glGetProgramivARB(ShaderObj.hProgramObject, GL_LINK_STATUS, &linked);
	if(!linked)
		return 0;

	return ShaderObjectsArray.AddElement(ShaderObj);
}

//////////////////////////////////////////////////////////////////////////////////////////
// Deleting Shader Object From ShaderArray
bool CGLObj::DeleteShader(uint ShaderObjIdx)
{
	sGLShaderObject *pShaderObj = ShaderObjectsArray.GetElement(ShaderObjIdx);
	if(!ReleaseShaderElement(pShaderObj))
		return false;

	return ShaderObjectsArray.DeleteElement(ShaderObjIdx);
}

//////////////////////////////////////////////////////////////////////////////////////////
// Release Shader Object 
bool CGLObj::ReleaseShaderElement(sGLShaderObject *pShaderObj)
{
	if(!pShaderObj)
		return false;

	//Release Shader Object
	if(pShaderObj->hVertrexShader)
	{
		glDetachObjectARB(pShaderObj->hProgramObject, pShaderObj->hVertrexShader);
		glDeleteObjectARB(pShaderObj->hVertrexShader);
		pShaderObj->hVertrexShader = 0;
	}

	if(pShaderObj->hFragmentShader)
	{
		glDetachObjectARB(pShaderObj->hProgramObject, pShaderObj->hFragmentShader);
		glDeleteObjectARB(pShaderObj->hFragmentShader);
		pShaderObj->hFragmentShader = 0;
	}

	if(pShaderObj->hProgramObject)
	{
		glDeleteObjectARB(pShaderObj->hProgramObject);
		pShaderObj->hProgramObject = 0;
	}

	return true;
}

///////////////////////////////////////////////////////////////////////////////////
// Setting Shader Object And All Uniforms
bool CGLObj::SetShaderObject(uint ShaderIdx, CGPUProgramDesc *pShaderDesc)
{
	if(!pShaderDesc)
		return false;

	if(!ShaderIdx)
		return true; //Thats Mean That We Do Not Use Shader

	sGLShaderObject *pShaderObj = ShaderObjectsArray.GetElement(ShaderIdx);
	if(!pShaderObj)
		return false;

	glUseProgramObjectARB(pShaderObj->hProgramObject);

	//Setting Shader Uniform Params
	ShaderUniformDescriptor *pShaderUniDesc = pShaderDesc->GetShaderDescList();
	//Some Temp Variables
	GLint uniloc;
	GLint iparam;
	GLfloat fparam;
	GLfloat *fvparam;
	//pointers To Material And Light Managers
	CGLMaterialSystem *pGLMatSys = (CGLMaterialSystem*)pMatMgr;
	CGLightSystem *pGLightSys = (CGLightSystem*)pLightMgr;
	sMaterial *pGLMat = NULL;
	sGLight *pGLight = NULL;
	uint index = 0;
	uint MatIdx = 0;
	uint LightIdx = 0;

	while(pShaderUniDesc!=NULL)
	{
		uniloc = glGetUniformLocationARB(pShaderObj->hProgramObject,pShaderUniDesc->name);
		if(uniloc == -1) {pShaderUniDesc = pShaderUniDesc->next; continue;}
		switch(pShaderUniDesc->UniformType)
		{
		case MATERIAL_AMBIENT_COLOR:
			if ((index = ((uint*)pShaderUniDesc->param)[0]) == 0) break;
			if(index != MatIdx)
			{if((pGLMat = pGLMatSys->GetMaterial(index)) == NULL) break;}
			glUniform4fvARB(uniloc,1,pGLMat->Ambient);
			MatIdx = index;
			break;
		case MATERIAL_DIFFUSE_COLOR:
			if ((index = ((uint*)pShaderUniDesc->param)[0]) == 0) break;
			if(index != MatIdx)
			{if((pGLMat = pGLMatSys->GetMaterial(index)) == NULL) break;}
			glUniform4fvARB(uniloc,1,pGLMat->Diffuse);
			MatIdx = index;
			break;
		case MATERIAL_SPECULAR_COLOR:
			if ((index = ((uint*)pShaderUniDesc->param)[0]) == 0) break;
			if(index != MatIdx)
			{if((pGLMat = pGLMatSys->GetMaterial(index)) == NULL) break;}
			glUniform4fvARB(uniloc,1,pGLMat->Specular);
			MatIdx = index;
			break;
		case MATERIAL_EMISSION_COLOR:
			if ((index = ((uint*)pShaderUniDesc->param)[0]) == 0) break;
			if(index != MatIdx)
			{if((pGLMat = pGLMatSys->GetMaterial(index)) == NULL) break;}
			glUniform4fvARB(uniloc,1,pGLMat->Emission);
			MatIdx = index;
			break;
		case MATERIAL_KA_KD_KS_COEF_VECTOR3:
			if ((index = ((uint*)pShaderUniDesc->param)[0]) == 0) break;
			if(index != MatIdx)
			{if((pGLMat = pGLMatSys->GetMaterial(index)) == NULL) break;}
			glUniform3fARB(uniloc,pGLMat->Ambient.x,pGLMat->Diffuse.x,pGLMat->Specular.x);
			MatIdx = index;
			break;
		case MATERIAL_KA_KD_KS_KE_COEF_VECTOR4:
			if ((index = ((uint*)pShaderUniDesc->param)[0]) == 0) break;
			if(index != MatIdx)
			{if((pGLMat = pGLMatSys->GetMaterial(index)) == NULL) break;}
			glUniform4fARB(uniloc,pGLMat->Ambient.x,pGLMat->Diffuse.x,pGLMat->Specular.x,pGLMat->Emission.x);
			MatIdx = index;
			break;
		case MATERIAL_POWER :
			if ((index = ((uint*)pShaderUniDesc->param)[0]) == 0) break;
			if(index != MatIdx)
			{if((pGLMat = pGLMatSys->GetMaterial(index)) == NULL) break;}
			glUniform1fARB(uniloc,pGLMat->Power);
			MatIdx = index;
			break;

		case TEXTURE_UNIT:
			iparam = GLint(((uint*)pShaderUniDesc->param)[0]);
			glUniform1iARB(uniloc,iparam);
			break;

		case USER_MATRIX4X4:
			fvparam = (GLfloat*)pShaderUniDesc->param;
			glUniformMatrix4fvARB(uniloc,1,true,fvparam);
			break;
		case USER_MATRIX3X3:
			fvparam = (GLfloat*)pShaderUniDesc->param;
			glUniformMatrix3fvARB(uniloc,1,true,fvparam);
			break;
		case USER_VECTOR4:
			fvparam = (GLfloat*)pShaderUniDesc->param;
			glUniform4fvARB(uniloc,1,fvparam);
			break;
		case USER_VECTOR3:
			fvparam = (GLfloat*)pShaderUniDesc->param;
			glUniform3fvARB(uniloc,1,fvparam);
			break;
		case USER_VECTOR2:
			fvparam = (GLfloat*)pShaderUniDesc->param;
			glUniform2fvARB(uniloc,1,fvparam);
			break;
		case USER_FLOAT:
			fparam = ((GLfloat*)pShaderUniDesc->param)[0];
			glUniform1fARB(uniloc,fparam);
			break;
		}
		pShaderUniDesc = pShaderUniDesc->next;
	}

	
	return true;
}

///////////////////////////////////////////////////////////////////////////////////
// Turn Off Shader Object
void CGLObj::TurnOffShader()
{
	glUseProgramObjectARB(0);
}