////////////////////////////////////////////////////////////////////////////////////////////////
//	D3DRenderer.cpp 
//	Direct3D Rendering core class functions definitions
//	Created: 15th February 2006
//	
////////////////////////////////////////////////////////////////////////////////////////////////
#include <windows.h>
#include <stdio.h>

#include "../Math/MathLib.h"
#include "../Types/types.h"
#include "../Camera/Camera.h"
#include "../Templates/Array.h"

#include "../3DObjects/3DObject.h"
#include "../Image/Image.h"
#include "../Image/ImageIO.h"
#include "../Image/ImgTypes.h"

#include "../Renderer/Renderer.h"


#include "D3DRenderer.h"
//#include "D3DRenderElement.h"
#include "D3DConvFunc.h"

#include "D3DDevCaps.h"
#include "D3DLightMgr.h"
#include "D3DMaterialMgr.h"
#include "D3DTextureMgr.h"


#include <memory.h>

///////////////////////////////////////////////////////////////
//Default Class Constructor
CD3DObj::CD3DObj():CGraphicAPI()
{
	hWndRect=NULL;
	m_pD3D=NULL;
	m_pDeviceD3D=NULL;

	background = D3DCOLOR_ARGB(0,0,0,0);
	D3DXMatrixIdentity(&mWorldMatrix);
}

///////////////////////////////////////////////////////////////
//Class Destructor
CD3DObj::~CD3DObj()
{
	if(m_pD3D!=NULL)
		m_pD3D->Release();

	if(m_pDeviceD3D!=NULL)
		m_pDeviceD3D->Release();

	m_pD3D=NULL;
	m_pDeviceD3D=NULL;

	//Releasing All Buffers From Render Elements Array
	D3DRenderElement *pRenderElement = NULL;
	RenderElementArray.SetPointerToStart();
	while(true)
	{
		pRenderElement = RenderElementArray.GetElementAndMovePointer();
		if(pRenderElement)
			ReleaseRenderElement(pRenderElement);
		else
			break;
	}

	//Releasing All Shaders Objects
	D3DEffectObject *pEffectObj = NULL;
	EffectsArray.SetPointerToStart();
	while(true)
	{
		pEffectObj = EffectsArray.GetElementAndMovePointer();
		if(pEffectObj)
			pEffectObj->pEffect->Release();
		else
			break;
	}
}

/////////////////////////////////////////////////////////////////////
//Creating Direct3D Window
bool CD3DObj::Create(HWND hWnd)
{
	hWndRect = hWnd;

	m_pD3D=Direct3DCreate9(D3D_SDK_VERSION);
	if(m_pD3D==NULL)
	{
		MessageBox(hWnd,"Unable to Create Direct3D Interface","Error",MB_OK|MB_ICONEXCLAMATION);
		return FALSE;
	}

	D3DPRESENT_PARAMETERS mPresentParameters;
	D3DDEVICE_CREATION_PARAMETERS mCreationParameters;

	memset(&mPresentParameters,0,sizeof(D3DPRESENT_PARAMETERS));
	memset(&mCreationParameters,0,sizeof(D3DDEVICE_CREATION_PARAMETERS));

	
	m_pD3D->GetDeviceCaps(D3DADAPTER_DEFAULT,D3DDEVTYPE_HAL,&mDevCaps);

	pDevCaps = new CD3DDeviceCaps(&mDevCaps);

	if(mDevCaps.VertexShaderVersion>=D3DVS_VERSION(1,0))
	{
		D3DDISPLAYMODE DisplayMode;
		if(SUCCEEDED(m_pD3D->GetAdapterDisplayMode(D3DADAPTER_DEFAULT,&DisplayMode)))
		{
			mPresentParameters.Windowed=TRUE;
			mPresentParameters.SwapEffect=D3DSWAPEFFECT_DISCARD;
			mPresentParameters.BackBufferFormat=DisplayMode.Format;
			mPresentParameters.EnableAutoDepthStencil=TRUE;
			mPresentParameters.AutoDepthStencilFormat=D3DFMT_D24S8;
			mPresentParameters.PresentationInterval=D3DPRESENT_INTERVAL_IMMEDIATE;

			if(FAILED(m_pD3D->CreateDevice(D3DADAPTER_DEFAULT,D3DDEVTYPE_HAL,
											hWndRect,D3DCREATE_HARDWARE_VERTEXPROCESSING,
											&mPresentParameters,&m_pDeviceD3D)))
			{
				MessageBox(hWnd,"Failed to Create Direct3D Device","Error",MB_OK|MB_ICONEXCLAMATION);
				return FALSE;
			}
		}
		else
		{
			MessageBox(hWnd,"Failed to Get Adapter Display Modes","Error",MB_OK|MB_ICONEXCLAMATION);
			return FALSE;
		}
	}
	else
	{
		MessageBox(hWnd,"Graphic card doesn't Support Vertex Shaders","Error",MB_OK|MB_ICONEXCLAMATION);
		return FALSE;
	}

	return TRUE;
}

///////////////////////////////////////////////////////////////////////////////
// All Initializing Setup For DirectGraphics Goes Here
bool CD3DObj::Init()
{
	m_pDeviceD3D->SetRenderState(D3DRS_ZENABLE, true);
	m_pDeviceD3D->SetRenderState(D3DRS_CULLMODE, D3DCULL_CW);
	m_pDeviceD3D->SetRenderState(D3DRS_LIGHTING, false);
	m_pDeviceD3D->SetRenderState(D3DRS_STENCILENABLE, false);
	m_pDeviceD3D->SetRenderState(D3DRS_FOGENABLE, false);
	//m_pDeviceD3D->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME );

	pLightMgr = new CD3DLightSystem(m_pDeviceD3D,mDevCaps.MaxActiveLights);
	pMatMgr = new CD3DMaterialSystem(m_pDeviceD3D);
	pTexMgr = new CD3DTextureSystem(hWndRect, m_pDeviceD3D, &mDevCaps);

	return TRUE;
}

///////////////////////////////////////////////////////////////////////////////
//Drawing Scene Using DirectGraphics API
void CD3DObj::Render()
{
	m_pDeviceD3D->Clear(0,NULL, D3DCLEAR_TARGET|D3DCLEAR_ZBUFFER,
						background,1.0f,0);
	m_pDeviceD3D->BeginScene();

	D3DXMatrixIdentity(&mWorldMatrix);
	m_pDeviceD3D->SetTransform(D3DTS_WORLD,&mWorldMatrix);

	m_pDeviceD3D->EndScene();
	
	//Swap Buffers 
	m_pDeviceD3D->Present(NULL,NULL,NULL,NULL);
}

//////////////////////////////////////////////////////////////////////////////////
// Resize And Initialize
void CD3DObj::Resize(int width, int height)
{
	D3DXMatrixPerspectiveFovRH(&mProjMatrix,mFOV,(float)width/(float)height,mNear,mFar);
	if(m_pDeviceD3D)
		m_pDeviceD3D->SetTransform(D3DTS_PROJECTION,&mProjMatrix);

	Width = width;
	Height = height;
}

//////////////////////////////////////////////////////////////////////////////////
// Updating And Setting Camera Parameters
void CD3DObj::SetView(const CCamera &Cam)
{
	memcpy(&mPosVec, &Cam.Position(), sizeof(D3DXVECTOR3));
	memcpy(&mViewVec, &Cam.ViewVec(), sizeof(D3DXVECTOR3));
	memcpy(&mUpVec, &Cam.UpVec(),sizeof(D3DXVECTOR3));

	D3DXMatrixLookAtRH(&mViewMatrix,&mPosVec,&mViewVec,&mUpVec);
	m_pDeviceD3D->SetTransform(D3DTS_VIEW,&mViewMatrix);

	mFOV  = DegToRad(Cam.FOV());
	mNear = Cam.Near();
	mFar  = Cam.Far();
}

//////////////////////////////////////////////////////////////////////////////////
// Setting Camera Options
void CD3DObj::SetView()
{
	D3DXMatrixLookAtRH(&mViewMatrix,&mPosVec,&mViewVec,&mUpVec);
	m_pDeviceD3D->SetTransform(D3DTS_VIEW,&mViewMatrix);
}

//////////////////////////////////////////////////////////////////////////////////
// Setting Viewport Size
void CD3DObj::SetViewport(int width,int height)
{
	D3DVIEWPORT9 viewport;
	viewport.X = viewport.Y = 0;
	viewport.MinZ = 0.0f;
	viewport.MaxZ = 1.0f;
	viewport.Width = width;
	viewport.Height = height;

	Width = width;
	Height = height;

	m_pDeviceD3D->SetViewport(&viewport);
}

//////////////////////////////////////////////////////////////////////////////////
// Setting Orthographic Projection
void CD3DObj::SetOrthoProj(float l,float r,float b,float t,float zn,float zf)
{
	D3DXMatrixOrthoOffCenterRH(&mProjMatrix,l,r,b,t,zn,zf);
	m_pDeviceD3D->SetTransform(D3DTS_PROJECTION,&mProjMatrix);
}

//////////////////////////////////////////////////////////////////////////////////
// Setting Orthographic Projection
void CD3DObj::SetOrthoProj(float left,float right,float bottom,float top)
{
	D3DXMatrixOrthoRH(&mProjMatrix,left,right,bottom,top);
	m_pDeviceD3D->SetTransform(D3DTS_PROJECTION,&mProjMatrix);
}

//////////////////////////////////////////////////////////////////////////////////
// Picking Screen.
// Getting Point In Global Space Were User Picked Camera Near Clip Plane
Vector3D CD3DObj::PickScreen(float MouseX,float MouseY)
{
	D3DXMATRIX InvViewProj=mViewMatrix*mProjMatrix;
	D3DXMatrixInverse(&InvViewProj, NULL, &InvViewProj);

	D3DVIEWPORT9 Viewport;
	m_pDeviceD3D->GetViewport(&Viewport);

	D3DXVECTOR4 cPos(2*MouseX/float(Viewport.Width)-1, 1-2*MouseY/float(Viewport.Height),0,1);

	D3DXVec4Transform(&cPos, &cPos, &InvViewProj);
	
	return Vector3D(cPos.x,cPos.y,cPos.z);
}

//////////////////////////////////////////////////////////////////////////////////
// Clearing All Buffers
void CD3DObj::Clear()
{
	m_pDeviceD3D->Clear(0,NULL, D3DCLEAR_TARGET|D3DCLEAR_ZBUFFER|D3DCLEAR_STENCIL,
						background,1.0f,0);
}

//////////////////////////////////////////////////////////////////////////////////
// Clearing Only Chosen Buffers
void CD3DObj::Clear(bool Color,bool Depth,bool Stencil)
{
	DWORD flags=0;

	if(Color)
		flags = flags | D3DCLEAR_TARGET;
	if(Depth)
		flags = flags | D3DCLEAR_ZBUFFER;
	if(Stencil)
		flags = flags | D3DCLEAR_STENCIL;

	m_pDeviceD3D->Clear(0,NULL,flags,background,1.0f,0);
}

//////////////////////////////////////////////////////////////////////////////////
// Setting Polygon Fill Mode
void CD3DObj::SetFillMode(uint fillmode)
{
	switch(fillmode)
	{
	case 0:
		m_pDeviceD3D->SetRenderState(D3DRS_FILLMODE, D3DFILL_POINT);
		break;

	case 1:
		m_pDeviceD3D->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
		break;

	case 2:
		m_pDeviceD3D->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
		break;

	default:
		m_pDeviceD3D->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
		break;
	}
}

//////////////////////////////////////////////////////////////////////////////////
// Setting Polygon Cull Mode
void CD3DObj::SetCullMode(bool CullFace,bool CCW)
{
	if(CullFace)
	{
		if(CCW)
			m_pDeviceD3D->SetRenderState(D3DRS_CULLMODE, D3DCULL_CW);
		else
			m_pDeviceD3D->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	}
	else
	{
		m_pDeviceD3D->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	}	
}

//////////////////////////////////////////////////////////////////////////////////
// Setting Backgrounf Color
void CD3DObj::SetBgColor(ubyte R,ubyte G,ubyte B)
{
	background = D3DCOLOR_XRGB(R,G,B);
}

//////////////////////////////////////////////////////////////////////////////////
// Setting Shading Mode (Gouraud or Flat Shading)
void CD3DObj::SetShadingMode(bool Smooth)
{
	if(Smooth)
		m_pDeviceD3D->SetRenderState(D3DRS_SHADEMODE, D3DSHADE_GOURAUD);
	else
		m_pDeviceD3D->SetRenderState(D3DRS_SHADEMODE, D3DSHADE_FLAT);
}

//////////////////////////////////////////////////////////////////////////////////
// Enabling/Disabling Shading And Specular Calculations
void CD3DObj::LightingEnable(bool flag)
{
	if(flag)
	{
		m_pDeviceD3D->SetRenderState(D3DRS_SPECULARENABLE, TRUE);
		m_pDeviceD3D->SetRenderState(D3DRS_LIGHTING, TRUE);
	}
	else
	{
		m_pDeviceD3D->SetRenderState(D3DRS_SPECULARENABLE, FALSE);
		m_pDeviceD3D->SetRenderState(D3DRS_LIGHTING, FALSE);
	}
}

//////////////////////////////////////////////////////////////////////////////////
// Enabling/Disabling Normals Normalization. In Graphics Cards With T&L 
// Application Should Run Faster That Normalizing On CPU
void CD3DObj::SetNormalsNormalization(bool Normalize)
{
	if(Normalize)
		m_pDeviceD3D->SetRenderState(D3DRS_NORMALIZENORMALS,TRUE);
	else
		m_pDeviceD3D->SetRenderState(D3DRS_NORMALIZENORMALS,FALSE);
}

//////////////////////////////////////////////////////////////////////////////////
// Enabling/Disabling Colors Blending
void CD3DObj::AlphaBlendEnable(bool flag)
{
	if(flag)
		m_pDeviceD3D->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	else
		m_pDeviceD3D->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
}

//////////////////////////////////////////////////////////////////////////////////
// Setting Blending Mode
void CD3DObj::SetBlendingMode(BLENDOP Source, BLENDOP Destination)
{
	m_pDeviceD3D->SetRenderState(D3DRS_SRCBLEND, GetD3DBlendOp(Source));
	m_pDeviceD3D->SetRenderState(D3DRS_DESTBLEND, GetD3DBlendOp(Destination));
}

//////////////////////////////////////////////////////////////////////////////////
// Enabling/Disabling Alpha Test
void CD3DObj::AlphaTestEnable(bool flag)
{
	if(flag)
		m_pDeviceD3D->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	else
		m_pDeviceD3D->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
}

//////////////////////////////////////////////////////////////////////////////////
// Setting Alpha Test Function
void CD3DObj::AlphaFunc(FUNC func,float ref)
{
	if(ref<0.0f)
		ref=0.0f;
	else if(ref>1.0f)
		ref=1.0f;

	m_pDeviceD3D->SetRenderState(D3DRS_ALPHAFUNC, GetD3DFunc(func));
	m_pDeviceD3D->SetRenderState(D3DRS_ALPHAREF, DWORD(ref*255.0f));
}

//////////////////////////////////////////////////////////////////////////////////
//Depth Testing Setting Functions
//////////////////////////////////////////////////////////////////////////////////
// Enabling/Disabling Depth Test
void CD3DObj::DepthTestEnable(bool flag)
{
	if(flag)
		m_pDeviceD3D->SetRenderState(D3DRS_ZENABLE, TRUE);
	else
		m_pDeviceD3D->SetRenderState(D3DRS_ZENABLE, FALSE);
}

//////////////////////////////////////////////////////////////////////////////////
// Enabling/Disabling Writing To Depth Buffer
void CD3DObj::DepthWriteEnable(bool flag)
{
	if(flag)
		m_pDeviceD3D->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
	else
		m_pDeviceD3D->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);
}

//////////////////////////////////////////////////////////////////////////////////
// Setting Depth Test Function
void CD3DObj::DepthFunc(FUNC func)
{
	m_pDeviceD3D->SetRenderState(D3DRS_ZFUNC,GetD3DFunc(func));
}

//////////////////////////////////////////////////////////////////////////////////
// Enabling/Disabling Writing To Color Buffer
void CD3DObj::ColorsWriteEnable(bool R,bool G,bool B,bool A)
{
	DWORD colors=0;

	if(R)
		colors = colors | D3DCOLORWRITEENABLE_RED;
	if(G)
		colors = colors | D3DCOLORWRITEENABLE_GREEN;
	if(B)
		colors = colors | D3DCOLORWRITEENABLE_BLUE;
	if(A)
		colors = colors | D3DCOLORWRITEENABLE_ALPHA;

	m_pDeviceD3D->SetRenderState(D3DRS_COLORWRITEENABLE, colors);
}

//////////////////////////////////////////////////////////////////////////////////
// Stencil Testing Setting Functions
//////////////////////////////////////////////////////////////////////////////////
// Write mask applied to values written into the stencil buffer
void CD3DObj::StencilWriteMask(uint mask)
{
	m_pDeviceD3D->SetRenderState(D3DRS_STENCILWRITEMASK,mask);
}
//////////////////////////////////////////////////////////////////////////////////
// Enabling/Disabling Stencil Test
void CD3DObj::StencilTestEnable(bool flag)
{
	if(flag)
		m_pDeviceD3D->SetRenderState(D3DRS_STENCILENABLE, TRUE);
	else
		m_pDeviceD3D->SetRenderState(D3DRS_STENCILENABLE, FALSE);
}
//////////////////////////////////////////////////////////////////////////////////
// Setting Stencil Test Function
void CD3DObj::StencilFunc(FUNC func,uint ref,uint mask)
{
	m_pDeviceD3D->SetRenderState(D3DRS_STENCILFUNC, GetD3DFunc(func));
	m_pDeviceD3D->SetRenderState(D3DRS_STENCILREF, ref);
	m_pDeviceD3D->SetRenderState(D3DRS_STENCILMASK, mask);
}
//////////////////////////////////////////////////////////////////////////////////
// Setting Stencil Test Operations
void CD3DObj::StencilOp(OP fail, OP zfail, OP zpass)
{
	m_pDeviceD3D->SetRenderState(D3DRS_STENCILFAIL, GetD3DOp(fail));
	m_pDeviceD3D->SetRenderState(D3DRS_STENCILZFAIL, GetD3DOp(zfail));
	m_pDeviceD3D->SetRenderState(D3DRS_STENCILPASS, GetD3DOp(zpass));
}
//////////////////////////////////////////////////////////////////////////////////
// Enabling/Disabling Two Side Stencil Test. Culling Mode Must Be Turned Off !!!
void CD3DObj::TwoSideStencilEnable(bool flag)
{
	m_pDeviceD3D->SetRenderState(D3DRS_TWOSIDEDSTENCILMODE, DWORD(flag));
}

//////////////////////////////////////////////////////////////////////////////////
// Setting Stencil For Back Faces. In This Graphic API Front Face Has CCW Order
void CD3DObj::SetStencilForBackFace(FUNC func,uint ref,uint mask,OP fail,OP zfail,OP zpass)
{
	this->StencilFunc(func,ref,mask);
	this->StencilOp(fail,zfail,zpass);
}
//////////////////////////////////////////////////////////////////////////////////
// Setting Stencil For Front Faces. In This Graphic API Front Face Has CCW Order
void CD3DObj::SetStencilForFrontFace(FUNC func,uint ref,uint mask,OP fail,OP zfail,OP zpass)
{
	m_pDeviceD3D->SetRenderState(D3DRS_CCW_STENCILFUNC, GetD3DFunc(func));
	//m_pDeviceD3D->SetRenderState(D3DRS_STENCILREF, ref);
	//m_pDeviceD3D->SetRenderState(D3DRS_STENCILMASK, mask);

	m_pDeviceD3D->SetRenderState(D3DRS_CCW_STENCILFAIL, GetD3DOp(fail));
	m_pDeviceD3D->SetRenderState(D3DRS_CCW_STENCILZFAIL, GetD3DOp(zfail));
	m_pDeviceD3D->SetRenderState(D3DRS_CCW_STENCILPASS, GetD3DOp(zpass));
}

//////////////////////////////////////////////////////////////////////////////////
// Clip Planes Setting Functions
//////////////////////////////////////////////////////////////////////////////////
// Enabling/Disabling Clip Plane
void CD3DObj::ClipPlaneEnable(uint Index, bool flag)
{
	if(Index>5)
		return ;

	DWORD Status = 0;
	
	m_pDeviceD3D->GetRenderState(D3DRS_CLIPPLANEENABLE,&Status);
	if(flag)
		Status = Status | (1<<Index);
	else
		Status = Status & (~(1<<Index));
	
	m_pDeviceD3D->SetRenderState(D3DRS_CLIPPLANEENABLE,Status);
}

//////////////////////////////////////////////////////////////////////////////////
// Setting Specified Clip Plane Equation
bool CD3DObj::SetClipPlane(uint Index, Vector4D &PlaneEq)
{
	if(Index>5)
		return false;

	m_pDeviceD3D->SetClipPlane(Index,PlaneEq);

	return true;
}


//////////////////////////////////////////////////////////////////////////////////
// Enabling/Disabling Scissor Test
void CD3DObj::ScissorTestEnable(bool flag)
{
	if(flag)
		m_pDeviceD3D->SetRenderState(D3DRS_SCISSORTESTENABLE, TRUE);
	else
		m_pDeviceD3D->SetRenderState(D3DRS_SCISSORTESTENABLE, FALSE);
}

//////////////////////////////////////////////////////////////////////////////////
// Setting Scissor Rectangle In Scissor Test
void CD3DObj::SetScissorRect(uint x, uint y, uint w, uint h)
{
	RECT rect = { x, Height - (y + h), x + w, Height - y };
	m_pDeviceD3D->SetScissorRect(&rect);
}

//////////////////////////////////////////////////////////////////////////////////
// Setting Polygon Offset
// It Is Used To Solve Depth Test Artifacts
void CD3DObj::SetPolygonOffset(bool flag, float bias, float slopeBias)
{
	if(flag)
	{
		m_pDeviceD3D->SetRenderState(D3DRS_DEPTHBIAS, *(DWORD *) &bias);
		m_pDeviceD3D->SetRenderState(D3DRS_SLOPESCALEDEPTHBIAS, *(DWORD *) &slopeBias);
	}
	else
	{
		m_pDeviceD3D->SetRenderState(D3DRS_DEPTHBIAS, 0);
		m_pDeviceD3D->SetRenderState(D3DRS_SLOPESCALEDEPTHBIAS, 0);
	}
}

//////////////////////////////////////////////////////////////////////////////////
// Setting New World Transformation Matrix
void CD3DObj::SetWorldMatrix(Matrix4x4 &M)
{
	if(Matrix4x4()==M)
		D3DXMatrixIdentity(&mWorldMatrix);
	else
		memcpy(&mWorldMatrix,M,sizeof(float)*16);

	D3DXMatrixTranspose(&mWorldMatrix,&mWorldMatrix);
}

//////////////////////////////////////////////////////////////////////////////////
// Saving Screen Image
void CD3DObj::GrabScreenShot(char* file_name)
{
	POINT topLeft;
	topLeft.x = 0;
	topLeft.y = 0;
	ClientToScreen(hWndRect, &topLeft);

	int mw = GetSystemMetrics(SM_CXSCREEN);
	int mh = GetSystemMetrics(SM_CYSCREEN);

	LPDIRECT3DSURFACE9 Surface;
	//bool result = false;
	// Create a screen-size surface
	if (m_pDeviceD3D->CreateOffscreenPlainSurface(mw, mh, D3DFMT_A8R8G8B8, D3DPOOL_SCRATCH, 
		&Surface, NULL) == D3D_OK)
	{
		// Copy the front buffer into it	
		if (m_pDeviceD3D->GetFrontBufferData(0, Surface) == D3D_OK)
		{
			D3DLOCKED_RECT LockedRect;
			if (Surface->LockRect(&LockedRect, NULL, D3DLOCK_READONLY) == D3D_OK)
			{
				CImage *Img = new CImage;
				CBMP *pBMPImage = new CBMP();

				// Copy RGB data from the surface into image
				ubyte *src, *dat = (ubyte*)Img->Create(FORMAT_RGB8, Width, Height, 1, 1);
				ubyte *dst = dat;
				for (int y = 0; y < Height; y++)
				{
					src = ((ubyte *)LockedRect.pBits) + ((topLeft.y + y) * mw + topLeft.x) * 4;
					dst = dat + Width*(Height-y-1)*3;
					for (int x = 0; x < Width; x++){
						*dst++ = *src++;
						*dst++ = *src++;
						*dst++ = *src++;
						src++;
					}
					src += Img->getPitch()*3;
				}

				Surface->UnlockRect();

				// If Function Recieved filename Then Save Image
				
				if(file_name)
				{
					pBMPImage->SaveImage(Img,file_name); 
				}
				else
				{
					// Find the first unused filename and save to it
					char fileName[32];
					FILE *file;
					for (int i = 0; i < 100; i++)
					{
						sprintf(fileName, "Screenshot%02d.bmp", i);
						if ((file=fopen(&fileName[0],"rb"))==NULL)
						{
							pBMPImage->SaveImage(Img,fileName);
							//result = true;
							break;
						}
						else
							fclose(file);
					}
				}

				delete (Img);
				delete (pBMPImage);
			}
		}
		Surface->Release();
	}
}



//////////////////////////////////////////////////////////////////////////////////
// 3DObject Manipulating Methods
//////////////////////////////////////////////////////////////////////////////////
// Adding New 3DObject To Array And Creating Data Buffers
uint CD3DObj::Add3DObjToBuffer(C3DObject *pObj3D,C3DObjectDesc *ObjDesc,bool dynamic)
{
	if(pObj3D == NULL)
		return 0;

	D3DRenderElement RenderElement;
	//Reseting All Values And Setting All Pointers To NULL In This Structure
	ClearD3DRenderElement(RenderElement);

	memcpy(&RenderElement.VertStruct,&pObj3D->GetVertexDescription(),sizeof(VertexDesc));
	RenderElement.IndexNum = pObj3D->GetIndexNum();
	RenderElement.VertexNum = pObj3D->GetVertexNum();
	RenderElement.TexNum = pObj3D->GetTexNum();
	RenderElement.dynamic = dynamic; //Do We Have To Create Dynamic Buffres 

	memcpy(&RenderElement.ModelMatrix,&pObj3D->GetModelMatrix(),sizeof(Matrix4x4));
	D3DXMatrixTranspose(&RenderElement.ModelMatrix, &RenderElement.ModelMatrix);
	
	RenderElement.ObjDesc = new C3DObjectDesc();
	if(ObjDesc!=NULL)
		RenderElement.ObjDesc->CopyData(ObjDesc);

	int i=0;
	D3DVERTEXELEMENT9 element[64];
	VOID *data=NULL;
	//Copy Data Into Buffers
	//Setting Vertex Array
	if(RenderElement.VertStruct.VertDesc & VERTEX_POSITION_FLAG)
	{
		SetElement(element[i], i, 0, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, 
					D3DDECLUSAGE_POSITION, 0);
		i+=1;

		//vertex buffer
		if(FAILED(m_pDeviceD3D->CreateVertexBuffer(sizeof(D3DXVECTOR3)*RenderElement.VertexNum,
			0,0,dynamic ? D3DPOOL_MANAGED : D3DPOOL_DEFAULT, 
			&RenderElement.pVertexVB, NULL)))
		return false;
	
		if(!CopyDataToBuffer(RenderElement.pVertexVB,pObj3D->GetVertexArray(),
			sizeof(D3DXVECTOR3)*RenderElement.VertexNum))
			return false;
	}

	//Setting Normals Array
	if(RenderElement.VertStruct.VertDesc & VERTEX_NORMAL_FLAG)
	{
		SetElement(element[i], i, 0, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, 
			D3DDECLUSAGE_NORMAL, 0);
		i+=1;

		//Normal Buffer
		if(FAILED(m_pDeviceD3D->CreateVertexBuffer(sizeof(D3DXVECTOR3)*RenderElement.VertexNum,
			0,0,dynamic ? D3DPOOL_MANAGED : D3DPOOL_DEFAULT,
			&RenderElement.pNormalVB,NULL)))
			return false;

		if(!CopyDataToBuffer(RenderElement.pNormalVB,pObj3D->GetNormalArray(),
			sizeof(D3DXVECTOR3)*RenderElement.VertexNum))
			return false;
	}

	//Setting Diffuse Color Array
	if(RenderElement.VertStruct.VertDesc & VERTEX_PRIMARY_COLOR_FLAG)
	{
		SetElement(element[i], i, 0, D3DDECLTYPE_D3DCOLOR, D3DDECLMETHOD_DEFAULT, 
			D3DDECLUSAGE_COLOR, 0);
		i+=1;

		//Diffuse Color Buffer
		if(FAILED(m_pDeviceD3D->CreateVertexBuffer(sizeof(D3DCOLOR)*RenderElement.VertexNum,
			0,0,dynamic ? D3DPOOL_MANAGED : D3DPOOL_DEFAULT,
			&RenderElement.pDiffuseVB,NULL)))
			return false;
		//Creating Temp Color Buffer To Match Colors To Direct3D Color Type
		ubyte *clr = pObj3D->GetPrimaryColorArray();
		DWORD *d3dclr = new DWORD[RenderElement.VertexNum];
		DWORD *ptr = d3dclr;
		for(DWORD i=0; i<RenderElement.VertexNum; i++)
		{*ptr++ = D3DCOLOR_ARGB(clr[3],clr[0],clr[1],clr[2]); clr+=4;}

		if(!CopyDataToBuffer(RenderElement.pDiffuseVB,d3dclr,//pObj3D->GetPrimaryColorArray(),
			sizeof(D3DCOLOR)*RenderElement.VertexNum))
		{ delete [] d3dclr; return false;}
		delete [] d3dclr;
	}

	//Setting Specular Color Array
	if(RenderElement.VertStruct.VertDesc & VERTEX_SECONDARY_COLOR_FLAG)
	{
		SetElement(element[i], i, 0, D3DDECLTYPE_D3DCOLOR, D3DDECLMETHOD_DEFAULT, 
			D3DDECLUSAGE_COLOR, 1);
		i+=1;

		//Specular Color Buffer
		if(FAILED(m_pDeviceD3D->CreateVertexBuffer(sizeof(D3DCOLOR)*RenderElement.VertexNum,
			0,0,dynamic ? D3DPOOL_MANAGED : D3DPOOL_DEFAULT,
			&RenderElement.pSpecularVB,NULL)))
			return false;
		//Creating Temp Color Buffer To Match Colors To Direct3D Color Type
		ubyte *clr = pObj3D->GetSecondaryColorArray();
		DWORD *d3dclr = new DWORD[RenderElement.VertexNum];
		DWORD *ptr = d3dclr;
		for(DWORD i=0; i<RenderElement.VertexNum; i++)
		{*ptr++ = D3DCOLOR_ARGB(255,clr[0],clr[1],clr[2]); clr+=3;}

		if(!CopyDataToBuffer(RenderElement.pSpecularVB,d3dclr,
			sizeof(D3DCOLOR)*RenderElement.VertexNum))
		{ delete [] d3dclr; return false; }
		delete [] d3dclr;
	}

	//Setting Index Buffer
	if(RenderElement.VertStruct.VertDesc & VERTEX_INDEXED_FLAG)
	{
		if(m_pDeviceD3D->CreateIndexBuffer(sizeof(DWORD)*RenderElement.IndexNum,0,
			D3DFMT_INDEX32,dynamic ? D3DPOOL_MANAGED : D3DPOOL_DEFAULT,
			&RenderElement.pIndexBuffer,NULL)!= D3D_OK)
			return false;

		if(!CopyDataToIndexBuffer(RenderElement.pIndexBuffer,pObj3D->GetIndexArray(),
			sizeof(uint)*RenderElement.IndexNum))
			return false;
	}

	//Setting Texture Coords Arrays
	for(uint n=0; n<RenderElement.TexNum; n++)
	{	
		ubyte tn = pObj3D->GetTexCountCoord(n);
		if(!tn)
			continue;

		SetElement(element[i], i, 0, tn-1, D3DDECLMETHOD_DEFAULT, 
			D3DDECLUSAGE_TEXCOORD, n);
		i+=1;

		//Diffuse Color Buffer
		if(FAILED(m_pDeviceD3D->CreateVertexBuffer(sizeof(float)*tn*RenderElement.VertexNum,
			0,0,dynamic ? D3DPOOL_MANAGED : D3DPOOL_DEFAULT,
			&RenderElement.pTextureVB[n],NULL)))
			return false;

		if(!CopyDataToBuffer(RenderElement.pTextureVB[n],pObj3D->GetTexCoordArray(n),
			sizeof(float)*tn*RenderElement.VertexNum))
			return false;
	}

	SetElement(element[i++], 0xFF, 0, D3DDECLTYPE_UNUSED, 0, 0, 0);
	if(m_pDeviceD3D->CreateVertexDeclaration(element, &RenderElement.pVertexDeclaration) 
		!= D3D_OK)
		return FALSE;
		
	return RenderElementArray.AddElement(RenderElement);
}

//////////////////////////////////////////////////////////////////////////////////
// Deleting 3DObject Data From Buffers
bool CD3DObj::Delete3DObjFromBuffer(uint ObjIdx)
{
	D3DRenderElement* RenderElement = RenderElementArray.GetElement(ObjIdx);

	if(!ReleaseRenderElement(RenderElement))
		return false;

	return RenderElementArray.DeleteElement(ObjIdx);
}

//////////////////////////////////////////////////////////////////////////////////
// Updating Data In Buffers From 3DObject
bool CD3DObj::UpdateBufferFrom3DObj(uint ObjIdx, C3DObject *pObj3D, uint64 flags)
{
	D3DRenderElement* Elem = RenderElementArray.GetElement(ObjIdx);

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

	uint VertexNum = Elem->VertexNum;

	memcpy(Elem->ModelMatrix,pObj3D->GetModelMatrix(),sizeof(float)*16);
	D3DXMatrixTranspose(&Elem->ModelMatrix,&Elem->ModelMatrix);

	//Updating Vertex Buffer
	if(Elem->VertStruct.VertDesc & VERTEX_POSITION_FLAG)
	{
		if(All || (flags & VERTEX_POSITION_FLAG))
		{
			if(!CopyDataToBuffer(Elem->pVertexVB,pObj3D->GetVertexArray(),
				sizeof(D3DXVECTOR3)*VertexNum))
				return false;
		}
	}

	//Updating Normal Buffer
	if(Elem->VertStruct.VertDesc & VERTEX_NORMAL_FLAG)
	{
		if(All || (flags & VERTEX_NORMAL_FLAG))
		{
			if(!CopyDataToBuffer(Elem->pNormalVB,pObj3D->GetNormalArray(),
				sizeof(D3DXVECTOR3)*VertexNum))
				return false;
		}
	}

	//Updating Diffuse Color Buffer
	if(Elem->VertStruct.VertDesc & VERTEX_PRIMARY_COLOR_FLAG)
	{
		if(All || (flags & VERTEX_PRIMARY_COLOR_FLAG))
		{
			DWORD *d3dclr = new DWORD[VertexNum];
			DWORD *ptr = d3dclr;
			ubyte *clr = pObj3D->GetPrimaryColorArray();
			for(DWORD i=0; i<VertexNum; i++)
			{*ptr++ = D3DCOLOR_ARGB(clr[3],clr[0],clr[1],clr[2]); clr+=4;}

			if(!CopyDataToBuffer(Elem->pDiffuseVB,d3dclr,
				sizeof(D3DCOLOR)*VertexNum))
			{ delete [] d3dclr; return false;}
			delete [] d3dclr;
		}
	}

	//Updating Specular Color Buffer
	if(Elem->VertStruct.VertDesc & VERTEX_SECONDARY_COLOR_FLAG)
	{
		if(All || (flags & VERTEX_SECONDARY_COLOR_FLAG))
		{
			DWORD *d3dclr = new DWORD[VertexNum];
			DWORD *ptr = d3dclr;
			ubyte *clr = pObj3D->GetSecondaryColorArray();
			for(DWORD i=0; i<VertexNum; i++)
			{*ptr++ = D3DCOLOR_ARGB(255,clr[0],clr[1],clr[2]); clr+=3;}

			if(!CopyDataToBuffer(Elem->pSpecularVB,d3dclr,sizeof(D3DCOLOR)*VertexNum))
			{ delete [] d3dclr; return false; }
			delete [] d3dclr;
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

			if(!CopyDataToBuffer(Elem->pTextureVB[i],pObj3D->GetTexCoordArray(i),
				sizeof(float)*tn*VertexNum))
				return false;
		}
	}

	return true;
}

//////////////////////////////////////////////////////////////////////////////////
// Updating Index Buffer
bool CD3DObj::UpdateIndexBufferFrom3DObj(uint ObjIdx,C3DObject* pObj3D)
{
	D3DRenderElement *Elem = RenderElementArray.GetElement(ObjIdx);
	if(Elem == NULL)
		return false;

	if(Elem->pIndexBuffer == NULL)
		return false;

	if(!CopyDataToIndexBuffer(Elem->pIndexBuffer,pObj3D->GetIndexArray(),
		sizeof(uint)*Elem->IndexNum))
		return false;

	return true;
}
/*
//////////////////////////////////////////////////////////////////////////////////
// Render One 3DObject 
bool CD3DObj::Render(uint ObjIdx,uint64 flags)
{
	return Render(1,&ObjIdx,flags,NULL);
}

//////////////////////////////////////////////////////////////////////////////////
// Render One 3DObject With Another 3DObject Description
bool CD3DObj::Render(uint ObjIdx, uint64 flags, C3DObjectDesc *pObjDesc)
{
	return Render(1, &ObjIdx, flags, &pObjDesc);
}

//////////////////////////////////////////////////////////////////////////////////
//Render Many 3DObjects From Index Array
bool CD3DObj::Render(uint ObjNum, uint *ObjIdx,uint64 flags)
{
	return Render(ObjNum,ObjIdx,flags,NULL);
}
*/
//////////////////////////////////////////////////////////////////////////////////
// Rendering Data From Specified Element
bool CD3DObj::Render(uint ObjNum, uint *ObjIdx, uint64 flags, C3DObjectDesc **ppObjDesc)
{
	C3DObjectDesc *pObj3DDesc;

	m_pDeviceD3D->BeginScene();//Starting Scene
	for(uint i=0; i<ObjNum; i++)
	{
		D3DRenderElement *Elem = RenderElementArray.GetElement(ObjIdx[i]);
		//If RenderElement Exists In Array
		if(Elem == NULL)
			continue;

		if(ppObjDesc)
			pObj3DDesc = ppObjDesc[i];
		else
			pObj3DDesc = Elem->ObjDesc;

		if(!SetBuffersData(Elem,flags,pObj3DDesc))
			 continue;

		//If Object Descriptor Contains GPU Program, Then Render Using Shader Object
		if(pObj3DDesc->GetGPUProgramID()!=0)
		{
			RenderEffect(Elem,pObj3DDesc,flags); 
			//Turn Off Textures
			pTexMgr->DisableTextures();
			continue;
		}

		//Setting World Transformation Matrix
		m_pDeviceD3D->SetTransform(D3DTS_WORLD,&(Elem->ModelMatrix*mWorldMatrix));

		//Rendering Data From Buffers
		m_pDeviceD3D->SetVertexDeclaration(Elem->pVertexDeclaration);
		if(Elem->pIndexBuffer != NULL)
		{
			m_pDeviceD3D->SetIndices(Elem->pIndexBuffer);
			m_pDeviceD3D->DrawIndexedPrimitive(D3DPT_TRIANGLELIST,0,0,Elem->VertexNum,0,
				Elem->IndexNum/3);
		}
		else
			m_pDeviceD3D->DrawPrimitive(D3DPT_TRIANGLELIST,0,Elem->VertexNum/3);

		//Turn Off Textures
		pTexMgr->DisableTextures();
	}

	m_pDeviceD3D->EndScene();

	return true;
}

//////////////////////////////////////////////////////////////////////////////////
// Protected Method For Rendering Using Shader Effect Object
void CD3DObj::RenderEffect(D3DRenderElement *Elem, C3DObjectDesc *pObjDesc,uint64 flags)
{
	D3DEffectObject *pEffectObj = EffectsArray.GetElement(pObjDesc->GetGPUProgramID());
	if(!pEffectObj)
		return;

	LPD3DXEFFECT pEffect = pEffectObj->pEffect;

	//Setting Shader Entry Point
	pEffect->SetTechnique("main");

	//Setting All Shader Data
	if(!SetEffect(pEffect,Elem,pObjDesc,flags))
		return ;

	uint NumOfPasses;
	pEffect->Begin(&NumOfPasses,0);

	for(uint i=0; i<NumOfPasses; i++)
	{
		//Begin 'ith' Pass Of The Selected Technique
		pEffect->BeginPass(i);

		//Rendering Data From Buffers
		m_pDeviceD3D->SetVertexDeclaration(Elem->pVertexDeclaration);
		if(Elem->pIndexBuffer != NULL)
		{
			m_pDeviceD3D->SetIndices(Elem->pIndexBuffer);
			m_pDeviceD3D->DrawIndexedPrimitive(D3DPT_TRIANGLELIST,0,0,Elem->VertexNum,0,
				Elem->IndexNum/3);
		}
		else
			m_pDeviceD3D->DrawPrimitive(D3DPT_TRIANGLELIST,0,Elem->VertexNum/3);

		//End 'ith' Pass
		pEffect->EndPass();
	}

	pEffect->End();

	pEffect->SetTechnique(NULL);
}

//////////////////////////////////////////////////////////////////////////////////
// Setting New ModelMatrix For Rendering Element
bool CD3DObj::SetRenderElementModelMatrix(uint ObjIdx, Matrix4x4& M)
{
	D3DRenderElement *Elem = RenderElementArray.GetElement(ObjIdx);

	if(Elem == NULL)
		return false;

	if(Matrix4x4()==M)
		D3DXMatrixIdentity(&Elem->ModelMatrix);
	else
		memcpy(&Elem->ModelMatrix,M,sizeof(float)*16);

	D3DXMatrixTranspose(&Elem->ModelMatrix,&Elem->ModelMatrix);

	return true;
}

//////////////////////////////////////////////////////////////////////////////////
// Setting New Object Description For Rendering Element
bool CD3DObj::SetRenderElement3DObjDesc(uint ObjIdx, C3DObjectDesc* ObjDesc)
{
	D3DRenderElement *Elem = RenderElementArray.GetElement(ObjIdx);

	if(Elem == NULL)
		return false;

	if(ObjDesc == NULL)
		return false;
	Elem->ObjDesc->CopyData(ObjDesc);

	return true;
}

//////////////////////////////////////////////////////////////////////////////////
// Copy Data From 3DObject To Specified Buffer
bool CD3DObj::CopyDataToBuffer(LPDIRECT3DVERTEXBUFFER9 pVB, VOID *data, ULONG size)
{
	if(!pVB && !data && !size)
		return false;

	VOID *bufdata = NULL;

	if(pVB->Lock(0,0,(void**)&bufdata,0) != D3D_OK)
	{
		pVB->Release();
		return false;
	}

	memcpy(bufdata,data,size);
		
	if(pVB->Unlock() != D3D_OK)
	{
		pVB->Release();
		return false;
	}

	return true;
}

//////////////////////////////////////////////////////////////////////////////////
// Copy Data From 3DObject To IndexBuffer
bool CD3DObj::CopyDataToIndexBuffer(LPDIRECT3DINDEXBUFFER9 pIB,VOID *data,ULONG size)
{
	if(!pIB && !data && !size)
		return false;

	VOID *indexbuf = NULL;

	if(pIB->Lock(0,0,(void**)&indexbuf,0) != D3D_OK)
	{
		pIB->Release();
		pIB=NULL;
		return false;
	}

	memcpy(indexbuf,data,size);

	if(pIB->Unlock() != D3D_OK)
	{
		pIB->Release();
		pIB=NULL;
		return false;
	}

	return true;
}

//////////////////////////////////////////////////////////////////////////////////
// Setting Stream Data For Rendering
bool CD3DObj::SetBuffersData(D3DRenderElement *Elem,uint64 flags,C3DObjectDesc *pObjDesc)
{
	if(Elem==NULL)
		return false;

	bool All = false;
	if(flags==0)
		All=true;

	C3DObjectDesc *pObj3DDesc;
	if(pObjDesc)
		pObj3DDesc = pObjDesc;
	else
		pObj3DDesc = Elem->ObjDesc;

	uint stream = 0;
	m_pDeviceD3D->SetVertexDeclaration(Elem->pVertexDeclaration);
	//Setting Vertex Buffer
	if(Elem->VertStruct.VertDesc & VERTEX_POSITION_FLAG)
	{
		if(All || (flags & VERTEX_POSITION_FLAG))
			m_pDeviceD3D->SetStreamSource(stream++,Elem->pVertexVB,0,sizeof(D3DXVECTOR3));
		else
			stream++;
			//m_pDeviceD3D->SetStreamSource(stream++,NULL,0,sizeof(D3DXVECTOR3));
	}

	//Setting Normal Buffer
	if(Elem->VertStruct.VertDesc & VERTEX_NORMAL_FLAG)
	{
		if(All || (flags & VERTEX_NORMAL_FLAG))
			m_pDeviceD3D->SetStreamSource(stream++,Elem->pNormalVB,0,sizeof(D3DXVECTOR3));
		else
			stream++;
			//m_pDeviceD3D->SetStreamSource(stream++,NULL,0,sizeof(D3DXVECTOR3));
	}

	//Setting Diffuse Color Buffer
	if(Elem->VertStruct.VertDesc & VERTEX_PRIMARY_COLOR_FLAG)
	{
		if(All || (flags & VERTEX_PRIMARY_COLOR_FLAG))
			m_pDeviceD3D->SetStreamSource(stream++,Elem->pDiffuseVB,0,sizeof(D3DCOLOR));
		else 
			stream++;
			//m_pDeviceD3D->SetStreamSource(stream++,NULL,0,sizeof(D3DCOLOR));
	}

	//Setting Specular Color Buffer
	if(Elem->VertStruct.VertDesc & VERTEX_SECONDARY_COLOR_FLAG)
	{
		if(All || (flags & VERTEX_SECONDARY_COLOR_FLAG))
			m_pDeviceD3D->SetStreamSource(stream++,Elem->pSpecularVB,0,sizeof(D3DCOLOR));
		else
			stream++;
	}

	//Setting Texture Coords And Texture Units
	uint tn=0;
	uint end = 0;
	uint64 x=1;
	
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

				m_pDeviceD3D->SetStreamSource(stream++,Elem->pTextureVB[i],0,
					sizeof(float)*tn);
			}
			else
				stream++;
		}
	
		// Setting Texture Samplers
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

//////////////////////////////////////////////////////////////////////////////////
// Releasing All Buffers From Render Element
bool CD3DObj::ReleaseRenderElement(D3DRenderElement *Elem)
{
	if(Elem == NULL)
		return false;

	if(Elem->pVertexVB != NULL)
		Elem->pVertexVB->Release();
	if(Elem->pNormalVB != NULL)
		Elem->pNormalVB->Release();
	if(Elem->pDiffuseVB != NULL)
		Elem->pDiffuseVB->Release();
	if(Elem->pSpecularVB != NULL)
		Elem->pSpecularVB->Release();
	if(Elem->pBlendIndicesVB != NULL)
		Elem->pBlendIndicesVB->Release();
	if(Elem->pVertexWeightsVB != NULL)
		Elem->pVertexWeightsVB->Release();
	if(Elem->pIndexBuffer != NULL)
		Elem->pIndexBuffer->Release();
	
	Elem->pVertexDeclaration->Release();

	for(uint i=0; i<Elem->TexNum; i++)
		Elem->pTextureVB[i]->Release();

	delete (Elem->ObjDesc);
	Elem->ObjDesc = NULL;

	return true;
}

//////////////////////////////////////////////////////////////////////////////////
// Creates Shader (Or Effect That Is Called In DirectX API) From File
uint  CD3DObj::CreateShaderFromFile(char *EffectFile)
{
	if(!EffectFile)
		return 0;

	LPD3DXBUFFER ErrorBuffer = NULL;//Buffer For Getting Error String
	D3DEffectObject EffectObj;//Our Shader Object
	EffectObj.pEffect = NULL;
	LPD3DXEFFECTPOOL EffectPool; // Effect Pool To Share parameters Between Effects
	D3DXCreateEffectPool(&EffectPool);
	//Creating Effect From File
	if(D3DXCreateEffectFromFile(m_pDeviceD3D,EffectFile,NULL,NULL, 0, EffectPool,
		&EffectObj.pEffect,&ErrorBuffer) != D3D_OK)
	{
		//Get Error String
		const char *err = NULL;
		if(ErrorBuffer)
			err = (const char*)ErrorBuffer->GetBufferPointer();
		else
			err = "Could Not Create Shader Effect";

		//Show MessageBox With Errors
		MessageBox(hWndRect,err,"Shader Effect Creation Error",MB_OK|MB_ICONERROR);
		return 0;
	}
	
	return EffectsArray.AddElement(EffectObj);
}

//////////////////////////////////////////////////////////////////////////////////
// Creates Shader (Or Effect That Is Called In DirectX API) From String
uint CD3DObj::CreateShader(char *strEffect)
{
	if(!strEffect)
		return 0;

	LPD3DXBUFFER ErrorBuffer = NULL;//Buffer For Getting Error String
	D3DEffectObject EffectObj;//Our Shader Object
	EffectObj.pEffect = NULL;
	//Creating Effect From String
	if(D3DXCreateEffect(m_pDeviceD3D, strEffect, UINT(strlen(strEffect)), NULL, NULL, 0, 
		NULL, &EffectObj.pEffect, &ErrorBuffer) != D3D_OK)
	{
		//Get Error String
		const char *err = NULL;
		if(ErrorBuffer)
			err = (const char*)ErrorBuffer->GetBufferPointer();
		else
			err = "Could Not Create Shader Effect";

		//Show MessageBox With Errors
		MessageBox(hWndRect,err,"Shader Effect Creation Error",MB_OK|MB_ICONERROR);
	}
	
	return EffectsArray.AddElement(EffectObj);
}

//////////////////////////////////////////////////////////////////////////////////
// Deleting Shader Object From Effects Array
bool CD3DObj::DeleteShader(uint ShaderObjIdx)
{
	D3DEffectObject *pEffectObj = EffectsArray.GetElement(ShaderObjIdx);
	if(!pEffectObj)
		return false;

	pEffectObj->pEffect->Release();
	pEffectObj->pEffect = NULL;

	return EffectsArray.DeleteElement(ShaderObjIdx);
}

//////////////////////////////////////////////////////////////////////////////////
// Setting Shader Effect Object Data
bool CD3DObj::SetEffect(LPD3DXEFFECT pEffect, D3DRenderElement *Elem, C3DObjectDesc *pObjDesc, 
						uint64 flags)
{
	if(!pEffect && !pObjDesc)
		return false;

	//Getting Shader Uniform Descriptor List
	CGPUProgramDesc *pShaderDesc = pObjDesc->GetShaderDesc();
	ShaderUniformDescriptor *pShaderUniDesc = pShaderDesc->GetShaderDescList();

	//Some Temp Variables
	D3DXMATRIX matrix;
	D3DXVECTOR4 vec4;
//	uint sampler;
//	uint TexIdx;
	//Pointers To Material, Light And Texture Managers
	CD3DTextureSystem *pD3DTexSys = (CD3DTextureSystem*)pTexMgr;

	//Setting Shader Uniform
	while(pShaderUniDesc!=NULL)
	{
		switch(pShaderUniDesc->UniformType)
		{
		case WORLD_VIEW_PROJECTION_MATRIX:
			matrix = Elem->ModelMatrix*mWorldMatrix*mViewMatrix*mProjMatrix;
			pEffect->SetMatrix(pShaderUniDesc->name,&matrix);
			break;
			
		case TEXTURE_UNIT:
			//sampler = ((uint*)pShaderUniDesc->param)[0];
			//TexIdx = pObjDesc->GetTextureID(sampler);
			//pD3DTexSys->SetTexture(pEffect,pShaderUniDesc->name,TexIdx,sampler);
			break;

		case USER_VECTOR4:
			pEffect->SetFloatArray(pShaderUniDesc->name,(float*)pShaderUniDesc->param,4);
			break;
		}
		pShaderUniDesc = pShaderUniDesc->next;
	}
	



	return true;
}