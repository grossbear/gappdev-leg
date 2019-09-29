///////////////////////////////////////////////////////////////////////////////////////////////
//	TGA.h
//	Targa Image Format Loading/Saving Class Declaration
//	
////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef __TGA_H__
#define __TGA_H__

#include <PlatformTypes.h>
#include "Image/ImageIO.h"

struct  TGAHeader;
class   IDataStream;
class   CBMImage;

////////////////////////////////////////////////////////////////////////////////////////////////
class CTGAImageIO : public CBMImageIO
{
public:
    CTGAImageIO();
    ~CTGAImageIO();

    const char* GetInfoString(uint32t idx=0) const;
    const char* GetExtString(uint32t idx=0) const;
    int         GetExtSupNum() const { return 1; }
    CBMImage*   LoadBMImage(IDataStream *datastream, uint32t loadflags);
    bool        SaveBMImage(IDataStream *datastream, const CBMImage *image, ImgIOParam *params);


protected:
    bool    CheckFileHeader(const TGAHeader& header);
    void    SetFileHeader(TGAHeader& header, const CBMImage &image);

    CBMImage* LoadUncompressedImage(IDataStream *file, const TGAHeader &header, uint32t loadflags);
    CBMImage* LoadCompressedImage(IDataStream *file, const TGAHeader &header, uint32t loadflags);
    CBMImage* LoadUncompressedColorMappedImage(IDataStream *file, const TGAHeader &header, uint32t loadflags);
    CBMImage* LoadCompressedColorMappedImage(IDataStream *file, const TGAHeader &header, uint32t loadflags);

    bool    DecompressData(ubyte *pixels, uint32t imgchannels, IDataStream *file, const TGAHeader &header, ubyte *palette);

    void    RevertChannels(ubyte *pixels, uint32t width, uint32t height, uint32t channels);
    void    VertFlipImage(ubyte *pixels, uint32t width, uint32t height, uint32t channels);
    void    HoriFlipImage(ubyte *pixels, uint32t width, uint32t height, uint32t channels);

    static char m_strInfo[];
    static char m_strExt[];
};

////////////////////////////////////////////////////////////////////////////////////////////////

#endif //__TGA_H__