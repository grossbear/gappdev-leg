////////////////////////////////////////////////////////////////////////////////////////////////
//	WGL_ARB_render_texture.cpp 
//	WGL_ARB_render_texture Extension Setup
//	Created: 6th August 2006
//	
////////////////////////////////////////////////////////////////////////////////////////////////

#include <windows.h>
#include <gl/gl.h>
#include "glext.h"
#include "wglext.h"

#include "WGL_ARB_render_texture.h"

extern bool IsExtensionSupported(const char *extension);
extern bool IsWGLARBExtensionSupported(const char *extension,HDC hDC);


//function pointers
PFNWGLBINDTEXIMAGEARBPROC		wglBindTexImageARB	   = NULL;
PFNWGLRELEASETEXIMAGEARBPROC	wglReleaseTexImageARB  = NULL;
PFNWGLSETPBUFFERATTRIBARBPROC	wglSetPbufferAttribARB = NULL;
	
bool SetupWGLARBRenderTexture(HDC hDC)
{
	if(!IsWGLARBExtensionSupported("WGL_ARB_render_texture",hDC))
		return false;

	//get function pointers
	wglBindTexImageARB			=	(PFNWGLBINDTEXIMAGEARBPROC)
									wglGetProcAddress("wglBindTexImageARB");
	wglReleaseTexImageARB		=	(PFNWGLRELEASETEXIMAGEARBPROC)
									wglGetProcAddress("wglReleaseTexImageARB");
	wglSetPbufferAttribARB		=	(PFNWGLSETPBUFFERATTRIBARBPROC)
									wglGetProcAddress("wglSetPbufferAttribARB");
	return true;

}