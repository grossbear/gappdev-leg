/////////////////////////////////////////////////////////////////////////////////////////////
// LibraryLoader.h
//
// Dynamic Link Library Loader Class
/////////////////////////////////////////////////////////////////////////////////////////////

#ifndef __LIBRARYLOADER_H__
#define __LIBRARYLOADER_H__

class IGraphicAPI;

/////////////////////////////////////////////////////////////////////////////////////////////
class CLibraryLoader
{
public:
    static IGraphicAPI *LoadRenderModule(const char *name);
    static void ReleaseRenderModule();

protected:
    static HMODULE  m_hRenderLibModule;
};

/////////////////////////////////////////////////////////////////////////////////////////////

#endif //__LIBRARYLOADER_H__