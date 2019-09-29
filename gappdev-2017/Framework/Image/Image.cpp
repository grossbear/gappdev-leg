///////////////////////////////////////////////////////////////////////////////////////////////
//	Image.cpp
//	Image Class Definition
//	
////////////////////////////////////////////////////////////////////////////////////////////////

#include <stdio.h>
#include <memory.h>
#include <math.h>
#include "Image.h"
#include "Utils/mem/memory_allocator.h"

#define IMGMAX(a,b) (a>b)?(a):(b)
#define IMGMIN(a,b) (a<b)?(a):(b)
#define ISPOW2(n) (n>0) ? (!(n & (n-1))) : (0)

#define LOG2F   0.30102999566398119521373889472449f


////////////////////////////////////////////////////////////////////////////////////////////////
CBMImage::CBMImage(BMIFMT fmt, uint32t w, uint32t h, uint32t align):
Palette(NULL),PalFormat(IMGFMT_NONE),nPalColors(0),InfoDataSize(0),InfoData(NULL),Depth(1),
nImages(1),nMipMaps(0)

{
    ASSERT(fmt != IMGFMT_NONE);

    Format = fmt;
    Width = IMGMAX(w,1);
    Height = IMGMAX(h,1);

    Align = align;

    Stride = (Align>0) ? ( ((Width%Align)>0)?(Align-Width%Align):(0) ) : (0);

    if(CBMImgFmtInfo::IsCompressedFormat(Format))
    {
        Width += CBMImgInfo::CalcStride(fmt,w,4);
        Height += CBMImgInfo::CalcStride(fmt,h,4);
        nMipMaps = IMGMIN(CBMImgInfo::CalcMipMapsNum(Width,Height,Depth),nMipMaps);
        Align = 0;
        Stride = 0;
    }

    //Calc Image Size
    uint32t imgsize = CBMImgInfo::CalcImageSize(Format,Width,Height,Depth,nMipMaps,align);

    //Getting Memory Allocator
    mem_alloc = memory_allocator_selector::get_allocator(img_alloc,(uisize_t)imgsize);

    ASSERT(mem_alloc);

    //Allocating Memory
    Pixels = (ubyte*)mem_alloc->alloc((uisize_t)imgsize);
}

////////////////////////////////////////////////////////////////////////////////////////////////
/*CBMImage::CBMImage(BMIFMT fmt, uint32t w, uint32t h, uint32t d, uint32t align)
{
    CBMImage(fmt,w,h,d,IMGFMT_NONE,0,0,align,0,1);
}*/

////////////////////////////////////////////////////////////////////////////////////////////////
/*CBMImage::CBMImage(BMIFMT fmt, uint32t w, uint32t h, uint32t d, uint32t align, uint32t mipmaps,uint32t imgnum)
{
    CBMImage(fmt,w,h,d,IMGFMT_NONE,0,0,align,mipmaps,imgnum);
}*/

////////////////////////////////////////////////////////////////////////////////////////////////
CBMImage::CBMImage(BMIFMT fmt, uint32t w, uint32t h, uint32t d, BMIFMT palfmt, uint32t palclrnum, uint32t infosize, 
                   uint32t align,uint32t mipmaps,uint32t imgnum):
Palette(NULL),PalFormat(IMGFMT_NONE),nPalColors(0),InfoDataSize(0),InfoData(NULL)

{
    ASSERT(fmt != IMGFMT_NONE);

    Format = fmt;
    Width = IMGMAX(w,1);
    Height = IMGMAX(h,1);
    Depth = IMGMAX(d,1);
    nImages = IMGMAX(imgnum,1);
    nMipMaps = IMGMIN(CBMImgInfo::CalcMipMapsNum(Width,Height,Depth),mipmaps);
    Align = align;

    Stride = (Align>0) ? ( ((Width%Align)>0)?(Align-Width%Align):(0) ) : (0);

    if(CBMImgFmtInfo::IsCompressedFormat(Format))
    {
        Width += CBMImgInfo::CalcStride(fmt,w,4);
        Height += CBMImgInfo::CalcStride(fmt,h,4);
        nMipMaps = IMGMIN(CBMImgInfo::CalcMipMapsNum(Width,Height,Depth),mipmaps);
        Align = 0;
        Stride = 0;
    }

    //Calc Image Size
    uint32t imgsize = CBMImgInfo::CalcImageSize(Format,Width,Height,Depth,nMipMaps,align);
    imgsize *= nImages;

    //Set Palette Info
    uint32t palsize = 0;
    if(palfmt!=IMGFMT_NONE && palclrnum > 2 && !CBMImgFmtInfo::IsCompressedFormat(palfmt))
    {
        PalFormat = palfmt;
        nPalColors = palclrnum;
        uint32t pixsize = CBMImgFmtInfo::GetBytesPerPixel(PalFormat);
        palsize = pixsize*nPalColors;
    }

    //Additional Image Info Size
    InfoDataSize = infosize;

    uint32t fullsize = imgsize + palsize + InfoDataSize;

    //Getting Memory Allocator
    mem_alloc = memory_allocator_selector::get_allocator(img_alloc,(uisize_t)fullsize);

    ASSERT(mem_alloc);

    //Allocating Memory
    Pixels = (ubyte*)mem_alloc->alloc((uisize_t)fullsize);
    if(PalFormat != IMGFMT_NONE)
        Palette = Pixels + imgsize;
    if(InfoDataSize)
        InfoData = Pixels + imgsize + palsize;

}

////////////////////////////////////////////////////////////////////////////////////////////////
CBMImage::~CBMImage()
{
    mem_alloc->dealloc(Pixels);

    Pixels = NULL;
    Palette = NULL;
    InfoData = NULL;
}

////////////////////////////////////////////////////////////////////////////////////////////////
ubyte* CBMImage::GetPixels(uint32t miplevel, uint32t img) const
{
    if(miplevel > nMipMaps || img >= nImages) return NULL;

    uint32t imgmips = (miplevel==0)?(0):(CBMImgInfo::CalcImageSize(Format,Width,Height,Depth,miplevel-1,Align));
    uint32t imgsize = CBMImgInfo::CalcImageSize(Format,Width,Height,Depth,nMipMaps,Align);

    return Pixels + img * imgsize + imgmips;
}

////////////////////////////////////////////////////////////////////////////////////////////////
uint32t CBMImage::GetWidth(uint32t miplevel) const
{
    if(miplevel > nMipMaps) return 0;

    uint32t mipwidth = Width >> miplevel;
    mipwidth = IMGMAX(mipwidth,1);
    return mipwidth;
}

////////////////////////////////////////////////////////////////////////////////////////////////
uint32t CBMImage::GetHeight(uint32t miplevel) const
{
    if(miplevel > nMipMaps) return 0;

    uint32t mipheight = Height >> miplevel;
    mipheight = IMGMAX(mipheight,1);
    return mipheight;
}

////////////////////////////////////////////////////////////////////////////////////////////////
uint32t CBMImage::GetDepth(uint32t miplevel) const
{
    if(miplevel > nMipMaps) return 0;
    uint32t mipdepth = Depth >> miplevel;
    mipdepth = IMGMAX(mipdepth,1);
    return mipdepth;
}
////////////////////////////////////////////////////////////////////////////////////////////////
uint32t CBMImage::GetStride(uint32t miplevel) const
{
    if(Align==0) return 0;
    uint32t mipwidth = GetWidth(miplevel);
    if(mipwidth==0) return 0;

    return ((mipwidth%Align)>0) ? (Align - mipwidth%Align) : (0);
}


////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////
uint32t CBMImgInfo::CalcWidthSize(const BMIFMT fmt, uint32t w)
{
    uint32t width = w * CBMImgFmtInfo::GetBytesPerPixel(fmt);
    return width;
}

////////////////////////////////////////////////////////////////////////////////////////////////
uint32t CBMImgInfo::CalcStride(const BMIFMT fmt, uint32t w, uint32t align)
{
    if(align==0) return 0;

    uint32t mod = w*CBMImgFmtInfo::GetBytesPerPixel(fmt) % align;
    if(mod == 0) return 0;

    uint32t stride = align - mod;

    return stride;
}

////////////////////////////////////////////////////////////////////////////////////////////////
//Not For Compressed Formats
uint32t CBMImgInfo::CalcFullWidthSize(const BMIFMT fmt, uint32t w, uint32t align)
{
    uint32t width = w * CBMImgFmtInfo::GetBytesPerPixel(fmt);
    uint32t stride = CBMImgInfo::CalcStride(fmt,w,align);
    return width+stride;
}

////////////////////////////////////////////////////////////////////////////////////////////////
uint32t CBMImgInfo::CalcMipMapsNum(uint32t w, uint32t h, uint32t d)
{
    uint32t maxval = IMGMAX(w,(uint32t)IMGMAX(h,d));
    float fmipnum = log10f((float)w) / LOG2F;
    uint32t mipnum = (uint32t) fmipnum; //(uint32t) (log10f(float(IMGMAX(w,(uint32t)IMGMAX(h,d)))) / LOG2F);
 
    return mipnum;
}

////////////////////////////////////////////////////////////////////////////////////////////////
/*uint32t CBMImgInfo::CalcMipMapsFullSize(BMIFMT fmt, uint32t w, uint32t h, uint32t d, uint32t levels, uint32t align)
{
    bool ispow2 = ISPOW2(w) && ISPOW2(h) && ISPOW2(d);
    if(!ispow2) return 0;

    uint32t mipnum = (uint32t) (logf(float(IMGMAX(w,(uint32t)IMGMAX(h,d)))) / LOG2F);
    mipnum = (levels < mipnum) ? (levels) : (mipnum);

    uint32t fullsize = 0;
    for(uint32t i = 0; i < mipnum; i++)
    {
        w = IMGMAX(w>>1,1);
        h = IMGMAX(h>>1,1);
        d = IMGMAX(d>>1,1);

        uint32t mipwidth = CBMImgInfo::CalcFullWidthSize(fmt,w,align);
        fullsize += mipwidth*h*d;
    }

    return fullsize;
}*/

////////////////////////////////////////////////////////////////////////////////////////////////
/*uint32t CBMImgInfo::CalcMipMapSize(BMIFMT fmt, uint32t w, uint32t h, uint32t d, uint32t level, uint32t align)
{
    bool ispow2 = ISPOW2(w) && ISPOW2(h) && ISPOW2(d);
    if(!ispow2) return 0;

    uint32t mipnum = (uint32t) (logf(float(IMGMAX(w,(uint32t)IMGMAX(h,d)))) / LOG2F);

    if(level > mipnum) return 0;

    for(uint32t i = 0; i < level; i++)
    {
        w = IMGMAX(w>>1,1);
        h = IMGMAX(h>>1,1);
        d = IMGMAX(d>>1,1);
    }

    uint32t mipwidth = CBMImgInfo::CalcFullWidthSize(fmt,w,align);
    uint32t mipmapsize = mipwidth*h*d;

    return mipmapsize;
}*/

////////////////////////////////////////////////////////////////////////////////////////////////
uint32t CBMImgInfo::CalcImageSize(BMIFMT fmt, uint32t w, uint32t h, uint32t d, uint32t miplevels, uint32t align)
{
    //bool ispow2 = ISPOW2(w) && ISPOW2(h) && ISPOW2(d);
    uint32t mipnum = CBMImgInfo::CalcMipMapsNum(w,h,d); //(uint32t) (log10f(float(IMGMAX(w,(uint32t)IMGMAX(h,d)))) / LOG2F);
    if(miplevels > mipnum) return 0;
    //if(miplevels > 0 && !ispow2) return 0;
    if(CBMImgFmtInfo::IsCompressedFormat(fmt) && (w%4 || h%4)) return 0;

    uint32t size = 0;
    uint32t level = miplevels + 1;
    while(level)
    {
        uint32t stride = CBMImgInfo::CalcStride(fmt,w,align);
        uint32t fullwidth = w + stride;
        if(CBMImgFmtInfo::IsCompressedFormat(fmt))
            size += ((w+3)>>2) *  ((h+3)>>2) * d;
        else
            size += fullwidth * h * d;

        w = IMGMAX(w>>1,1);
        h = IMGMAX(h>>1,1);
        d = IMGMAX(d>>1,1);
        level--;
    }

    if(CBMImgFmtInfo::IsCompressedFormat(fmt))
        size *= (uint32t)CBMImgFmtInfo::GetBytesPerBlock(fmt);
    else
        size *= (uint32t)CBMImgFmtInfo::GetBytesPerPixel(fmt);

    return size;
}

////////////////////////////////////////////////////////////////////////////////////////////////
uint32t CBMImgInfo::CalcMipImgSize(BMIFMT fmt, uint32t w, uint32t h, uint32t d, uint32t level, uint32t align)
{
    uint32t miplevels = CBMImgInfo::CalcMipMapsNum(w,h,d); //(uint32t) (log10f(float(IMGMAX(w,(uint32t)IMGMAX(h,d)))) / LOG2F);
    if(level > miplevels) return 0;
    if(CBMImgFmtInfo::IsCompressedFormat(fmt) && (w%4 || h%4)) return 0;

    w = IMGMAX(w>>level,1);
    h = IMGMAX(h>>level,1);
    d = IMGMAX(d>>level,1);

    uint32t stride = CBMImgInfo::CalcStride(fmt,w,align);
    uint32t fullwidth = w + stride;

    uint32t size = 0;

    if(CBMImgFmtInfo::IsCompressedFormat(fmt))
        size = (((w+3)>>2) * ((h+3)>>2) * d) * CBMImgFmtInfo::GetBytesPerBlock(fmt);
    else
        size = fullwidth * h * d * CBMImgFmtInfo::GetBytesPerPixel(fmt);

    return size;
}


////////////////////////////////////////////////////////////////////////////////////////////////
bool CBMImgInfo::IsImg1D(uint32t w, uint32t h, uint32t d)
{
    return h == 1 && d == 1;
}
////////////////////////////////////////////////////////////////////////////////////////////////
bool CBMImgInfo::IsImg2D(uint32t w, uint32t h, uint32t d)
{
    return h > 1 && d == 1;
}
////////////////////////////////////////////////////////////////////////////////////////////////
bool CBMImgInfo::IsImg3D(uint32t w, uint32t h, uint32t d)
{
    return h > 1 && d > 1;
}
////////////////////////////////////////////////////////////////////////////////////////////////
uint32t CBMImgInfo::GetImgDim(uint32t w, uint32t h, uint32t d)
{
    uint32t dim = 1;
    if(h > 1)
    {
        dim += 1;
        if(d > 1) dim += 1;
    }
    return dim;
}

////////////////////////////////////////////////////////////////////////////////////////////////
