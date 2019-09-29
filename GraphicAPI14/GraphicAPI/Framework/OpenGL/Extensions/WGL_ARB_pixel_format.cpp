////////////////////////////////////////////////////////////////////////////////////////////////
//	WGL_ARB_pixel_format.cpp 
//	WGL_ARB_pixel_format Extension Setup
//	Created: 6th August 2006
//	
////////////////////////////////////////////////////////////////////////////////////////////////

#include <windows.h>
#include <gl/gl.h>
#include "glext.h"
#include "wglext.h"

#include "WGL_ARB_pixel_format.h"

extern bool IsExtensionSupported(const char *extension);
extern bool IsWGLARBExtensionSupported(const char *extension,HDC hDC);

/* WGL_ARB_pixel_format */
#ifdef WGL_ARB_pixel_format
//function pointers
PFNWGLGETPIXELFORMATATTRIBIVARBPROC	wglGetPixelFormatAttribivARB = NULL;
PFNWGLGETPIXELFORMATATTRIBFVARBPROC	wglGetPixelFormatAttribfvARB = NULL;
PFNWGLCHOOSEPIXELFORMATARBPROC		wglChoosePixelFormatARB		 = NULL;

bool SetupWGLARBPixelFormat(HDC hDC)
{
	if(!IsWGLARBExtensionSupported("WGL_ARB_pixel_format",hDC))
		return false;

	//get function pointers
	wglGetPixelFormatAttribivARB			=	(PFNWGLGETPIXELFORMATATTRIBIVARBPROC)
												wglGetProcAddress("wglGetPixelFormatAttribivARB");
	wglGetPixelFormatAttribfvARB			=	(PFNWGLGETPIXELFORMATATTRIBFVARBPROC)
												wglGetProcAddress("wglGetPixelFormatAttribfvARB");
	wglChoosePixelFormatARB					=	(PFNWGLCHOOSEPIXELFORMATARBPROC)
												wglGetProcAddress("wglChoosePixelFormatARB");
	
	return true;
}

#endif // WGL_ARB_pixel_format