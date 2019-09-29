/////////////////////////////////////////////////////////////////////////////////////////////
// WGLDevContext.h
//
// Window OpenGL Device Context Handle Class
/////////////////////////////////////////////////////////////////////////////////////////////

#ifndef __WGLDEVCONTEXT_H__
#define __WGLDEVCONTEXT_H__

#include <windows.h>
#include "GL/gl.h"
#include "GL/glext.h" //GL extensions
#include "GL/wglext.h"  //WGL extensions

#include "Renderapi.h" 
#include "WGLSupExtStruct.h"

#include <map>

class CWGLDeviceContext
{
public:
    static bool CreateRenderContext(const GAPICREATIONINFO *info);
    static void DestroyRenderContext();
    static bool SwapRenderBuffers();

    static bool IsVSyncSupported();
    static bool IsVSyncEnabled();
    static void EnableVSync(bool enable);

    static bool IsVSyncTearSupported();
    static bool IsVSyncTearEnabled();
    static void EnableVSyncTear(bool enable);

    static bool IsMutisampleSupported();
    static int GetMaxMultiSamples();
    static bool SetMultisample(int samples);


protected:  
    static bool SetupMultisampleFormats(const PIXELFORMATDESCRIPTOR &pfd);
    static bool InitEntryPoint(HWND hwnd, const PIXELFORMATDESCRIPTOR &pfd);
    
    inline static void SetPixelFormatDesc(PIXELFORMATDESCRIPTOR &pfd);

    static HWND CreateDummyWindow(const GAPICREATIONINFO *info);
    static void GetWGLSupExtInfo();
    static void SetupWGLExtProc();

    static std::map<int,int> m_PixelFormats;
    static WGLSupExtStruct m_WGLSupExt;

    static HWND    m_hWnd;
    static HDC     m_hDC;
    static HGLRC   m_hRC;
};

#endif //__WGLDEVCONTEXT_H__