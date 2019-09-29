///////////////////////////////////////////////////////////////////////////////////////////////
//	DDS.h
//	Direct Draw Surface Image Format Loading/Saving Class Declaration
//	
////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef __DDS_H__
#define __DDS_H__

#include <PlatformTypes.h>
#include "Image/ImageIO.h"

struct  DDSHeader;
class   IDataStream;
class   CBMImage;

////////////////////////////////////////////////////////////////////////////////////////////////
class CDDSImageIO : public CBMImageIO
{
public:
    CDDSImageIO();
    ~CDDSImageIO();

    const char* GetInfoString(uint32t idx=0) const;
    const char* GetExtString(uint32t idx=0) const;
    int         GetExtSupNum() const { return 1; }
    CBMImage*   LoadBMImage(IDataStream *datastream, uint32t loadflags);
    bool        SaveBMImage(IDataStream *datastream, const CBMImage *image, ImgIOParam *params);


protected:
    static BMIFMT GetFormat(const DDSHeader &header);
    static bool HasPalette(const DDSHeader &header);
    bool LoadPalImage(IDataStream *iostream, ubyte *data, ubyte *palette, uint32t size, bool rgba);

    bool IsFmtSupForSaving(BMIFMT fmt);
    void SetFileHeader(DDSHeader &header, const CBMImage *image);
    uint32t GetFourCCFmt(BMIFMT fmt);
    uint32t GetRGBABitMask(BMIFMT fmt,uint32t channel);

    void VertFlipImage(uint32t width, uint32t height, uint32t depth, uint32t mipmaps, uint32t align, BMIFMT fmt, ubyte *data); 
    void VertFlipCompressedImage(uint32t width, uint32t height, uint32t depth, uint32t mipmaps, uint32t align, BMIFMT fmt, ubyte *data);
    void VertFlipBlock(uint32t w, uint32t h, BMIFMT fmt, ubyte *data);
    void VertFlipDXT1Block(bool halfblock, ubyte *data);
    void VertFlipDXT3Block(bool halfblock, ubyte *data);
    void VertFlipDXT5Block(bool halfblock, ubyte *data);
    void VertFlipATI1NBlock(bool halfblock, ubyte *data);
    void VertFlipATI2NBlock(bool halfblock, ubyte *data);

    inline void Flip2BitClrBlock(ubyte *data);
    inline void Flip2BitClrBlock_2(ubyte *data);

    inline void Flip3BitClrBlock(ubyte *data);
    inline void Flip3BitClrBlock_2(ubyte *data);

    static const char m_strInfo[];
    static const char m_strExt[];
};




////////////////////////////////////////////////////////////////////////////////////////////////

#endif //__DDS_H__