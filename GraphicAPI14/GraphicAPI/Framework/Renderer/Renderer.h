///////////////////////////////////////////////////////////////////////////////////////////////
//	Renderer.h 
//	Class That Creates Interface For Rendering System 
//	Created: 2005
//	
////////////////////////////////////////////////////////////////////////////////////////////////

#include <windows.h>

#include "DevCaps.h"
#include "LightMgr.h"
#include "MaterialMgr.h"
#include "TextureMgr.h"
#include "3DObjDesc.h"


enum FUNCTION {
	NEVER    = 0,
	LESS	 = 1,
	EQUAL	 = 2,
	LEQUAL	 = 3,
	GREATER	 = 4,
	NOTEQUAL = 5,
	GEQUAL	 = 6,
	ALWAYS	 = 7,
};

enum OPERATION {
	KEEP	= 0,
	ZERO	= 1,
	REPLACE	= 2,
	INCRSAT	= 3,
	DECRSAT	= 4,
	INVERT	= 5,
	INCR	= 6,
	DECR	= 7,
};

enum BLENDOPERATION {
	BLENDOP_ZERO				= 0,
	BLENDOP_ONE					= 1,				
	BLENDOP_DST_COLOR			= 2, //For Source-Blending Factor Only
	BLENDOP_ONE_MINUS_DST_COLOR	= 3, //For Source-Blending Factor Only
	BLENDOP_SRC_ALPHA			= 4,
	BLENDOP_ONE_MINUS_SRC_ALPHA	= 5,
	BLENDOP_DST_ALPHA			= 6,
	BLENDOP_ONE_MINUS_DST_ALPHA = 7,
	BLENDOP_SRC_ALPHA_SATURATE	= 8,
	BLENDOP_SRC_COLOR			= 9, //For Destination-Blending  Factor Only
	BLENDOP_ONE_MINUS_SRC_COLOR	= 10,//For Destination-Blending  Factor Only
};

typedef OPERATION OP;
typedef FUNCTION FUNC;
typedef BLENDOPERATION BLENDOP;


class CGraphicAPI
{
public:
	CGraphicAPI():pDevCaps(NULL),pLightMgr(NULL),pMatMgr(NULL),pTexMgr(NULL){}
	
	virtual ~CGraphicAPI(){delete(pDevCaps);delete(pTexMgr);delete(pLightMgr);delete(pMatMgr);}

	virtual bool Create(HWND hWnd){return false;}
	virtual bool Init(){return false;}
	virtual void Render(){}
	virtual void Resize(int Width, int Height){}

	virtual void SetView(const CCamera & cam){}
	virtual void SetView(){}
	virtual void SetViewport(int width,int height){}
	virtual void SetOrthoProj(float l, float r, float b, float t, float zn, float zf){}
	virtual void SetOrthoProj(float left, float right, float bottom, float top){}
	virtual Vector3D PickScreen(float MouseX,float MouseY){return Vector3D(0,0,0);}

	virtual void Clear(){}
	virtual void Clear(bool Color,bool Depth,bool Stencil){}

	virtual void SetFillMode(uint fillmode){}
	virtual void SetCullMode(bool CullFace,bool CCW){}
	virtual void SetBgColor(ubyte R,ubyte G,ubyte B){}
	virtual void SetShadingMode(bool Smooth){}
	virtual void LightingEnable(bool flag){}
	virtual void SetNormalsNormalization(bool Normalize){}

	virtual void SwapImageBuffers(){}

	virtual void AlphaBlendEnable(bool flag){}
	virtual void SetBlendingMode(BLENDOP Source, BLENDOP Destination){}
	virtual void AlphaTestEnable(bool flag){}
	virtual void AlphaFunc(FUNC func,float ref){}

	virtual void DepthTestEnable(bool flag){}
	virtual void DepthWriteEnable(bool flag){}
	virtual void DepthFunc(FUNC func){}

	virtual void ColorsWriteEnable(bool R,bool G,bool B,bool A){}

	virtual void StencilWriteMask(uint mask){}
	virtual void StencilTestEnable(bool flag){}
	virtual void StencilFunc(FUNC func,uint ref,uint mask){}
	virtual void StencilOp(OP fail, OP zfail, OP zpass){}

	virtual void TwoSideStencilEnable(bool flag){}
	virtual void SetStencilForBackFace(FUNC func,uint ref,uint mask,OP fail,OP zfail,OP zpass){}
	virtual void SetStencilForFrontFace(FUNC func,uint ref,uint mask,OP fail,OP zfail,OP zpass){}

	virtual void ClipPlaneEnable(uint Index, bool flag){}
	virtual bool SetClipPlane(uint Index, Vector4D &PlaneEq){return false;}

	virtual void ScissorTestEnable(bool flag){}
	virtual void SetScissorRect(uint x, uint y, uint w, uint h){}

	virtual void SetWorldMatrix(Matrix4x4 &M){}
	virtual void ResetWorldMatrix(){}

	virtual void GrabScreenShot(char* file_name=NULL){}

	virtual void SetPolygonOffset(bool enable, float bias, float slopeBias){}

	//Setting Vertex Buffers Functions
	virtual uint Add3DObjToBuffer(C3DObject* pObj3D, C3DObjectDesc* ObjDesc, bool dynamic=true){return 0;}
	virtual bool Delete3DObjFromBuffer(uint ObjIdx){return false;}
	virtual bool UpdateBufferFrom3DObj(uint ObjIdx,C3DObject* pObj3D, uint64 flags=0){return false;}
	virtual bool UpdateIndexBufferFrom3DObj(uint ObjIdx,C3DObject* pObj3D){return false;}
	
	virtual bool DrawNormals(uint ObjIdx,bool dynamic=true){return false;}
	virtual bool SetRenderElementModelMatrix(uint ObjIdx, Matrix4x4& M){return false;}
	virtual bool SetRenderElement3DObjDesc(uint ObjIdx, C3DObjectDesc *pObjDesc){return false;}

	//Rendering Methods

	//Render One 3DObject 
	///virtual bool Render(uint ObjIdx,uint64 flags=0){return false;}
	//Render One 3DObject With Another 3DObject Description
	///virtual bool Render(uint ObjIdx, uint64 flags=0, C3DObjectDesc *pObjDesc=NULL){return false;}
	//Render Many 3DObjects From Index Array
	///virtual bool Render(uint ObjNum, uint *ObjIdx,uint64 flags=0){return false;}
	//Redner Many 3DObjects From Index Array With Specified Descriptions
	virtual bool Render(uint ObjNum, uint *ObjIdx,uint64 flags=0, 
		C3DObjectDesc **ppObjDesc=NULL){return false;}

	//Shader System 

	//For OpenGL Shading Language. Often Shader Is Created From Two Files
	//Or For OpenGL And DirectX Low Level Shaders. Low Level Shaders I'll Maybe
	//Implement Later. But From The Recent News Microsoft DirectX 10 Will Not
	//Support Assembler Shaders. (29th August 2006)
	virtual uint CreateShaderFromFiles(char *VertexShader, char *FragmentShader, 
		bool IsHighLevelShader=true){return 0;}
	virtual uint CreateShader(char *strVertexShader, char *strFragmentShader,
		bool IsHighLevelShader=true){return 0;}

	// For DirectX 9 High Level Shaders. Shader Is Situated In a Single File
	virtual uint CreateShaderFromFile(char *EffectFile){return 0;}
	virtual uint CreateShader(char *strEffect){return 0;}
	virtual bool SetShaderEntryPoint(uint ShaderObjIdx, const char *EntryPointName){return false;}

	virtual bool DeleteShader(uint ShaderObjIdx){return false;}

	//This Methods Will Be Implemented Later
	//Enables Direct Rendering Without Creating Render Elements Array
	virtual bool RenderUp(C3DObject *pObj3D, uint64 flags=0, C3DObjectDesc *pObjDesc=NULL)
				{return false;}
	virtual bool RenderUp(C3DObject *pObj3D, uint ObjNum, uint64 flags=0, 
		C3DObjectDesc **ppObjDesc=NULL){return false;}
	virtual bool RenderUpSameType(C3DObject *pObj3D, uint ObjNum, uint64 flags=0, 
		C3DObjectDesc **ppObjDesc=NULL){return false;}


	IDeviceCaps *pDevCaps;
	ILightSystem *pLightMgr;
	IMaterialSystem *pMatMgr;
	ITextureSystem *pTexMgr;
protected:
	
};