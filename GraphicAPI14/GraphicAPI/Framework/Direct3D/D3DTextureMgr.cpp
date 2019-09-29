////////////////////////////////////////////////////////////////////////////////////////////////
//	D3DTextureMgr.h 
//	Direct3D Texture Managment System Class
//	Created: 23d August 2006
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
//#include "../Image/ImageIO.h"
//#include "../Image/ImgTypes.h"

#include "../Renderer/Renderer.h"

#include "D3DRenderer.h"
#include "D3DConvFunc.h"
#include "D3DTextureMgr.h"

//////////////////////////////////////////////////////////////////////////////////
// Constructor
CD3DTextureSystem::CD3DTextureSystem(HWND hWnd, LPDIRECT3DDEVICE9 pDeviceD3D, D3DCAPS9 *pDevCaps)
{
	m_hWnd = hWnd;
	m_pDevD3D = pDeviceD3D;
	m_pDevCaps = pDevCaps;

	m_pFXStateMgr = NULL;

	pTexSamplers = new bool[pDevCaps->MaxSimultaneousTextures];
	memset(pTexSamplers,0,sizeof(bool));

	pDeviceD3D->GetRenderTarget(0,&m_pColorBuffer);
	pDeviceD3D->GetDepthStencilSurface(&m_pDepthStencilBuffer);

	ActiveSampler = 0;

	D3DXMatrixScaling(&mTextureMatrix, 1, -1, 1);
}

/////////////////////////////////////////////////////////////////////////////////
// Destructor
CD3DTextureSystem::~CD3DTextureSystem()
{
	m_hWnd = NULL;
	m_pDevD3D = NULL;
	m_pDevCaps = NULL;

	delete [] pTexSamplers;

	m_pColorBuffer->Release();
	m_pDepthStencilBuffer->Release();

	sD3DTexture *TexElem = NULL;
	TextureArray.SetPointerToStart();

	while(true)
	{
		TexElem = TextureArray.GetElementAndMovePointer();

		if(TexElem!=NULL)
		{
			if(TexElem->DepthStencilSurface)
			{
				if(TexElem->pCubeTex)
				{
					for(uint i=0; i<6; i++)
						TexElem->DepthStencilSurface[i]->Release();
				}
				else
					TexElem->DepthStencilSurface[0]->Release();

				delete [] TexElem->DepthStencilSurface;
			}

			if(TexElem->RenderTargetSurface)
			{
				if(TexElem->pCubeTex)
				{
					for(uint i=0; i<6; i++)
						TexElem->RenderTargetSurface[i]->Release();
				}
				else
					TexElem->RenderTargetSurface[0]->Release();
				
				delete [] TexElem->RenderTargetSurface;
			}

			if(TexElem->pTex)
				TexElem->pTex->Release();
			if(TexElem->pCubeTex)
				TexElem->pCubeTex->Release();
			if(TexElem->pVolTex)
				TexElem->pVolTex->Release();
			
		}
		else
			break;
	}
}

/////////////////////////////////////////////////////////////////////////////////
// Loading Only 2D Images (And 1D Images) Using Windows GetOpenFileName Function
uint CD3DTextureSystem::CreateTextureFromFile(bool genMipMaps)
{
	OPENFILENAME ofn = {0};
	char file_name[2048] = {0};
	char LoadString[2048]={"All Supported Types \0*.bmp;*.tga\0"
							"All Files (*.*)\0*.*\0\0"};

	ofn.lStructSize = sizeof(OPENFILENAME);
	ofn.hwndOwner = m_hWnd;
	ofn.lpstrFilter = LoadString;
	ofn.lpstrFile = file_name;
	ofn.nMaxFile = 2048; 
	ofn.lpstrInitialDir = NULL;
	ofn.lpstrFileTitle = NULL;
	ofn.Flags = OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST | OFN_NOCHANGEDIR
								| OFN_HIDEREADONLY;

	if(GetOpenFileName(&ofn))
		return this->CreateTextureFromFile(ofn.lpstrFile,genMipMaps);
	
	return 0;
}

/////////////////////////////////////////////////////////////////////////////////
// Loading Only 2D Images (And 1D Images)
uint CD3DTextureSystem::CreateTextureFromFile(char *file_name, bool genMipMaps)
{
	if(!file_name)
		return 0;

	D3DXIMAGE_INFO ImgInfo;
	HRESULT res = 0;
	LPDIRECT3DTEXTURE9 pTex=NULL;
	LPDIRECT3DCUBETEXTURE9 pCubeTex=NULL;
	LPDIRECT3DVOLUMETEXTURE9 pVolTex=NULL;
	
	//Try To Load Image
	if((res = D3DXCreateTextureFromFileEx(m_pDevD3D, file_name, D3DX_DEFAULT, D3DX_DEFAULT, 
		(genMipMaps) ? (D3DX_DEFAULT) : (D3DX_FROM_FILE), 
		0, D3DFMT_UNKNOWN, D3DPOOL_MANAGED, D3DX_DEFAULT, D3DX_DEFAULT, 0, 
		&ImgInfo, NULL, &pTex)) != D3D_OK)
		pTex=NULL;

	//Try To Load Cube Map Texture
	if(res!=D3D_OK)
		if((res=D3DXCreateCubeTextureFromFileEx(m_pDevD3D, file_name,D3DX_DEFAULT,
			(genMipMaps) ? (D3DX_DEFAULT) : (D3DX_FROM_FILE), 0, D3DFMT_UNKNOWN, 
			D3DPOOL_MANAGED, D3DX_DEFAULT, D3DX_DEFAULT, 0, NULL, NULL, &pCubeTex))!=D3D_OK)
			pCubeTex=NULL;

	//Try To Load Volume Texture
	if(res!=D3D_OK)
		if((res=D3DXCreateVolumeTextureFromFileEx(m_pDevD3D, file_name, 
			D3DX_DEFAULT, D3DX_DEFAULT, D3DX_DEFAULT,
			(genMipMaps) ? (D3DX_DEFAULT) : (D3DX_FROM_FILE), 0, D3DFMT_UNKNOWN,
			D3DPOOL_MANAGED, D3DX_DEFAULT, D3DX_DEFAULT, 0, NULL, NULL, 
			&pVolTex)) != D3D_OK)
			pVolTex=NULL;

	if(res!=D3D_OK)
		return 0;

	sD3DTexture Elem;
	memset(&Elem,0,sizeof(sD3DTexture));

	Elem.genMipMaps = genMipMaps;
	Elem.pCubeTex = pCubeTex;
	Elem.pVolTex = pVolTex;
	Elem.MagFilter = D3DTEXF_POINT;
	Elem.MinFilter = D3DTEXF_POINT;
	Elem.MipFilter = D3DTEXF_POINT;
	Elem.pTex = pTex;
	Elem.CreatedFromFile = true;
	Elem.RenderTargetSurface = NULL;
	Elem.DepthStencilSurface = NULL;

	D3DSURFACE_DESC pDesc;
	pTex->GetLevelDesc(0, &pDesc);
	if(pDesc.Height==1)
		Elem.is1D = true;

	return TextureArray.AddElement(Elem);
}

//////////////////////////////////////////////////////////////////////////////////
// Creating Cube Map Texture From File
uint CD3DTextureSystem::CreateCubeMapTextureFromFile(char *file_name, bool genMipMaps)
{
	if(!file_name)
		return 0;

	LPDIRECT3DCUBETEXTURE9 pCubeTex=NULL;

	//Try To Load Cube Map Texture
	if(D3DXCreateCubeTextureFromFileEx(m_pDevD3D, file_name,D3DX_DEFAULT,
		(genMipMaps) ? (D3DX_DEFAULT) : (D3DX_FROM_FILE), 0, D3DFMT_UNKNOWN, 
		D3DPOOL_MANAGED, D3DX_DEFAULT, D3DX_DEFAULT, 0, NULL, NULL, &pCubeTex)!=D3D_OK)
			return 0;

	sD3DTexture Elem;
	memset(&Elem,0,sizeof(sD3DTexture));

	Elem.genMipMaps = genMipMaps;
	Elem.pCubeTex = pCubeTex;
	Elem.pVolTex = NULL;
	Elem.MagFilter = D3DTEXF_POINT;
	Elem.MinFilter = D3DTEXF_POINT;
	Elem.MipFilter = D3DTEXF_POINT;
	Elem.pTex = NULL;
	Elem.CreatedFromFile = true;
	Elem.RenderTargetSurface = NULL;
	Elem.DepthStencilSurface = NULL;


	return TextureArray.AddElement(Elem);
}

//////////////////////////////////////////////////////////////////////////////////
// Creating Cube Map Texture From 6 Image Files
uint CD3DTextureSystem::CreateCubeMapTexFromFiles(char *PosX, char *NegX,char *PosY,char *NegY,
		char *PosZ, char *NegZ, bool genMipMaps)
{
	if((PosX==NULL) || (NegX==NULL) || (PosY==NULL) || (NegY==NULL) || (PosZ==NULL) || (NegZ==NULL))
		return 0;

	LPDIRECT3DTEXTURE9 *pTex = new LPDIRECT3DTEXTURE9[6];
	memset(pTex,0,sizeof(LPDIRECT3DTEXTURE9)*6);
	bool Ok = true;


	if(D3DXCreateTextureFromFileEx(m_pDevD3D, PosX, D3DX_DEFAULT, D3DX_DEFAULT, 
		D3DX_FROM_FILE, 
		0, D3DFMT_UNKNOWN, D3DPOOL_MANAGED, D3DX_DEFAULT, D3DX_DEFAULT, 0, 
		NULL, NULL, &pTex[0])!=D3D_OK)
		Ok = false;
	
	if(D3DXCreateTextureFromFileEx(m_pDevD3D, NegX, D3DX_DEFAULT, D3DX_DEFAULT, 
		D3DX_FROM_FILE, 
		0, D3DFMT_UNKNOWN, D3DPOOL_MANAGED, D3DX_DEFAULT, D3DX_DEFAULT, 0, 
		NULL, NULL, &pTex[1])!=D3D_OK)
		Ok = false;

	if(D3DXCreateTextureFromFileEx(m_pDevD3D, PosY, D3DX_DEFAULT, D3DX_DEFAULT, 
		D3DX_FROM_FILE, 
		0, D3DFMT_UNKNOWN, D3DPOOL_MANAGED, D3DX_DEFAULT, D3DX_DEFAULT, 0, 
		NULL, NULL, &pTex[2])!=D3D_OK)
		Ok = false;

	if(D3DXCreateTextureFromFileEx(m_pDevD3D, NegY, D3DX_DEFAULT, D3DX_DEFAULT, 
		D3DX_FROM_FILE, 
		0, D3DFMT_UNKNOWN, D3DPOOL_MANAGED, D3DX_DEFAULT, D3DX_DEFAULT, 0, 
		NULL, NULL, &pTex[3])!=D3D_OK)
		Ok = false;

	if(D3DXCreateTextureFromFileEx(m_pDevD3D, PosZ, D3DX_DEFAULT, D3DX_DEFAULT, 
		D3DX_FROM_FILE, 
		0, D3DFMT_UNKNOWN, D3DPOOL_MANAGED, D3DX_DEFAULT, D3DX_DEFAULT, 0, 
		NULL, NULL, &pTex[4])!=D3D_OK)
		Ok = false;

	if(D3DXCreateTextureFromFileEx(m_pDevD3D, NegZ, D3DX_DEFAULT, D3DX_DEFAULT, 
		D3DX_FROM_FILE, 
		0, D3DFMT_UNKNOWN, D3DPOOL_MANAGED, D3DX_DEFAULT, D3DX_DEFAULT, 0, 
		NULL, NULL, &pTex[5])!=D3D_OK)
		Ok = false;

	//If Some Image Failed To Load
	if(!Ok)
	{
		for(uint i=0; i<6; i++)
			if(pTex[i])
				pTex[i]->Release();

		return 0;
	}

	D3DSURFACE_DESC Desc;
	pTex[0]->GetLevelDesc(0,&Desc);
	if(Desc.Width != Desc.Height)
		Ok = false;

	for(uint i=1; i<6; i++)
	{
		D3DSURFACE_DESC ImgDesc;
		pTex[i]->GetLevelDesc(0,&ImgDesc);

		if(Desc.Format != ImgDesc.Format)
			Ok = false;
		if(Desc.Usage != ImgDesc.Usage)
			Ok = false;
		if(Desc.Width != ImgDesc.Width)
			Ok = false;
		if(Desc.Height != ImgDesc.Height)
			Ok = false;
		if(Desc.Type != ImgDesc.Type)
			Ok = false;

		if(!Ok)
			break;
	}

	DWORD usage = 0;
	if(genMipMaps)
		usage |= D3DUSAGE_AUTOGENMIPMAP;

	LPDIRECT3DCUBETEXTURE9 pCubeTex = NULL;
	if(D3DXCreateCubeTexture(m_pDevD3D,Desc.Width,(genMipMaps==true)?(0):(1),
		usage,Desc.Format, D3DPOOL_MANAGED, &pCubeTex)!=D3D_OK)
		Ok = false;

	if(!Ok)
	{
		for(uint i=0; i<6; i++)
			pTex[i]->Release();

		pCubeTex->Release();
		return 0;
	}

	D3DSURFACE_DESC CubeDesc;
	pCubeTex->GetLevelDesc(0,&CubeDesc);

	int fmtsize = GetD3DFORMATSize(CubeDesc.Format);
	D3DLOCKED_RECT CubeLockedRect;
	D3DLOCKED_RECT LockedRect;
	for(uint i=0; i<6; i++)
	{
		pCubeTex->LockRect((D3DCUBEMAP_FACES)i,0,&CubeLockedRect,NULL,D3DLOCK_DISCARD);
		pTex[i]->LockRect(0,&LockedRect,NULL,D3DLOCK_DISCARD);
		ubyte *ptr1;
		ubyte *ptr2;
		for(uint j=0; j<CubeDesc.Width; j++)
		{
			ptr1 = (ubyte*)CubeLockedRect.pBits + CubeLockedRect.Pitch*j;
			ptr2 = (ubyte*)LockedRect.pBits + LockedRect.Pitch*j;
			memcpy(ptr1,ptr2,fmtsize*CubeDesc.Width);
		}
		pCubeTex->UnlockRect((D3DCUBEMAP_FACES)i,0);
		pTex[i]->UnlockRect(0);
	}
	if(genMipMaps)
		pCubeTex->GenerateMipSubLevels();

	sD3DTexture Elem;
	memset(&Elem,0,sizeof(sD3DTexture));

	Elem.genMipMaps = genMipMaps;
	Elem.pCubeTex = pCubeTex;
	Elem.pVolTex = NULL;
	Elem.MagFilter = D3DTEXF_POINT;
	Elem.MinFilter = D3DTEXF_POINT;
	Elem.MipFilter = D3DTEXF_POINT;
	Elem.pTex = NULL;
	Elem.RenderTargetSurface = NULL;
	Elem.DepthStencilSurface = NULL;
	Elem.CreatedFromFile = true;

	for(uint i=0; i<6; i++)
		pTex[i]->Release();

	return TextureArray.AddElement(Elem);
}

//////////////////////////////////////////////////////////////////////////////////
// Create Render Target Texture
uint CD3DTextureSystem::CreateRenderTarget(uint Width, uint Height, RSURFMT SurfFmt,
										   TEXTYPE TexType, bool genMipMaps)
{
	if(Width<2)	Width = 2;
	if(Height<2) Height = 2;

	if(!IsPowerOf2(Width))
		Width = GetLowestPowerOf2(Width);

	if(!IsPowerOf2(Height))
		Height = GetLowestPowerOf2(Height);

	sD3DTexture sTex;
	memset(&sTex,0,sizeof(sD3DTexture));

	sTex.MagFilter = D3DTEXF_POINT;
	sTex.MinFilter = D3DTEXF_POINT;
	sTex.MipFilter = D3DTEXF_POINT;
	sTex.genMipMaps = genMipMaps;

	DWORD usage = D3DUSAGE_RENDERTARGET;
	if(genMipMaps)
		usage |= D3DUSAGE_AUTOGENMIPMAP;


	// Haven't Checked This For rendering To DepthStencil And Color Buffer
	if(TexType==TEXT_CUBE)
	{
		if(Width!=Height)
			return 0;

		if(D3DXCreateCubeTexture(m_pDevD3D, Width, genMipMaps ? 0 : 1,
			usage, GetD3DFMTFromRSURFMT(SurfFmt), D3DPOOL_DEFAULT, &sTex.pCubeTex)!=D3D_OK)
			return 0;
	
		if(SurfFmt < RSURFMT_DEPTH16)
		{
			sTex.RenderTargetSurface = new LPDIRECT3DSURFACE9[6];
			sTex.DepthStencilSurface = new LPDIRECT3DSURFACE9[6];

			for(uint i=0; i<6; i++)
			{
				sTex.pCubeTex->GetCubeMapSurface((D3DCUBEMAP_FACES)i, 0, 
					&sTex.RenderTargetSurface[i]); 
				m_pDevD3D->CreateDepthStencilSurface(Width, Height, D3DFMT_D24S8, 
					D3DMULTISAMPLE_NONE, 0,
					true, &sTex.DepthStencilSurface[i], NULL);
			}
		}
		else
		{
			sTex.RenderTargetSurface = new LPDIRECT3DSURFACE9[6];
			sTex.DepthStencilSurface = NULL;

			for(uint i=0; i<6; i++)
			{
				sTex.pCubeTex->GetCubeMapSurface((D3DCUBEMAP_FACES)i, 0, 
					&sTex.RenderTargetSurface[i]); 
			}
		}
	}
	else
	{
		if(D3DXCreateTexture(m_pDevD3D, Width, Height, genMipMaps ? 0 : 1, 
			usage, GetD3DFMTFromRSURFMT(SurfFmt), D3DPOOL_DEFAULT, &sTex.pTex) != D3D_OK)
			return 0;

		if(SurfFmt < RSURFMT_DEPTH16)
		{
			sTex.RenderTargetSurface = new LPDIRECT3DSURFACE9[1];
			sTex.DepthStencilSurface = new LPDIRECT3DSURFACE9[1];

			sTex.pTex->GetSurfaceLevel(0,&sTex.RenderTargetSurface[0]);
			m_pDevD3D->CreateDepthStencilSurface(Width, Height, D3DFMT_D24S8, 
					D3DMULTISAMPLE_NONE, 0,true, &sTex.DepthStencilSurface[0], NULL);
			
		}
		else
		{
			sTex.RenderTargetSurface = new LPDIRECT3DSURFACE9[1];
			sTex.DepthStencilSurface = NULL;

			sTex.pTex->GetSurfaceLevel(0,&sTex.RenderTargetSurface[0]);
		}
	}

	return TextureArray.AddElement(sTex);
}

//////////////////////////////////////////////////////////////////////////////////
// Binding Render Target
bool CD3DTextureSystem::BindRenderTarget(uint texid, CUBEFACE face)
{
	sD3DTexture *Elem = TextureArray.GetElement(texid);

	if(!Elem)
		return false;
	if(!Elem->RenderTargetSurface)
		return false;

	// Getting Application Viewport
	m_pDevD3D->GetViewport(&Viewport);

	// Setting Render Target Viewport
	D3DVIEWPORT9 RTViewport;
	D3DSURFACE_DESC Desc;
	if(Elem->pTex)
		Elem->pTex->GetLevelDesc(0,&Desc);
	else if(Elem->pCubeTex)
		Elem->pCubeTex->GetLevelDesc(0,&Desc);

	memset(&RTViewport,0,sizeof(D3DVIEWPORT9));
	RTViewport.Width = Desc.Width;
	RTViewport.Height = Desc.Height;
	RTViewport.MaxZ = 1.0f;
	m_pDevD3D->SetViewport(&RTViewport);

	//Setting Render Target
	if(Elem->pTex)
	{
		m_pDevD3D->SetRenderTarget(0,Elem->RenderTargetSurface[0]);
		m_pDevD3D->SetDepthStencilSurface(Elem->DepthStencilSurface[0]);
	}
	else if(Elem->pCubeTex)
	{
		m_pDevD3D->SetRenderTarget(0,Elem->RenderTargetSurface[int(face)]);
		m_pDevD3D->SetDepthStencilSurface(Elem->DepthStencilSurface[int(face)]);
	}

	return true;
}

//////////////////////////////////////////////////////////////////////////////////
// Unbinding Render Target
void CD3DTextureSystem::UnBindRenderTarget()
{
	m_pDevD3D->SetViewport(&Viewport);
	m_pDevD3D->SetRenderTarget(0,m_pColorBuffer);
	m_pDevD3D->SetDepthStencilSurface(m_pDepthStencilBuffer);
}

//////////////////////////////////////////////////////////////////////////////////
// Updating Texture Data
bool CD3DTextureSystem::UpdateTexture(uint texid, CImage *Img)
{
	sD3DTexture *Elem = TextureArray.GetElement(texid);

	if(!Elem)
		return false;

	// If It Is Render Target, Than Return False (We Can't Update Render Target)
	if(Elem->RenderTargetSurface)
		return false;

	// If It Is 1D Or 2D Texture
	if(Elem->pTex)
	{
		if(Elem->is1D == Img->is1D())
			return false;

		if(!Img->is2D())
			return false;

		D3DSURFACE_DESC Desc;
		Elem->pTex->GetLevelDesc(0,&Desc);

		if(Desc.Format != GetD3DFmtFromImageFmt(Img->getFormat()))
			return false;

		uint width = uint(Img->getWidth());
		uint height = uint(Img->getHeight());

		if((Desc.Width != width) || (Desc.Height != height))
			return false;

		D3DLOCKED_RECT LockedRect;
		if(Elem->pTex->LockRect(0,&LockedRect,NULL,D3DLOCK_DISCARD) != D3D_OK)
			return false;

		if(!FillLockedRect(Img->getPixels(),width, height,&LockedRect,Desc.Format))
			return false;

		Elem->pTex->UnlockRect(0);

		if(Elem->genMipMaps)
			Elem->pTex->GenerateMipSubLevels();
	}
	//If Texture Is Cube Map
	else if(Elem->pCubeTex)
	{
		if(!Img->is2D())
			return false;

		if(Img->getImageCount()<6)
			return false;

		D3DSURFACE_DESC Desc;
		Elem->pTex->GetLevelDesc(0,&Desc);

		if(Desc.Format != GetD3DFmtFromImageFmt(Img->getFormat()))
			return false;

		uint width = uint(Img->getWidth());
		uint height = uint(Img->getHeight());

		if((Desc.Width != width) || (Desc.Height != height))
			return false;

		D3DLOCKED_RECT LockedRect;
		for(uint i=0; i<6; i++)
		{
			if(Elem->pCubeTex->LockRect((D3DCUBEMAP_FACES)i, 0, &LockedRect, NULL, 
				D3DLOCK_DISCARD) != D3D_OK)
			return false;

			if(!FillLockedRect(Img->getPixels(i+1),width,height,&LockedRect,Desc.Format))
				return false;

			Elem->pCubeTex->UnlockRect((D3DCUBEMAP_FACES)i, 0);
		}
		if(Elem->genMipMaps)
			Elem->pCubeTex->GenerateMipSubLevels();
	}
	// If It Is Volume Texture
	else if(Elem->pVolTex)
	{
		if(!Img->is3D())
			return false;

		D3DVOLUME_DESC Desc;
		Elem->pVolTex->GetLevelDesc(0,&Desc);

		if(Desc.Format != GetD3DFmtFromImageFmt(Img->getFormat()))
			return false;

		uint width = uint(Img->getWidth());
		uint height = uint(Img->getHeight());
		uint depth = uint(Img->getDepth());

		if((Desc.Width != width) || (Desc.Height != height) || (Desc.Depth != depth))
			return false;

		D3DLOCKED_BOX LockedBox;
		if(Elem->pVolTex->LockBox(0,&LockedBox,NULL,D3DLOCK_DISCARD) != D3D_OK)
			return false;

		if(!FillLockedBox(Img->getPixels(),width,height,depth,&LockedBox,Desc.Format))
			return false;

		Elem->pVolTex->UnlockBox(0);

		if(Elem->genMipMaps)
			Elem->pVolTex->GenerateMipSubLevels();
	}

	return true;
}

//////////////////////////////////////////////////////////////////////////////////
// Deleting Texture From Array
bool CD3DTextureSystem::DeleteTexture(uint TexInd)
{
	sD3DTexture *Elem = TextureArray.GetElement(TexInd);

	if(!Elem)
		return false;

	if(Elem->DepthStencilSurface)
	{
		if(Elem->pCubeTex)
		{
			for(uint i=0; i<6; i++)
				Elem->DepthStencilSurface[i]->Release();
		}
		else
			Elem->DepthStencilSurface[0]->Release();

		delete [] Elem->DepthStencilSurface;
	}

	if(Elem->RenderTargetSurface)
	{
		if(Elem->pCubeTex)
		{
			for(uint i=0; i<6; i++)
				Elem->RenderTargetSurface[i]->Release();
		}
		else
			Elem->RenderTargetSurface[0]->Release();
				
		delete [] Elem->RenderTargetSurface;
	}

	if(Elem->pTex)
		Elem->pTex->Release();

	if(Elem->pCubeTex)
		Elem->pCubeTex->Release();

	if(Elem->pVolTex)
		Elem->pVolTex->Release();

	
	return TextureArray.DeleteElement(TexInd);
}

//////////////////////////////////////////////////////////////////////////////////
// Setting Texture Filtration Type
bool CD3DTextureSystem::SetTexFilter(uint texind,TEXF MagFilter,TEXF MinFilter, TEXF MipFilter)
{
	sD3DTexture *Elem = TextureArray.GetElement(texind);
	if(!Elem)
		return false;

	if(MagFilter == TEXF_LINEAR)
		Elem->MagFilter = D3DTEXF_LINEAR;
	else
		Elem->MagFilter = D3DTEXF_POINT;

	if(MinFilter == TEXF_LINEAR)
		Elem->MinFilter = D3DTEXF_LINEAR;
	else
		Elem->MinFilter = D3DTEXF_POINT;

	if(MipFilter == TEXF_LINEAR)
		Elem->MipFilter = D3DTEXF_LINEAR;
	else
		Elem->MipFilter = D3DTEXF_POINT;

	return true;
}

//////////////////////////////////////////////////////////////////////////////////
// Setting Texture Anisotropy Filtration
bool CD3DTextureSystem::SetAnisotropicFilter(uint texind, uint Anisotropy)
{
	sD3DTexture *Elem = TextureArray.GetElement(texind);
	if(!Elem)
		return false;

	Elem->Anisotropy = DWORD(Anisotropy);
	if(Elem->Anisotropy > m_pDevCaps->MaxAnisotropy)
		Elem->Anisotropy = m_pDevCaps->MaxAnisotropy;

	Elem->MagFilter = D3DTEXF_ANISOTROPIC;
	//Elem->MinFilter = D3DTEXF_ANISOTROPIC;
	Elem->MipFilter = D3DTEXF_ANISOTROPIC;
	
	return true;
}

//////////////////////////////////////////////////////////////////////////////////
// Setting Texture In Specified Unit
bool CD3DTextureSystem::SetTexture(uint TexInd, uint sampler)
{
	if(sampler >= m_pDevCaps->MaxSimultaneousTextures)
		return false;

	sD3DTexture *Elem = TextureArray.GetElement(TexInd);
	if(!Elem)
		return false;

	DWORD Stage = DWORD(sampler);

	if(Elem->pTex)
		m_pDevD3D->SetTexture(Stage,Elem->pTex);
	else if(Elem->pCubeTex)
		m_pDevD3D->SetTexture(Stage,Elem->pCubeTex);
	else
		m_pDevD3D->SetTexture(Stage,Elem->pVolTex);

	//Setting Texture Filtration
	m_pDevD3D->SetSamplerState(Stage,D3DSAMP_MAGFILTER,Elem->MagFilter);
	m_pDevD3D->SetSamplerState(Stage,D3DSAMP_MINFILTER,Elem->MinFilter);
	m_pDevD3D->SetSamplerState(Stage,D3DSAMP_MIPFILTER,Elem->MipFilter);
	m_pDevD3D->SetSamplerState(Stage,D3DSAMP_MAXANISOTROPY,Elem->Anisotropy);

	//Set Texture Color Operation Mode (Default Set To Multiply All Textures)
	m_pDevD3D->SetTextureStageState(Stage, D3DTSS_COLORARG1, D3DTA_CURRENT);
	m_pDevD3D->SetTextureStageState(Stage, D3DTSS_ALPHAARG1, D3DTA_CURRENT);
	m_pDevD3D->SetTextureStageState(Stage, D3DTSS_COLORARG2, D3DTA_TEXTURE);
	m_pDevD3D->SetTextureStageState(Stage, D3DTSS_ALPHAARG2, D3DTA_TEXTURE);
	m_pDevD3D->SetTextureStageState(Stage, D3DTSS_COLOROP, D3DTOP_MODULATE);
	m_pDevD3D->SetTextureStageState(Stage, D3DTSS_ALPHAOP, D3DTOP_MODULATE);

	//Enabling Texture Transformation 
	m_pDevD3D->SetTextureStageState(Stage,D3DTSS_TEXTURETRANSFORMFLAGS,D3DTTFF_COUNT4);
	//Setting Texture Matrix, So Texture Coords Can Match OpenGL Texture Coords
	m_pDevD3D->SetTransform((D3DTRANSFORMSTATETYPE)(D3DTS_TEXTURE0+sampler),&mTextureMatrix);

	ActiveSampler = Stage;
	pTexSamplers[Stage] = true;

	return true;
}

//////////////////////////////////////////////////////////////////////////////////
// Setting Texture Address Type. Must be Called After SetTexture Method
void CD3DTextureSystem::SetTextureAddress(TEXADDR U, TEXADDR V, TEXADDR W)
{
	if(U != TEXADDR_NONE)
		m_pDevD3D->SetSamplerState(ActiveSampler, D3DSAMP_ADDRESSU, 
									GetD3DTexAddrFromTEXADDR(U));

	if(V != TEXADDR_NONE)
		m_pDevD3D->SetSamplerState(ActiveSampler, D3DSAMP_ADDRESSU, 
									GetD3DTexAddrFromTEXADDR(V));

	if(W != TEXADDR_NONE)
		m_pDevD3D->SetSamplerState(ActiveSampler, D3DSAMP_ADDRESSU, 
									GetD3DTexAddrFromTEXADDR(W));
}

//////////////////////////////////////////////////////////////////////////////////
// Disabling All Texture stages. Getting Information From pTexSamplers Array
void CD3DTextureSystem::DisableTextures()
{
	for(DWORD i=0; i<m_pDevCaps->MaxSimultaneousTextures; i++)
	{
		if(pTexSamplers[i])
		{
			m_pDevD3D->SetTexture(i,NULL);
			m_pDevD3D->SetTextureStageState(i,D3DTSS_TEXTURETRANSFORMFLAGS,D3DTTFF_DISABLE);
			m_pDevD3D->SetTextureStageState(i,D3DTSS_COLOROP, D3DTOP_DISABLE);
			m_pDevD3D->SetTextureStageState(i,D3DTSS_ALPHAOP, D3DTOP_DISABLE);
			pTexSamplers[i] = false;
		}
	}
}

///////////////////////////////////////////////////////////////////////////////////
// Method For Setting Effect Texture
bool CD3DTextureSystem::SetTexture(LPD3DXEFFECT pEffect,const char* name, uint TexIdx,
								   uint Sampler)
{
	if(!pEffect && !name)
		return false;

	if(Sampler >= m_pDevCaps->MaxSimultaneousTextures)
		return false;

	sD3DTexture *Elem = TextureArray.GetElement(TexIdx);
	if(!Elem)
		return false;

	//Setting Texture
	if(Elem->pTex)
		pEffect->SetTexture(name,Elem->pTex);
	else if(Elem->pCubeTex)
		pEffect->SetTexture(name,Elem->pCubeTex);
	else
		pEffect->SetTexture(name,Elem->pVolTex);

	
	return true;
}


//////////////////////////////////////////////////////////////////////////////////
// Check If Texture Is 1D
bool CD3DTextureSystem::Is1DMap(uint texind)
{
	sD3DTexture *Elem = TextureArray.GetElement(texind);
	if(!Elem)
		return false;

	if(Elem->is1D)
		return true;

	return false;
}
//////////////////////////////////////////////////////////////////////////////////
// Check If Texture Is 2D
bool CD3DTextureSystem::Is2DMap(uint texind)
{
	sD3DTexture *Elem = TextureArray.GetElement(texind);
	if(!Elem)
		return false;

	if(Elem->pTex && !Elem->is1D)
		return true;

	return false;
}
//////////////////////////////////////////////////////////////////////////////////
// Check If Texture Is 3D
bool CD3DTextureSystem::Is3DMap(uint texind)
{
	sD3DTexture *Elem = TextureArray.GetElement(texind);
	if(!Elem)
		return false;

	if(Elem->pVolTex)
		return true;

	return false;
}
/////////////////////////////////////////////////////////////////////////////////
// Check If Texture Is CubeMap
bool CD3DTextureSystem::IsCubeMap(uint texind)
{
	sD3DTexture *Elem = TextureArray.GetElement(texind);
	if(!Elem)
		return false;

	if(Elem->pCubeTex)
		return true;

	return false;
}


/////////////////////////////////////////////////////////////////////////////////
// Creating Texture From CImage Class
uint CD3DTextureSystem::CreateTexture(CImage *Img, bool genMipMaps)
{
	if(!Img)
		return 0;

	D3DFORMAT fmt = GetD3DFmtFromImageFmt(Img->getFormat());

	if(fmt == D3DFMT_UNKNOWN)
		return 0;

	sD3DTexture Elem;
	memset(&Elem,0,sizeof(sD3DTexture));
	Elem.genMipMaps = genMipMaps;
	Elem.pCubeTex = NULL;
	Elem.pVolTex = NULL;
	Elem.pTex = NULL;
	Elem.MagFilter = D3DTEXF_POINT;
	Elem.MinFilter = D3DTEXF_POINT;
	Elem.MipFilter = D3DTEXF_POINT;
	Elem.RenderTargetSurface = NULL;
	Elem.DepthStencilSurface = NULL;

	DWORD usage = 0;//D3DUSAGE_DYNAMIC;
	if(genMipMaps)
		usage |= D3DUSAGE_AUTOGENMIPMAP;

	if(Img->is1D())
	{
		uint width = uint(Img->getWidth());
		if(!IsPowerOf2(width))
			return 0;

		if(width > uint(m_pDevCaps->MaxTextureWidth))
			return 0; 

		Elem.is1D = true;

		if(D3DXCreateTexture(m_pDevD3D,width,1,
			(genMipMaps) ? (0) : (1), usage, fmt, D3DPOOL_MANAGED, &Elem.pTex)!=D3D_OK)
			return 0;
	}
	else if(Img->is2D())
	{
		uint width = uint(Img->getWidth());
		uint height = uint(Img->getHeight());

		if((!IsPowerOf2(width)) || (!IsPowerOf2(height)))
			return 0;
		if(width>m_pDevCaps->MaxTextureWidth || height>m_pDevCaps->MaxTextureHeight)
			return 0;

		if(Img->getImageCount()==6)
		{
			if(width != height)
				return 0;

			if(D3DXCreateCubeTexture(m_pDevD3D, width, (genMipMaps) ? (0) : (1),
				usage, fmt, D3DPOOL_MANAGED, &Elem.pCubeTex) != D3D_OK)
				return 0;
		}
		else
		{
			if(D3DXCreateTexture(m_pDevD3D,width,height,
			(genMipMaps) ? (0) : (1), usage, fmt, D3DPOOL_MANAGED, &Elem.pTex)!=D3D_OK)
				return 0;
		}
	}
	else if(Img->is3D())
	{
		uint width = uint(Img->getWidth());
		uint height = uint(Img->getHeight());
		uint depth = uint(Img->getDepth());

		if((!IsPowerOf2(width)) || (!IsPowerOf2(height)) || (!IsPowerOf2(depth)))
			return 0;

		if(width>m_pDevCaps->MaxTextureWidth || height>m_pDevCaps->MaxTextureHeight
			|| depth>m_pDevCaps->MaxVolumeExtent)
			return 0;

		if(D3DXCreateVolumeTexture(m_pDevD3D,width,height,depth,
			(genMipMaps) ? (0) : (1), 0, fmt, D3DPOOL_MANAGED, &Elem.pVolTex)!=D3D_OK)
			return 0;
	}
	else
		return 0;

	if(!this->SetTextureData(Img,Elem))
		return 0;

	return TextureArray.AddElement(Elem);
}


//////////////////////////////////////////////////////////////////////////////////
// Class Inside Method
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
// Creating All Types Of Texture From CImage
bool CD3DTextureSystem::SetTextureData(CImage *Img, sD3DTexture &sTex)
{
	if(!Img)
		return false;

	D3DFORMAT format = GetD3DFmtFromImageFmt(Img->getFormat());
	uint w = uint(Img->getWidth());
	uint h = uint(Img->getHeight());
	//1D And 2D Images
	if(sTex.pTex)
	{
		D3DLOCKED_RECT LockedRect;
		
		if(sTex.pTex->LockRect(0,&LockedRect,NULL,D3DLOCK_DISCARD) != D3D_OK)
			return false;

		if(!FillLockedRect(Img->getPixels(),w,h,&LockedRect,format))
			return false;
		
		if(sTex.pTex->UnlockRect(0) != D3D_OK)
			return false;

		if(sTex.genMipMaps)
			sTex.pTex->GenerateMipSubLevels();
		
	}
	// Cube Map
	else if(sTex.pCubeTex)
	{
		D3DLOCKED_RECT LockedRect;
		D3DCUBEMAP_FACES face;
		for(uint i=0; i<6; i++)
		{
			face = (D3DCUBEMAP_FACES)i;
			if(sTex.pCubeTex->LockRect(face,0,&LockedRect,NULL,D3DLOCK_DISCARD)!=D3D_OK)
				return false;

			if(!FillLockedRect(Img->getPixels(i+1),w,h,&LockedRect,format))
				return false;

			if(sTex.pCubeTex->UnlockRect(face,0)!=D3D_OK)
				return false;

			if(sTex.genMipMaps)
				sTex.pCubeTex->GenerateMipSubLevels();
		}
	}
	// Volume Texture
	else if(sTex.pVolTex)
	{
		D3DLOCKED_BOX LockedVolume;
		uint d = uint(Img->getDepth());
		if(sTex.pVolTex->LockBox(0,&LockedVolume,NULL,D3DLOCK_DISCARD)!=D3D_OK)
			return false;

		if(!FillLockedBox(Img->getPixels(),w,h,d,&LockedVolume,format))
			return false;

		if(sTex.pVolTex->UnlockBox(0)!=D3D_OK)
			return false;

		if(sTex.genMipMaps)
			sTex.pVolTex->GenerateMipSubLevels();
	}
	else
		return false;


	return true;
}

//////////////////////////////////////////////////////////////////////////////////
// Fill With Data Locked Texture Rectangle
bool CD3DTextureSystem::FillLockedRect(void *data,  uint w, uint h,
									   D3DLOCKED_RECT *LockedRect, D3DFORMAT format)
{

	switch(format)
	{
	case D3DFMT_L8:
	case D3DFMT_L16:
		{
			uint fmtsize = (format==D3DFMT_L8)?(sizeof(ubyte)):(sizeof(ushort));
			ubyte *ptr = (ubyte*)LockedRect->pBits;
			ubyte *pix = (ubyte*)data;

			for(uint y=0; y<h; y++)
			{
				memcpy(ptr, pix, fmtsize*w);
				ptr += LockedRect->Pitch;
				pix += w*fmtsize;
			}
		}
		return true;

	case D3DFMT_X8R8G8B8:
	case D3DFMT_A8R8G8B8:
		{
			ubyte *ptr;
			ubyte *pix = (ubyte*)data;
			ubyte PixOffset = (format==D3DFMT_A8R8G8B8)?(4):(3);
			D3DCOLOR d3dclr;

			for(uint y=0; y<h; y++)
			{
				ptr = (ubyte*)LockedRect->pBits + LockedRect->Pitch*y;
				
				for(uint x=0; x<w; x++)
				{
					if(format==D3DFMT_X8R8G8B8)
						d3dclr = D3DCOLOR_XRGB(pix[0],pix[1],pix[2]);
					else
						d3dclr = D3DCOLOR_ARGB(pix[3],pix[0],pix[1],pix[2]);
					
					memcpy(ptr,&d3dclr,sizeof(D3DCOLOR));

					ptr+=4;
					pix += PixOffset;
				}
			}
		}
		return true;

	case D3DFMT_A16B16G16R16:
		{
			ushort *ptr = (ushort*)LockedRect->pBits;
			ushort *pix = (ushort*)data;

			uint offset = LockedRect->Pitch/8 - w;

			for(uint y=0; y<h; y++)
			{
				ptr = (ushort*)((ubyte*)LockedRect->pBits + LockedRect->Pitch*y);
				for(uint x=0; x<w; x++)
				{
					ptr[0] = pix[3];
					ptr[1] = pix[2];
					ptr[2] = pix[1];
					ptr[3] = pix[0];

					//ptr+=4;
					pix+=4;
				}
				//pix += offset*4;
			}
		}
		return true;

	case D3DFMT_A16B16G16R16F:
		{
			float *ptr = (float*)LockedRect->pBits;
			float *pix = (float*)data;

			for(uint y=0; y<h; y++)
			{
				ptr = (float*)((ubyte*)LockedRect->pBits + LockedRect->Pitch*y);
				for(uint x=0; x<w; x++)
				{
					ptr[0] = pix[3];
					ptr[1] = pix[2];
					ptr[2] = pix[1];
					ptr[3] = pix[0];

					ptr+=4;
					pix+=4;
				}
				//pix += offset*4;
			}
		}
		return true;
	}

	return false;
}

//////////////////////////////////////////////////////////////////////////////////
// Fill New Data To Texture Locked Box
bool CD3DTextureSystem::FillLockedBox(void *data, uint w, uint h, uint d, 
									  D3DLOCKED_BOX *LockedBox, D3DFORMAT format)
{

	switch(format)
	{
	case D3DFMT_L8:
	case D3DFMT_L16:
		{
			uint fmtsize = (format==D3DFMT_L8)?(sizeof(ubyte)):(sizeof(ushort));
			ubyte *ptr = (ubyte*)LockedBox->pBits;
			ubyte *pix = (ubyte*)data;

			for(uint z=0; z<d; z++)
			{
				for(uint y=0; y<h; y++)
				{
					ptr = ((ubyte*)LockedBox->pBits + LockedBox->SlicePitch*z + 
						LockedBox->RowPitch*y);
					memcpy(ptr, pix, fmtsize*w);
					pix+=w*fmtsize;
				}
			}
		}
		return true;

	case D3DFMT_X8R8G8B8:
	case D3DFMT_A8R8G8B8:
		{
			ubyte *ptr;
			ubyte *pix = (ubyte*)data;
			ubyte offset = (format==D3DFMT_A8R8G8B8)?(4):(3);
			D3DCOLOR d3dclr;

			for(uint z=0; z<d; z++)
			{
				for(uint y=0; y<h; y++)
				{
					ptr = (ubyte*)LockedBox->pBits + LockedBox->SlicePitch*z 
						+ LockedBox->RowPitch*y;

					for(uint x=0; x<w; x++)
					{
						if(format==D3DFMT_A8R8G8B8)
							d3dclr = D3DCOLOR_ARGB(pix[3],pix[0],pix[1],pix[2]);
						else
							d3dclr = D3DCOLOR_XRGB(pix[0],pix[1],pix[2]);

						memcpy(ptr,&d3dclr,sizeof(D3DCOLOR));
						ptr+=4;
						pix+=offset;
					}
				}
			}
		}
		return true;

	case D3DFMT_A16B16G16R16:
		{
			ushort *ptr;
			ushort *pix = (ushort*)data;

			for(uint z=0; z<d; z++)
			{
				for(uint y=0; y<h; y++)
				{
					ptr = (ushort*)LockedBox->pBits + LockedBox->SlicePitch*z + 
						LockedBox->RowPitch*y;
					for(uint x=0; x<w; x++)
					{
						ptr[0] = pix[3];
						ptr[1] = pix[2];
						ptr[2] = pix[1];
						ptr[3] = pix[0];

						pix+=4;
					}
				}
			}
		}
		return true;

	case D3DFMT_A16B16G16R16F:
		{
			float *ptr;
			float *pix = (float*)data;

			for(uint z=0; z<d; z++)
			{
				for(uint y=0; y<h; y++)
				{
					ptr = (float*)LockedBox->pBits + LockedBox->SlicePitch*z + 
						LockedBox->RowPitch*y;
					for(uint x=0; x<w; x++)
					{
						ptr[0] = pix[3];
						ptr[1] = pix[2];
						ptr[2] = pix[1];
						ptr[3] = pix[0];

						pix+=4;
					}
				}
			}
		}
		return true;
	}

	return false;
}