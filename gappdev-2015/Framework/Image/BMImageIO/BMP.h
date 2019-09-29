///////////////////////////////////////////////////////////////////////////////////////////////
//	BMP.h
//	BMP Format Loading/Saving Class Declaration
//	
////////////////////////////////////////////////////////////////////////////////////////////////


#ifndef __BMP_H__
#define __BMP_H__


#include <PlatformTypes.h>
#include "Image/ImageIO.h"

struct  BMPHeader;
class   IDataStream;
class   CBMImage;

////////////////////////////////////////////////////////////////////////////////////////////////

class CBMPImageIO : public CBMImageIO
{
public:
    CBMPImageIO();
    virtual ~CBMPImageIO();

    const char* GetInfoString(uint32t idx=0) const;
    const char* GetExtString(uint32t idx=0) const;
    int         GetExtSupNum() const { return 0; }
    CBMImage*   LoadBMImage(IDataStream *datastream, uint32t loadflags);
    bool        SaveBMImage(IDataStream *datastream, const CBMImage *image, ImgIOParam *params);

protected:
    bool CheckFileHeader(const BMPHeader& header);
    uint32t GetPaletteSize(const BMPHeader& header);
    void LoadExtraData(IDataStream *datastream, CBMImage *img, bool bread);
    void SetFileHeader(BMPHeader& header,uint32t width, uint32t height, BMIFMT fmt, uint32t palclrnum, uint32t infosize);

    CBMImage* LoadRGBImage(IDataStream *datastream, const BMPHeader& header, uint32t loadflags);
    CBMImage* Load8BitImage(IDataStream *datastream, const BMPHeader& header, const ubyte *palette, uint palclr, uint32t loadflags);
    CBMImage* Load8BitCompressedImage(IDataStream *datastream, const BMPHeader& header, ubyte *palette, uint32t loadflags);
    CBMImage* Load4BitImage(IDataStream *datastream, const BMPHeader& header, ubyte *palette, uint32t loadflags);
    CBMImage* Load1BitImage(IDataStream *datastream, const BMPHeader& header, ubyte *palette, uint32t loadflags);

    uint32t DecompressRLE8(ubyte *pixels, const ubyte *compdata, const BMPHeader& header);

    void ExpandPalette(ubyte *out, const ubyte *src, const ubyte *palette, uint32t width, uint32t channels);
    void RevertChannels(ubyte *pixels, uint32t width, uint32t height, uint32t stride, uint32t channels);

    static char m_strInfo[];
    static char m_strExt[];
};

////////////////////////////////////////////////////////////////////////////////////////////////


#endif //__BMP_H__