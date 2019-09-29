////////////////////////////////////////////////////////////////////////////////////////////////
//	TextureMgr.h
//	Class That Creates Interface For Textures Managment System
//	Created: 9th August 2006
//
////////////////////////////////////////////////////////////////////////////////////////////////
#include "../Image/Image.h"



enum TEXTUREFILTER {
	TEXF_NONE			=	0,
	TEXF_NEAREST		=	1,
	TEXF_LINEAR			=	2,
	//TEXF_ANISOTROPIC	=	2,
};

enum TEXTUREADDRESS {
	TEXADDR_NONE		=	0,
	TEXADDR_REPEAT		=	1,
	TEXADDR_CLAMP		=	2,
	TEXADDR_BORDER		=	3,
	TEXADDR_MIRROR		=	4,
	TEXADDR_MIRRORONCE	=	5,
};

enum TEXTURETYPE {
	TEXT_1D,
	TEXT_2D,
	TEXT_3D,
	TEXT_CUBE,
};

enum RENDERSURFACEFORMAT {
	RSURFMT_RGBA8,
	RSURFMT_RGBA16,
	RSURFMT_DEPTH16,
	RSURFMT_DEPTH24,
	RSURFMT_DEPTH32,
};

enum CUBEMAPFACE {
	FACE_POSITIVE_X = 0,
	FACE_NEGATIVE_X = 1,
	FACE_POSITIVE_Y = 2,
	FACE_NEGATIVE_Y = 3,
	FACE_POSITIVE_Z = 4,
	FACE_NEGATIVE_Z = 5,
};

typedef TEXTUREFILTER TEXF;
typedef TEXTUREADDRESS TEXADDR;
typedef TEXTURETYPE TEXTYPE;
typedef RENDERSURFACEFORMAT RSURFMT;
typedef CUBEMAPFACE CUBEFACE;

class ITextureSystem
{
public:
	ITextureSystem(){}
	virtual ~ITextureSystem(){}

	virtual uint CreateTextureFromFile(bool genMipMaps=true){return 0;}
	virtual uint CreateTextureFromFile(char *file_name, bool genMipMaps=true){return 0;}
	virtual uint CreateCubeMapTextureFromFile(char *file_name, bool genMipMaps=true){return 0;}
	virtual uint CreateTexture(CImage *Img, bool genMipMaps=true){return 0;}
	virtual uint CreateCubeMapTexFromFiles(char *PosX,char *NegX,char *PosY,char *NegY,
		char *PosZ, char *NegZ, bool genMipMaps=true){return 0;}

	virtual uint CreateRenderTarget(uint Width, uint Height, RSURFMT SurfFmt,TEXTYPE TexType,
		bool genMipMaps=true){return 0;}
	virtual bool BindRenderTarget(uint texid, CUBEFACE face = FACE_POSITIVE_X){return false;}
	virtual void UnBindRenderTarget(){}
	virtual void SaveViewport(){}

	virtual bool DeleteTexture(uint texid){return false;}

	virtual bool UpdateTexture(uint texid, CImage *Img){return false;}

	virtual bool SetTexFilter(uint texind,TEXF MagFilter,TEXF MinFilter, TEXF MipFilter){return false;}
	virtual bool SetAnisotropicFilter(uint texind, uint Anisotropy){return false;}
	virtual bool SetTexture(uint texind, uint TexUnit){return false;}
	virtual void SetTextureAddress(TEXADDR S, TEXADDR T, TEXADDR R = TEXADDR_NONE){}
	virtual void DisableTextures(){}

	
	virtual bool Is1DMap(uint texind){return false;}
	virtual bool Is2DMap(uint texind){return false;}
	virtual bool Is3DMap(uint texind){return false;}
	virtual bool IsCubeMap(uint texind){return false;}

protected:
};