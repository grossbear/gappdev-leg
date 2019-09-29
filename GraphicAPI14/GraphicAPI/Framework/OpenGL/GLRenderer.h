///////////////////////////////////////////////////////////////////////////////////////////////
//	GLRenderer.h 
//	OpenGL Rendering core class
//	Created: January 2006
//	
////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _GL_OBJ_H_
#define _GL_OBJ_H_

//Pixel Format Setting For OpenGL Rendering
/*
	sizeof(PIXELFORMATDESCRIPTOR),				// Size Of This Pixel Format Descriptor
	1,											// Version Number
	PFD_DRAW_TO_WINDOW |						// Format Must Support Window
	PFD_SUPPORT_OPENGL |						// Format Must Support OpenGL
	PFD_DOUBLEBUFFER,							// Must Support Double Buffering
	PFD_TYPE_RGBA,								// Request An RGBA Format
	32,											// Select Our Color Depth
	0, 0, 0, 0, 0, 0,							// Color Bits Ignored
	0,											// Alpha Buffer Bits
	0,											// Shift Bit Ignored
	0,											// Accumulation Buffer Bits
	0, 0, 0, 0,									// Accumulation Bits Ignored
	24,											// Z-Buffer Bits (Depth Buffer)  
	8,											// Stencil Buffer Bits
	0,											// Auxiliary Buffer Bits
	PFD_MAIN_PLANE,								// Main Drawing Layer
	0,											// Reserved
	0, 0, 0	
*/

class CGLObj:public CGraphicAPI
{
public:
	CGLObj();
	~CGLObj();

	bool Create(HWND hWnd);
	bool Init();
	void Render();
	void Resize(int width, int height);

	void SetView(const CCamera & cam);
	void SetView();
	void SetViewport(int width,int height);
	void SetOrthoProj(float l,float r,float b,float t,float zn,float zf);
	void SetOrthoProj(float left,float right,float bottom,float top);
	Vector3D PickScreen(float MouseX,float MouseY);

	// Buffers Clearing Functions
	void Clear();
	void Clear(bool Color,bool Depth,bool Stencil);

	void SetFillMode(uint fillmode);
	void SetCullMode(bool CullFace,bool CCW);
	void SetBgColor(ubyte R,ubyte G,ubyte B);
	void SetShadingMode(bool Smooth);
	void LightingEnable(bool flag);
	void SetNormalsNormalization(bool Normalize);

	void SwapImageBuffers(){SwapBuffers(hDC);}

	void AlphaBlendEnable(bool flag);
	void SetBlendingMode(BLENDOP Source, BLENDOP Destination);
	void AlphaTestEnable(bool flag);
	void AlphaFunc(FUNC func,float ref);

	void DepthTestEnable(bool flag);
	void DepthWriteEnable(bool flag);
	void DepthFunc(FUNC func);

	void ColorsWriteEnable(bool R,bool G,bool B,bool A);

	void StencilWriteMask(uint mask);
	void StencilTestEnable(bool flag);
	void StencilFunc(FUNC func,uint ref,uint mask);
	void StencilOp(OP fail, OP zfail, OP zpass);
	void TwoSideStencilEnable(bool flag);
	void SetStencilForBackFace(FUNC func,uint ref,uint mask,OP fail,OP zfail,OP zpass);
	void SetStencilForFrontFace(FUNC func,uint ref,uint mask,OP fail,OP zfail,OP zpass);

	void ClipPlaneEnable(uint Index, bool flag);
	bool SetClipPlane(uint Index, Vector4D &PlaneEq);

	void ScissorTestEnable(bool flag); 
	void SetScissorRect(uint x, uint y, uint w, uint h);

	void SetPolygonOffset(bool enable, float bias, float slopeBias);

	// Setting New World Transformation Matrix
	void SetWorldMatrix(Matrix4x4 &M){(Matrix4x4()==M)?(mWorldMat.Identity()):(mWorldMat=M);}
	// Reseting To Identity World Transformation Matrix
	void ResetWorldMatrix(){mWorldMat.Identity();}

	//Object Manipulating Methods
	uint Add3DObjToBuffer(C3DObject* pObj3D, C3DObjectDesc* ObjDesc, bool dynamic=true);
	bool Delete3DObjFromBuffer(uint ObjIdx);
	bool UpdateBufferFrom3DObj(uint ObjIdx,C3DObject *pObj3D,uint64 flags=0);
	bool UpdateIndexBufferFrom3DObj(uint ObjIdx,C3DObject* pObj3D);
	
	bool SetRenderElementModelMatrix(uint ObjIdx, Matrix4x4& M);
	bool SetRenderElement3DObjDesc(uint ObjIdx, C3DObjectDesc* ObjDesc);

	// Saving Screen Image
	void GrabScreenShot(char* file_name=NULL);

	//Render One 3DObject 
	//bool Render(uint ObjIdx,uint64 flags=0);
	//Render One 3DObject With Another 3DObject Description
	//bool Render(uint ObjIdx, uint64 flags=0, C3DObjectDesc *pObjDesc=NULL);
	//Render Many 3DObjects From Index Array
	//bool Render(uint ObjNum, uint *ObjIdx,uint64 flags=0);
	//Redner Many 3DObjects From Index Array With Specified Descriptions
	bool Render(uint ObjNum, uint *ObjIdx,uint64 flags=0, C3DObjectDesc **ppObjDesc=NULL);

	uint CreateShaderFromFiles(char *VertexShader, char *FragmentShader, 
		bool IsHighLevelShader=true);
	uint CreateShader(char *strVertexShader, char *strFragmentShader,
		bool IsHighLevelShader=true);
	bool DeleteShader(uint ShaderObjIdx);
protected:
	bool SetBuffersData(GLARBRenderElement *Elem,uint64 flags,C3DObjectDesc *pObjDesc);
	bool SetShaderObject(uint ShaderIdx, CGPUProgramDesc *pShaderDesc);
	void TurnOffShader();

	bool ReleaseRenderElement(GLARBRenderElement *RenderElement);
	bool ReleaseShaderElement(sGLShaderObject *ShaderElement);

	//pointer to the class method
	void (CGLObj::*SetTwoSideStencil)(bool front,FUNC ,uint ,uint ,OP ,OP ,OP );
	//different functions for setting two side stencil
	void SetTwoSideStencilNULL(bool front,FUNC func,uint ref,uint mask,OP fail,OP zfail,OP zpass){}
	void SetTwoSideStencilEXT(bool front,FUNC func,uint ref,uint mask,OP fail,OP zfail,OP zpass);
	void SetTwoSideStencilATI(bool front,FUNC func,uint ref,uint mask,OP fail,OP zfail,OP zpass);

	HWND	hWndRect;
	HDC		hDC;
	HGLRC	hRC;

	//Virtual Camera Parameters
	Vector3D mPos;//Position of the Camera
	Vector3D mView;//Targent Point of the Camera (Were Camera is Looking)
	Vector3D mUp;//Up Vector

	float mFOV;//Lens Field of View
	float mNear;//Near Clip Plane
	float mFar;//Far Clip Plane

	Matrix4x4 mWorldMat; //World Transformation Matrix
	
	int Width,Height;//Screen Resolution

	sGLDevCaps mDevCaps;
	sGLExtensions mGLExt;
	TArray <GLARBRenderElement> RenderElementArray;
	//TArray <C3DObjectDesc> ObjectDescArray;
	TArray <sGLShaderObject> ShaderObjectsArray;
};

#endif  //_GL_OBJ_H_