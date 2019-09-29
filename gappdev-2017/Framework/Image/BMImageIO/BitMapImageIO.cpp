///////////////////////////////////////////////////////////////////////////////////////////////
//	BitMapImageIO.cpp
//	BitMapImage Loading/Saving Class Definition
//	
////////////////////////////////////////////////////////////////////////////////////////////////

#include "Utils/mem/memory_allocator.h"
#include "Utils/IOStream/IDataStream.h"
#include "Image/Image.h"
#include "BitMapImageIO.h"

#include <string>


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

#define ATTRIB_VERT     0x20
#define ATTRIB_HORI     0x10

#pragma pack (push,1)

// Targa Image File Header
struct TGAHeader 
{
	ubyte  descLen;     // identsize - size of ID field that follows 18 byte header (0 usually)
	ubyte  cmapType;    // Color Map Type - type of colour map 0=none, 1=has palette
	ubyte  imageType;   // type of image 0=none,1=indexed,2=rgb,3=grey,+8=rle packed
	ushort cmapStart;   // Color Map Start - first colour map entry in palette
	ushort cmapEntries; // Color Map Length - number of colours in palette
	ubyte  cmapBits;    // Color Map Bits - number of bits per palette entry 15,16,24,32
	ushort xOffset;     // image x origin
	ushort yOffset;     // image y origin
	ushort width;       // image width in pixel
	ushort height;      // image height in pixel
	ubyte  bpp;         // image bits per pixel 8,16,24,32
	ubyte  attrib;      // Descriptor - image descriptor bits (vh flip bits)
};

#pragma pack (pop)

////////////////////////////////////////////////////////////////////////////////////////////////
#pragma pack(push,1)
// PCX File Header Struct
struct PCXHeader
{
    ubyte   Manufacturer;           //Should always be 0Ah
    ubyte   Version;                //0=PC Paintbrush v2.5
                                    //2=PC Paintbrush v2.8 w palette information
                                    //3=PC Paintbrush v2.8 w/o palette information
                                    //4=PC Paintbrush/Windows
                                    //5=PC Paintbrush v3.0+
    ubyte   Encoding;               //Should always be 01h
    ubyte   Bpp;                    //Bits per pixel
    ushort  xMin;                   //Left margin of image
    ushort  yMin;                   //Upper margin of image
    ushort  xMax;                   //Right margin of image
    ushort  yMax;                   //Lower margin of image
    ushort  horDPI;                 //Horizontal DPI resolution
    ushort  vertDPI;                //Vertical DPI resolution
    ubyte   palette[48];            //Color palette setting for 16-color images 16 RGB triplets
    ubyte   reserved;               //Reserved
    ubyte   colorPlanes;            //Number of color planes: 4 -- 16 colors, 3 -- 24 bit color (16.7 million colors) 
    ushort  bytesPerLines;          //Number of bytes per line (the width of the image in bytes). For 320x200, 256-color images, this is 320 bytes per line. 
    ushort  PaletteType;            //Palette information 1=color/bw palette, 2=grayscale image
    ushort  hScrSize;               //Horizontal screen size   
    ushort  vScrSize;               //Vertical screen size   
    ubyte   filler[54];             //reserved, set to 0     
};

#pragma pack(pop)

//  Planes  Bit Depth   Display Type 
//      1       1           Monochrome 
//      1       2   `       CGA (4 colour palleted) 
//      3       1           EGA (8 color palletted) 
//      4       1           EGA or high-res. (S)VGA (16 color palletted) 
//      1       8           XGA or low-res. VGA (256 color palletted/greyscale)
 
//  3 or 4      8           SVGA/XGA and above (24/32-bit "true color", 
//                          3x greyscale planes with optional 8-bit alpha channel)
 



////////////////////////////////////////////////////////////////////////////////////////////////
char CBitMapImageIO::m_strInfo[SUP_FMT_NUM][FMT_INFO_STR_SIZE] = {{"Bitmap File"},{"TrueVision Image File"},{"zSoft Paintbrush"}};

////////////////////////////////////////////////////////////////////////////////////////////////
char CBitMapImageIO::m_strExt[SUP_FMT_NUM][FMT_EXT_STR_SIZE] = {{"bmp"},{"tga"},{"pcx"}};

////////////////////////////////////////////////////////////////////////////////////////////////
const char* CBitMapImageIO::GetInfoString(uint32t idx) const
{
    if (idx >= SUP_FMT_NUM)
        return NULL;

    return m_strInfo[idx];
}

////////////////////////////////////////////////////////////////////////////////////////////////
const char* CBitMapImageIO::GetExtString(uint32t idx) const
{
    return m_strExt[idx];
}

////////////////////////////////////////////////////////////////////////////////////////////////
int CBitMapImageIO::GetExtSupNum() const
{
    return SUP_FMT_NUM;
}

////////////////////////////////////////////////////////////////////////////////////////////////
CBMImage* CBitMapImageIO::LoadBMImage(IDataStream *datastream, uint32t loadflags)
{
    if(datastream == NULL)
        return NULL;

    ubyte bytedat = 0;
    datastream->Read(&bytedat,1);
    FMT_TYPE type = TYPE_NONE;
    switch(bytedat)
    {
    case 'B':
        type = TYPE_BMP;
        break;

    case 0xA:
        type = TYPE_PCX;
        break;

    case 0:
        type = TYPE_TGA;
        break;

    default:
        type = TYPE_TGA;
        break;
    }

    if(type == TYPE_NONE)
        return NULL;

    datastream->Seek(0,seek_set);
    if(type == TYPE_BMP)
    {
        BMPHeader bmpheader = {0};
        datastream->Read(&bmpheader,sizeof(BMPHeader));
    }
    else if(type == TYPE_PCX)
    {
        PCXHeader pcxheader = {0};
        datastream->Read(&pcxheader,sizeof(PCXHeader));
    }
    else if(type == TYPE_TGA)
    {
        TGAHeader tgaheader = {0};
        datastream->Read(&tgaheader,sizeof(TGAHeader));
    }



    return NULL;
}

////////////////////////////////////////////////////////////////////////////////////////////////
bool CBitMapImageIO::SaveBMImage(IDataStream *datastream, const CBMImage *image, ImgIOParam *params)
{
    return false;
}


////////////////////////////////////////////////////////////////////////////////////////////////
// Protected Methods
////////////////////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////////////////////
// Check BMP File Header
bool CBitMapImageIO::CheckBMPFileHeader(const BMPHeader& header)
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
// Check TGA File Header
bool CBitMapImageIO::CheckTGAFileHeader(const TGAHeader& header)
{
    if(header.width == 0 || header.height == 0 || header.width > MAX_WIDTH || header.height > MAX_HEIGHT)
        return false;

    //cmapType - Color Map Type 0=none, 1=has palette
    if(header.cmapType != 0 && header.cmapType != 1)
        return false;

    //imageType
    ubyte imgtype = header.imageType;
    if(imgtype != 1 && imgtype != 2 && imgtype != 3 && imgtype != 9 && imgtype != 10 && imgtype != 11)
        return false;

    ubyte bpp = header.bpp;
    if(bpp != 32 && bpp != 24 && bpp != 8)
        return false;

    //Checking Color Map Entries
    if(imgtype == 1 || imgtype == 9)
    {
        if(header.cmapBits != 24 && header.cmapBits != 32)
            return false;
        if(header.cmapEntries < 2 || header.cmapEntries > 256)
            return false;
    }

    return true;
}

////////////////////////////////////////////////////////////////////////////////////////////////
// Check PCX File Header
bool CBitMapImageIO::CheckPCXFileHeader(const PCXHeader& header)
{
	return false;
}
////////////////////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////////////////////