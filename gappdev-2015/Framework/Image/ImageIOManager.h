/////////////////////////////////////////////////////////////////////////////////////////////
// ImageIOManager.h
/////////////////////////////////////////////////////////////////////////////////////////////

#ifndef __IMAGEIOMANAGER_H__
#define __IMAGEIOMANAGER_H__

#define _SET_IMAGE_IO_FORMATS


#include "ImageIO.h"
#include <string>
#include <map>

/////////////////////////////////////////////////////////////////////////////////////////////
class CBMImageIOSystem
{
protected:
    CBMImageIOSystem();
    static CBMImageIOSystem *m_pInstance;
public:
    static CBMImageIOSystem* GetSingleton();
    virtual ~CBMImageIOSystem();

    CBMImage*   LoadBMImage(const char *filename, uint32t loadflags = IMGIO_COMMON);
    bool        SaveBMImage(const CBMImage *image, const char *filename, ImgIOParam *params=NULL);

    bool        AddImageIO(CBMImageIO *imageio);

protected:
    //--CBMImageIO *m_pImageIO[IMGIO_PTR_SIZE];
    //--unsigned    m_currpos;
    std::map<std::string,CBMImageIO*> m_imgio_map;

private:
    bool        GetFileExtension(char *ext, const char *filename);
    void        ToLower(char *ext);

#ifdef _SET_IMAGE_IO_FORMATS
	void SetImageIOMap();
#endif //_SET_IMAGE_IO_FORMATS
};
/////////////////////////////////////////////////////////////////////////////////////////////

//extern CBMImageIOSystem gBMImgIOSys;

#endif //__IMAGEIOMANAGER_H__