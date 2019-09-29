/////////////////////////////////////////////////////////////////////////////////////////////
// ImageIO.h
/////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _IMAGEIO_H_
#define _IMAGEIO_H_

#include <TCHAR.H>
#include "Library/LibraryTypes.h"

class CBMImage;
class IDataStream;


//Loading Image Flags
#define IMGIO_EXPAND_PALETTE        0x1
#define IMGIO_BGR_TO_RGB            0x2
#define IMGIO_LOAD_ALPHA            0x4
#define IMGIO_LOAD_PALETTE_ALPHA    0x8
#define IMGIO_SWAP_ALPHA            0x10
#define IMGIO_VERT_FLIP             0x20
#define IMGIO_LOAD_INFO             0x40

enum ImgIOParamType
{
    IMGIOPARAM_NONE,
    IMGIOPARAM_RLE,
    IMGIOPARAM_JPEG_QUALITY,
};

#define IMGIOPARAM_RLE8     0x1
#define IMGIOPARAM_RLE4     0x2

struct ImgIOParam
{
    ImgIOParamType type;
    uint32t value;
};

#define IMGIO_COMMON    IMGIO_EXPAND_PALETTE | IMGIO_LOAD_PALETTE_ALPHA | IMGIO_LOAD_ALPHA | IMGIO_VERT_FLIP

/////////////////////////////////////////////////////////////////////////////////////////////
class CBMImageIO
{
public:
    CBMImageIO(){}
    virtual ~CBMImageIO(){}

    //virtual const char* GetSupportedFormats() = 0;
    virtual const char* GetInfoString(uint32t idx=0) const = 0;
    virtual const char* GetExtString(uint32t idx=0) const = 0;
    virtual int         GetExtSupNum() const = 0;
    virtual CBMImage*   LoadBMImage(IDataStream *datastream, uint32t loadflags) = 0;
    virtual bool        SaveBMImage(IDataStream *datastream, const CBMImage *image, ImgIOParam *params) = 0;
};


/*
/////////////////////////////////////////////////////////////////////////////////////////////
typedef struct tagImgDescData
{
    ubyte *descdata;
    uint32t datasize;
}ImgDescData;

/////////////////////////////////////////////////////////////////////////////////////////////
class CBMImageIO
{
public:
    CBMImageIO(){}
    virtual ~CBMImageIO(){}

    virtual const wchar_t*    GetSupportedFormats() = 0;
    virtual CBMImage*   LoadBMImage(const wchar_t *filename, uint32t loadflags = IMGIO_COMMON, ImgDescData *descdata=NULL) = 0;
    virtual bool        SaveBMImage(const CBMImage *image, const wchar_t *filename, uint32t descsize=0, const ImgDescData *descdata=NULL) = 0;

protected:
    virtual void VertFlipImage(uint32t width, uint32t height, uint32t depth, uint32t mipmaps, uint32t align, BMIFMT fmt, ubyte *data); 
};


/////////////////////////////////////////////////////////////////////////////////////////////
class CBMImageIOSystem
{
protected:
    CBMImageIOSystem();
    static CBMImageIOSystem *m_pInstance;
public:
    static CBMImageIOSystem* GetSingleton();
    virtual ~CBMImageIOSystem();

    CBMImage*   LoadBMImage(const wchar_t *filename, uint32t loadflags = IMGIO_COMMON, ImgDescData *descdata=NULL);
    bool        SaveBMImage(const CBMImage *image, const wchar_t *filename, uint32t descsize=0, const ImgDescData *descdata=NULL);

    bool        AddImageIO(CBMImageIO *imageio);

protected:
    CBMImageIO *m_pImageIO[IMGIO_PTR_SIZE];
    unsigned    m_currpos;

private:
    bool        GetFileExtension(wchar_t *ext, const wchar_t *filename);
    void        ToLower(wchar_t *ext);
};


//////////////////////////////////////////////////////////////////////////////////////////////
// External ImageIO Library Functions
typedef unsigned int (*GETIMAGEIONUMPROC) (void);
typedef CBMImageIO* (*CREATEIMAGEIOPROC) (unsigned int idx);
*/

#endif //_IMAGEIO_H_