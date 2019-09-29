///////////////////////////////////////////////////////////////////////////////////////////////
//	DDS.cpp
//	Direct Draw Surface Image Format Loading/Saving Class Definition
//	
////////////////////////////////////////////////////////////////////////////////////////////////

#include "Utils/mem/memory_allocator.h"
#include "Utils/IOStream/IDataStream.h"
#include "Image/Image.h"
#include "DDS.h"

#include <memory.h>

#define MAX_WIDTH       32768
#define MAX_HEIGHT      32768

#define IMGMAX(A,B) (A>B)?(A):(B)

// Pack header tightly
#pragma pack (push, 1)

struct DDSHeader 
{
	uint32t dwMagic;
	uint32t dwSize;
	uint32t dwFlags;
	uint32t dwHeight;
	uint32t dwWidth;
	uint32t dwPitchOrLinearSize;
	uint32t dwDepth; 
	uint32t dwMipMapCount;
	uint32t dwReserved[11];

    // DDPIXELFORMAT
	struct {
		uint32t dwSize;
		uint32t dwFlags;
		uint32t dwFourCC;
		uint32t dwRGBBitCount;
		uint32t dwRBitMask;
		uint32t dwGBitMask;
		uint32t dwBBitMask;
		uint32t dwRGBAlphaBitMask; 
	} ddpfPixelFormat;

    //  DDCAPS2
    uint32t           dwSurfaceFlags;
    uint32t           dwCubemapFlags;
    uint32t           dwReserved2[3];
};

#pragma pack (pop)


// DDS file format flags
#define DDPF_ALPHAPIXELS 0x00000001
#define DDPF_FOURCC      0x00000004
#define DDPF_INDEXED     0x00000020
#define DDPF_RGB         0x00000040
#define DDPF_YUV         0x00000200  
#define DDPF_LUMINANCE   0x00020000

//dwHeaderFlags
#define DDSD_CAPS        0x00000001     //Required in every .dds file.
#define DDSD_HEIGHT      0x00000002     //Required in every .dds file.
#define DDSD_WIDTH       0x00000004     //Required in every .dds file.
#define DDSD_PITCH       0x00000008     //Required when pitch is provided for an uncompressed texture.
#define DDSD_PIXELFORMAT 0x00001000     //Required in every .dds file.
#define DDSD_MIPMAPCOUNT 0x00020000     //Required in a mipmapped texture.
#define DDSD_LINEARSIZE  0x00080000     //Required when pitch is provided for a compressed texture.
#define DDSD_DEPTH       0x00800000     //Required in a depth texture.


//dwSurfaceFlags
#define DDSCAPS_COMPLEX  0x00000008     //Optional; must be used on any file that contains more than one surface (a mipmap, a cubic environment map, or mipmapped volume texture).
#define DDSCAPS_TEXTURE  0x00001000     //Required
#define DDSCAPS_MIPMAP   0x00400000     //Optional; should be used for a mipmap.

//dwCubemapFlags
#define DDSCAPS2_VOLUME  0x00200000     //Required for a volume texture.
#define DDSCAPS2_CUBEMAP 0x00000200     //Required for a cube map.  

#define DDSCAPS2_CUBEMAP_POSITIVEX 0x00000400   //Required when these surfaces are stored in a cube map.
#define DDSCAPS2_CUBEMAP_NEGATIVEX 0x00000800   //Required when these surfaces are stored in a cube map.
#define DDSCAPS2_CUBEMAP_POSITIVEY 0x00001000   //Required when these surfaces are stored in a cube map.    
#define DDSCAPS2_CUBEMAP_NEGATIVEY 0x00002000   //Required when these surfaces are stored in a cube map.
#define DDSCAPS2_CUBEMAP_POSITIVEZ 0x00004000   //Required when these surfaces are stored in a cube map.
#define DDSCAPS2_CUBEMAP_NEGATIVEZ 0x00008000   //Required when these surfaces are stored in a cube map.


#define DDSCAPS2_CUBEMAP_ALL_FACES (DDSCAPS2_CUBEMAP_POSITIVEX | DDSCAPS2_CUBEMAP_NEGATIVEX | DDSCAPS2_CUBEMAP_POSITIVEY | DDSCAPS2_CUBEMAP_NEGATIVEY | DDSCAPS2_CUBEMAP_POSITIVEZ | DDSCAPS2_CUBEMAP_NEGATIVEZ)

//DDS Image Formats 
#define D3DFMT_A16B16G16R16             36


#define DXGI_FORMAT_R16G16B16A16_SNORM  110
#define D3DFMT_Q16W16V16U16             110

#define DXGI_FORMAT_R16_FLOAT           111
#define D3DFMT_R16F                     111

#define DXGI_FORMAT_R16G16_FLOAT        112
#define D3DFMT_G16R16F                  112

#define DXGI_FORMAT_R16G16B16A16_FLOAT  113 
#define D3DFMT_A16B16G16R16F            113    

#define DXGI_FORMAT_R32_FLOAT           114
#define D3DFMT_R32F                     114

#define DXGI_FORMAT_R32G32_FLOAT        115    
#define D3DFMT_G32R32F                  115

#define DXGI_FORMAT_R32G32B32A32_FLOAT  116
#define D3DFMT_A32B32G32R32F            116

#define D3DFMT_CxV8U8                   117 


////////////////////////////////////////////////////////////////////////////////////////////////
const char CDDSImageIO::m_strExt[] = {"dds"};
////////////////////////////////////////////////////////////////////////////////////////////////
const char CDDSImageIO::m_strInfo[] = {"Direct Draw Surface"};

////////////////////////////////////////////////////////////////////////////////////////////////
CDDSImageIO::CDDSImageIO()
{
}

////////////////////////////////////////////////////////////////////////////////////////////////
CDDSImageIO::~CDDSImageIO()
{
}

////////////////////////////////////////////////////////////////////////////////////////////////
const char* CDDSImageIO::GetInfoString(uint32t idx) const
{
    return &m_strInfo[0]; 
}

////////////////////////////////////////////////////////////////////////////////////////////////
const char* CDDSImageIO::GetExtString(uint32t idx) const
{
    return &m_strExt[0];
}

////////////////////////////////////////////////////////////////////////////////////////////////
CBMImage* CDDSImageIO::LoadBMImage(IDataStream *fs, uint32t loadflags)
{
    if(!fs || !fs->IsOpened())
        return NULL;

    DDSHeader header = {0};
    uint32t ireaded = fs->Read(&header,sizeof(DDSHeader));

    if(ireaded != sizeof(DDSHeader))
        return NULL;

    // Check If File Is A DDS File Format
    if(header.dwMagic != FOURCC('D','D','S',' '))
        return NULL;
    if(header.dwSize != 124)
        return NULL;
    //if(header.dwFlags & 0x8)//Has Pitch
    //    return NULL;

    uint32t width = IMGMAX(header.dwWidth,1);
    uint32t height = IMGMAX(header.dwHeight,1);
    uint32t depth = IMGMAX(header.dwDepth,1); 
    uint32t nmipmaps = (header.dwMipMapCount==0)? (0) : (header.dwMipMapCount-1);
    BMIFMT  fmt = CDDSImageIO::GetFormat(header);
    if(fmt == IMGFMT_NONE)
        return NULL;

    //Checking If DDS Image Has Palette Data
    bool haspalette = CDDSImageIO::HasPalette(header);
    BMIFMT palfmt = IMGFMT_NONE;
    uint32t palclrnum = 0;
    if(haspalette){
        fmt = (loadflags & IMGIO_EXPAND_PALETTE) ? ((loadflags & IMGIO_LOAD_PALETTE_ALPHA)?(IMGFMT_BGRA8):(IMGFMT_BGR8)) : (fmt);
        palfmt = (loadflags & IMGIO_EXPAND_PALETTE) ? (IMGFMT_NONE) : (IMGFMT_BGRA8);
        palclrnum = (loadflags & IMGIO_EXPAND_PALETTE) ? (0) : (256);
    }

    CBMImage *image = new CBMImage(fmt,width,height,depth,palfmt,palclrnum,0,0,nmipmaps,1);//CBMImage(fmt,width,height,depth,nmipmaps);
    if(!image)
        return NULL;

    ubyte *imgpixels = image->GetPixels();
    
    uint32t pixsize = CBMImgFmtInfo::GetBytesPerPixel(fmt);
    uint32t size = CBMImgInfo::CalcImageSize(fmt,width,height,depth,nmipmaps,0);

    ubyte *ptr = imgpixels;
    ubyte palette[1024] = {0};

    if((header.ddpfPixelFormat.dwRBitMask == 0xe0) && (header.ddpfPixelFormat.dwGBitMask == 0x1c) && (header.ddpfPixelFormat.dwBBitMask == 0x3) &&
                (header.ddpfPixelFormat.dwRGBAlphaBitMask == 0xff00))
    {
        uint32t widthsize = pixsize*width;
        uint16t *line = new uint16t[width];
        uint32t linesize = width*sizeof(uint16t);
        for(uint32t h = 0; h < height; h++)
        {
            ptr = imgpixels + widthsize*h;
            fs->Read(line,linesize);
            uint32t *pclr = reinterpret_cast<unsigned int*>(ptr);
            for(uint32t i = 0; i < width; i++)
                pclr[i] = CBMImgFmtConv::A8R3G3B2ToRGBA8(line[i]);        
        }
        delete [] line;
    }
    else
    {
        if(haspalette)
        {
            if(fs->Read(palette,1024)!=1024)
            {
                delete (image);
                return NULL;
            }
            if(fmt != IMGFMT_L8)
            {
                uint32t imgsize = CBMImgInfo::CalcImageSize(IMGFMT_L8,width,height,depth,nmipmaps,0);
                if(!LoadPalImage(fs,ptr,palette,imgsize,(fmt==IMGFMT_BGRA8 || fmt==IMGFMT_RGBA8)))
                {
                    delete (image);
                    return NULL;
                }
            }
            else
            {
                uint32t ireaded = fs->Read(ptr,size);
                ubyte *paldata = image->GetPalette();
                if(ireaded != size || paldata == NULL)
                {
                    delete (image);
                    return NULL;
                }
                memcpy(paldata,palette,1024);
            }
        }
        else
        {   
            if(fs->Read(ptr,size) != size)
            {
                delete (image);
                return NULL;
            }
        }
    }

    //Swap channels 
    if((header.ddpfPixelFormat.dwRBitMask == 0x3ff00000) && (header.ddpfPixelFormat.dwGBitMask == 0xffc00) && (header.ddpfPixelFormat.dwBBitMask == 0x3ff))
    {
        uint32t *pclr = reinterpret_cast<uint32t*>(imgpixels);
        for(uint32t i = 0; i < width*height; i++)
        {
            pclr[i] = CBMImgFmtConv::A2BGR10ToA2RGB10(pclr[i]);
        }
    }
    else if(header.ddpfPixelFormat.dwRBitMask == 0xf && header.ddpfPixelFormat.dwRGBAlphaBitMask == 0xf0)
    {
        ubyte *pclr = imgpixels;
        for(uint32t i = 0; i < width*height; i++)
            pclr[i] = CBMImgFmtConv::A4L4ToL4A4(pclr[i]);
    }

    //Flip Image
    if(loadflags & IMGIO_VERT_FLIP)
    {
        if(CBMImgFmtInfo::IsCompressedFormat(fmt))
            this->VertFlipCompressedImage(width,height,depth,nmipmaps,0,fmt,imgpixels);
        else
            this->VertFlipImage(width,height,depth,nmipmaps,0,fmt,imgpixels);
    }

    return image;
}


////////////////////////////////////////////////////////////////////////////////////////////////
bool CDDSImageIO::SaveBMImage(IDataStream *datastream, const CBMImage *image, ImgIOParam *params)
{
    return false;
}


////////////////////////////////////////////////////////////////////////////////////////////////
// Protected Functions
////////////////////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////////////////////
//--DDS_FOURCC --//
//DXGI_FORMAT_BC1_UNORM
//D3DFMT_DXT1               "DXT1"

//DXGI_FORMAT_BC2_UNORM
//D3DFMT_DXT3               "DXT3"

//DXGI_FORMAT_BC3_UNORM
//D3DFMT_DXT5               "DXT5"

//DXGI_FORMAT_BC4_UNORM     "BC4U"

//DXGI_FORMAT_BC4_SNORM     "BC4S"

//DXGI_FORMAT_BC5_UNORM     "ATI2"

//DXGI_FORMAT_BC5_SNORM     "BC5S"

//DXGI_FORMAT_R8G8_B8G8_UNORM
//D3DFMT_R8G8_B8G8          "RGBG"    

//DXGI_FORMAT_G8R8_G8B8_UNORM
//D3DFMT_G8R8_G8B8          "GRGB"

//DXGI_FORMAT_R16G16B16A16_UNORM
//D3DFMT_A16B16G16R16       36

//DXGI_FORMAT_R16G16B16A16_SNORM
//D3DFMT_Q16W16V16U16       110 

//DXGI_FORMAT_R16_FLOAT
//D3DFMT_R16F               111 

//DXGI_FORMAT_R16G16_FLOAT
//D3DFMT_G16R16F            112 

//DXGI_FORMAT_R16G16B16A16_FLOAT
//D3DFMT_A16B16G16R16F      113 

//DXGI_FORMAT_R32_FLOAT
//D3DFMT_R32F               114 

//DXGI_FORMAT_R32G32_FLOAT
//D3DFMT_G32R32F            115 


//DXGI_FORMAT_R32G32B32A32_FLOAT
//D3DFMT_A32B32G32R32F      116 

//D3DFMT_DXT2               "DXT2" 
//D3DFMT_DXT4               "DXT4" 
//D3DFMT_UYVY               "UYVY" 
//D3DFMT_YUY2               "YUY2" 
//D3DFMT_CxV8U8               117 
//Any DXGI format DDS_FOURCC "DX10" 

/*
typedef enum DXGI_FORMAT {
  DXGI_FORMAT_UNKNOWN                      = 0,
  DXGI_FORMAT_R32G32B32A32_TYPELESS        = 1,
  DXGI_FORMAT_R32G32B32A32_FLOAT           = 2,
  DXGI_FORMAT_R32G32B32A32_UINT            = 3,
  DXGI_FORMAT_R32G32B32A32_SINT            = 4,
  DXGI_FORMAT_R32G32B32_TYPELESS           = 5,
  DXGI_FORMAT_R32G32B32_FLOAT              = 6,
  DXGI_FORMAT_R32G32B32_UINT               = 7,
  DXGI_FORMAT_R32G32B32_SINT               = 8,
  DXGI_FORMAT_R16G16B16A16_TYPELESS        = 9,
  DXGI_FORMAT_R16G16B16A16_FLOAT           = 10,
  DXGI_FORMAT_R16G16B16A16_UNORM           = 11,
  DXGI_FORMAT_R16G16B16A16_UINT            = 12,
  DXGI_FORMAT_R16G16B16A16_SNORM           = 13,
  DXGI_FORMAT_R16G16B16A16_SINT            = 14,
  DXGI_FORMAT_R32G32_TYPELESS              = 15,
  DXGI_FORMAT_R32G32_FLOAT                 = 16,
  DXGI_FORMAT_R32G32_UINT                  = 17,
  DXGI_FORMAT_R32G32_SINT                  = 18,
  DXGI_FORMAT_R32G8X24_TYPELESS            = 19,
  DXGI_FORMAT_D32_FLOAT_S8X24_UINT         = 20,
  DXGI_FORMAT_R32_FLOAT_X8X24_TYPELESS     = 21,
  DXGI_FORMAT_X32_TYPELESS_G8X24_UINT      = 22,
  DXGI_FORMAT_R10G10B10A2_TYPELESS         = 23,
  DXGI_FORMAT_R10G10B10A2_UNORM            = 24,
  DXGI_FORMAT_R10G10B10A2_UINT             = 25,
  DXGI_FORMAT_R11G11B10_FLOAT              = 26,
  DXGI_FORMAT_R8G8B8A8_TYPELESS            = 27,
  DXGI_FORMAT_R8G8B8A8_UNORM               = 28,
  DXGI_FORMAT_R8G8B8A8_UNORM_SRGB          = 29,
  DXGI_FORMAT_R8G8B8A8_UINT                = 30,
  DXGI_FORMAT_R8G8B8A8_SNORM               = 31,
  DXGI_FORMAT_R8G8B8A8_SINT                = 32,
  DXGI_FORMAT_R16G16_TYPELESS              = 33,
  DXGI_FORMAT_R16G16_FLOAT                 = 34,
  DXGI_FORMAT_R16G16_UNORM                 = 35,
  DXGI_FORMAT_R16G16_UINT                  = 36,
  DXGI_FORMAT_R16G16_SNORM                 = 37,
  DXGI_FORMAT_R16G16_SINT                  = 38,
  DXGI_FORMAT_R32_TYPELESS                 = 39,
  DXGI_FORMAT_D32_FLOAT                    = 40,
  DXGI_FORMAT_R32_FLOAT                    = 41,
  DXGI_FORMAT_R32_UINT                     = 42,
  DXGI_FORMAT_R32_SINT                     = 43,
  DXGI_FORMAT_R24G8_TYPELESS               = 44,
  DXGI_FORMAT_D24_UNORM_S8_UINT            = 45,
  DXGI_FORMAT_R24_UNORM_X8_TYPELESS        = 46,
  DXGI_FORMAT_X24_TYPELESS_G8_UINT         = 47,
  DXGI_FORMAT_R8G8_TYPELESS                = 48,
  DXGI_FORMAT_R8G8_UNORM                   = 49,
  DXGI_FORMAT_R8G8_UINT                    = 50,
  DXGI_FORMAT_R8G8_SNORM                   = 51,
  DXGI_FORMAT_R8G8_SINT                    = 52,
  DXGI_FORMAT_R16_TYPELESS                 = 53,
  DXGI_FORMAT_R16_FLOAT                    = 54,
  DXGI_FORMAT_D16_UNORM                    = 55,
  DXGI_FORMAT_R16_UNORM                    = 56,
  DXGI_FORMAT_R16_UINT                     = 57,
  DXGI_FORMAT_R16_SNORM                    = 58,
  DXGI_FORMAT_R16_SINT                     = 59,
  DXGI_FORMAT_R8_TYPELESS                  = 60,
  DXGI_FORMAT_R8_UNORM                     = 61,
  DXGI_FORMAT_R8_UINT                      = 62,
  DXGI_FORMAT_R8_SNORM                     = 63,
  DXGI_FORMAT_R8_SINT                      = 64,
  DXGI_FORMAT_A8_UNORM                     = 65,
  DXGI_FORMAT_R1_UNORM                     = 66,
  DXGI_FORMAT_R9G9B9E5_SHAREDEXP           = 67,
  DXGI_FORMAT_R8G8_B8G8_UNORM              = 68,
  DXGI_FORMAT_G8R8_G8B8_UNORM              = 69,
  DXGI_FORMAT_BC1_TYPELESS                 = 70,
  DXGI_FORMAT_BC1_UNORM                    = 71,
  DXGI_FORMAT_BC1_UNORM_SRGB               = 72,
  DXGI_FORMAT_BC2_TYPELESS                 = 73,
  DXGI_FORMAT_BC2_UNORM                    = 74,
  DXGI_FORMAT_BC2_UNORM_SRGB               = 75,
  DXGI_FORMAT_BC3_TYPELESS                 = 76,
  DXGI_FORMAT_BC3_UNORM                    = 77,
  DXGI_FORMAT_BC3_UNORM_SRGB               = 78,
  DXGI_FORMAT_BC4_TYPELESS                 = 79,
  DXGI_FORMAT_BC4_UNORM                    = 80,
  DXGI_FORMAT_BC4_SNORM                    = 81,
  DXGI_FORMAT_BC5_TYPELESS                 = 82,
  DXGI_FORMAT_BC5_UNORM                    = 83,
  DXGI_FORMAT_BC5_SNORM                    = 84,
  DXGI_FORMAT_B5G6R5_UNORM                 = 85,
  DXGI_FORMAT_B5G5R5A1_UNORM               = 86,
  DXGI_FORMAT_B8G8R8A8_UNORM               = 87,
  DXGI_FORMAT_B8G8R8X8_UNORM               = 88,
  DXGI_FORMAT_R10G10B10_XR_BIAS_A2_UNORM   = 89,
  DXGI_FORMAT_B8G8R8A8_TYPELESS            = 90,
  DXGI_FORMAT_B8G8R8A8_UNORM_SRGB          = 91,
  DXGI_FORMAT_B8G8R8X8_TYPELESS            = 92,
  DXGI_FORMAT_B8G8R8X8_UNORM_SRGB          = 93,
  DXGI_FORMAT_BC6H_TYPELESS                = 94,
  DXGI_FORMAT_BC6H_UF16                    = 95,
  DXGI_FORMAT_BC6H_SF16                    = 96,
  DXGI_FORMAT_BC7_TYPELESS                 = 97,
  DXGI_FORMAT_BC7_UNORM                    = 98,
  DXGI_FORMAT_BC7_UNORM_SRGB               = 99,
  DXGI_FORMAT_FORCE_UINT                   = 0xffffffffUL 
} DXGI_FORMAT, *LPDXGI_FORMAT;

*/


////////////////////////////////////////////////////////////////////////////////////////////////
BMIFMT CDDSImageIO::GetFormat(const DDSHeader &header)
{
    BMIFMT fmt = IMGFMT_NONE;

    //Identify The Image Format
    switch(header.ddpfPixelFormat.dwFourCC)
    {
    case FOURCC('D', 'X', 'T', '1'):
        fmt = IMGFMT_DXT1;
        break;

    case FOURCC('D', 'X', 'T', '3'):
        fmt = IMGFMT_DXT3;
        break;

    case FOURCC('D', 'X', 'T', '5'):
        fmt = IMGFMT_DXT5;
        break;

    case FOURCC('A', 'T', 'I', '1'):
    case FOURCC('B', 'C', '4', 'U'):
        fmt = IMGFMT_ATI1N;
        break;

    case FOURCC('A', 'T', 'I', '2'):
    case FOURCC('B', 'C', '5', 'U'):
        fmt = IMGFMT_ATI2N;
        break;

    case FOURCC('P','8',' ',' '):
        fmt = IMGFMT_L8;
        break;

    case D3DFMT_A16B16G16R16:
        fmt = IMGFMT_RGBA16;
        break;

    case D3DFMT_R16F:
        fmt = IMGFMT_R16f;
        break;

    case D3DFMT_G16R16F:
        fmt = IMGFMT_RG16f;
        break;

    case D3DFMT_A16B16G16R16F:
        fmt = IMGFMT_RGBA16f;
        break;

    case D3DFMT_R32F:
        fmt = IMGFMT_R32f;
        break;

    case D3DFMT_G32R32F:
        fmt = IMGFMT_RG32f;
        break;

    case D3DFMT_A32B32G32R32F:
        fmt = IMGFMT_RGBA32f;
        break;

    case D3DFMT_Q16W16V16U16:
        fmt = IMGFMT_UVWQ16;
        break;
        /*
        //No For Signed Formats
        0,                      //TEXFMT_UV8
        0,                      //TEXFMT_UVWQ8
        0,                      //TEXFMT_UV16
        0,                      //TEXFMT_UVWQ16 
        */
        /*
        IMGFMT_UV5L6
        IMGFMT_UVW10A2
        */


    default:
        switch(header.ddpfPixelFormat.dwRGBBitCount)
        {
        case 8:
            if(header.ddpfPixelFormat.dwRBitMask == 0xE0){
                fmt = IMGFMT_R3G3B2;
                break;
            }
            else if(header.ddpfPixelFormat.dwRBitMask == 0xf && header.ddpfPixelFormat.dwRGBAlphaBitMask == 0xf0)
            {
                fmt = IMGFMT_L4A4;
                break;
            }
            else if(header.ddpfPixelFormat.dwRBitMask == 0xff && header.ddpfPixelFormat.dwGBitMask == 0 && header.ddpfPixelFormat.dwBBitMask == 0)
            {
                fmt = IMGFMT_L8;
                break;
            }       
            else if(header.ddpfPixelFormat.dwRGBAlphaBitMask == 0xff && header.ddpfPixelFormat.dwRBitMask == 0 && header.ddpfPixelFormat.dwGBitMask == 0 &&
                header.ddpfPixelFormat.dwBBitMask == 0)
            {
                fmt = IMGFMT_L8;
                break;
            }
            //else if(header.ddpfPixelFormat.dwFlags & DDPF_INDEXED && header.ddpfPixelFormat.dwRGBBitCount == 8)
            //{
            //    fmt = IMGFMT_L8;
            //    break;
            //}
           
            break;

             
        case 16:
            if((header.ddpfPixelFormat.dwRBitMask == 0xf800) && (header.ddpfPixelFormat.dwGBitMask == 0x7e0) && (header.ddpfPixelFormat.dwBBitMask == 0x1f))
            {
                fmt = IMGFMT_R5G6B5;
                break;
            }
            else if((header.ddpfPixelFormat.dwRBitMask == 0x7c00) && (header.ddpfPixelFormat.dwGBitMask == 0x3e0) && (header.ddpfPixelFormat.dwBBitMask == 0x1f))
            {
                fmt = IMGFMT_A1RGB5;
                break;
            }
            else if((header.ddpfPixelFormat.dwRBitMask == 0xf00) && (header.ddpfPixelFormat.dwGBitMask == 0xf0) && (header.ddpfPixelFormat.dwBBitMask == 0xf))
            {
                fmt = IMGFMT_A4RGB4; 
                break;
            }
            else if((header.ddpfPixelFormat.dwRBitMask == 0xffff) && (header.ddpfPixelFormat.dwGBitMask == 0) && (header.ddpfPixelFormat.dwBBitMask == 0))
            {
                fmt = IMGFMT_L16;
                break;
            }
            else if((header.ddpfPixelFormat.dwRBitMask == 0xff) && (header.ddpfPixelFormat.dwGBitMask == 0) && (header.ddpfPixelFormat.dwBBitMask == 0) &&
                (header.ddpfPixelFormat.dwRGBAlphaBitMask == 0xff00))
            {
                fmt = IMGFMT_LA8;
                break;
            }
            //D3DFMT_A8R3G3B2 DDS_RGBA 16 0xe0 0x1c 0x3 0xff00 
            else if((header.ddpfPixelFormat.dwRBitMask == 0xe0) && (header.ddpfPixelFormat.dwGBitMask == 0x1c) && (header.ddpfPixelFormat.dwBBitMask == 0x3) &&
                (header.ddpfPixelFormat.dwRGBAlphaBitMask == 0xff00))
            {
                fmt = IMGFMT_RGBA8;
                break;
            }
            break;

        case 24:
            if((header.ddpfPixelFormat.dwRBitMask == 0xff0000) && (header.ddpfPixelFormat.dwGBitMask == 0xff00) && (header.ddpfPixelFormat.dwBBitMask == 0xff))
            {
                fmt = IMGFMT_BGR8;
            }
            break;

        case 32:
            if(((header.ddpfPixelFormat.dwRBitMask == 0x3ff) && (header.ddpfPixelFormat.dwGBitMask == 0xffc00) && (header.ddpfPixelFormat.dwBBitMask == 0x3ff00000)) || 
                ((header.ddpfPixelFormat.dwRBitMask == 0x3ff00000) && (header.ddpfPixelFormat.dwGBitMask == 0xffc00) && (header.ddpfPixelFormat.dwBBitMask == 0x3ff)))
            {
                fmt = IMGFMT_A2RGB10;
                break;
            }            

            else if((header.ddpfPixelFormat.dwRBitMask == 0xff0000) && (header.ddpfPixelFormat.dwGBitMask == 0xff00) && (header.ddpfPixelFormat.dwBBitMask == 0xff))
            {
                fmt = IMGFMT_BGRA8;
                break;
            }
            else if((header.ddpfPixelFormat.dwRBitMask == 0xff) && (header.ddpfPixelFormat.dwGBitMask == 0xff00) && (header.ddpfPixelFormat.dwBBitMask == 0xff0000))
            {
                fmt = IMGFMT_RGBA8;
                break;
            }
            else if((header.ddpfPixelFormat.dwRBitMask == 0xffff) && (header.ddpfPixelFormat.dwGBitMask == 0xffff0000) && (header.ddpfPixelFormat.dwBBitMask == 0))
            {
                fmt = IMGFMT_RG16;
                break;
            }
            break;
        }
    }

    //If Has Palette
    //--if(fmt == IMGFMT_NONE && ((header.ddpfPixelFormat.dwFlags & 0x4) && (header.dwSurfaceFlags & 0x401008)))
    //--    fmt = IMGFMT_L8;

    return fmt;
}

////////////////////////////////////////////////////////////////////////////////////////////////
bool CDDSImageIO::HasPalette(const DDSHeader &header)
{
    //if((header.ddpfPixelFormat.dwFlags & 0x4) && (header.dwSurfaceFlags == 0x401008))
    //    return true;

    if(header.ddpfPixelFormat.dwFourCC == FOURCC('P','8',' ',' '))
        return true;

    return false;
}



////////////////////////////////////////////////////////////////////////////////////////////////
bool CDDSImageIO::LoadPalImage(IDataStream *fs, ubyte *data, ubyte *palette, uint32t size, bool rgba)
{
    ubyte compdata[4096] = {0};
    uint32t len = size;
    ubyte *ptr = data;
    
    while(len)
    {
        ubyte compdata[4096] = {0};
        uint32t readsize = (len >= 4096) ? (4096) : (4096-len);
        uint32t ireaded = fs->Read(compdata,readsize);
        if(ireaded != readsize)
            return false;

        for(uint32t i = 0; i < readsize; i++)
        {
            uint32t palidx = compdata[i]*4;
            *ptr++ = palette[palidx++];
            *ptr++ = palette[palidx++];
            *ptr++ = palette[palidx++];

            if(rgba)
                *ptr++ = palette[palidx];
        }

        len = (len > 4096) ? (len-4096) : (0);
    }
    
    return true;
}

////////////////////////////////////////////////////////////////////////////////////////////////
bool CDDSImageIO::IsFmtSupForSaving(BMIFMT fmt)
{
    return (fmt != IMGFMT_NONE) && (fmt != IMGFMT_DEPTH16) && (fmt != IMGFMT_DEPTH24);
}


////////////////////////////////////////////////////////////////////////////////////////////////
void CDDSImageIO::SetFileHeader(DDSHeader &header, const CBMImage *image)
{
    header.dwMagic = FOURCC('D','D','S',' ');
    header.dwSize = 124;
    header.dwWidth = image->GetWidth();
    header.dwHeight = image->GetHeight();
    header.dwDepth = (image->GetDepth()==1) ? (0) : (image->GetDepth());

    //Setting Flags
    header.dwPitchOrLinearSize = 0;//??
    header.dwMipMapCount = image->GetMipMapCount();
    header.dwFlags = (DDSD_PIXELFORMAT | DDSD_CAPS | DDSD_WIDTH | DDSD_HEIGHT);
    if(header.dwDepth > 0)
        header.dwFlags |= DDSD_DEPTH;
    if(header.dwMipMapCount > 0)
        header.dwFlags |= DDSD_MIPMAPCOUNT;
    
    //Setting Surface Flags
    header.dwSurfaceFlags = DDSCAPS_TEXTURE;
    if(header.dwMipMapCount > 0)
        header.dwSurfaceFlags |= DDSCAPS_TEXTURE;
    if(header.dwDepth > 0 || image->GetImageCount()>=6)
        header.dwSurfaceFlags |= DDSCAPS_COMPLEX;

    //Setting Pixel Format
    header.ddpfPixelFormat.dwSize = 32;
    BMIFMT imgfmt = image->GetFormat();
    header.ddpfPixelFormat.dwFourCC = GetFourCCFmt(imgfmt);
    if(header.ddpfPixelFormat.dwFourCC != 0)
    {
        header.ddpfPixelFormat.dwFlags = DDPF_FOURCC;
    }
    else
    {
        if((imgfmt >= IMGFMT_UV8 && imgfmt <= IMGFMT_UVWQ16) || (imgfmt >= IMGFMT_UV5L6 && imgfmt <= IMGFMT_UVW10A2))
            header.ddpfPixelFormat.dwFlags = DDPF_YUV;
        else if(imgfmt == IMGFMT_L8 || imgfmt == IMGFMT_LA8 || imgfmt == IMGFMT_L16 || imgfmt == IMGFMT_LA16)
            header.ddpfPixelFormat.dwFlags = DDPF_LUMINANCE;
        else
            header.ddpfPixelFormat.dwFlags = DDPF_RGB;

        if(imgfmt == IMGFMT_LA8 || imgfmt == IMGFMT_LA16 || imgfmt == IMGFMT_RGBA8 || imgfmt == IMGFMT_BGRA8 || imgfmt == IMGFMT_RGBA16 ||
            imgfmt == IMGFMT_RGBA16f || imgfmt == IMGFMT_RGBA32f || imgfmt == IMGFMT_L4A4 || imgfmt == IMGFMT_A4RGB4 ||
            imgfmt == IMGFMT_A1RGB5 || imgfmt == IMGFMT_A2RGB10 || imgfmt == IMGFMT_UVW10A2)
            header.ddpfPixelFormat.dwFlags |= DDPF_ALPHAPIXELS;

        header.ddpfPixelFormat.dwRGBBitCount = CBMImgFmtInfo::GetBytesPerPixel(imgfmt)*8;

    
    }


    if(header.dwDepth > 0)
    {
        header.dwCubemapFlags = DDSCAPS2_VOLUME;
    }
    else
    {
        if(image->GetImageCount() >= 6)
            header.dwCubemapFlags = DDSCAPS2_CUBEMAP;
    }

}

////////////////////////////////////////////////////////////////////////////////////////////////
uint32t CDDSImageIO::GetFourCCFmt(BMIFMT fmt)
{
    uint32t fourcc = 0;
    switch(fmt)
    {
    case IMGFMT_RGBA16:
        fourcc = D3DFMT_A16B16G16R16;
        break;
        
    case IMGFMT_UVWQ16:
        fourcc = D3DFMT_Q16W16V16U16;
        break;

    case IMGFMT_R16f:
        fourcc = D3DFMT_R16F;
        break;

    case IMGFMT_RG16f:
        fourcc = D3DFMT_G16R16F;
        break;

    case IMGFMT_R32f:
        fourcc = D3DFMT_R32F;
        break;

    case IMGFMT_RG32f:
        fourcc = D3DFMT_G32R32F;
        break;

    case IMGFMT_RGB32f:
        fourcc = D3DFMT_A32B32G32R32F;
        break;

    case IMGFMT_DXT1:
        fourcc = FOURCC('D','X','T','1');
        break;

    case IMGFMT_DXT3:
        fourcc = FOURCC('D','X','T','3');
        break;

    case IMGFMT_DXT5:
        fourcc = FOURCC('D','X','T','5');
        break;

    case IMGFMT_ATI1N:
        fourcc = FOURCC('A','T','I','1');
        break;

    case IMGFMT_ATI2N:
        fourcc = FOURCC('A','T','I','2');
        break;
    }

    return fourcc;
}

////////////////////////////////////////////////////////////////////////////////////////////////
uint32t CDDSImageIO::GetRGBABitMask(BMIFMT bmifmt,uint32t channel)
{
    if(channel < 1 || channel > 4)
        return 0;

    static const int rgbamask[][4] = { 
    
        {0,0,0,0},      //IMGFMT_NONE 

        //Unsigned Formats
        {0xff,0,0,0},                       //IMGFMT_L8
        {0xff,0,0,0xff},                    //IMGFMT_LA8
        {0xff,0xff00,0xff0000,0},           //IMGFMT_RGB8
        {0xff,0xff00,0xff0000,0xff000000},  //IMGFMT_RGBA8
        {0xff0000,0xff00,0xff,0},           //IMGFMT_BGR8
        {0xff0000,0xff00,0xff,0xff000000},  //IMGFMT_BGRA8
        {0,0,0,0},                          //IMGFMT_L16
        {0,0,0,0},                          //IMGFMT_LA16
        {0,0,0,0},                          //IMGFMT_RG16
        {0,0,0,0},                          //IMGFMT_RGB16
        {0,0,0,0},                          //IMGFMT_RGBA16

        //Float Formats
        {0,0,0,0},      //IMGMFT_R16f
        {0,0,0,0},      //IMGFMT_RG16f
        {0,0,0,0},      //IMGFMT_RGB16f
        {0,0,0,0},      //IMGFMT_RGBA16f
        {0,0,0,0},      //IMGFMT_R32f
        {0,0,0,0},      //IMGFMT_RG32f
        {0,0,0,0},      //IMGFMT_RGB32f
        {0,0,0,0},      //IMGFMT_RGBA32f

        //Signed Formats
        {0,0,0,0},      //IMGFMT_UV8
        {0,0,0,0},      //IMGFMT_UVWQ8
        {0,0,0,0},      //IMGFMT_UV16
        {0,0,0,0},      //IMGFMT_UVWQ16

        //Depth Formats
        {0,0,0,0},     //IMGFMT_DEPTH16
        {0,0,0,0},     //IMGFMT_DEPTH24

        //Packed Formats
        {0,0,0,0},      //IMGFMT_L4A4
        {0,0,0,0},      //IMGFMT_A4RGB4
        {0,0,0,0},      //IMGFMT_R3G3B2
        {0,0,0,0},      //IMGFMT_R5G6B5
        {0,0,0,0},      //IMGFMT_A1RGB5
        {0,0,0,0},      //IMGFMT_A2RGB10
        {0,0,0,0},      //IMGFMT_RG11B10
        {0,0,0,0},      //IMGFMT_RGB9E5

        //Mixed Formats
        {0,0,0,0},      //IMGFMT_UV5L6
        {0,0,0,0},      //IMGFMT_UVW10A2

        //Compressed Formats
        {0,0,0,0},      //IMGFMT_DXT1
        {0,0,0,0},      //IMGFMT_DXT3
        {0,0,0,0},      //IMGFMT_DXT5 
        {0,0,0,0},      //IMGFMT_ATI1N
        {0,0,0,0},      //IMGFMT_ATI2N
    };

    return rgbamask[bmifmt][channel-1];
}

////////////////////////////////////////////////////////////////////////////////////////////////
void CDDSImageIO::VertFlipImage(uint32t width, uint32t height, uint32t depth, uint32t mipmaps, uint32t align, BMIFMT fmt, ubyte *data)
{
    if(CBMImgFmtInfo::IsCompressedFormat(fmt)) return ;

    uint32t mipnum = CBMImgInfo::CalcMipMapsNum(width,height,depth);
    mipnum = (mipmaps < mipnum) ? (mipmaps) : (mipnum);

    depth = (depth==0)?(1):(depth);
    height = (height==0)?(1):(height);

    ubyte *base = data;
    ubyte *ptr1 = data;
    ubyte *ptr2 = data;

    uint32t maxwidth = CBMImgInfo::CalcFullWidthSize(fmt,width,align);
    ubyte *tmpdat = new ubyte[maxwidth];

    for(uint32t d = 0; d < depth; d++)
    {
        uint32t mipwidth = width;
        uint32t mipheight = height;
        for(uint32t level = 0; level <= mipnum; level++)
        {
            uint32t fullwidth = CBMImgInfo::CalcFullWidthSize(fmt,mipwidth,align);
            ptr1 = base;
            ptr2 = base + fullwidth*(mipheight-1);
            for(uint32t h = 0; h < mipheight>>1; h++)
            {
                memcpy(tmpdat,ptr1,fullwidth);
                memcpy(ptr1,ptr2,fullwidth);
                memcpy(ptr2,tmpdat,fullwidth);
                ptr1 += fullwidth;
                ptr2 -= fullwidth;
            }
            base += fullwidth*mipheight;
            mipwidth = ((mipwidth>>1)>0)?(mipwidth>>1):(1);
            mipheight = ((mipheight>>1)>0)?(mipheight>>1):(1);
        }
    }

    delete [] tmpdat;
}


////////////////////////////////////////////////////////////////////////////////////////////////
void CDDSImageIO::VertFlipCompressedImage(uint32t width, uint32t height, uint32t depth, uint32t mipmaps, uint32t align, BMIFMT fmt, ubyte *imgdata)
{
    uint32t blocksize = CBMImgFmtInfo::GetBytesPerBlock(fmt);
    //uint32t blocklen = ((width+3)>>2)*blocksize;
    //uint32t blockheight = height>>2;
    ubyte *data = imgdata;
   
    for(uint32t mip = 0; mip <= mipmaps; mip++)
    {
        uint32t mipdepth = IMGMAX(1,depth>>mip);
        uint32t mipheight = IMGMAX(1,(height>>mip)>>2);
        uint32t mipwidth = IMGMAX(1,(width>>mip)>>2);

        if(mipheight <= 1)
            break;

        uint32t blocklen = mipwidth*blocksize;

        for(uint32t d = 0; d < mipdepth; d++)
        {
            ubyte *ptr1 = data;
            ubyte *ptr2 = data + blocklen*(mipheight-1);
            uint32t bheight = (mipheight >= 4) ? (4) : (2);
            for(uint32t h = 0; h < (mipheight>>1); h++)
            {
                for(uint32t w = 0; w < blocklen; w++)
                {
                   ptr1[w] ^= ptr2[w] ^= ptr1[w] ^= ptr2[w];
                }
                for(uint32t w = 0; w < mipwidth; w++)
                {
                    VertFlipBlock(mipwidth,bheight,fmt,&ptr1[w*blocksize]);
                    VertFlipBlock(mipwidth,bheight,fmt,&ptr2[w*blocksize]);
                }
                ptr1 += blocklen;
                ptr2 -= blocklen;
            }
            data += blocklen*mipheight;
        }

        
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////
void CDDSImageIO::VertFlipBlock(uint32t w, uint32t h, BMIFMT fmt, ubyte *data)
{
    if(h <= 1) return ;

    switch(fmt)
    {
    case IMGFMT_DXT1:
        VertFlipDXT1Block(h==2,data);
        break;

    case IMGFMT_DXT3:
        VertFlipDXT3Block(h==2,data);
        break;

    case IMGFMT_DXT5:
        VertFlipDXT5Block(h==2,data);
        break;

    case IMGFMT_ATI1N:
        VertFlipATI1NBlock(h==2,data);
        break;

    case IMGFMT_ATI2N:
        VertFlipATI2NBlock(h==2,data);
        break;
    };
}
////////////////////////////////////////////////////////////////////////////////////////////////
void CDDSImageIO::VertFlipDXT1Block(bool halfblock, ubyte *data)
{
    //          MSB                                 LSB
    //          --------------------------------------
    //2 Bytes   |   color0 R[15:11] G[10:5] B[4:0]   |
    //          --------------------------------------
    //          |               color1               |
    //          --------------------------------------
    //1 Byte    | d | c | b | a |
    //          -----------------
    //          | h | g | f | e |
    //          -----------------
    //          | l | k | j | i |
    //          -----------------
    //          | p | o | n | m |
    //          -----------------

    if(halfblock)
    {
        data[4] ^= data[5] ^= data[4] ^= data[5];
    }
    else
    {
        data[4] ^= data[7] ^= data[4] ^= data[7];
        data[5] ^= data[6] ^= data[5] ^= data[6];
    }

}
////////////////////////////////////////////////////////////////////////////////////////////////
void CDDSImageIO::VertFlipDXT3Block(bool halfblock, ubyte *data)
{
    //          MSB                                   LSB
    //          -----------------------------------------
    //2 Bytes   | alpha d | alpha c | alpha b | alpha a |
    //          -----------------------------------------
    //          | alpha h | alpha g | alpha f | alpha e |
    //          -----------------------------------------
    //          | alpha l | alpha k | alpha j | alpha i |
    //          -----------------------------------------
    //          | alpha p | alpha o | alpha n | alpha m |
    //          -----------------------------------------
    //2 Bytes   |    color0 R[15:11] G[10:5] B[4:0]     |
    //          -----------------------------------------
    //          |                color1                 |
    //          -----------------------------------------
    //1 Byte    | d | c | b | a |
    //          -----------------
    //          | h | g | f | e |
    //          -----------------
    //          | l | k | j | i |
    //          -----------------
    //          | p | o | n | m |
    //          -----------------

    if(halfblock)
    {
        uint16t *alpha = reinterpret_cast<uint16t*>(data);
        alpha[0] ^= alpha[1] ^= alpha[0] ^= alpha[1];
        data[12] ^= data[13] ^= data[12] ^= data[13];
    }
    else
    {
        uint16t *alpha = reinterpret_cast<uint16t*>(data);
        alpha[0] ^= alpha[3] ^= alpha[0] ^= alpha[3];
        alpha[1] ^= alpha[2] ^= alpha[1] ^= alpha[2];

        data[12] ^= data[15] ^= data[12] ^= data[15];
        data[13] ^= data[14] ^= data[13] ^= data[14];
    }
}
////////////////////////////////////////////////////////////////////////////////////////////////
void CDDSImageIO::VertFlipDXT5Block(bool halfblock, ubyte *data)
{
    //          MSB            LSB
    //          -----------------
    //          |    alpha_0    |
    //          -----------------
    //          |    alpha_1    |
    //          ---------------------------------------------------------------------------------
    //3 Bytes   | alpha h | alpha g | alpha f | alpha e | alpha d | alpha c | alpha b | alpha a |
    //          ---------------------------------------------------------------------------------
    //          | alpha p | alpha o | alpha n | alpha m | alpha l | alpha k | alpha j | alpha i |          
    //          ---------------------------------------------------------------------------------
    //2 Bytes   |   color_0 R[15:11] G[10:5] B[4:0]     |
    //          -----------------------------------------
    //          |               color_1                 |
    //          -----------------------------------------
    //1 Byte    | d | c | b | a |
    //          -----------------
    //          | h | g | f | e |
    //          -----------------
    //          | l | k | j | i |
    //          -----------------
    //          | p | o | n | m |
    //          -----------------

    if(halfblock)
    {
        Flip3BitClrBlock_2(&data[2]);
        Flip2BitClrBlock_2(&data[12]);
    }
    else
    {
        Flip3BitClrBlock(&data[2]);
        Flip2BitClrBlock(&data[12]);
    }
}
////////////////////////////////////////////////////////////////////////////////////////////////
void CDDSImageIO::VertFlipATI1NBlock(bool halfblock, ubyte *data)
{
    //          MSB          LSB
    //          ---------------
    //1 Byte    |    red_0    |
    //          ---------------
    //          |    red_1    |
    //          -----------------------------------------------------------------
    //3 Bytes   | red h | red g | red f | red e | red d | red c | red b | red a |
    //          -----------------------------------------------------------------
    //          | red p | red o | red n | red m | red l | red k | red j | red i |
    //          -----------------------------------------------------------------

    if(halfblock)
    {
        Flip3BitClrBlock_2(&data[2]);
    }
    else 
    {
        Flip3BitClrBlock(&data[2]);
    }
}
////////////////////////////////////////////////////////////////////////////////////////////////
void CDDSImageIO::VertFlipATI2NBlock(bool halfblock, ubyte *data)
{
    //          MSB          LSB
    //          ---------------
    //1 Byte    |    red_0    |
    //          ---------------
    //          |    red_1    |
    //          -----------------------------------------------------------------
    //3 Bytes   | red h | red g | red f | red e | red d | red c | red b | red a |
    //          -----------------------------------------------------------------
    //          | red p | red o | red n | red m | red l | red k | red j | red i |
    //          -----------------------------------------------------------------
    //1 Byte    |   green_0   |
    //          ---------------
    //          |   green_1   |
    //          -----------------------------------------------------------------
    //3 Bytes   | grn h | grn g | grn f | grn e | grn d | grn c | grn b | grn a |
    //          -----------------------------------------------------------------
    //          | grn p | grn o | grn n | grn m | grn l | grn k | grn j | grn i |
    //          -----------------------------------------------------------------

    if(halfblock)
    {
        Flip3BitClrBlock_2(&data[2]);
        Flip3BitClrBlock_2(&data[10]);
    }
    else 
    {
        Flip3BitClrBlock(&data[2]);
        Flip3BitClrBlock(&data[10]);
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////
void CDDSImageIO::Flip2BitClrBlock(ubyte *data)
{
    data[0] ^= data[3] ^= data[0] ^= data[3];
    data[1] ^= data[2] ^= data[1] ^= data[2];
}
////////////////////////////////////////////////////////////////////////////////////////////////
void CDDSImageIO::Flip2BitClrBlock_2(ubyte *data)
{
    data[0] ^= data[1] ^= data[0] ^= data[1];
}

////////////////////////////////////////////////////////////////////////////////////////////////
void CDDSImageIO::Flip3BitClrBlock(ubyte *data)
{
    ubyte line1[3] = {data[0],data[1],data[2]};
    ubyte line2[3] = {data[3],data[4],data[5]};

    data[0] = ((line2[1]>>4) & 0xF) | ((line2[2]<<4) & 0xF0);
    data[1] = ((line2[2]>>4) & 0xF) | ((line2[0]<<4) & 0xF0);
    data[2] = ((line2[0]>>4) & 0xF) | ((line2[1]<<4) & 0xF0);

    data[3] = ((line1[1]>>4) & 0xF) | ((line1[2]<<4) & 0xF0);
    data[4] = ((line1[2]>>4) & 0xF) | ((line1[0]<<4) & 0xF0);
    data[5] = ((line1[0]>>4) & 0xF) | ((line1[1]<<4) & 0xF0);
}

////////////////////////////////////////////////////////////////////////////////////////////////
void CDDSImageIO::Flip3BitClrBlock_2(ubyte *data)
{
    ubyte line[3] = {data[0],data[1],data[2]};
    data[0] = ((line[1]>>4) & 0xF) | (line[2] & 0xF);
    data[1] = ((line[2]>>4) & 0xF) | ((line[0]<<4) & 0xF0);
    data[2] = ((line[0]>>4) & 0xF) | ((line[1]<<4) & 0xF0);
}

////////////////////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////////////////////