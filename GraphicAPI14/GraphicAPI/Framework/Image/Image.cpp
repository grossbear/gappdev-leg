/////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////
#include "../Types/types.h"
#include "Image.h"
#include <stdlib.h>
#include <memory.h>

#define WIN32_LEAN_AND_MEAN

CImage::CImage()
{
	width=depth=height=pitch=0;
	format=FORMAT_NONE;
	nImages=0;
	nMipMaps=0;
	pixels=NULL;
	palette=NULL;
}

CImage::CImage(const CImage *copy)
{
	width = copy->width;
	height = copy->height;
	depth = copy->depth;
	pitch = copy->pitch;
	format = copy->format;
	nImages = copy->nImages;
	nMipMaps = copy->nMipMaps;

	ulong size = getBytesPerPixel(format)*pitch*height*depth*nImages;
	pixels = new ubyte[size];
	memcpy(pixels,copy->pixels,size);

	palette = NULL;
	if(isPalettedFormat(format))
	{	
		palette = new ubyte[getPaletteSize(format)];
		memcpy(palette,copy->palette,getPaletteSize(format));
	}
}

bool CImage::CopyImage(CImage *copy)
{
	this->Release();
	
	width = copy->width;
	height = copy->height;
	depth = copy->depth;
	pitch = copy->pitch;
	format = copy->format;
	nImages = copy->nImages;
	nMipMaps = copy->nMipMaps;

	ulong size = getBytesPerPixel(format)*pitch*height*depth*nImages;
	pixels = new ubyte[size];
	memcpy(pixels,copy->pixels,size);

	palette = NULL;
	if(isPalettedFormat(format))
	{
		palette = new ubyte[getPaletteSize(format)];
		memcpy(palette,copy->palette,getPaletteSize(format));
	}

	return true;
}

CImage::~CImage()
{
	this->Release();
}

void CImage::Release()
{
	width=height=depth=pitch=0;
	format=FORMAT_NONE;
	nImages=0;
	nMipMaps=0;
	delete [] pixels;
	delete [] palette;
	pixels=NULL;
	palette=NULL;
}

void* CImage::Create(FORMAT fmt,int w,int h, int d,int imgCount,int mipMapCount)
{
	if(fmt == FORMAT_NONE)
		return NULL;
	
	if(isPalettedFormat(fmt))
	{
		//uint sz = getPaletteSize(fmt);
		palette = new ubyte[getPaletteSize(fmt)];
	}
	
	if(w<1)
		w=1;
	if(h<1)
		h=1;
	if(d<1)
		d=1;
	if(imgCount<1)
		imgCount=1;
	if(mipMapCount<1)
		mipMapCount=1;

	format = fmt;
	width = w;
	height = h;
	depth = d;
	nImages = imgCount;
	nMipMaps = mipMapCount;

	pitch = w;

	if((pitch%4) && (pitch>4))
		while(pitch%4)
			pitch++;

	return (pixels=new ubyte[getBytesPerPixel(fmt)*pitch*h*d*nImages]);
}

void* CImage::getPixels(int imgNum)
{
	if(imgNum>nImages || imgNum<1)
		return NULL;

	if(imgNum==1)
		return pixels;

	ubyte* p=pixels;
	return (p+(pitch*height*depth*getBytesPerPixel(format)*(imgNum-1)));
}

void CImage::LoadTestPattern()
{
	this->Release();
	static bool swap=false;
	width=128;
	height=128;
	depth=1;
	pitch=128;
	nImages=1;
	format=FORMAT_GREY8;

	pixels=new ubyte[128*128];
	ubyte *p=pixels;
	for (int i = 0; i < 128; ++i) 
	{
		for (int j = 0; j < 128; ++j) 
		{
			if ((i ^ j) & 16) 
			{
				if(swap)
					p[0] = 0;
				else
					p[0] = 245;
				//p[1] = 0;
				//p[2] = 0;				
			} 
			else 
			{
				if(swap)
					p[0] = 255;
				else
					p[0] = 100;
				//p[1] = 255;
				//p[2] = 255;				
			}
			p += 1;
		}
	}
	swap=!swap;
}


void CImage::setPalette(ubyte *pal, uint size)
{
	if(palette)
		delete [] palette;

	palette = new ubyte[size];
	memcpy(palette,pal,size);	
}

CImage* CImage::getImage(int num)
{
	if(num<1 || num>nImages)
		return NULL;

	CImage* image=new CImage();

	ubyte* data = (ubyte*)image->Create(format,width,height,depth,1,1);
	ubyte* pix  = (ubyte*)this->getPixels(num);
	memcpy(data,pix,pitch*height*depth*getBytesPerPixel(format));

	if(isPalettedFormat(format))
	{
		ubyte* pal= (ubyte*)image->getPalette();
		memcpy(pal,palette,getPaletteSize(format));
	}

	return image;
	/*CImage image;

	if(num<1 || num>nImages)
		return image;

	ubyte* data = (ubyte*)image.Create(format,width,height,depth,1,1);
	ubyte* p=pixels;
	p = p+width*height*depth*(num-1)*getBytesPerPixel(format);

	memcpy(data,p,width*height*depth*getBytesPerPixel(format));

	if(isPalettedFormat(format))
	{
		ubyte* pal=(ubyte*)image.getPalette();
		memcpy(pal,palette,getPaletteSize(format));
	}

	return image;*/
}


void CImage::ExpandPalette(bool ExpandAlphaChannel)
{
	if(!isPalettedFormat(format))
		return ;

	if(format == FORMAT_I8)
	{
		uint nChannels;
		FORMAT newFmt;
		if(ExpandAlphaChannel)
		{
			newFmt = FORMAT_RGBA8;
			nChannels = 4;
		}
		else
		{
			newFmt = FORMAT_RGB8;
			nChannels = 3;
		}

		ubyte *pPixel = pixels;
		ubyte *pPal = palette;
		ubyte *data = new ubyte [pitch*height*depth*nChannels];
		ubyte *ptr = data;

		ubyte index=0;
		for(uint i=0; i<pitch*height*depth*nChannels; i+=nChannels)
		{
			pPal = palette+pPixel[0]*4;
			memcpy(ptr,pPal,sizeof(ubyte)*nChannels);
			ptr+=nChannels;
			pPixel+=1;
		}

		delete [] pixels;
		delete [] palette;

		pixels = data;
		palette = NULL;
		format = newFmt;
	}
}