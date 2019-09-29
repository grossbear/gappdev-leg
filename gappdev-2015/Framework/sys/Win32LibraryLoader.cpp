/////////////////////////////////////////////////////////////////////////////////////////////
// Win32LibraryLoader.cpp
//
// Windows Dynamic Link Library Loader Class
/////////////////////////////////////////////////////////////////////////////////////////////

#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <windows.h>
#include "Renderer.h"
#include "Image/Image.h"
#include "Image/ImageIO.h"
#include "Image/ImageIOManager.h"
#include "LibraryLoader.h"
#include "LibraryTypes.h"

/////////////////////////////////////////////////////////////////////////////////////////////
typedef DYNAMIC_LIB_TYPE (*GETLIBRARYTYPEPROC)(void);
typedef const char* (GETLIBRARYINFOSTRING)(void);
typedef GAPILIBRARY (GETGRAPHICLIBRARYINFO)(void);
typedef IGraphicAPI* (*CREATEGRAPHICDEVINSTANCEPROC) (void);
//typedef void (*RELEASEGRAPHICDEVINSTANCEPROC) (void);

typedef int (*GETIMAGEIOCLASSCOUNTPROC)(void);
typedef CBMImageIO* (*CREATEIMAGEIOOBJECTPROC)(int);

/////////////////////////////////////////////////////////////////////////////////////////////
HMODULE  CWin32LibraryLoader::m_hRenderLibModule = NULL;
IGraphicAPI* CWin32LibraryLoader::m_pGraphicAPI = NULL;
HMODULE CWin32LibraryLoader::m_hBMImgIOLibModule = NULL;


/////////////////////////////////////////////////////////////////////////////////////////////
IGraphicAPI *CWin32LibraryLoader::LoadRenderModule(const char *name)
{
    char strpath[256]={0};
    strcat(strpath,"../../Bin/x86/");
    strcat(strpath,name);
    strcat(strpath,".dll");

    if(!m_hRenderLibModule)
        m_hRenderLibModule = LoadLibraryA(strpath);

    if(!m_hRenderLibModule)
        return NULL;

    GETLIBRARYTYPEPROC GetLibraryType = (GETLIBRARYTYPEPROC)GetProcAddress(m_hRenderLibModule,"GetLibraryType");
    if(!GetLibraryType)
        return NULL;

    DYNAMIC_LIB_TYPE libtype = GetLibraryType();
    if(libtype != LIB_RENDERER)
        return NULL;

    CREATEGRAPHICDEVINSTANCEPROC CreateGraphicDevInstFunc = 
        (CREATEGRAPHICDEVINSTANCEPROC)GetProcAddress(m_hRenderLibModule,"CreateGraphicDevInstance");

    if(!CreateGraphicDevInstFunc)
    {
        return NULL;
    }

    //Creating Graphic API
    IGraphicAPI *gapi = CreateGraphicDevInstFunc();
    m_pGraphicAPI = gapi;

    return gapi;
}

/////////////////////////////////////////////////////////////////////////////////////////////
void CWin32LibraryLoader::ReleaseRenderModule()
{
    if(m_hRenderLibModule)
    {
        delete (m_pGraphicAPI);
        m_pGraphicAPI = NULL;

        FreeLibrary(m_hRenderLibModule);
        m_hRenderLibModule = NULL;
    }
}


/////////////////////////////////////////////////////////////////////////////////////////////
bool CWin32LibraryLoader::LoadImageIOModule(const char *name)
{
    char strpath[256]={0};
    strcat(strpath,"../../Bin/x86/");
    strcat(strpath,name);
    strcat(strpath,".dll");

    if(!m_hBMImgIOLibModule)
        m_hBMImgIOLibModule = LoadLibraryA(strpath);
    
    DWORD error = GetLastError();

    if(!m_hBMImgIOLibModule)
        return false;

    GETLIBRARYTYPEPROC GetLibraryType = (GETLIBRARYTYPEPROC)GetProcAddress(m_hBMImgIOLibModule,"GetLibraryType");
    if(!GetLibraryType)
    {
        ReleaseImageIOModule();
        return false;
    }


    DYNAMIC_LIB_TYPE libtype = GetLibraryType();
    if(libtype != LIB_IMAGEIO)
    {
        ReleaseImageIOModule();
        return false;
    }

    GETIMAGEIOCLASSCOUNTPROC GetImageIOClassCount = 
        (GETIMAGEIOCLASSCOUNTPROC)GetProcAddress(m_hBMImgIOLibModule,"GetImageIOClassCount");

    CREATEIMAGEIOOBJECTPROC CreateImageIOObject = 
        (CREATEIMAGEIOOBJECTPROC)GetProcAddress(m_hBMImgIOLibModule,"CreateImageIOObject");

    if(!GetImageIOClassCount || !GetImageIOClassCount)
    {
        ReleaseImageIOModule();
        return false;
    }

    int count = GetImageIOClassCount();
    for(int i = 0; i < count; i++)
    {
        CBMImageIO *imageio = CreateImageIOObject(i);
        CBMImageIOSystem::GetSingleton()->AddImageIO(imageio);
    }

    return true;
}

/////////////////////////////////////////////////////////////////////////////////////////////
void CWin32LibraryLoader::ReleaseImageIOModule()
{
    if(m_hBMImgIOLibModule)
    {
        FreeLibrary(m_hBMImgIOLibModule);
        m_hBMImgIOLibModule = NULL;
    }
}

/////////////////////////////////////////////////////////////////////////////////////////////