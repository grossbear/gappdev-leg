///////////////////////////////////////////////////////////////////////////////////////////////
//	BitMapImageIO.h
//	BitMapImage Loading/Saving Class Definition
//  Supportes Some Common Formats
//	
////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef __BITMAPIMAGEIO_H__
#define __BITMAPIMAGEIO_H__

#include <PlatformTypes.h>
#include "Image/ImageIO.h"

struct  BMPHeader;
struct  TGAHeader;
struct  PCXHeader;
class   IDataStream;
class   CBMImage;

#define SUP_FMT_NUM 3
#define FMT_INFO_STR_SIZE 32
#define FMT_EXT_STR_SIZE 8

////////////////////////////////////////////////////////////////////////////////////////////////
class CBitMapImageIO : public CBMImageIO
{
public:
    const char* GetInfoString(uint32t idx=0) const;
    const char* GetExtString(uint32t idx=0) const;
    int         GetExtSupNum() const;
    CBMImage*   LoadBMImage(IDataStream *datastream, uint32t loadflags);
    bool        SaveBMImage(IDataStream *datastream, const CBMImage *image, ImgIOParam *params);

protected:
    bool CheckBMPFileHeader(const BMPHeader& header);
    bool CheckTGAFileHeader(const TGAHeader& header);
    bool CheckPCXFileHeader(const PCXHeader& header);

    static char m_strInfo[SUP_FMT_NUM][FMT_INFO_STR_SIZE];
    static char m_strExt[SUP_FMT_NUM][FMT_EXT_STR_SIZE];

    enum FMT_TYPE
    {
        TYPE_NONE,
        TYPE_BMP,
        TYPE_TGA,
        TYPE_PCX,
    };
};
////////////////////////////////////////////////////////////////////////////////////////////////

#endif //__BITMAPIMAGEIO_H__
