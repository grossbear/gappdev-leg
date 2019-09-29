////////////////////////////////////////////////////////////////////////////////////////////////
//	GLTextureMgr.cpp 
//	OpenGL Texture Managment System Class
//	Created: 15th August 2006
//	
////////////////////////////////////////////////////////////////////////////////////////////////
#include <windows.h>
#include <memory.h>
#include <stdio.h>

#include "../Types/types.h"
#include "../Templates/Array.h"
#include "../Math/MathLib.h"
#include "../Camera/Camera.h"
#include "../3DObjects/3DObject.h"

#include "../Image/Image.h"
#include "../Image/ImageIO.h"
#include "../Image/ImgTypes.h"

#include "../Renderer/Renderer.h"

#include <gl/gl.h>
#include <gl/glu.h>

#include "Extensions/glext.h"
#include "Extensions/wglext.h"
#include "GLExtStruct.h"
#include "Extensions/GLExtensions.h"
#include "GLConvFunc.h"
#include "GLDevCaps.h"
#include "GLTextureMgr.h"

//////////////////////////////////////////////////////////////////////////////////
// Constructor
CGLTextureSystem::CGLTextureSystem(HWND hWnd,IDeviceCaps *pGLDevCaps,sGLExtensions *pExt)
{
	m_pExt = pExt;
	this->hWnd = hWnd;
	pBMIFile = new CBMIFile();

	MaxAnisotropy = pGLDevCaps->MaxAnisotropy();
	MaxTexUnits = pGLDevCaps->MaxTexUnits();
	MaxTexSize = pGLDevCaps->MaxTexSize();
	MaxVolTexSize = pGLDevCaps->MaxVolumeTexSize();
	MaxCubeMapSize = pGLDevCaps->MaxCubeMapSize();

	if(!MaxTexUnits)
		MaxTexUnits = 1;

	pTexTargets = new GLenum[MaxTexUnits];
	memset(pTexTargets,0,sizeof(GLenum)*MaxTexUnits);

	ActiveRenderTarget = 0;
	ActiveRenderTexture = 0;
	genRTargetMipMaps = false;
}

/////////////////////////////////////////////////////////////////////////////////
// Destructor
CGLTextureSystem::~CGLTextureSystem()
{
	m_pExt = NULL;
	hWnd = NULL;
	delete [] pTexTargets;

	sGLTexture *TexElem = NULL;
	TextureArray.SetPointerToStart();

	while(true)
	{
		TexElem = TextureArray.GetElementAndMovePointer();

		if(TexElem!=NULL)
		{
			if(TexElem->FBO)
				glDeleteFramebuffersEXT(1,&TexElem->FBO);
			if(TexElem->depthBuffer)
				glDeleteRenderbuffersEXT(1,&TexElem->depthBuffer);

			glEnable(TexElem->target);
			glBindTexture(TexElem->target, TexElem->TexID);
			glDeleteTextures(1,&TexElem->TexID);
			glDisable(TexElem->target);
		}
		else
			break;
	}
}

/////////////////////////////////////////////////////////////////////////////////
// Creating Texture Using Windows GetOpenFileName Function
uint CGLTextureSystem::CreateTextureFromFile(bool genMipMaps)
{
	sGLTexture TexElem;
	memset(&TexElem,0,sizeof(sGLTexture));

	CImage *Img = pBMIFile->Load(hWnd);
	if(Img==NULL)
		return 0;

	if(!SetTextureFromImage(Img,TexElem,genMipMaps))
	{
		delete (Img);
		return false;
	}

	delete (Img);

	return TextureArray.AddElement(TexElem);
}

//////////////////////////////////////////////////////////////////////////////////
// Creating Texture From File Name String
uint CGLTextureSystem::CreateTextureFromFile(char *file_name,bool genMipMaps)
{
	sGLTexture TexElem;
	memset(&TexElem,0,sizeof(sGLTexture));

	CImage *Img = pBMIFile->LoadBitmapImage(file_name);
	if(Img==NULL)
		return 0;
	

	if(!SetTextureFromImage(Img,TexElem,genMipMaps))
	{
		delete (Img);
		return false;
	}

	delete (Img);

	return TextureArray.AddElement(TexElem);
}

//////////////////////////////////////////////////////////////////////////////////
// Creating Texture From Image
uint CGLTextureSystem::CreateTexture(CImage *Img, bool genMipMaps)
{
	if(Img==NULL)
		return 0;

	sGLTexture TexElem;
	memset(&TexElem,0,sizeof(sGLTexture));

	if(!SetTextureFromImage(Img,TexElem,genMipMaps))
		return 0;

	return TextureArray.AddElement(TexElem);
}

//////////////////////////////////////////////////////////////////////////////////
// Creating Cube Map Texture From 6 Image Files
uint CGLTextureSystem::CreateCubeMapTexFromFiles(char *PosX, char *NegX,char *PosY,char *NegY,
		char *PosZ, char *NegZ, bool genMipMaps)
{
	if((PosX==NULL) || (NegX==NULL) || (PosY==NULL) || (NegY==NULL) || (PosZ==NULL) || (NegZ==NULL))
		return 0;

	CImage **Images = new CImage*[6];
	//Images[0]=Images[1]=Images[2]=Images[3]=Images[4]=Images[5]=NULL;
	Images[0] = pBMIFile->LoadBitmapImage(PosX);
	Images[1] = pBMIFile->LoadBitmapImage(NegX);	
	Images[2] = pBMIFile->LoadBitmapImage(PosY);
	Images[3] = pBMIFile->LoadBitmapImage(NegY);
	Images[4] = pBMIFile->LoadBitmapImage(PosZ);
	Images[5] = pBMIFile->LoadBitmapImage(NegZ);

	bool Failed = false;

	for(uint i=0; i<6; i++)
	{
		if(Images[i]==NULL)
			Failed = true;
	}

	if(Failed)
	{
		for(uint i=0; i<6; i++)
				delete (Images[i]);
		
		delete [] Images;
		return 0;
	}

	for(i=0; i<6; i++)
	{
		if(isPalettedFormat(Images[i]->getFormat()))
			Images[i]->ExpandPalette();
	}

	if(!Images[0]->is2D())
		return 0;

	int width = Images[0]->getWidth();
	int height = Images[0]->getHeight();
	FORMAT fmt = Images[0]->getFormat();

	for(i=1; i<6; i++)
	{
		if((width != Images[i]->getWidth()) || (height != Images[i]->getHeight()) 
			|| (fmt != Images[i]->getFormat()))
		{ Failed = true; }
	}

	if(Failed)
	{
		for(uint i=0; i<6; i++)
				delete (Images[i]);
		
		delete [] Images;
		return 0;
	}

	//Putting It 6 Images Into One
	CImage *Img = new CImage();
	Img->Create(fmt,width,height,1,6);

	ubyte *data;
	ubyte *pixels;
	for(i=0; i<6; i++)
	{
		data = (ubyte*)Images[i]->getPixels();
		pixels = (ubyte*)Img->getPixels(i+1);

		memcpy(pixels,data,getBytesPerPixel(fmt)*width*height);
	}

	for(uint i=0; i<6; i++)
		delete (Images[i]);
		
	delete [] Images;

	sGLTexture sTex;

	if(!SetTextureFromImage(Img,sTex,genMipMaps))
	{
		delete (Img);
		return 0;
	}

	delete (Img);

	return TextureArray.AddElement(sTex);
}

//////////////////////////////////////////////////////////////////////////////////
// Create Render Target Texture
uint CGLTextureSystem::CreateRenderTarget(uint Width, uint Height, RSURFMT SurfFmt,
										  TEXTYPE TexType, bool genMipMaps)
{
	if(Width<2)	Width = 2;
	if(Height<2) Height = 2;

	if(!IsPowerOf2(Width))
		Width = GetLowestPowerOf2(Width);

	if(!IsPowerOf2(Height))
		Height = GetLowestPowerOf2(Height);

	sGLTexture sTex;
	memset(&sTex,0,sizeof(sGLTexture));

	//glEnable(sTex.target);
	glGenTextures(1, &sTex.TexID);
	if(!sTex.TexID)
		return 0;
	if(TexType == TEXT_CUBE)
		sTex.target = GL_TEXTURE_CUBE_MAP;
	else
		sTex.target = GL_TEXTURE_2D;
	sTex.Width = Width;
	sTex.Height = Height;
	sTex.format = GetGLFormatFromRSurfFmt(SurfFmt);
	sTex.datatype = GetGLDataTypeFromRSurfFmt(SurfFmt);
	sTex.genMipMaps = genMipMaps;
	sTex.MinFilter = GL_NEAREST;
	sTex.MagFilter = GL_LINEAR;

	glBindTexture(sTex.target, sTex.TexID);
	// Creating Buffer And Initialize All MipMap Levels 
	if(sTex.target == GL_TEXTURE_CUBE_MAP)
	{	
		uint size = 1;
		if(genMipMaps)
			size = min(sTex.Width, sTex.Height);
		
		for(uint mipsize = size, level=0; mipsize>0; mipsize>>=1, level++)
			for(uint i=0; i<6; i++)
				glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X_ARB + i, level, sTex.format,
					sTex.Width>>level, sTex.Height>>level, 0, sTex.format, sTex.datatype, NULL);	
	}
	else
	{
		uint size = 1;
		if(genMipMaps)
			size = min(sTex.Width, sTex.Height);
		for(uint mipsize = size, level=0; mipsize>0; mipsize>>=1, level++)
			glTexImage2D(sTex.target, level, sTex.format,
				sTex.Width>>level, sTex.Height>>level, 0, sTex.format, sTex.datatype, NULL);		
	}

	if(genMipMaps)
		sTex.MinFilter = GL_LINEAR_MIPMAP_LINEAR;

	glTexParameteri(sTex.target,GL_TEXTURE_MIN_FILTER,sTex.MinFilter);
	glTexParameteri(sTex.target,GL_TEXTURE_MAG_FILTER,sTex.MagFilter);

	// Creating Depth Buffer If Render Target Is Color Image
	if(SurfFmt<RSURFMT_DEPTH16)
	{
		glGenRenderbuffersEXT(1, &sTex.depthBuffer);
		glBindRenderbufferEXT(GL_RENDERBUFFER_EXT, sTex.depthBuffer);
		glRenderbufferStorageEXT(GL_RENDERBUFFER_EXT, GL_DEPTH_COMPONENT24, sTex.Width, sTex.Height);

		//Creating Stencil Buffer For Render Target
		//glGenRenderbuffersEXT(1, &sTex.stencilBuffer);
		//glBindRenderbufferEXT(GL_RENDERBUFFER_EXT, sTex.stencilBuffer);
		//glRenderbufferStorageEXT(GL_RENDERBUFFER_EXT, GL_STENCIL_INDEX_EXT, sTex.Width, sTex.Height);

		glBindRenderbufferEXT(GL_RENDERBUFFER_EXT, 0);
	}

	// Create And Setup FBO
	glGenFramebuffersEXT(1, &sTex.FBO);
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, sTex.FBO);
	if(SurfFmt<RSURFMT_DEPTH16)
	{
		// Attach CubeMap
		if(sTex.target == GL_TEXTURE_CUBE_MAP)
			glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0_EXT, 
				GL_TEXTURE_CUBE_MAP_POSITIVE_X, sTex.TexID, 0); 
		// Attach 2D Color Buffer
		else
			glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0_EXT,
				GL_TEXTURE_2D, sTex.TexID, 0);
		
		// Attach Depth Buffer
		glFramebufferRenderbufferEXT(GL_FRAMEBUFFER_EXT, GL_DEPTH_ATTACHMENT_EXT,
			GL_RENDERBUFFER_EXT, sTex.depthBuffer);

		// Attach Stencil Buffer
		//glFramebufferRenderbufferEXT(GL_FRAMEBUFFER_EXT, GL_STENCIL_ATTACHMENT_EXT,
		//	GL_RENDERBUFFER_EXT, sTex.stencilBuffer);
	}
	else
	{
		// Attach CubeMap
		if(sTex.target == GL_TEXTURE_CUBE_MAP)
			glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_DEPTH_ATTACHMENT_EXT, 
				GL_TEXTURE_CUBE_MAP_POSITIVE_X, sTex.TexID, 0); 
		// Attach Depth texture
		else
			glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_DEPTH_ATTACHMENT_EXT,
				GL_TEXTURE_2D, sTex.TexID, 0);

		// There are no color buffers attached
		glDrawBuffer(GL_NONE);
		glReadBuffer(GL_NONE);
	}

	GLenum status = glCheckFramebufferStatusEXT(GL_FRAMEBUFFER_EXT);
	if(status != GL_FRAMEBUFFER_COMPLETE_EXT)
	{
		glDeleteTextures(1, &sTex.TexID);
		return 0;
	}
	// Restore to normal framebuffer operation
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);
	//glDisable(sTex.target);

	return TextureArray.AddElement(sTex);
}

//////////////////////////////////////////////////////////////////////////////////
// Binding Render Target
bool CGLTextureSystem::BindRenderTarget(uint TexInd, CUBEFACE face)
{
	sGLTexture *pTex =TextureArray.GetElement(TexInd);
	if(!pTex)
		return false;

	if(!pTex->FBO)
		return false;

	// Saving Active Vieport
	glGetIntegerv(GL_VIEWPORT, viewport); 

	// Begin rendering to FBO
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, pTex->FBO);
	glViewport(0, 0, pTex->Width, pTex->Height);

	// Change cubemap face to render to
	if(pTex->target == GL_TEXTURE_CUBE_MAP)
	{
		glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0_EXT, 
			GL_TEXTURE_CUBE_MAP_POSITIVE_X + face, pTex->TexID, 0);
	}

	ActiveRenderTarget = pTex->target;
	ActiveRenderTexture = pTex->TexID;
	genRTargetMipMaps = pTex->genMipMaps;

	return true;
}

//////////////////////////////////////////////////////////////////////////////////
// Unbinding Render Target
void CGLTextureSystem::UnBindRenderTarget()
{
	// Return to main framebuffer
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);
	glViewport(viewport[0], viewport[1], viewport[2], viewport[3]);

	if(genRTargetMipMaps)
	{
		glEnable(ActiveRenderTarget);
		glBindTexture(ActiveRenderTarget,ActiveRenderTexture);
		glGenerateMipmapEXT(ActiveRenderTarget);
		glDisable(ActiveRenderTarget);
	}
	ActiveRenderTarget = 0;
	ActiveRenderTexture = 0;
	genRTargetMipMaps = 0;
}

//////////////////////////////////////////////////////////////////////////////////
// Setting Texture In Specified Unit
bool CGLTextureSystem::SetTexture(uint TexInd, uint sampler)
{
	sGLTexture *pTex = TextureArray.GetElement(TexInd);

	if(!pTex)
		return false;

	if(sampler>MaxTexUnits-1)
		return false;

	glActiveTextureARB(GL_TEXTURE0_ARB+sampler);
	glEnable(pTex->target);
	glBindTexture(pTex->target, pTex->TexID);

	//glTexParameteri(pTex->target, GL_TEXTURE_MAG_FILTER, pTex->MagFilter);
	//glTexParameteri(pTex->target, GL_TEXTURE_MIN_FILTER, pTex->MinFilter);

	pTexTargets[sampler] = pTex->target;
	ActiveTarget = pTex->target;

	return true;
}

//////////////////////////////////////////////////////////////////////////////////
// Setting Texture Address Type. Must be Called After SetTexture Method
void CGLTextureSystem::SetTextureAddress(TEXADDR S, TEXADDR T, TEXADDR R)
{
	if(S != TEXADDR_NONE)
		glTexParameteri(ActiveTarget,GL_TEXTURE_WRAP_S,GetGLTexWrapFromTEXADDR(S));

	if(T != TEXADDR_NONE)
		glTexParameteri(ActiveTarget,GL_TEXTURE_WRAP_T,GetGLTexWrapFromTEXADDR(T));

	if(R != TEXADDR_NONE)
		glTexParameteri(ActiveTarget,GL_TEXTURE_WRAP_R,GetGLTexWrapFromTEXADDR(R));
}

//////////////////////////////////////////////////////////////////////////////////
// Disabling All Texture Units. Getting Information From TexTarget Array
void CGLTextureSystem::DisableTextures()
{
	for(uint i=0; i<MaxTexUnits; i++)
	{
		if(pTexTargets[i])
		{
			glActiveTextureARB(GL_TEXTURE0_ARB+i);
			glDisable(pTexTargets[i]);
			pTexTargets[i] = 0;
		}
	}
}

//////////////////////////////////////////////////////////////////////////////////
// Delete Texture Form The Array
bool CGLTextureSystem::DeleteTexture(uint texid)
{
	sGLTexture *pTex = TextureArray.GetElement(texid);
	if(!pTex)
		return false;

	if(pTex->FBO)
		glDeleteFramebuffersEXT(1,&pTex->FBO);
	if(pTex->depthBuffer)
		glDeleteRenderbuffersEXT(1,&pTex->depthBuffer);

	glEnable(pTex->target);
	glBindTexture(pTex->target, pTex->TexID);
	
	glDeleteTextures(1, &pTex->TexID);

	glDisable(pTex->target);

	return TextureArray.DeleteElement(texid);
}

//////////////////////////////////////////////////////////////////////////////////
// Updating Texture From Image Class
bool CGLTextureSystem::UpdateTexture(uint texid, CImage *Img)
{
	sGLTexture *pTex = TextureArray.GetElement(texid);

	if(pTex==NULL)
		return false;

	// If It Is Render Target, Than Return False (We Can't Update Render Target)
	if(pTex->FBO)
		return false;

	if(!Img)
		return false;

	FORMAT fmt = Img->getFormat();

	// Checking if formats are equal
	if(pTex->format != GetGLTexFormatFromCImageFmt(fmt))
		return false;

	// Checking if data types are equal
	if(pTex->datatype != GetGLDataTypeFromCImageFmt(fmt))
		return false;

	// Updating Texels Of 1D Image
	if(Img->is1D())
	{
		if(pTex->target != GL_TEXTURE_1D)
			return false;

		if(pTex->Width != Img->getWidth())
			return false;

		glEnable(GL_TEXTURE_1D);
		glBindTexture(GL_TEXTURE_1D,pTex->TexID);

		glTexSubImage1D(GL_TEXTURE_1D, 0, 0, pTex->Width, pTex->format,
			pTex->datatype, Img->getPixels());

		//Haven't Checked And I Do Not Know If It Works For Simple Textures
		if(pTex->genMipMaps)
			glGenerateMipmapEXT(GL_TEXTURE_1D);
		
		glDisable(GL_TEXTURE_1D);
	}
	// Updating Texels Of 2D Image
	else if(Img->is2D())
	{
		//If It Is Cube Map
		if(Img->getImageCount()==6)
		{
			if(pTex->target != GL_TEXTURE_CUBE_MAP_ARB)
				return false;

			if((pTex->Width != Img->getWidth()) || (pTex->Height != Img->getHeight()))
				return false;

			glEnable(GL_TEXTURE_CUBE_MAP_ARB);
			glBindTexture(GL_TEXTURE_CUBE_MAP_ARB,pTex->TexID);

			// Updating All Six Cube Faces
			for(uint i=0; i<6; i++)
			{
				glTexSubImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X_ARB + i, 0, 0, 0, 
					pTex->Width, pTex->Height, pTex->format, pTex->datatype, 
					Img->getPixels(i+1));
			}
			//Haven't Checked And I Do Not Know If It Works For Simple Textures
			if(pTex->genMipMaps)
				glGenerateMipmapEXT(GL_TEXTURE_CUBE_MAP_ARB);

			glDisable(GL_TEXTURE_CUBE_MAP_ARB);
		}
		else
		{
			if(pTex->target != GL_TEXTURE_2D)
				return false;

			if((pTex->Width != Img->getWidth()) || (pTex->Height != Img->getHeight()))
				return false;

			glEnable(GL_TEXTURE_2D);
			glBindTexture(GL_TEXTURE_2D,pTex->TexID);
				
			glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, pTex->Width, pTex->Height, pTex->format,
				pTex->datatype, Img->getPixels());

			//Haven't Checked And I Do Not Know If It Works For Simple Textures
			if(pTex->genMipMaps)
				glGenerateMipmapEXT(GL_TEXTURE_2D);

			glDisable(GL_TEXTURE_2D);
		}
	}
	//Updating Texels Of 3D Image
	else if(Img->is3D())
	{
		if(pTex->target != GL_TEXTURE_3D_EXT)
			return false;

		if((pTex->Width != Img->getWidth()) || (pTex->Height != Img->getHeight()) || 
			(pTex->Depth != Img->getDepth()))
			return false;

			glEnable(GL_TEXTURE_3D_EXT);
			glBindTexture(GL_TEXTURE_3D_EXT,pTex->TexID);
				
			glTexSubImage3DEXT(GL_TEXTURE_3D_EXT, 0, 0, 0, 0, pTex->Width, pTex->Height, 
				pTex->Depth, pTex->format, pTex->datatype, Img->getPixels());

			//Haven't Checked And I Do Not Know If It Works For Simple Textures
			if(pTex->genMipMaps)
				glGenerateMipmapEXT(GL_TEXTURE_3D_EXT);

			glDisable(GL_TEXTURE_3D_EXT);
	}

	return true;
}

//////////////////////////////////////////////////////////////////////////////////
// Setting Texture Filtration Type
bool CGLTextureSystem::SetTexFilter(uint texid, TEXF MagFilter, TEXF MinFilter, TEXF MipFilter)
{
	sGLTexture *pTex = TextureArray.GetElement(texid);
	if(pTex==NULL)
		return false;

	if(MagFilter == TEXF_LINEAR)
		pTex->MagFilter = GL_LINEAR;
	else if(MagFilter == TEXF_NEAREST)
		pTex->MagFilter = GL_NEAREST;

	if(MipFilter == TEXF_NONE)
	{
		if(MinFilter == TEXF_LINEAR)
			pTex->MinFilter = GL_LINEAR;
		else if(MinFilter == TEXF_NEAREST)
			pTex->MinFilter = GL_NEAREST;
	}
	else
	{
		if(MinFilter == TEXF_NEAREST)
		{
			if(MipFilter == TEXF_NEAREST)
				pTex->MinFilter = GL_NEAREST_MIPMAP_NEAREST;
			else
				pTex->MinFilter = GL_NEAREST_MIPMAP_LINEAR;
		}
		else if(MinFilter == TEXF_LINEAR)
		{
			if(MipFilter == TEXF_NEAREST)
				pTex->MinFilter = GL_LINEAR_MIPMAP_NEAREST;
			else
				pTex->MinFilter = GL_LINEAR_MIPMAP_LINEAR;
		}
	}

	//Setting OpenGL Texture Filtration Type
	//glEnable(pTex->target);
	glBindTexture(pTex->target, pTex->TexID);

	glTexParameteri(pTex->target,GL_TEXTURE_MAG_FILTER ,pTex->MagFilter);
	glTexParameteri(pTex->target,GL_TEXTURE_MIN_FILTER ,pTex->MinFilter);

	//glDisable(pTex->target);

	return true;
}

//////////////////////////////////////////////////////////////////////////////////
// Setting Texture Anisotropy Filtration
bool CGLTextureSystem::SetAnisotropicFilter(uint texind, uint Anisotropy)
{
	sGLTexture *pTex = TextureArray.GetElement(texind);
	if(pTex==NULL)
		return false;

	if(Anisotropy > MaxAnisotropy)
		Anisotropy = MaxAnisotropy;

	pTex->Anisotropy = Anisotropy;

	//glEnable(pTex->target);
	glBindTexture(pTex->target, pTex->TexID);

	GLint param = GLint(Anisotropy);
	glTexParameteri(pTex->target, GL_TEXTURE_MAX_ANISOTROPY_EXT, param );

	//glDisable(pTex->target);

	return false;
}

//////////////////////////////////////////////////////////////////////////////////
// Check If Texture Is 1D
bool CGLTextureSystem::Is1DMap(uint texid)
{
	sGLTexture *sTex = TextureArray.GetElement(texid);
	if(!sTex)
		return false;

	if(sTex->target == GL_TEXTURE_1D)
		return true;

	return false;
}
//////////////////////////////////////////////////////////////////////////////////
// Check If Texture Is 2D
bool CGLTextureSystem::Is2DMap(uint texid)
{
	sGLTexture *sTex = TextureArray.GetElement(texid);
	if(!sTex)
		return false;

	if(sTex->target == GL_TEXTURE_2D)
		return true;

	return false;
}
//////////////////////////////////////////////////////////////////////////////////
// Check If Texture Is 3D
bool CGLTextureSystem::Is3DMap(uint texid)
{
	sGLTexture *sTex = TextureArray.GetElement(texid);
	if(!sTex)
		return false;

	if(sTex->target == GL_TEXTURE_3D_EXT)
		return true;

	return false;
}
/////////////////////////////////////////////////////////////////////////////////
// Check If Texture Is CubeMap
bool CGLTextureSystem::IsCubeMap(uint texid)
{
	sGLTexture *sTex = TextureArray.GetElement(texid);
	if(!sTex)
		return false;

	if(sTex->target == GL_TEXTURE_CUBE_MAP_ARB)
		return true;

	return false;
}

//////////////////////////////////////////////////////////////////////////////////
// Class Inside Methods
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
// Creating All Types Of Texture From CImage
bool CGLTextureSystem::SetTextureFromImage(CImage *Img,sGLTexture &sTex,bool genMipMaps)
{
	if(Img == NULL)
		return false;

	FORMAT fmt = Img->getFormat();

	if(isPalettedFormat(fmt))
	{
		Img->ExpandPalette(true);
		fmt = Img->getFormat();
	}

	sTex.format = GetGLTexFormatFromCImageFmt(fmt);
	if(sTex.format == 0)
		return false;
	
	sTex.datatype = GetGLDataTypeFromCImageFmt(fmt);
	if(sTex.datatype == 0)
		return false;

	if(!SetTextureDim(Img,sTex))
		return false;

	//Creating Texture Object And Setting Texture
	glEnable(sTex.target);
	glGenTextures(1,&sTex.TexID);
	glBindTexture(sTex.target,sTex.TexID);

	if(!sTex.TexID)
	{
		glDisable(sTex.target);
		return false;
	}

	// Setting Default Texture Filter Type 
	sTex.MinFilter = sTex.MagFilter = GL_NEAREST;
	glTexParameteri(sTex.target, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(sTex.target, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	// Setting Texture Data And Generating MipMaps
	if(!SetTextureData(Img,sTex,genMipMaps))
	{
		glDeleteTextures(1,&sTex.TexID);
		glDisable(sTex.target);
		return false;
	}

	glDisable(sTex.target);

	return true;
}

//////////////////////////////////////////////////////////////////////////////////
// Setting Texture Dimention
bool CGLTextureSystem::SetTextureDim(CImage *Img, sGLTexture &sTex)
{
	if(!Img)
		return false;

	// Getting Image Dimention And Size In Pixels
	if(Img->is1D())
	{
		sTex.Width = Img->getWidth();
		sTex.target = GL_TEXTURE_1D;

		if(sTex.Width>MaxTexSize)
			return false;
		if(sTex.Width%4)
			return false;
		//if(!IsPowerOf2(sTex.Width))
		//	return false;
	}
	else if(Img->is2D())
	{
		if(Img->getImageCount()==6)
			sTex.target = GL_TEXTURE_CUBE_MAP_ARB;		
		else
			sTex.target = GL_TEXTURE_2D;

		sTex.Width = Img->getWidth();
		sTex.Height = Img->getHeight();
		
		if((sTex.target == GL_TEXTURE_CUBE_MAP_ARB) && (sTex.Width != sTex.Height))
			return false;
		if(sTex.Width>MaxTexSize || sTex.Width>MaxTexSize)
			return false;
		if(sTex.Width%4 || sTex.Height%4)
			return false;
		//if((!IsPowerOf2(sTex.Width)) || (!IsPowerOf2(sTex.Height)))
		//	return false;
	}
	else if(Img->is3D())
	{
		sTex.Width = Img->getWidth();
		sTex.Height = Img->getHeight();
		sTex.Depth = Img->getDepth();
		sTex.target = GL_TEXTURE_3D_EXT;

		if(sTex.Width>MaxVolTexSize || sTex.Height>MaxVolTexSize || sTex.Depth>MaxVolTexSize)
			return false;
		if(sTex.Width%4 || sTex.Height%4 || sTex.Depth%4)
			return false;
		//if((!IsPowerOf2(sTex.Width)) || (!IsPowerOf2(sTex.Height)) || (!IsPowerOf2(sTex.Depth)))
		//	return false;
	}

	return true;
}

//////////////////////////////////////////////////////////////////////////////////
// Setting Texture Data And Generating MipMaps
bool CGLTextureSystem::SetTextureData(CImage *Img,sGLTexture &sTex,bool genMipMaps)
{
	if(!Img)
		return false;

	if(genMipMaps)
	{
		sTex.genMipMaps = genMipMaps;
/*		if(m_pExt->GL_SGIS_generate_mipmap_flag)
		{
			//Creating Texture From 1D Image
			if(sTex.target == GL_TEXTURE_1D)
			{
				glTexImage1D(sTex.target, 0, sTex.format, sTex.Width, 0, sTex.format,
					sTex.datatype, Img->getPixels());
			}
			//Creating Texture From 2D Image
			else if(sTex.target == GL_TEXTURE_2D)
			{
				glTexImage2D(sTex.target, 0, sTex.format, sTex.Width, sTex.Height, 0,
					sTex.format, sTex.datatype, Img->getPixels());
			}
			//Creating Texture From 3D Image
			else if(sTex.target == GL_TEXTURE_3D_EXT)
			{
				glTexImage3DEXT(sTex.target, 0, sTex.format, sTex.Width, sTex.Height,
					sTex.Depth, 0, sTex.format, sTex.datatype, Img->getPixels());
			}
			//Creating Cube Map From Image
			else if(sTex.target == GL_TEXTURE_CUBE_MAP_ARB)
			{
				for(uint i=0; i<6; i++)
				{
					glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X_ARB + i, 0, sTex.format,
						sTex.Width, sTex.Height, 0, sTex.format,
						sTex.datatype, Img->getPixels(i+1));
				}
			}
			else
				return false;

			// Setting Generation Of The MipMaps
			//glTexParameteri(sTex.target, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
			glTexParameteri(sTex.target, GL_GENERATE_MIPMAP_SGIS, true);
			
			////glTexParameteri(sTex.target, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR); 
			////sTex.MinFilter = GL_LINEAR_MIPMAP_LINEAR;
			//glHint(GL_GENERATE_MIPMAP_HINT_SGIS, GL_NICEST);
		}
		// Or using glu Function For Building MipMaps
		else*/
		{
			//Creating Texture From 1D Image
			if(sTex.target == GL_TEXTURE_1D)
			{
				gluBuild1DMipmaps(sTex.target, sTex.format, sTex.Width, sTex.format,
					sTex.datatype, Img->getPixels());
				
			}
			//Creating Texture From 2D Image
			else if(sTex.target == GL_TEXTURE_2D)
			{
				gluBuild2DMipmaps(sTex.target, sTex.format, sTex.Width, sTex.Height,
					sTex.format, sTex.datatype, Img->getPixels());
			}
			// Creating Cube Map From 2D Image
			else if(sTex.target == GL_TEXTURE_CUBE_MAP_ARB)
			{
				for(uint i=0; i<6; i++)
				{
					gluBuild2DMipmaps(GL_TEXTURE_CUBE_MAP_POSITIVE_X_ARB + i, sTex.format,
						sTex.Width, sTex.Height, sTex.format, sTex.datatype, 
						Img->getPixels(i+1));
				}
			}
			else if(sTex.target == GL_TEXTURE_3D_EXT)
			{
				uint size = min(sTex.Width,min(sTex.Height,sTex.Depth));
				glTexImage3DEXT(sTex.target, 0, sTex.format, sTex.Width, sTex.Height,
					sTex.Depth, 0, sTex.format, sTex.datatype, Img->getPixels());
				for(uint i=2 ,mipsize=size, level=1; (mipsize%2)==0 ;i*=2,mipsize/=2,level++)
				{
					glTexImage3DEXT(sTex.target, level, sTex.format, sTex.Width/2, sTex.Height/2,
					sTex.Depth/2, 0, sTex.format, sTex.datatype, NULL);
				}
				sTex.MinFilter = GL_LINEAR_MIPMAP_LINEAR;
				glTexParameteri(sTex.target,GL_TEXTURE_MIN_FILTER,sTex.MinFilter);
				glGenerateMipmapEXT(sTex.target);
			}
			else
				return false;
		}
	}
	// DO not generate MipMaps
	else
	{
		//Creating Texture From 1D Image
		if(sTex.target == GL_TEXTURE_1D)
		{
			glTexImage1D(sTex.target, 0, sTex.format, sTex.Width, 0, sTex.format,
				sTex.datatype, Img->getPixels());
		}
		//Creating Texture From 2D Image
		else if(sTex.target == GL_TEXTURE_2D)
		{
			glTexImage2D(sTex.target, 0, sTex.format, sTex.Width, sTex.Height, 0,
					sTex.format, sTex.datatype, Img->getPixels());
		}
		//Creating Texture From 3D Image
		else if(sTex.target == GL_TEXTURE_3D_EXT)
		{
			glTexImage3DEXT(sTex.target, 0, sTex.format, sTex.Width, sTex.Height,
				sTex.Depth, 0, sTex.format, sTex.datatype, Img->getPixels());
		}
		//Creating Cube Map From Image
		else if(sTex.target == GL_TEXTURE_CUBE_MAP_ARB)
		{
			for(uint i=0; i<6; i++)
			{
				glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X_ARB + i, 0, sTex.format,
						sTex.Width, sTex.Height, 0, sTex.format,
						sTex.datatype, Img->getPixels(i+1));
			}
		}
		else
			return false;
	}

	return true;
}