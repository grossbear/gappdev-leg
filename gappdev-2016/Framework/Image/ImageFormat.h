///////////////////////////////////////////////////////////////////////////////////////////////
//	ImageFormat.h
//	Bitmap Image Format Enum Type And Helper Functions
//	
////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _BMIMAGEFORMAT_H_
#define _BMIMAGEFORMAT_H_

//_PLATFORMTYPES_H_ include needed

//Bitmap Image Format Enum
enum BMIMAGEFORMAT  
{
    IMGFMT_NONE, 
    IMGFMT_L8,
    IMGFMT_LA8,
    IMGFMT_RGB8,
    IMGFMT_RGBA8,
    IMGFMT_BGR8,
    IMGFMT_BGRA8,
    IMGFMT_L16,
    IMGFMT_LA16,
    IMGFMT_RG16,
    IMGFMT_RGB16,
    IMGFMT_RGBA16,
    IMGFMT_R16f,
    IMGFMT_RG16f,
    IMGFMT_RGB16f,
    IMGFMT_RGBA16f,
    IMGFMT_R32f,
    IMGFMT_RG32f,
    IMGFMT_RGB32f,
    IMGFMT_RGBA32f,

    IMGFMT_UV8,
    IMGFMT_UVWQ8,
    IMGFMT_UV16,
    IMGFMT_UVWQ16,

    IMGFMT_DEPTH16,
    IMGFMT_DEPTH24,
    IMGFMT_L4A4,
    IMGFMT_A4RGB4,
    IMGFMT_R3G3B2,
    IMGFMT_R5G6B5,
    IMGFMT_A1RGB5,
    IMGFMT_A2RGB10,
    IMGFMT_RG11B10,
    IMGFMT_RGB9E5,

    IMGFMT_UV5L6,
    IMGFMT_UVW10A2,
    
    IMGFMT_DXT1,
    IMGFMT_DXT3,
    IMGFMT_DXT5,
    IMGFMT_ATI1N,
    IMGFMT_ATI2N,
};

typedef BMIMAGEFORMAT BMIFMT;
////////////////////////////////////////////////////////////////////////////////////////////////
class CBMImgFmtInfo
{
public:
    static inline bool IsPlainFormat(const BMIFMT fmt);
    static inline bool IsPackedFormat(const BMIFMT fmt);
    static inline bool IsDepthFormat(const BMIFMT fmt);
    static inline bool IsCompressedFormat(const BMIFMT fmt);
    static inline bool IsFloatFormat(const BMIFMT fmt);
    static inline bool IsSignedFormat(const BMIFMT fmt);
    static inline bool IsMixedFormat(const BMIFMT fmt);
    static inline int GetBytesPerBlock(const BMIFMT fmt);

    static inline int GetChannelsCount(const BMIFMT bmifmt);
    static inline int GetBytesPerPixel(const BMIFMT bmifmt);
    static inline int GetChannelSize(const BMIFMT bmifmt, int channel);
};

////////////////////////////////////////////////////////////////////////////////////////////////
bool CBMImgFmtInfo::IsPlainFormat(const BMIFMT fmt)
{
    return (fmt <= IMGFMT_UVWQ16);
}

////////////////////////////////////////////////////////////////////////////////////////////////
bool CBMImgFmtInfo::IsPackedFormat(const BMIFMT fmt)
{
    return (fmt >= IMGFMT_L4A4 && fmt <= IMGFMT_RGB9E5);
}

////////////////////////////////////////////////////////////////////////////////////////////////
bool CBMImgFmtInfo::IsDepthFormat(const BMIFMT fmt)
{
    return (fmt >= IMGFMT_DEPTH16 && fmt <= IMGFMT_DEPTH24);
}

////////////////////////////////////////////////////////////////////////////////////////////////
bool CBMImgFmtInfo::IsCompressedFormat(const BMIFMT fmt)
{
    return (fmt >= IMGFMT_DXT1);
}

////////////////////////////////////////////////////////////////////////////////////////////////
bool CBMImgFmtInfo::IsFloatFormat(const BMIFMT fmt)
{
    return (fmt >= IMGFMT_R16f && fmt <= IMGFMT_RGBA32f);
}

////////////////////////////////////////////////////////////////////////////////////////////////
bool CBMImgFmtInfo::IsSignedFormat(const BMIFMT fmt)
{
    return (fmt >= IMGFMT_UV8 && fmt <= IMGFMT_UVWQ16);
}

////////////////////////////////////////////////////////////////////////////////////////////////
bool CBMImgFmtInfo::IsMixedFormat(const BMIFMT fmt)
{
    return (fmt >= IMGFMT_UV5L6 && fmt <= IMGFMT_UVW10A2);
}

////////////////////////////////////////////////////////////////////////////////////////////////
// Accepts compressed formats only
int CBMImgFmtInfo::GetBytesPerBlock(const BMIFMT fmt)
{
    return (CBMImgFmtInfo::IsCompressedFormat(fmt)) ? ((fmt == IMGFMT_DXT1 || fmt == IMGFMT_ATI1N)? 8 : 16) : (0);
}

////////////////////////////////////////////////////////////////////////////////////////////////
int CBMImgFmtInfo::GetChannelsCount(const BMIFMT bmifmt)
{
    
    static const int channels[] = {     0,  //IMGFMT_NONE 

                                        //Unsigned Formats
                                        1,  //IMGFMT_L8
                                        2,  //IMGFMT_LA8
                                        3,  //IMGFMT_RGB8
                                        4,  //IMGFMT_RGBA8
                                        3,  //IMGFMT_BGR8
                                        4,  //IMGFMT_BGRA8
                                        1,  //IMGFMT_L16
                                        2,  //IMGFMT_LA16
                                        2,  //IMGFMT_RG16
                                        3,  //IMGFMT_RGB16
                                        4,  //IMGFMT_RGBA16

                                        //Float Formats
                                        1,  //IMGFMT_R16f
                                        2,  //IMGFMT_RG16f
                                        3,  //IMGFMT_RGB16f
                                        4,  //IMGFMT_RGBA16f
                                        1,  //IMGFMT_R32f,
                                        2,  //IMGFMT_RG32f,
                                        3,  //IMGFMT_RGB32f
                                        4,  //IMGFMT_RGBA32f

                                        //Signed Formats
                                        2,  //IMGFMT_UV8
                                        4,  //IMGFMT_UVWQ8
                                        2,  //IMGFMT_UV16
                                        4,  //IMGFMT_UVWQ16

                                        //Depth Formats
                                        1,  //IMGFMT_DEPTH16
                                        1,  //IMGFMT_DEPTH24

                                        //Packed Formats
                                        2,  //IMGFMT_L4A4
                                        4,  //IMGFMT_A4RGB4
                                        3,  //IMGFMT_R3G3B2
                                        3,  //IMGFMT_R5G6B5
                                        4,  //IMGFMT_A1RGB5
                                        4,  //IMGFMT_A2RGB10
                                        3,  //IMGFMT_RG11B10
                                        4,  //IMGFMT_RGB9E5

                                        //Mixed Formats
                                        2,  //IMGFMT_UV5L6,
                                        4,  //IMGFMT_UVW10A2,

                                        //Compressed Formats
                                        3,  //IMGFMT_DXT1
                                        4,  //IMGFMT_DXT3
                                        4,  //IMGFMT_DXT5
                                        1,  //IMGFMT_ATI1N
                                        2,  //IMGFMT_ATI2N
    };

    return channels[bmifmt];
}

////////////////////////////////////////////////////////////////////////////////////////////////
int CBMImgFmtInfo::GetBytesPerPixel(const BMIFMT bmifmt)
{
    static const int bytes[] =    {     0,  //IMGFMT_NONE 

                                        //Unsigned Formats
                                        1,  //IMGFMT_L8
                                        2,  //IMGFMT_LA8
                                        3,  //IMGFMT_RGB8
                                        4,  //IMGFMT_RGBA8
                                        3,  //IMGFMT_BGR8
                                        4,  //IMGFMT_BGRA8
                                        2,  //IMGFMT_L16
                                        4,  //IMGFMT_LA16
                                        4,  //IMGFMT_RG16
                                        6,  //IMGFMT_RGB16
                                        8,  //IMGFMT_RGBA16

                                        //Float Formats
                                        2,  //IMGFMT_R16f
                                        4,  //IMGFMT_RG16f    
                                        6,  //IMGFMT_RGB16f
                                        8,  //IMGFMT_RGBA16f
                                        4,  //IMGFMT_R32f,
                                        8,  //IMGFMT_RG32f,
                                        12, //IMGFMT_RGB32f
                                        16, //IMGFMT_RGBA32f

                                        //Signed Format
                                        2, //IMGFMT_UV8
                                        4, //IMGFMT_UVWQ8
                                        4, //IMGFMT_UV16
                                        8, //IMGFMT_UVWQ16

                                        //Depth Formats
                                        2,  //IMGFMT_DEPTH16
                                        3,  //IMGFMT_DEPTH24

                                        //Packed Formats
                                        1,  //IMGFMT_L4A4
                                        2,  //IMGFMT_A4RGB4
                                        1,  //IMGFMT_R3G3B2
                                        2,  //IMGFMT_R5G6B5
                                        2,  //IMGFMT_A1RGB5
                                        4,  //IMGFMT_A2RGB10
                                        4,  //IMGFMT_RG11B10
                                        4,  //IMGFMT_RGB9E5

                                        //Mixed Formats
                                        2,  //IMGFMT_UV5L6
                                        4,  //IMGFMT_UVW10A2

                                        //Compressed Formats
                                        1,  //IMGFMT_DXT1
                                        2,  //IMGFMT_DXT3
                                        2,  //IMGFMT_DXT5
                                        0,  //IMGFMT_ATI1N
                                        0,  //IMGFMT_ATI2N
    };

    return bytes[bmifmt];
}

////////////////////////////////////////////////////////////////////////////////////////////////
// Getting Channel Size In Bits
int CBMImgFmtInfo::GetChannelSize(const BMIFMT bmifmt, int channel)
{
    if(channel < 1 || channel > 4)
        return 0;

    static const int channels[][4] = { 
    
        {0,0,0,0},      //IMGFMT_NONE 

        //Unsigned Formats
        {8,0,0,0},      //IMGFMT_L8
        {8,8,0,0},      //IMGFMT_LA8
        {8,8,8,0},      //IMGFMT_RGB8
        {8,8,8,8},      //IMGFMT_RGBA8
        {8,8,8,0},      //IMGFMT_BGR8
        {8,8,8,8},      //IMGFMT_BGRA8
        {16,0,0,0},     //IMGFMT_L16
        {16,16,0,0},    //IMGFMT_LA16
        {16,16,0,0},    //IMGFMT_RG16
        {16,16,16,0},   //IMGFMT_RGB16
        {16,16,16,16},  //IMGFMT_RGBA16

        //Float Formats
        {16,0,0,0},     //IMGMFT_R16f
        {16,16,0,0},    //IMGFMT_RG16f
        {16,16,16,0},   //IMGFMT_RGB16f
        {16,16,16,16},  //IMGFMT_RGBA16f
        {32,0,0,0},     //IMGFMT_R32f
        {32,32,0,0},    //IMGFMT_RG32f
        {32,32,32,0},   //IMGFMT_RGB32f
        {32,32,32,32},  //IMGFMT_RGBA32f

        //Signed Formats
        {8,8,0,0},      //IMGFMT_UV8
        {8,8,8,8},      //IMGFMT_UVWQ8
        {16,16,0,0},    //IMGFMT_UV16
        {16,16,16,16},  //IMGFMT_UVWQ16

        //Depth Formats
        {16,0,0,0},     //IMGFMT_DEPTH16
        {24,0,0,0},     //IMGFMT_DEPTH24

        //Packed Formats
        {4,4,0,0},      //IMGFMT_L4A4
        {4,4,4,4},      //IMGFMT_A4RGB4
        {3,3,2,0},      //IMGFMT_R3G3B2
        {5,6,5,0},      //IMGFMT_R5G6B5
        {1,5,5,5},      //IMGFMT_A1RGB5
        {2,10,10,10},   //IMGFMT_A2RGB10
        {11,11,10,0},   //IMGFMT_RG11B10
        {9,9,9,5},      //IMGFMT_RGB9E5

        //Mixed Formats
        {5,5,6,0},      //IMGFMT_UV5L6
        {10,10,10,2},   //IMGFMT_UVW10A2

        //Compressed Formats
        {0,0,0,0},      //IMGFMT_DXT1
        {0,0,0,0},      //IMGFMT_DXT3
        {0,0,0,0},      //IMGFMT_DXT5 
        {0,0,0,0},      //IMGFMT_ATI1N
        {0,0,0,0},      //IMGFMT_ATI2N
    };

    return channels[bmifmt][channel-1];
}

////////////////////////////////////////////////////////////////////////////////////////////////
// Format Convertion functions

////////////////////////////////////////////////////////////////////////////////////////////////
class CBMImgFmtConv
{
public:
    static inline uint32t   A2BGR10ToA2RGB10(uint32t c);
    static inline ubyte     A4L4ToL4A4(ubyte c);
    static inline uint32t   A8R3G3B2ToRGBA8(uint16t c);
};

////////////////////////////////////////////////////////////////////////////////////////////////
uint32t CBMImgFmtConv::A2BGR10ToA2RGB10(uint32t c)
{
    return (c & 0xc0000000) | ((c & 0x3ff)<<20) | (c & 0xfcc00) | ((c >> 20)& 0x3ff);
}

////////////////////////////////////////////////////////////////////////////////////////////////
ubyte CBMImgFmtConv::A4L4ToL4A4(ubyte c)
{
    return ((c << 4) & 0xf0) | ((c >> 4) & 0xf);
}

////////////////////////////////////////////////////////////////////////////////////////////////
uint32t CBMImgFmtConv::A8R3G3B2ToRGBA8(uint16t c)
{
    //return ((c >> 8) & 0xff) | ( (c<<24) & 0xe0000000 ) | ( (c<<19) & 0xe00000  ) | ( (c<<14) & 0xc000 );
    return ((c << 16) & 0xe0000000) | ((c) & 0xe0) | ((c << 11) & 0xe000) | ((c << 22) & 0xc00000);
}

////////////////////////////////////////////////////////////////////////////////////////////////

#endif //_BMIMAGEFORMAT_H_