///////////////////////////////////////////////////////////////////////////////////////////////
//	Image.h
//	Image Class Declaration
//	
////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _BMIMAGE_H_
#define _BMIMAGE_H_

#include <PlatformTypes.h>
#include "ImageFormat.h"

class memory_allocator;

////////////////////////////////////////////////////////////////////////////////////////////////
// Bitmap Image Class
class CBMImage
{
public:
    CBMImage(BMIFMT fmt, uint32t w, uint32t h, uint32t align=0);
    //CBMImage(BMIFMT fmt, uint32t w, uint32t h, uint32t d, uint32t align=0);
    //CBMImage(BMIFMT fmt, uint32t w, uint32t h, uint32t d, uint32t align=0, uint32t mipmaps=0,uint32t imgnum=1);
    CBMImage(BMIFMT fmt, uint32t w, uint32t h, uint32t d, BMIFMT palfmt, uint32t palclrnum, uint32t infosize, uint32t align=0,uint32t mipmaps=0,uint32t imgnum=1);
    virtual ~CBMImage();

    ubyte* GetPixels() const {return Pixels;}
    ubyte* GetPixels(uint32t miplevel, uint32t img=0) const;

    uint32t GetWidth() const {return Width;}
    uint32t GetHeight() const {return Height;}
    uint32t GetDepth() const {return Depth;}
    uint32t GetStride() const {return Stride;}

    uint32t GetWidth(uint32t miplevel) const;
    uint32t GetHeight(uint32t miplevel) const;
    uint32t GetDepth(uint32t miplevel) const;
    uint32t GetStride(uint32t miplevel) const;

    BMIFMT  GetFormat() const {return Format;}

    uint32t GetImageCount() const {return nImages;}
    uint32t GetMipMapCount() const {return nMipMaps;}

    ubyte*  GetPalette() const {return Palette;}
    BMIFMT  GetPaletteFormat() const {return PalFormat;}
    uint32t GetPalColorsNum() const {return nPalColors;}

    ubyte*  GetInfoData() const {return InfoData;}
    uint32t GetInfoDataSize() const {return InfoDataSize;}

protected:
    uint32t Width;
    uint32t Height;
    uint32t Depth;
    uint32t Stride;
    uint32t Align;

    uint32t nImages;
    uint32t nMipMaps;

    BMIFMT  Format;

    uint32t nPalColors;
    BMIFMT  PalFormat;

    uint32t InfoDataSize;

    ubyte   *Pixels;
    ubyte   *Palette;
    ubyte   *InfoData;

    memory_allocator *mem_alloc;
};

////////////////////////////////////////////////////////////////////////////////////////////////
class CBMImgInfo
{
public:
    static uint32t  CalcWidthSize(const BMIFMT fmt, uint32t w);
    static uint32t  CalcStride(const BMIFMT fmt, uint32t w, uint32t align=0);
    static uint32t  CalcFullWidthSize(const BMIFMT fmt, uint32t w, uint32t align=0);

    static uint32t  CalcMipMapsNum(uint32t w, uint32t h, uint32t d);
    //static uint32t  CalcMipMapsFullSize(BMIFMT fmt,uint32t w, uint32t h, uint32t d, uint32t levels, uint32t align=0);
    //static uint32t  CalcMipMapSize(BMIFMT fmt, uint32t w, uint32t h, uint32t d, uint32t level, uint32t align=0);
    static uint32t  CalcImageSize(BMIFMT fmt, uint32t w, uint32t h, uint32t d, uint32t miplevels, uint32t align=0);
    static uint32t  CalcMipImgSize(BMIFMT fmt, uint32t w, uint32t h, uint32t d, uint32t level, uint32t align=0);

    static bool IsImg1D(uint32t w, uint32t h, uint32t d);
    static bool IsImg2D(uint32t w, uint32t h, uint32t d);
    static bool IsImg3D(uint32t w, uint32t h, uint32t d);
    static uint32t  GetImgDim(uint32t w, uint32t h, uint32t d);
};


////////////////////////////////////////////////////////////////////////////////////////////////

#endif //_BMIMAGE_H_