///////////////////////////////////////////////////////////////////////////////////////////////
//	BMP.cpp
//	BMP Format Loading/Saving Class Definition
//	
////////////////////////////////////////////////////////////////////////////////////////////////

#include "Utils/mem/memory_allocator.h"
#include "Utils/IOStream/IDataStream.h"
#include "Image/Image.h"
#include "BMP.h"

#include <memory.h>

// constants for the biCompression field 
#define BI_RGB        0L
#define BI_RLE8       1L
#define BI_RLE4       2L
#define BI_BITFIELDS  3L
#define BI_JPEG       4L
#define BI_PNG        5L

#define MAX_WIDTH       32768
#define MAX_HEIGHT      32768

#pragma pack (push,1)

// Bitmap Header
struct BMPHeader {
	ushort bfType;
	uint   bfSize;
	ushort bfReserved1;
	ushort bfReserved2;
	uint   bfOffBits;
	uint   biSize;
	uint   biWidth;
	uint   biHeight;
	ushort biPlanes;
	ushort biBitCount;
	uint   biCompression;
	uint   biSizeImage;
	uint   biXPelsPerMeter;
	uint   biYPelsPerMeter;
	uint   biClrUsed;
	uint   biClrImportant;
};

#pragma pack (pop)



////////////////////////////////////////////////////////////////////////////////////////////////
char CBMPImageIO::m_strInfo[] = {"Bitmap File"};
////////////////////////////////////////////////////////////////////////////////////////////////
char CBMPImageIO::m_strExt[] = {"bmp"};

////////////////////////////////////////////////////////////////////////////////////////////////
CBMPImageIO::CBMPImageIO()
{
}

////////////////////////////////////////////////////////////////////////////////////////////////
CBMPImageIO::~CBMPImageIO()
{
}

////////////////////////////////////////////////////////////////////////////////////////////////
const char* CBMPImageIO::GetInfoString(uint32t idx) const
{
    return &m_strInfo[0];
}

////////////////////////////////////////////////////////////////////////////////////////////////
const char* CBMPImageIO::GetExtString(uint32t idx) const
{
    return &m_strExt[0];
}

////////////////////////////////////////////////////////////////////////////////////////////////
CBMImage* CBMPImageIO::LoadBMImage(IDataStream *datastream, uint32t loadflags)
{
    //Check If IDataStream Is Not NULL Pointer   
    if(datastream == NULL)
        return NULL;

    //Check If IO Stream Is Opened
    if(!datastream->IsOpened())
        return NULL;

    BMPHeader bmpheader = {0};
    //Read BMP File Header
    uint32t ireaded = datastream->Read(&bmpheader,sizeof(BMPHeader));
    if(ireaded < sizeof(BMPHeader))
        return NULL;

    //Check If File Header Is Correct
    if(!CheckFileHeader(bmpheader))
        return NULL;

    //Reading Palette Data
    ubyte palette[1024]={0};
    uint32t palsize = GetPaletteSize(bmpheader);
    if(palsize > 0)
    {
        uint32t ireaded = datastream->Read(&palette[0],palsize);
        if(ireaded != palsize)
            return NULL;
    }

    isize_t dspos = datastream->GetPos();
    isize_t offset = (isize_t)bmpheader.bfOffBits;
    if(dspos > offset)//If We Are Far From Offset
        return NULL;

    CBMImage *image = NULL;
    switch(bmpheader.biBitCount)
    {
    case 32:
    case 24:
        image = LoadRGBImage(datastream, bmpheader, loadflags);
        break;

    case 8:
        image = Load8BitImage(datastream, bmpheader, palette, palsize>>2, loadflags);
        break;

    case 4:
        image = Load4BitImage(datastream, bmpheader, palette, loadflags);
        break;

    case 1:
        image = Load1BitImage(datastream, bmpheader, palette, loadflags);
        break;

    }

    if(image && (loadflags & IMGIO_BGR_TO_RGB))
    {
        BMIFMT fmt = image->GetFormat();
        uint32t channels = CBMImgFmtInfo::GetChannelsCount(fmt);
        if(channels > 3)
        {
            RevertChannels(image->GetPixels(),image->GetWidth(),image->GetHeight(),image->GetStride(),channels);
        }
    }

    return image;
}

////////////////////////////////////////////////////////////////////////////////////////////////
bool CBMPImageIO::SaveBMImage(IDataStream *datastream, const CBMImage *image, ImgIOParam *params)
{
    //Check If IDataStream Is Not NULL Pointer   
    if(datastream == NULL)
        return NULL;

    //Check If IO Stream Is Opened
    if(!datastream->IsOpened())
        return NULL;

    BMIFMT imgfmt = image->GetFormat();
    if(imgfmt != IMGFMT_L8 && imgfmt != IMGFMT_RGB8 && imgfmt != IMGFMT_RGBA8 &&
        imgfmt != IMGFMT_BGR8 && imgfmt != IMGFMT_BGRA8)
        return false;

    BMIFMT palfmt = image->GetFormat();
    uint32t palclrnum = image->GetPalColorsNum();
    /*if((imgfmt == IMGFMT_L8) && (palfmt != IMGFMT_RGB8 || palfmt != IMGFMT_RGBA8 ||
        palfmt != IMGFMT_BGR8 || palfmt != IMGFMT_BGRA8))
    {
        return false;
    }*/
    palclrnum = ((imgfmt == IMGFMT_L8) && (palclrnum==0)) ? (256) : (palclrnum);
    

    uint32t width = image->GetWidth();
    uint32t height = image->GetHeight();
    uint32t depth = image->GetDepth();
    uint32t imgstride = image->GetStride();
    uint32t infosize = image->GetInfoDataSize();

    BMPHeader bmpheader = {0};
    SetFileHeader(bmpheader,width,height,imgfmt,palclrnum,infosize);
    datastream->Write(&bmpheader,sizeof(BMPHeader));

    //Writing Palette If Image Format Is IMGFMT_L8
    if(imgfmt == IMGFMT_L8)
    {
        ubyte *palptr = image->GetPalette();
        ubyte palette[1024];
        memset(palette,0xff,1024);
        palclrnum = 256; //--(palclrnum>256)?(256):(palclrnum);
        if(palptr)
        {
            
            if(palfmt == IMGFMT_RGB8 || palfmt == IMGFMT_BGR8)
            {
                ubyte *in  = palptr;
                ubyte *out = palette;
                for(uint32t i = 0; i < palclrnum; i++)
                {
                    out[0] = in[0];
                    out[1] = in[1];
                    out[2] = in[2];
                    out += 4;
                    in += 3;
                }
            }
            else
            {
                memcpy(palette,palptr,palclrnum*4*sizeof(ubyte));
            }

            if(palfmt == IMGFMT_RGBA8 || palfmt == IMGFMT_RGB8)
                RevertChannels(palette,palclrnum,1,0,4);
        }
        else
        {
            ubyte *ptr = palette;
            for(uint32t i = 0; i < palclrnum; i++)
            {
                ubyte val = (ubyte)i;
                *ptr++ = val;
                *ptr++ = val;
                *ptr++ = val;
                ptr++;
            }
        }


        datastream->Write(palette,palclrnum*4*sizeof(ubyte));
    }

    //Writing Extra Desc Data
    if(infosize)
    {
        datastream->Write(image->GetInfoData(),infosize);
    }
    
    uint32t widthsize = CBMImgInfo::CalcFullWidthSize(imgfmt,width,0);
    uint32t stride = CBMImgInfo::CalcStride(imgfmt,width,4);
    ubyte stride_tab[4]={0};
    ubyte *pixels = image->GetPixels();
    uint32t bpp = CBMImgFmtInfo::GetBytesPerPixel(imgfmt);
    bool brevert = imgfmt == IMGFMT_RGB8 || imgfmt == IMGFMT_RGBA8;
    if(brevert)
    {
        //Temp Line For Data Transfer
        memory_allocator *mem_alloc = memory_allocator_selector::get_allocator(img_alloc,widthsize);
        ubyte *linedata = (ubyte*)mem_alloc->alloc(widthsize);
        for(uint32t i = 0; i < height; i++)
        {
            memcpy(linedata,pixels,widthsize);
            RevertChannels(linedata,width,1,0,bpp);
            datastream->Write(linedata,widthsize);
            datastream->Write(stride_tab,stride);
            pixels += widthsize + imgstride;
        }

        mem_alloc->dealloc(linedata);
    }
    else
    {
        for(uint32t i = 0; i < height; i++)
        {
            datastream->Write(pixels,widthsize);
            pixels += widthsize + imgstride;
            datastream->Write(stride_tab,stride);
        }
    }

    return true;
}

////////////////////////////////////////////////////////////////////////////////////////////////
// Protected Methods
////////////////////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////////////////////
// Check BMP File Header
bool CBMPImageIO::CheckFileHeader(const BMPHeader& header)
{
    //Reading File Type, Must Be 'BM' In Big Endian Notation
    if(header.bfType != 0x4D42)
        return false;

    // Number Of Planes Must Be 1
    if(header.biPlanes != 1)
        return false;

    //Compression Not Supported
    if(header.biCompression != BI_RGB)
        return false;

    //Only Supported 32,24,1,4 Bit Images
    if(header.biBitCount != 32 && header.biBitCount != 24 && header.biBitCount != 8 &&
        header.biBitCount != 1 && header.biBitCount != 4)
        return false;

    //If Image Is Not Too Large
    if(header.biWidth > MAX_WIDTH || header.biHeight > MAX_HEIGHT)
        return false;

    //If Image Size Is Not Zero
    if(!header.biWidth || !header.biHeight)
        return false;

    return true;
}

////////////////////////////////////////////////////////////////////////////////////////////////
uint32t CBMPImageIO::GetPaletteSize(const BMPHeader& header)
{
    if(header.biBitCount > 8)
        return 0;

    uint32t clrnum = 1;
    for(ushort it = 0; it < header.biBitCount; it++)
        clrnum *= 2;

    uint32t palsize = clrnum * 4;
    if(header.biClrUsed != 0 && header.biClrUsed < clrnum)
        palsize = header.biClrUsed*4;

    return palsize;
}

////////////////////////////////////////////////////////////////////////////////////////////////
void CBMPImageIO::LoadExtraData(IDataStream *datastream, CBMImage *image, bool bread)
{
    uint32t infosize = image->GetInfoDataSize();
    //Reading Description Extra Data
    if(bread)
    {
        ubyte *info = image->GetInfoData();
        datastream->Read(info,(uint_t)infosize);
    }
    else
    {
        datastream->Seek(infosize,seek_curr);
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////
void CBMPImageIO::SetFileHeader(BMPHeader& header,uint32t width, uint32t height, BMIFMT fmt, uint32t palclrnum, uint32t infosize)
{
    palclrnum = (fmt!=IMGFMT_L8)?(0):(palclrnum);
    uint32t pxsize = CBMImgFmtInfo::GetBytesPerPixel(fmt);
    uint32t palsize = palclrnum * 4;
    palsize = (palsize>1024)?(1024):(palsize);

    header.bfType = 0x4D42;
    header.bfSize = width*height*pxsize + palsize + sizeof(BMPHeader) + infosize;
    header.bfOffBits = sizeof(BMPHeader) + palsize + infosize;
    header.biSize = 40;
    header.biWidth = (uint)width;
    header.biHeight = (uint)height;
    header.biPlanes = 1;
    header.biBitCount = CBMImgFmtInfo::GetChannelsCount(fmt)*8;
    header.biCompression = BI_RGB;
    header.biSizeImage = width*height*pxsize;
    header.biClrUsed = (uint)palclrnum;
}


////////////////////////////////////////////////////////////////////////////////////////////////
CBMImage* CBMPImageIO::LoadRGBImage(IDataStream *datastream, const BMPHeader& header, uint32t loadflags)
{
    //Do We Need Load Info Data
    isize_t dspos = datastream->GetPos();
    isize_t offset = (isize_t)header.bfOffBits;
    uint32t infosize = (uint32t)(offset - dspos); 

    //Getting Image Params
    uint32t width = (uint32t)header.biWidth;
    uint32t height = (uint32t)header.biHeight;
    uint32t channels = (uint32t)(header.biBitCount / 8);

    BMIFMT fmt = (channels==4)?(IMGFMT_BGRA8):(IMGFMT_BGR8);
    if(loadflags & IMGIO_BGR_TO_RGB)
        fmt = (fmt==IMGFMT_BGRA8)?(IMGFMT_RGBA8):(IMGFMT_RGB8);

    uint32t stride = CBMImgInfo::CalcStride(fmt,width,4);
    uint32t widthsize = CBMImgInfo::CalcWidthSize(fmt,width);
    uint32t fullwidth = widthsize + stride;

    //Creating Image
    CBMImage *image = new CBMImage(fmt,width,height,1,IMGFMT_NONE,0,(loadflags&IMGIO_LOAD_INFO)?(infosize):(0));
    //Reading Description Data
    LoadExtraData(datastream,image,(loadflags & IMGIO_LOAD_INFO)>0);

    ubyte *pixels = image->GetPixels();
    ubyte stridebytes[4];
    for(uint32t line = 0; line < height; line++)
    {
        datastream->Read(pixels,widthsize);
        datastream->Read(stridebytes,stride);
        pixels += widthsize;
    }
    
    return image;
}

////////////////////////////////////////////////////////////////////////////////////////////////
CBMImage* CBMPImageIO::Load8BitImage(IDataStream *datastream, const BMPHeader& header, const ubyte *palette, uint32t palcolors, uint32t loadflags)
{
    //Do We Need Load Info Data
    isize_t dspos = datastream->GetPos();
    isize_t offset = (isize_t)header.bfOffBits;
    uint32t infosize = (uint32t)(offset - dspos);

    //Getting Image Params
    uint32t width = (uint32t)header.biWidth;
    uint32t height = (uint32t)header.biHeight;
    uint32t channels = 1;
    BMIFMT fmt = IMGFMT_L8;
    uint32t stride = CBMImgInfo::CalcStride(fmt,width,4);
    bool swapchannels = (loadflags & IMGIO_BGR_TO_RGB) == 1;

    if(loadflags & IMGIO_EXPAND_PALETTE)
    {
        if(loadflags & IMGIO_LOAD_PALETTE_ALPHA)
        {
            fmt = (swapchannels)?(IMGFMT_RGBA8):(IMGFMT_BGRA8);
            channels = 4;
        }
        else
        {
            fmt = (swapchannels)?(IMGFMT_RGB8):(IMGFMT_BGR8);
            channels = 3;
        }
    }
    BMIFMT palfmt = (loadflags & IMGIO_EXPAND_PALETTE)?(IMGFMT_NONE):(IMGFMT_BGRA8);
    uint32t palclr = (loadflags & IMGIO_EXPAND_PALETTE)?(0):(palcolors);
    
    //Creating Image
    CBMImage *image = new CBMImage(fmt,width,height,1,palfmt,palclr,infosize);
    //Reading Desc Data
    LoadExtraData(datastream,image,(loadflags & IMGIO_LOAD_INFO)>0);

    //Copy Palette Data If Do Not Want To Extract Palette
    if(!(loadflags & IMGIO_EXPAND_PALETTE))
    {
        ubyte *paldata = image->GetPalette();
        memcpy(paldata,palette,sizeof(ubyte)*palclr*4);
    }
    
    //Get Image Pixels And Calc Image Width Size In Bytes
    ubyte *imagedata = image->GetPixels();
    uint32t imgwidthsize = CBMImgInfo::CalcWidthSize(fmt,width);

    //Reading Data From Data Stream
    if(loadflags & IMGIO_EXPAND_PALETTE)
    {
        uint32t fullwidthsize = CBMImgInfo::CalcFullWidthSize(IMGFMT_L8,width,4);
        //Temp Line For Data Transfer
        memory_allocator *mem_alloc = memory_allocator_selector::get_allocator(img_alloc,fullwidthsize);
        ubyte *linedata = (ubyte*)mem_alloc->alloc(fullwidthsize);
        for(uint32t i = 0; i < height; i++)
        {
            datastream->Read(linedata,fullwidthsize);
            ExpandPalette(imagedata,linedata,palette,width,channels);
            imagedata += imgwidthsize;
        }
        mem_alloc->dealloc(linedata);
    }
    else
    {
        ubyte stridebytes[4]={0};
        for(uint32t i=0; i<height; i++)
        {
            datastream->Read(imagedata,imgwidthsize);
            datastream->Read(stridebytes,stride);
            imagedata += imgwidthsize;
        }
    }

    return image;
}

////////////////////////////////////////////////////////////////////////////////////////////////
CBMImage* CBMPImageIO::Load8BitCompressedImage(IDataStream *datastream, const BMPHeader& header, ubyte *palette, uint32t loadflags)
{
    //Do We Need Load Info Data
    isize_t dspos = datastream->GetPos();
    isize_t offset = (isize_t)header.bfOffBits;
    uint32t infosize = (uint32t)(offset - dspos);

    uint32t width = (uint32t)header.biWidth;
    uint32t height = (uint32t)header.biHeight;

    BMIFMT fmt = (loadflags & IMGIO_EXPAND_PALETTE) ? ((loadflags & IMGIO_LOAD_PALETTE_ALPHA)?(IMGFMT_BGRA8):(IMGFMT_BGR8)) : (IMGFMT_L8);
    BMIFMT palfmt = (loadflags & IMGIO_EXPAND_PALETTE) ? (IMGFMT_NONE) : (IMGFMT_BGRA8);
    uint32t palcolors = (uint32t)header.biClrUsed;
    uint32t palclrnum = (loadflags & IMGIO_EXPAND_PALETTE) ? (0) : (palcolors);
    
    
    

    //--CBMImage *image = new CBMImage(fmt,width,height,1,palfmt,palclrnum,0,0,0,1);//CBMImage(fmt,width,height,1);
    //--ubyte *pixels = image->GetPixels();

    /*
    //Creating Image
    CBMImage *image = new CBMImage(fmt,width,height,1,palfmt,palclr,infosize);
    //Reading Desc Data
    LoadExtraData(datastream,image,(loadflags & IMGIO_LOAD_INFO)>0);
    */

    //Creating Image
    CBMImage *image = new CBMImage(fmt,width,height,1,palfmt,palclrnum,infosize,0,0,1);
    //Reading Desc Data
    LoadExtraData(datastream,image,(loadflags & IMGIO_LOAD_INFO)>0);
    
    memory_allocator *mem_alloc = memory_allocator_selector::get_allocator(img_alloc,(uisize_t)header.biSizeImage);
    ubyte *compdata = (ubyte*)mem_alloc->alloc((uisize_t)header.biSizeImage);

    //--ubyte *compdata = new ubyte[header.biSizeImage];
    uint32t ireaded = datastream->Read(compdata,(uint32t)header.biSizeImage);

    if(ireaded < header.biSizeImage)
    {
        //--delete [] compdata;
        delete (image);
        mem_alloc->dealloc(compdata);
        return NULL;
    }

    //--uint32t width = (uint32t)header.biWidth;
    //--uint32t height = (uint32t)header.biHeight;

    //--BMIFMT fmt = (loadflags & IMGIO_EXPAND_PALETTE) ? ((loadflags & IMGIO_LOAD_PALETTE_ALPHA)?(IMGFMT_BGRA8):(IMGFMT_BGR8)) : (IMGFMT_L8);
    //--BMIFMT palfmt = (loadflags & IMGIO_EXPAND_PALETTE) ? () : ();

    //--CBMImage *image = new CBMImage(fmt,width,height,1,palfmt,palclrnum,0,0,0,1);//CBMImage(fmt,width,height,1);
    //--ubyte *pixels = image->GetPixels();

    ubyte *pixels = image->GetPixels();
    ubyte *decomppix = pixels;
    if(fmt != IMGFMT_L8)
    {
        //--decomppix = new ubyte[width*height];
        decomppix = (ubyte*)mem_alloc->alloc(width*height);
    }

    uint32t lines = DecompressRLE8(decomppix,compdata,header);

    if(lines < (uint32t)header.biHeight)
    {
        //--delete [] compdata;
        mem_alloc->dealloc(compdata);
        delete image;
        if(fmt != IMGFMT_L8)
        {
            //--delete [] decomppix;
            mem_alloc->dealloc(decomppix);
        }

        return NULL;
    }
   
    //--delete [] compdata;
    mem_alloc->dealloc(compdata);

    if(fmt == IMGFMT_L8)
    {
        //ubyte *pal = image->CreatePaletteData(IMGFMT_RGBA8,256);
        //memcpy(pal,palette,1024);
        ubyte *pal = image->GetPalette();
        memcpy(pal,palette,palclrnum);
    }
    /*else if(fmt == IMGFMT_LA8)
    {
        ubyte *pix = pixels;
        for(uint32t i = 0; i < width*height; i++)
        {
            uint32t clridx = decomppix[i];
            *pix++ = clridx;
            *pix++ = palette[clridx*4+3];
        }
        ubyte *pal = image->CreatePaletteData(IMGFMT_RGBA8,256);
        memcpy(pal,palette,1024);
    }*/
    else
    {
        //--uint32t pixoff = (loadflags & IMGIO_LOAD_PALETTE_ALPHA)?(4):(3);
        //--ExpandPalette(pixels,decomppix,palette,width,height,0,loadflags);
        //--delete [] decomppix;
        uint32t channels = (loadflags & IMGIO_LOAD_PALETTE_ALPHA)?(4):(3);
        for(uint32t i = 0; i < height; i++)
            ExpandPalette(pixels,decomppix,palette,width,channels);

        mem_alloc->dealloc(decomppix);
    }

    return image;
}

////////////////////////////////////////////////////////////////////////////////////////////////
CBMImage* CBMPImageIO::Load4BitImage(IDataStream *datastream, const BMPHeader& header,  ubyte *palette, uint32t loadflags)
{
    //Do We Need Load Info Data
    isize_t dspos = datastream->GetPos();
    isize_t offset = (isize_t)header.bfOffBits;
    uint32t infosize = (uint32t)(offset - dspos);

    //Getting Image Params
    uint32t width = (uint32t)header.biWidth;
    uint32t height = (uint32t)header.biHeight;
    uint32t channels = 1;
    BMIFMT fmt = IMGFMT_L8;
    bool swapchannels = (loadflags & IMGIO_BGR_TO_RGB) == 1;

    if(loadflags & IMGIO_EXPAND_PALETTE)
    {
        if(loadflags & IMGIO_LOAD_PALETTE_ALPHA)
        {
            fmt = (swapchannels)?(IMGFMT_RGBA8):(IMGFMT_BGRA8);
            channels = 4;
        }
        else
        {
            fmt = (swapchannels)?(IMGFMT_RGB8):(IMGFMT_BGR8);
            channels = 3;
        }
    }
    uint32t palcolors = 16;
    BMIFMT palfmt = (loadflags & IMGIO_EXPAND_PALETTE)?(IMGFMT_NONE):(IMGFMT_BGRA8);
    uint32t palclr = (loadflags & IMGIO_EXPAND_PALETTE)?(0):(palcolors);
    
    //Creating Image
    CBMImage *image = new CBMImage(fmt,width,height,1,palfmt,palclr,infosize);
    //Reading Desc Data
    LoadExtraData(datastream,image,(loadflags & IMGIO_LOAD_INFO)>0);

    //Copy Palette Data If Do Not Want To Extract Palette
    if(!(loadflags & IMGIO_EXPAND_PALETTE))
    {
        ubyte *paldata = image->GetPalette();
        memcpy(paldata,palette,sizeof(ubyte)*palclr*4);
    }
    
    //Get Image Pixels And Calc Image Width Size In Bytes
    ubyte *pixels = image->GetPixels();
    uint32t imgwidthsize = CBMImgInfo::CalcWidthSize(fmt,width);
    uint32t datoff = (fmt==IMGFMT_L8) ? (0) : ((fmt==IMGFMT_RGBA8)?(4):(3));

    ////////////////
    uint32t datawidth = (width%2) ? ((width>>1) + 1) : (width>>1);
    uint32t stride = CBMImgInfo::CalcStride(IMGFMT_L8,datawidth,4);
    uint32t fulldatawidth = datawidth + stride;

    memory_allocator *mem_alloc = memory_allocator_selector::get_allocator(img_alloc,fulldatawidth);
    ubyte *linedata = (ubyte*)mem_alloc->alloc(fulldatawidth);
    ubyte clrtab[] = {0,17,34,51,68,85,102,119,136,153,170,187,204,221,238,255};

    for(uint32t h = 0; h < height; h++)
    {
        uint32t ireaded = datastream->Read(linedata,fulldatawidth);
        if(ireaded < fulldatawidth)
        {
            delete (image);
            mem_alloc->dealloc(linedata);
            return NULL;
        }

        uint32t width2 = width>>1;
        uint32t w = 0;
        for(w = 0; w < width2; w++)
        {
            ubyte clridx1 = linedata[w]>>4;
            ubyte clridx2 = linedata[w] & 0xf; //0x00001111b;

            if(fmt == IMGFMT_L8)
            {
                *pixels++ = clrtab[clridx1];
                *pixels++ = clrtab[clridx2];
            }
            else
            {
                memcpy(&pixels[0],&palette[clridx1*4],datoff);
                memcpy(&pixels[datoff],&palette[clridx2*4],datoff);
                pixels += datoff*2;
            }
        }

        if(width%2)
        {
            ubyte clridx = linedata[w]>>4;
            if(fmt = IMGFMT_L8)
                *pixels++ = clrtab[clridx];
            else
                memcpy(&pixels[0],&palette[clridx*4],datoff);

            pixels += datoff;
        }
    }

    mem_alloc->dealloc(linedata);

    return image;
}

////////////////////////////////////////////////////////////////////////////////////////////////
CBMImage* CBMPImageIO::Load1BitImage(IDataStream *datastream, const BMPHeader& header, ubyte *palette, uint32t loadflags)
{
    //Do We Need Load Info Data
    isize_t dspos = datastream->GetPos();
    isize_t offset = (isize_t)header.bfOffBits;
    uint32t infosize = (uint32t)(offset - dspos);

    //Getting Image Params
    uint32t width = (uint32t)header.biWidth;
    uint32t height = (uint32t)header.biHeight;
    uint32t channels = 1;
    BMIFMT fmt = IMGFMT_L8;
    bool swapchannels = (loadflags & IMGIO_BGR_TO_RGB) == 1;

    if(loadflags & IMGIO_EXPAND_PALETTE)
    {
        if(loadflags & IMGIO_LOAD_PALETTE_ALPHA)
        {
            fmt = (swapchannels)?(IMGFMT_RGBA8):(IMGFMT_BGRA8);
            channels = 4;
        }
        else
        {
            fmt = (swapchannels)?(IMGFMT_RGB8):(IMGFMT_BGR8);
            channels = 3;
        }
    }
    uint32t palcolors = 2;
    BMIFMT palfmt = (loadflags & IMGIO_EXPAND_PALETTE)?(IMGFMT_NONE):(IMGFMT_BGRA8);
    uint32t palclr = (loadflags & IMGIO_EXPAND_PALETTE)?(0):(palcolors);
    
    //Creating Image
    CBMImage *image = new CBMImage(fmt,width,height,1,palfmt,palclr,infosize);
    //Reading Desc Data
    LoadExtraData(datastream,image,(loadflags & IMGIO_LOAD_INFO)>0);

    //Copy Palette Data If Do Not Want To Extract Palette
    if(!(loadflags & IMGIO_EXPAND_PALETTE))
    {
        ubyte *paldata = image->GetPalette();
        memcpy(paldata,palette,sizeof(ubyte)*palclr*4);
    }

    //Get Image Pixels And Calc Image Width Size In Bytes
    ubyte *pixels = image->GetPixels();
    uint32t datoff = (fmt==IMGFMT_L8) ? (0) : ((fmt==IMGFMT_RGBA8)?(4):(3));

    ////////////////////////////
    //Calc width size
    uint32t widthsize = width/8;
    if(width%8)
        widthsize += 1;

    uint32t stride = CBMImgInfo::CalcStride(IMGFMT_L8,widthsize,4);
    uint32t fullwidth = widthsize + stride;

    memory_allocator *mem_alloc = memory_allocator_selector::get_allocator(img_alloc,fullwidth);
    ubyte *linedata = (ubyte*)mem_alloc->alloc(fullwidth);
    for(uint32t h = 0; h < height; h++)
    {
        uint32t ireaded = datastream->Read(linedata,fullwidth);
        if(ireaded < fullwidth)
        {
            mem_alloc->dealloc(linedata);
            delete (image);
            return NULL;
        }

        ubyte databyte = linedata[0];
        uint32t offbits = 7;
        for(uint32t w = 0; w < width; w++)
        {
            if(!(w%8))
            {
                databyte = linedata[w/8];
                offbits = 7;
            }

            ubyte clridx = (databyte >> offbits) & 0x1;
            if(fmt==IMGFMT_L8)
                *pixels++ = (clridx>0) ? (255) : (0);
            else
                memcpy(pixels,&palette[clridx*4],datoff);

            pixels += datoff;
            offbits -= 1;
        }
    }
    mem_alloc->dealloc(linedata);

    return image;
}

////////////////////////////////////////////////////////////////////////////////////////////////
uint32t CBMPImageIO::DecompressRLE8(ubyte *pixels, const ubyte *compressedData, const BMPHeader& header)
{
    const ubyte *compDataEnd = compressedData + header.biSizeImage; // This will point to the end of "compData"
	const ubyte *TwoFromEnd = compDataEnd - 2;	// This will used for error checking

    ubyte *pix = pixels;
    const ubyte *compData = compressedData;

    // Encoding Data In Loop
    for(uint32t h = 0; h < header.biHeight; h++)
    {
        uint32t x = 0; //We'll Increment This As We Move Along The Bitmap

        while(x < header.biWidth)
        {
            ubyte data = compData[0]; // Reading First Byte, Count Of "indexes into the RGBQUAD to be repeated"
            if(data==0)
            {
                data = compData[1]; // Get the next byte (we're in "absolute" decompression mode
								   // now so this byte is the number of bytes that follow which contain
				                  // the color index of a single pixel (don't blame me I didn't make the compression scheme :)
				switch(data)
				{
					case 0: // This normally mean "the end of a scan line"
							// but we check for this elsewhere, so we don't want to 
							// get "the end of a scan line" in the middle of a scan line
						
						return h; // Number of scan lines read
					
					case 1: // This normally means "the end of the image" -- But we 
							// don't want the "end of the image" to appear in the middle
							// of the image
						
						return h; // Number of scan lines read
					
					case 2: // This normally means "skip to new location" -- However,
							// this isn't defined on "disk" (bitmaps loaded from a file)
							// bitmaps so we're going to treat it as an error
						
						return h; // Number of scan lines read
					
					default:
						
						// If we've overstepped the width of the bitmap -- This is bad :(
                        if((x + data) > header.biWidth)
							return h; // Number of scan lines read

						// Okay first we need to move compData to the correct location
						// (we've "dealt with" two bytes remember)
						compData += 2;

						// Now we need to copy a chunk of memory (data length long) into our scanLine (at the correct offset)
						memcpy(pix, compData, data);
                        
						x += data; // We update our count (from left to right on the scan line)
						compData += data; // We update our position in the compData
                        pix += data;
						
						// If data was odd -- Move compData to the next byte -- 
						// Basically every time we "move in compData", compData MUST 
						// be WORD aligned (it says so in MSDN)
						if((data & 1) != 0)
							compData++;

						break;
				
				} // end of switch(data)
            }
            else
            {
                //if we overstep this is bad
                if((x+data) > header.biWidth)
                    return h;

                //Fill A Portion Of The Scan Line
                memset(pix,compData[1],data);

                pix += data;
                x += data; // Update our "x-offset" (how far we've traversed down the scan line)
                compData += 2; // Move past the two bytes we just "dealt with"
            }
        }// end of while(x < header.biWidth)

        // If we haven't reached 2 bytes from the end of line -- And if the first
		// two bytes in the compData (currently) both equal zero, we've reached the end
		// of a scan line in the compressed data
		if((compData <= TwoFromEnd) && (compData[0] == 0) && (compData[1] == 0))
			compData += 2;	// Skip over the end of line data.

        else // We better be on the last line :)
		{
            if(h != (header.biHeight - 1))
				return 0; // We're returning zero just to signify there was an error
		}
    }// end of for(int h = 0; h < header.biHeight; h++)

    // If we're at least two bytes away from the end of the compressed data AND
	// the current values of the compData (at index 0 and index 1) are zero and one (end of image sequence)
	// We've successfully decompressed and loaded the bitmap
	if((compData <= TwoFromEnd) && (compData[0] == 0) && (compData[1] == 1))
	{
		compData += 2; // We've dealt with the ending code 

		// After we're done these BETTER be equal -- If they aren't we'll return 0 to signify an error
		if(compData != compDataEnd)
			return 0;
		else
            return header.biHeight; // We were successful in loading all the scan lines
	}
	else
		return 0; // Again this will say "Something bad happened!"
}

////////////////////////////////////////////////////////////////////////////////////////////////
void CBMPImageIO::ExpandPalette(ubyte *out, const ubyte *src, const ubyte *palette, uint32t width, uint32t channels)
{
    ubyte *optr = out;
    const ubyte *sptr = src;

    for(uint32t i = 0; i < width; i++)
    {
        uint32t index = sptr[0]*4;
        optr[0] = palette[index];
        optr[1] = palette[index+1];
        optr[2] = palette[index+2];

        if(channels == 4)
            optr[3] = palette[index+3];

        optr += channels;
        sptr += 1;
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////
void CBMPImageIO::RevertChannels(ubyte *pixels, uint32t width, uint32t height, uint32t stride, uint32t channels)
{
    ubyte *ptr = pixels;
    uint32t choff = (channels==4) ? (1) : (0);

    for(uint32t h = 0; h < height; h++)
    {
        for(uint32t w = 0; w < width; w++)
        {
            ptr[0] ^= ptr[2] ^= ptr[0] ^= ptr[2];
            ptr += channels;
        }
        ptr += stride;
    }
    return ;
}
////////////////////////////////////////////////////////////////////////////////////////////////



