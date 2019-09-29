///////////////////////////////////////////////////////////////////////////////////////////////
//	PNG.h
//	PNG Image Format Loading/Saving Class Declaration
//	
////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _PNG_H_
#define _PNG_H_

#define _CRT_SECURE_NO_WARNINGS

#include <PlatformTypes.h>
#include "Image/ImageIO.h"
#include "libpng/png.h"

class  IDataStream;

////////////////////////////////////////////////////////////////////////////////////////////////
class CPNGImageIO : public CBMImageIO
{
public:
    CPNGImageIO();
    virtual ~CPNGImageIO();

    const char* GetInfoString(uint32t idx=0) const;
    const char* GetExtString(uint32t idx=0) const;
    int         GetExtSupNum() const { return 1; }
    CBMImage*   LoadBMImage(IDataStream *datastream, uint32t loadflags);
    bool        SaveBMImage(IDataStream *datastream, const CBMImage *image, ImgIOParam *params);

protected:
    BMIFMT SelImgFmt(int img_bpp, int color_type);

    static void PngErrorFunc(png_structp png_s, const char *png_msg);
	static void PngWarnFunc(png_structp png_s, const char *png_msg);
	static void PngReadFunc(png_structp png_s, png_bytep buff, png_size_t buffsize);
	static void PngWriteFunc(png_structp png_s, png_bytep buff, png_size_t buffsize);

    static const char m_strInfo[];
    static const char m_strExt[];
};

////////////////////////////////////////////////////////////////////////////////////////////////


#endif //_PNG_H_