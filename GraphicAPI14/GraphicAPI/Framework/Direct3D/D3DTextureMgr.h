////////////////////////////////////////////////////////////////////////////////////////////////
//	D3DTextureMgr.h 
//	Direct3D Texture Managment System Class
//	Created: 23d August 2006
//	
////////////////////////////////////////////////////////////////////////////////////////////////

typedef struct {
	LPDIRECT3DTEXTURE9 pTex;
	LPDIRECT3DCUBETEXTURE9 pCubeTex;
	LPDIRECT3DVOLUMETEXTURE9 pVolTex;
	D3DTEXTUREFILTERTYPE MagFilter;
	D3DTEXTUREFILTERTYPE MinFilter;
	D3DTEXTUREFILTERTYPE MipFilter;
	LPDIRECT3DSURFACE9 *RenderTargetSurface;
	LPDIRECT3DSURFACE9 *DepthStencilSurface;
	DWORD Anisotropy;
	bool genMipMaps;
	bool is1D;
	bool CreatedFromFile;
	
}sD3DTexture;

class CD3DTextureSystem:public ITextureSystem
{
public:
	CD3DTextureSystem(HWND hWnd, LPDIRECT3DDEVICE9 pDeviceD3D, D3DCAPS9 *pDevCaps);
	~CD3DTextureSystem();

	uint CreateTextureFromFile(bool genMipMaps=true);
	uint CreateTextureFromFile(char *file_name, bool genMipMaps=true);
	uint CreateCubeMapTextureFromFile(char *file_name, bool genMipMaps=true);
	uint CreateTexture(CImage *Img, bool genMipMaps=true);
	uint CreateCubeMapTexFromFiles(char *PosX, char *NegX,char *PosY,char *NegY,
		char *PosZ, char *NegZ, bool genMipMaps=true);

	uint CreateRenderTarget(uint Width, uint Height, RSURFMT SurfFmt,TEXTYPE TexType,
		bool genMipMaps=true);
	bool BindRenderTarget(uint texid, CUBEFACE face = FACE_POSITIVE_X);
	void UnBindRenderTarget();
	bool UpdateTexture(uint texid, CImage *Img);
	bool DeleteTexture(uint texid);

	bool SetTexFilter(uint texind,TEXF MagFilter,TEXF MinFilter, TEXF MipFilter);
	bool SetAnisotropicFilter(uint texind, uint Anisotropy);
	bool SetTexture(uint texind, uint TexUnit);
	void SetTextureAddress(TEXADDR U, TEXADDR V, TEXADDR W = TEXADDR_NONE);
	void DisableTextures();

	bool SetTexture(LPD3DXEFFECT pEffect,const char* name, uint TexIdx, uint sampler);
	LPD3DXEFFECTSTATEMANAGER GetFXStateManager(){m_pFXStateMgr;}

	bool Is1DMap(uint texind);
	bool Is2DMap(uint texind);
	bool Is3DMap(uint texind);
	bool IsCubeMap(uint texind);

protected:
	bool SetTextureData(CImage *Img, sD3DTexture &TexStruct);
	bool FillLockedRect(void *data, uint w, uint h, D3DLOCKED_RECT *LockedRect, 
		D3DFORMAT format);
	bool FillLockedBox(void *data, uint w, uint h, uint d, D3DLOCKED_BOX *LockedBox, 
		D3DFORMAT format);

	HWND m_hWnd;
	LPDIRECT3DDEVICE9 m_pDevD3D; 
	D3DCAPS9 *m_pDevCaps;
	bool *pTexSamplers; // For Turning Off Texture Units
	DWORD ActiveSampler; // For Setting Texture Addressing Types
	D3DVIEWPORT9 Viewport;
	D3DXMATRIX mTextureMatrix;
	LPD3DXEFFECTSTATEMANAGER m_pFXStateMgr;
	
	LPDIRECT3DSURFACE9 m_pColorBuffer;
	LPDIRECT3DSURFACE9 m_pDepthStencilBuffer;

	TArray <sD3DTexture> TextureArray;
};

