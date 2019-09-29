/////////////////////////////////////////////////////////////////////////////////////////////
// LibraryLoader.cpp
//
// Dynamic Link Library Loader Class
/////////////////////////////////////////////////////////////////////////////////////////////

#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <windows.h>
#include "Renderer.h"
#include "LibraryLoader.h"


typedef LIB_TYPE (*GETLIBRARYTYPEPROC)(void);
typedef const char* (GETLIBRARYINFOSTRING)(void);
typedef GAPILIBRARY (GETGRAPHICLIBRARYINFO)(void);
typedef IGraphicAPI* (*CREATEGRAPHICDEVINSTANCEPROC) (void);
typedef void (*RELEASEGRAPHICDEVINSTANCEPROC) (void);

/////////////////////////////////////////////////////////////////////////////////////////////
HMODULE  CLibraryLoader::m_hRenderLibModule = NULL;

/////////////////////////////////////////////////////////////////////////////////////////////
IGraphicAPI *CLibraryLoader::LoadRenderModule(const char *name)
{
    char strpath[256]={0};
    strcat(strpath,"../../../Bin/x86/");
    strcat(strpath,name);
    strcat(strpath,".dll");

    if(!m_hRenderLibModule)
        m_hRenderLibModule = LoadLibraryA(strpath);

    if(!m_hRenderLibModule)
        return NULL;

    GETLIBRARYTYPEPROC GetLibraryType = (GETLIBRARYTYPEPROC)GetProcAddress(m_hRenderLibModule,"GetLibraryType");
    if(!GetLibraryType)
        return NULL;

    LIB_TYPE libtype = GetLibraryType();
    if(libtype != LIB_RENDERER)
        return NULL;

    CREATEGRAPHICDEVINSTANCEPROC CreateGraphicDevInstFunc = 
        (CREATEGRAPHICDEVINSTANCEPROC)GetProcAddress(m_hRenderLibModule,"CreateGraphicDevInstance");

    if(!CreateGraphicDevInstFunc)
    {
        return NULL;
    }

    //DLL Release Graphic API
    IGraphicAPI *gapi = CreateGraphicDevInstFunc();

    return gapi;
}

/////////////////////////////////////////////////////////////////////////////////////////////
void CLibraryLoader::ReleaseRenderModule()
{
    if(m_hRenderLibModule)
    {
        RELEASEGRAPHICDEVINSTANCEPROC ReleaseGraphicDevInstFunc = 
            (RELEASEGRAPHICDEVINSTANCEPROC) GetProcAddress(m_hRenderLibModule,"ReleaseGraphicDevInstance");
        if(ReleaseGraphicDevInstFunc)
            ReleaseGraphicDevInstFunc();

        FreeLibrary(m_hRenderLibModule);
        m_hRenderLibModule = NULL;
    }
}

/////////////////////////////////////////////////////////////////////////////////////////////


/*
    if(!m_hRenderdllModule)
        m_hRenderdllModule = LoadLibrary(L"..\\..\\dll\\Render\\GLRender.dll"); //L"..\\dll\\Render\\GLRender.dll"

    if(!m_hRenderdllModule)
    {
        MessageBox(NULL,L"Failed To Load Render API Library",L"Library Load Error",MB_ICONERROR);
        return NULL;
    }

    CREATEGRAPICDEVINSTANCEPROC CreateGraphicDevInstFunc = 
        (CREATEGRAPICDEVINSTANCEPROC)GetProcAddress(m_hRenderdllModule,"CreateGraphicDevInstance");

    if(!CreateGraphicDevInstFunc)
    {
        MessageBox(NULL,L"Failed To Load Render Library Function",L"Library Function Load Error",MB_ICONERROR);
        return NULL;
    }

    //DLL Release Graphic API
    CNullGraphicAPI *gapi = CreateGraphicDevInstFunc();
    if(!gapi)
        MessageBox(NULL,L"Failed To Load Render Module From Library", L"Library Function Load Error",MB_ICONERROR);

    return gapi;
    */