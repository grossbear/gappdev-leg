///////////////////////////////////////////////////////////////////////////////////////////////
//	JPEG.h
//	JPEG Image Format Loading/Saving Class Declaration
//	
////////////////////////////////////////////////////////////////////////////////////////////////


#ifndef __CJPEGIMAGE_H__
#define __CJPEGIMAGE_H__

#include <PlatformTypes.h>
#include "Image/ImageIO.h"

#include "libjpeg/jpeglib.h"
#include "libjpeg/jerror.h"

class   IDataStream;
class   CBMImage;

////////////////////////////////////////////////////////////////////////////////////////////////

class CJPEGImageIO : public CBMImageIO
{
public:
    CJPEGImageIO();
    virtual ~CJPEGImageIO();

    const char* GetInfoString(uint32t idx=0) const;
    const char* GetExtString(uint32t idx=0) const;
    int         GetExtSupNum() const {return 1;}
    CBMImage*   LoadBMImage(IDataStream *datastream, uint32t loadflags);
    bool        SaveBMImage(IDataStream *datastream, const CBMImage *image, ImgIOParam *params);

protected:
    static boolean JpgReadFunc(j_decompress_ptr cinfo);
	static boolean JpgWriteFunc(j_compress_ptr cinfo);
    static void JpgTermDestFunc(j_compress_ptr cinfo);

    static char m_strInfo[];
    static char m_strExt[];
};

////////////////////////////////////////////////////////////////////////////////////////////////


#endif //__CJPEGIMAGE_H__

/*
#ifndef _JPEG_H_
#define _JPEG_H_

#define _CRT_SECURE_NO_WARNINGS

#include <PlatformTypes.h>

#include "Stream/IOStream.h"
#include "Image/ImageIO.h"

#include "libjpeg/jpeglib.h"

class  IOStream;

////////////////////////////////////////////////////////////////////////////////////////////////
class CJPEGFileIO : public CBMImageIO
{
public:
    CJPEGFileIO();
    virtual ~CJPEGFileIO();

    virtual const wchar_t*    GetSupportedFormats();
    virtual const wchar_t*    GetDescription();     

    virtual CBMImage*   LoadBMImage(const wchar_t *filename, uint32t loadflags, ImgDescData *pdesc=NULL);
    virtual CBMImage*   LoadBMImage(IOStream *iostream, uint32t loadflags, ImgDescData *pdesc=NULL);
    virtual bool        SaveBMImage(const CBMImage *image, const wchar_t *filename, uint32t descsize=0, const ImgDescData *descdata=NULL);
    virtual bool        SaveBMImage(const CBMImage *image, IOStream *iostream, uint32t descsize=0, const ImgDescData *descdata=NULL);

protected:
    static boolean JpgReadFunc(j_decompress_ptr cinfo);
	static boolean JpgWriteFunc(j_compress_ptr cinfo);
    static void JpgTermDestFunc(j_compress_ptr cinfo);

    static const wchar_t m_wstrExt[];
    static const wchar_t m_wstrDesc[];

    int m_quality;
};


////////////////////////////////////////////////////////////////////////////////////////////////

#endif //_JPEG_H_

*/