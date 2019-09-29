///////////////////////////////////////////////////////////////////////////////////////////////
//	PCX.h
//	PCX Image Format Loading/Saving Class Declaration
//	
////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef __PCX_H__
#define __PCX_H__

#include <PlatformTypes.h>


#include "Image/ImageIO.h"


class   IDataStream;
struct  PCXHeader;
class   CBMImage;

////////////////////////////////////////////////////////////////////////////////////////////////
class CPCXImageIO : public CBMImageIO
{
public:
    CPCXImageIO();
    virtual ~CPCXImageIO();

    const char* GetInfoString(uint32t idx=0) const;
    const char* GetExtString(uint32t idx=0) const;
    int         GetExtSupNum() const { return 1; }
    CBMImage*   LoadBMImage(IDataStream *datastream, uint32t loadflags);
    bool        SaveBMImage(IDataStream *datastream, const CBMImage *image, ImgIOParam *params);

protected:
    static const char m_strInfo[];
    static const char m_strExt[]; 

    bool        CheckHeader(const PCXHeader &header);
    CBMImage*   LoadRGBImage(const PCXHeader &header, IDataStream *fs, uint32t loadflags);
    CBMImage*   Load8BitImage(const PCXHeader &header, IDataStream *fs, uint32t loadflags);
    void        VertFlipImage(ubyte *pixels, uint32t width, uint32t height, uint32t channels);

    void        SetFileHeader(PCXHeader &header, uint32t width, uint32t height, uint32t clrplanes);
    bool        WriteLine(IDataStream *fs, uint32t width, uint32t offset, const ubyte *pixels);
    bool        WritePalette(IDataStream *fs, const CBMImage *image);
};

////////////////////////////////////////////////////////////////////////////////////////////////

#endif //__PCX_H__
