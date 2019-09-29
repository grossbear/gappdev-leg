////////////////////////////////////////////////////////////////////////////////////////////////
//	WGL_ARB_pbuffer.cpp 
//	WGL_ARB_pbuffer Extension Setup 
//	Created: 6th August 2006
//	
////////////////////////////////////////////////////////////////////////////////////////////////

#include <windows.h>
#include <gl/gl.h>
#include "glext.h"
#include "wglext.h"

#include "WGL_ARB_pbuffer.h"

extern bool IsExtensionSupported(const char *extension);
extern bool IsWGLARBExtensionSupported(const char *extension,HDC hDC);

//function pointers
PFNWGLCREATEPBUFFERARBPROC		wglCreatePbufferARB		= NULL;
PFNWGLGETPBUFFERDCARBPROC		wglGetPbufferDCARB		= NULL;
PFNWGLRELEASEPBUFFERDCARBPROC	wglReleasePbufferDCARB	= NULL;
PFNWGLDESTROYPBUFFERARBPROC		wglDestroyPbufferARB	= NULL;
PFNWGLQUERYPBUFFERARBPROC		wglQueryPbufferARB		= NULL;

bool SetupWGLARBPBuffer(HDC hDC)
{
	if(!IsWGLARBExtensionSupported("WGL_ARB_pbuffer",hDC))
		return false;

	//get function pointers
	wglCreatePbufferARB			=	(PFNWGLCREATEPBUFFERARBPROC)
									wglGetProcAddress("wglCreatePbufferARB");
	wglGetPbufferDCARB			=	(PFNWGLGETPBUFFERDCARBPROC)
									wglGetProcAddress("wglGetPbufferDCARB");
	wglReleasePbufferDCARB		=	(PFNWGLRELEASEPBUFFERDCARBPROC)
									wglGetProcAddress("wglReleasePbufferDCARB");
	wglDestroyPbufferARB		=	(PFNWGLDESTROYPBUFFERARBPROC)
									wglGetProcAddress("wglDestroyPbufferARB");
	wglQueryPbufferARB			=	(PFNWGLQUERYPBUFFERARBPROC)
									wglGetProcAddress("wglQueryPbufferARB");
	

	return true;
}