////////////////////////////////////////////////////////////////////////////////////////////////
//	GLTextureMgr.h 
//	OpenGL Texture Managment System Class
//	Created: 15th August 2006
//	
////////////////////////////////////////////////////////////////////////////////////////////////

typedef struct {
	GLuint TexID;
	GLenum target;
	GLenum format;
	GLenum datatype;
	uint Width;
	uint Height;
	uint Depth;
	GLenum MagFilter;
	GLenum MinFilter;
	uint Anisotropy;
	bool genMipMaps;

	GLuint FBO; // Framebuffer Object, For Rendering To Texture
	GLuint depthBuffer; // depth Buffer
	GLuint stencilBuffer; // Stencil Buffer
}sGLTexture;

class CGLTextureSystem:public ITextureSystem
{
public:
	CGLTextureSystem(HWND hWnd, IDeviceCaps *pGLDevCaps, GLExtensions *pExt);
	~CGLTextureSystem();

	uint CreateTextureFromFile(bool genMipMaps=true);
	uint CreateTextureFromFile(char *file_name, bool genMipMaps=true);
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
	void SetTextureAddress(TEXADDR S, TEXADDR T, TEXADDR R = TEXADDR_NONE);
	void DisableTextures();

	bool Is1DMap(uint texind);
	bool Is2DMap(uint texind);
	bool Is3DMap(uint texind);
	bool IsCubeMap(uint texind);

protected:
	bool SetTextureFromImage(CImage* Img, sGLTexture &TexStruct, bool genMipMaps=true);
	bool SetTextureDim(CImage *Img, sGLTexture &TexStruct);
	bool SetTextureData(CImage *Img, sGLTexture &TexStruct,bool genMipMaps);

	CBMIFile *pBMIFile;
	TArray <sGLTexture> TextureArray;
	sGLExtensions* m_pExt;
	GLenum *pTexTargets; // For Turning Off Texture Units
	GLenum ActiveTarget; // For Setting Texture Wrap 
	GLint viewport[4];

	//Active Render Target Properties
	GLenum ActiveRenderTarget;
	GLint ActiveRenderTexture;
	bool genRTargetMipMaps;

	uint MaxAnisotropy;
	uint MaxTexUnits;
	uint MaxTexSize;
	uint MaxVolTexSize;
	uint MaxCubeMapSize;

	HWND hWnd;
};