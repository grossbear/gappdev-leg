/////////////////////////////////////////////////////////////////////////////////////////////
// WGLDevContext.cpp
//
// Window OpenGL Device Context Handle Class
/////////////////////////////////////////////////////////////////////////////////////////////

#include "WGLDevContext.h"
#include "Utils/Logger/Logger.h"
#include "sys/getproc.h"
#include <TCHAR.H>
#include <string>

//--#define USE_OPENGL_3_API

//#include "GLExtProc.h"
/////////////////////////////////////////////////////////////////////////////////////////////
HWND    CWGLDeviceContext::m_hWnd = NULL;
HDC     CWGLDeviceContext::m_hDC = NULL;
HGLRC   CWGLDeviceContext::m_hRC = NULL;

std::map<int,int> CWGLDeviceContext::m_PixelFormats;
WGLSupExtStruct CWGLDeviceContext::m_WGLSupExt;

/////////////////////////////////////////////////////////////////////////////////////////////

// Function pointer for extended OpenGL context
PFNWGLGETEXTENSIONSSTRINGARBPROC    wglGetExtensionsStringARB = NULL;

// WGL_EXT_swap_control
PFNWGLSWAPINTERVALEXTPROC wglSwapIntervalEXT = NULL;
PFNWGLGETSWAPINTERVALEXTPROC wglGetSwapIntervalEXT = NULL;

// WGL_ARB_pixel_format
PFNWGLGETPIXELFORMATATTRIBIVARBPROC wglGetPixelFormatAttribivARB = NULL;
PFNWGLGETPIXELFORMATATTRIBFVARBPROC wglGetPixelFormatAttribfvARB = NULL;
PFNWGLCHOOSEPIXELFORMATARBPROC wglChoosePixelFormatARB = NULL;

// WGL_ARB_create_context
PFNWGLCREATECONTEXTATTRIBSARBPROC wglCreateContextAttribsARB = NULL;

/////////////////////////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////////////////////////////
bool CWGLDeviceContext::CreateRenderContext(const GAPICREATIONINFO *info)
{
    // Getting Window Handle
    m_hWnd = reinterpret_cast<HWND>(info->hWindow);
    // Did We Get Window Handle
    if(m_hWnd==NULL)
    {
        //logger
        return false;
    }
    // Did We Get A Device Context?
    if (!(m_hDC=GetDC(m_hWnd)))							
    {									
        //logger - GLAPIERROR_CREATE_DEVICE_CONTEXT_FAILED
        return false;								
    }
    
    PIXELFORMATDESCRIPTOR pfd;
    CWGLDeviceContext::SetPixelFormatDesc(pfd);

    // Getting Default Pixel Format
    m_PixelFormats[0] = ChoosePixelFormat(m_hDC,&pfd);

    HWND hPFwnd = CWGLDeviceContext::CreateDummyWindow(info);
    if(!hPFwnd)
    {
        // logger - failed to create dummy window 
        return false;
    }

    if(!CWGLDeviceContext::InitEntryPoint(hPFwnd,pfd))
    {
        // Close dummy window
        SendMessage(hPFwnd, WM_CLOSE, 0, 0);
        //logger
        return false;
    }

    // Close dummy window
    SendMessage(hPFwnd, WM_CLOSE, 0, 0);
   
    // Setup Multisample Formats
    CWGLDeviceContext::SetupMultisampleFormats(pfd);

    int samples = info->iMultisample;
    if(m_PixelFormats.count(samples)==0)
        samples = 0;
    int pixelformat = m_PixelFormats[samples];
    if(!pixelformat || !SetPixelFormat(m_hDC, pixelformat, &pfd))
    {
        LOG_ERROR(LOG_RENDER_ERROR,"Setting pixel format fail (%d)\n", GetLastError());
        return false;
    }
    

#ifdef USE_OPENGL_3_API
    // setting attributes for creating extended OpenGL context
    int attribs[] =
    {
        WGL_CONTEXT_MAJOR_VERSION_ARB, 3,
        WGL_CONTEXT_MINOR_VERSION_ARB, 3,
        WGL_CONTEXT_FLAGS_ARB,         WGL_CONTEXT_FORWARD_COMPATIBLE_BIT_ARB,
        WGL_CONTEXT_PROFILE_MASK_ARB,  WGL_CONTEXT_CORE_PROFILE_BIT_ARB,
        0
    };

    // creating temp render context 
    // it is necessary for getting wglCreateContextAttribsARB
    HGLRC hRCTemp = wglCreateContext(m_hDC);
    if (!hRCTemp || !wglMakeCurrent(m_hDC, hRCTemp))
    {
        DWORD err = GetLastError();
        //LOG_ERROR("Creating temp render context fail (%d)\n", GetLastError());
        return false;
    }

    // getting function pointer for setting render context attributes
    OPENGL_GET_PROC(PFNWGLCREATECONTEXTATTRIBSARBPROC,wglCreateContextAttribsARB);

    // we do not need any more temporary OpenGL context 
    wglMakeCurrent(NULL, NULL);
    wglDeleteContext(hRCTemp);

    if (!wglCreateContextAttribsARB)
    {
        return false;
    }


    // creating extended context that supports OpenGL 3
    m_hRC = wglCreateContextAttribsARB(m_hDC, 0, attribs);
#else
    // Are We Able To Get A Rendering Context?
    if (!(m_hRC=wglCreateContext(m_hDC)))				
    {	
        //--errinfo.SetDevErrorInfo(GLAPIERROR_CREATE_RENDERING_CONTEXT_FAILED);
        return false;								
    }
#endif //USE_OPENGL_3

    if (!m_hRC || !wglMakeCurrent(m_hDC, m_hRC))
    {
        //LOG_ERROR("Creating render context fail (%d)\n", GetLastError());
        return false;
    }

    //Setting Vertical Synchronization
    CWGLDeviceContext::EnableVSync(info->bVSync);
    
    return true;
}

/////////////////////////////////////////////////////////////////////////////////////////////
void CWGLDeviceContext::DestroyRenderContext()
{
    // Do We Have A Rendering Context?
    if (m_hRC)											
    {
        // Are We Able To Release The DC And RC Contexts?
        if (!wglMakeCurrent(NULL,NULL))					
        {
            //--CGLErrorInfo::GetSingleton().SetDevErrorInfo(GLAPIERROR_RELEASE_DC_RD_FAILED);
        }

        // Are We Able To Delete The RC?
        if (!wglDeleteContext(m_hRC))						
        {
            //--CGLErrorInfo::GetSingleton().SetDevErrorInfo(GLAPIERROR_RELEASE_RENDERING_CONTEXT_FAILED);
        }
        // Set RC To NULL
        m_hRC = NULL;										
    }

    // Are We Able To Release The DC
    if (ReleaseDC(m_hWnd,m_hDC))					
    {
        //--CGLErrorInfo::GetSingleton().SetDevErrorInfo(GLAPIERROR_RELEASE_DEVICE_CONTEXT_FAILED);
        m_hDC = NULL;   // Set DC To NULL										
    }
}

/////////////////////////////////////////////////////////////////////////////////////////////
bool CWGLDeviceContext::SwapRenderBuffers()
{
    BOOL res = SwapBuffers(m_hDC);
    return true;
}

/////////////////////////////////////////////////////////////////////////////////////////////
bool CWGLDeviceContext::IsVSyncSupported()
{
    return (bool)m_WGLSupExt.EXT_swap_control;
}

/////////////////////////////////////////////////////////////////////////////////////////////
bool CWGLDeviceContext::IsVSyncEnabled()
{
    if(m_WGLSupExt.EXT_swap_control)
        return false;

    int interval = wglGetSwapIntervalEXT();
    return interval!=0;
}
/////////////////////////////////////////////////////////////////////////////////////////////
void CWGLDeviceContext::EnableVSync(bool enable)
{
    if(m_WGLSupExt.EXT_swap_control)
    {
        wglSwapIntervalEXT((enable)?(1):(0));
    }
}

/////////////////////////////////////////////////////////////////////////////////////////////
bool CWGLDeviceContext::IsVSyncTearSupported()
{
    return (bool)m_WGLSupExt.EXT_swap_control_tear;
}
/////////////////////////////////////////////////////////////////////////////////////////////
bool CWGLDeviceContext::IsVSyncTearEnabled()
{
    if(m_WGLSupExt.EXT_swap_control)
        return false;

    int interval = wglGetSwapIntervalEXT();
    return interval==-1;
}
/////////////////////////////////////////////////////////////////////////////////////////////
void CWGLDeviceContext::EnableVSyncTear(bool enable)
{
    if(m_WGLSupExt.EXT_swap_control)
        wglSwapIntervalEXT((enable)?(-1):(0));
}

/////////////////////////////////////////////////////////////////////////////////////////////
bool CWGLDeviceContext::IsMutisampleSupported()
{
    return m_PixelFormats.size() > 1;
}
/////////////////////////////////////////////////////////////////////////////////////////////
int CWGLDeviceContext::GetMaxMultiSamples()
{
    std::map<int,int>::reverse_iterator rev_it = m_PixelFormats.rbegin();
    return rev_it->first;
}
/////////////////////////////////////////////////////////////////////////////////////////////
bool CWGLDeviceContext::SetMultisample(int samples)
{
    return false;
}

/////////////////////////////////////////////////////////////////////////////////////////////
// Protected Methods
/////////////////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////////////////
HWND CWGLDeviceContext::CreateDummyWindow(const GAPICREATIONINFO *info)
{
    HINSTANCE hInst = (HINSTANCE) (LONG_PTR) GetWindowLongPtr(m_hWnd, GWLP_HINSTANCE);
    WNDCLASS wincl;
    wincl.hInstance = hInst;
    wincl.lpszClassName = _T("PFrmt");
    wincl.lpfnWndProc = DefWindowProc;
    wincl.style = 0;
    wincl.hIcon = NULL;
    wincl.hCursor = NULL;
    wincl.lpszMenuName = NULL;
    wincl.cbClsExtra = 0;
    wincl.cbWndExtra = 0;
    wincl.hbrBackground = NULL;
    RegisterClass(&wincl);

    // Create a dummy window to get our extension entry points
    HWND hPFwnd = CreateWindow(_T("PFrmt"), _T("PFormat"), WS_POPUP | WS_CLIPCHILDREN | WS_CLIPSIBLINGS, 0, 0, 8, 8, HWND_DESKTOP, NULL, hInst, NULL);

    return hPFwnd;
}
/////////////////////////////////////////////////////////////////////////////////////////////
void CWGLDeviceContext::GetWGLSupExtInfo()
{
    const char *strext = wglGetExtensionsStringARB(wglGetCurrentDC());
    std::string strwgl(strext);


    m_WGLSupExt.ARB_extensions_string = OPENGL_GET_SUP_EXT(strwgl,WGL_ARB_extensions_string);
    m_WGLSupExt.ARB_pixel_format = OPENGL_GET_SUP_EXT(strwgl,WGL_ARB_pixel_format);
    m_WGLSupExt.ATI_pixel_format_float = OPENGL_GET_SUP_EXT(strwgl,WGL_ATI_pixel_format_float);
    m_WGLSupExt.ARB_pixel_format_float = OPENGL_GET_SUP_EXT(strwgl,WGL_ARB_pixel_format_float);
    m_WGLSupExt.ARB_multisample = OPENGL_GET_SUP_EXT(strwgl,WGL_ARB_multisample);
    m_WGLSupExt.EXT_swap_control = OPENGL_GET_SUP_EXT(strwgl,WGL_EXT_swap_control);
    m_WGLSupExt.EXT_swap_control_tear = OPENGL_GET_SUP_EXT(strwgl,WGL_EXT_swap_control_tear);
    m_WGLSupExt.ARB_pbuffer = OPENGL_GET_SUP_EXT(strwgl,WGL_ARB_pbuffer);
    m_WGLSupExt.ARB_render_texture = OPENGL_GET_SUP_EXT(strwgl,WGL_ARB_render_texture);
    m_WGLSupExt.ARB_make_current_read = OPENGL_GET_SUP_EXT(strwgl,WGL_ARB_make_current_read);
    m_WGLSupExt.EXT_extensions_string = OPENGL_GET_SUP_EXT(strwgl,WGL_EXT_extensions_string);
    m_WGLSupExt.ARB_buffer_region = OPENGL_GET_SUP_EXT(strwgl,WGL_ARB_buffer_region);
    m_WGLSupExt.EXT_framebuffer_sRGB = OPENGL_GET_SUP_EXT(strwgl,WGL_EXT_framebuffer_sRGB);
    m_WGLSupExt.ATI_render_texture_rectangle = OPENGL_GET_SUP_EXT(strwgl,WGL_ATI_render_texture_rectangle);
    m_WGLSupExt.EXT_pixel_format_packed_float = OPENGL_GET_SUP_EXT(strwgl,WGL_EXT_pixel_format_packed_float);
    m_WGLSupExt.I3D_genlock = OPENGL_GET_SUP_EXT(strwgl,WGL_I3D_genlock);
    m_WGLSupExt.NV_swap_group = OPENGL_GET_SUP_EXT(strwgl,WGL_NV_swap_group);
    m_WGLSupExt.ARB_create_context = OPENGL_GET_SUP_EXT(strwgl,WGL_ARB_create_context);
    m_WGLSupExt.AMD_gpu_association = OPENGL_GET_SUP_EXT(strwgl,WGL_AMD_gpu_association);
    m_WGLSupExt.AMDX_gpu_association = OPENGL_GET_SUP_EXT(strwgl,WGL_AMDX_gpu_association);
    m_WGLSupExt.ARB_create_context_profile = OPENGL_GET_SUP_EXT(strwgl,WGL_ARB_create_context_profile);
    m_WGLSupExt.NV_float_buffer = OPENGL_GET_SUP_EXT(strwgl,WGL_NV_float_buffer);

}
/////////////////////////////////////////////////////////////////////////////////////////////
void CWGLDeviceContext::SetupWGLExtProc()
{
    // WGL_EXT_swap_control
    if(m_WGLSupExt.EXT_swap_control)
    {
        OPENGL_GET_PROC(PFNWGLSWAPINTERVALEXTPROC,wglSwapIntervalEXT);
        OPENGL_GET_PROC(PFNWGLGETSWAPINTERVALEXTPROC,wglGetSwapIntervalEXT);
    }

    // WGL_ARB_pixel_format
    if(m_WGLSupExt.ARB_pixel_format)
    {
        OPENGL_GET_PROC(PFNWGLGETPIXELFORMATATTRIBIVARBPROC,wglGetPixelFormatAttribivARB);
        OPENGL_GET_PROC(PFNWGLGETPIXELFORMATATTRIBFVARBPROC,wglGetPixelFormatAttribfvARB);
        OPENGL_GET_PROC(PFNWGLCHOOSEPIXELFORMATARBPROC,wglChoosePixelFormatARB);
    }

    // WGL_ARB_create_context
    if(m_WGLSupExt.ARB_create_context)
    {
        OPENGL_GET_PROC(PFNWGLCREATECONTEXTATTRIBSARBPROC,wglCreateContextAttribsARB);
    }

}


/////////////////////////////////////////////////////////////////////////////////////////////
bool CWGLDeviceContext::SetupMultisampleFormats(const PIXELFORMATDESCRIPTOR &pfd)
{
    if(!m_WGLSupExt.ARB_multisample || !m_WGLSupExt.ARB_pixel_format)
        return false;

    int iAttribs[17] = {
        WGL_DRAW_TO_WINDOW_ARB, GL_TRUE,
        WGL_ACCELERATION_ARB,   WGL_FULL_ACCELERATION_ARB,
        WGL_DOUBLE_BUFFER_ARB,  GL_TRUE,
        WGL_COLOR_BITS_ARB,     32,
        WGL_DEPTH_BITS_ARB,     24,
        WGL_STENCIL_BITS_ARB,   8,
        WGL_ACCUM_BITS_ARB,     0,
        WGL_SAMPLE_BUFFERS_ARB, 1,
    };

    int pixelFormats[256]={0};
    unsigned int nPFormats;
    int bestFormat = 0;

    wglChoosePixelFormatARB(m_hDC, iAttribs, NULL, sizeof(pixelFormats) / sizeof(int), pixelFormats, &nPFormats);

    if(!nPFormats)//If No Formats
    {
        //logger
        return false;
    }

    int attrib = WGL_SAMPLES_ARB;
    int samples;

    // Search for the pixel format with the closest number of multisample samples to the requested
    for (unsigned int i = 0; i < nPFormats; i++)
    {
        wglGetPixelFormatAttribivARB(m_hDC, pixelFormats[i], 0, 1, &attrib, &samples);
        if(m_PixelFormats.count(samples) == 0)
            m_PixelFormats[samples] = pixelFormats[i];
    }
    
    return true;
}

/////////////////////////////////////////////////////////////////////////////////////////////
bool CWGLDeviceContext::InitEntryPoint(HWND hwnd, const PIXELFORMATDESCRIPTOR &pfd)
{
    HDC hdc = GetDC(hwnd);
    SetPixelFormat(hdc, ChoosePixelFormat(hdc, &pfd), &pfd);

    HGLRC hglrc = wglCreateContext(hdc);
    wglMakeCurrent(hdc, hglrc);

    OPENGL_GET_PROC(PFNWGLGETEXTENSIONSSTRINGARBPROC,wglGetExtensionsStringARB);
    if(wglGetExtensionsStringARB)
    {
        // Get WGL Supported Extensions
        CWGLDeviceContext::GetWGLSupExtInfo();
        //Get WGL Function Pointers
        CWGLDeviceContext::SetupWGLExtProc();
    }

    wglMakeCurrent(NULL, NULL);
    wglDeleteContext(hglrc);
    ReleaseDC(hwnd, hdc);

    return true;
}

/////////////////////////////////////////////////////////////////////////////////////////////
void CWGLDeviceContext::SetPixelFormatDesc(PIXELFORMATDESCRIPTOR &pfd)
{
    memset(&pfd,0,sizeof(PIXELFORMATDESCRIPTOR));

    pfd.nSize = sizeof(PIXELFORMATDESCRIPTOR);
    pfd.nVersion = 1;
    pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
    pfd.iPixelType = PFD_TYPE_RGBA;
    pfd.cColorBits = 32;
    pfd.cDepthBits = 24;
    pfd.cStencilBits = 8;
    pfd.iLayerType = PFD_MAIN_PLANE;
}

/////////////////////////////////////////////////////////////////////////////////////////////