////////////////////////////////////////////////////////////////////////////////////////////////
//	D3DRenderer.h 
//	Direct3D Rendering core class
//	Created: 15th February 2006
//	
////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _D3D_OBJ_H_
#define _D3D_OBJ_H_

#include <d3d9.h>
#include <d3dx9.h>
#include <d3dx9math.h>

#pragma comment (lib,"d3d9.lib")
#pragma comment (lib,"d3dx9.lib")

#include "D3DRenderElement.h"

typedef struct tagD3DEffectObject
{
	LPD3DXEFFECT pEffect;

}D3DEffectObject;

/////////////////////////////////////////////////////////////////////
//Class CD3DObj derives from abstract class CGraphicAPI
class CD3DObj:public CGraphicAPI
{
public:
	CD3DObj();
	~CD3DObj();

	bool Create(HWND hWnd);
	bool Init();
	void Render();
	void Resize(int width, int height);
	void SetView(const CCamera &cam);
	void SetView();
	void SetViewport(int width,int height);
	void SetOrthoProj(float l,float r,float b,float t,float zn,float zf);
	void SetOrthoProj(float left,float right,float bottom,float top);
	Vector3D PickScreen(float MouseX,float MouseY);

	void Clear();
	void Clear(bool Color,bool Depth,bool Stencil);

	//Swapping Buffers
	void SwapImageBuffers(){m_pDeviceD3D->Present(NULL,NULL,NULL,NULL);}

	void SetFillMode(uint fillmode);
	void SetCullMode(bool CullFace,bool CCW);
	void SetBgColor(ubyte R,ubyte G,ubyte B);
	void SetShadingMode(bool Smooth);
	void LightingEnable(bool flag);
	void SetNormalsNormalization(bool Normalize);

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
	void SetScissorRect(uint x,uint y, uint w, uint h);

	void SetPolygonOffset(bool enable, float bias, float slopeBias);

	void SetWorldMatrix(Matrix4x4 &M);
	// Reseting To Identity World Transformation Matrix
	void ResetWorldMatrix(){D3DXMatrixIdentity(&mWorldMatrix);}

	uint Add3DObjToBuffer(C3DObject* pObj3D, C3DObjectDesc* ObjDesc, bool dynamic=true);
	bool Delete3DObjFromBuffer(uint ObjIdx);
	bool UpdateBufferFrom3DObj(uint ObjIdx, C3DObject *pObj3D, uint64 flags=0);
	bool UpdateIndexBufferFrom3DObj(uint ObjIdx,C3DObject* pObj3D);
	//bool Render(uint ObjIdx,uint64 flags=0);
	bool SetRenderElementModelMatrix(uint ObjIdx, Matrix4x4& M);
	bool SetRenderElement3DObjDesc(uint ObjIdx, C3DObjectDesc* ObjDesc);

	// Saving Screen Image
	void GrabScreenShot(char* file_name=NULL);
/*
	//Render One 3DObject 
	bool Render(uint ObjIdx,uint64 flags=0);
	//Render One 3DObject With Another 3DObject Description
	bool Render(uint ObjIdx, uint64 flags=0, C3DObjectDesc *pObjDesc=NULL);
	//Render Many 3DObjects From Index Array
	bool Render(uint ObjNum, uint *ObjIdx,uint64 flags=0);*/
	//Redner Many 3DObjects From Index Array With Specified Descriptions
	bool Render(uint ObjNum, uint *ObjIdx,uint64 flags=0, C3DObjectDesc **ppObjDesc=NULL);

	uint CreateShaderFromFile(char *EffectFile);
	uint CreateShader(char *strEffect);
	bool DeleteShader(uint ShaderObjIdx);
protected:
	bool SetBuffersData(tagD3DRenderElement *Elem,uint64 flags,C3DObjectDesc *pObjDesc);
	bool CopyDataToBuffer(LPDIRECT3DVERTEXBUFFER9 pVB, VOID *data, ULONG size);
	bool CopyDataToIndexBuffer(LPDIRECT3DINDEXBUFFER9 pIB, VOID *data, ULONG size);
	bool ReleaseRenderElement(D3DRenderElement *RenderElement);
	void RenderEffect(D3DRenderElement *Elem, C3DObjectDesc *pObjDesc,uint64 flags);
	bool SetEffect(LPD3DXEFFECT pEffect, D3DRenderElement *Elem, C3DObjectDesc *pObjDesc, uint64 flags);

	HWND	hWndRect; //Window Handler

	LPDIRECT3D9 m_pD3D; 
	LPDIRECT3DDEVICE9 m_pDeviceD3D; 

	D3DCAPS9 mDevCaps;

	D3DCOLOR background;
	
	//DirectGraphics Operates On Three Independent Matricies
	D3DXMATRIX mWorldMatrix; //Transform Model To World Space
	D3DXMATRIX mViewMatrix; //Transform Model To Camera Space
	D3DXMATRIX mProjMatrix; //Projects Model On Screen

	//Virtual Camera Params
	D3DXVECTOR3 mPosVec;
	D3DXVECTOR3 mViewVec;
	D3DXVECTOR3 mUpVec;

	float mFOV;
	float mNear;
	float mFar;

	//Screen Resolution
	int Width, Height;

	//Render Objects Array
	TArray <tagD3DRenderElement> RenderElementArray;
	TArray <tagD3DEffectObject> EffectsArray;
};

#endif //_D3D_OBJ_H_