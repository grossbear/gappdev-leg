////////////////////////////////////////////////////////////////////////////////////////////////
//	WGL_ARB_extension_string.cpp 
//	WGL_ARB_extension_string Extension Setup 
//	Created: 6th August 2006
//	
////////////////////////////////////////////////////////////////////////////////////////////////

#include <windows.h>
#include <gl/gl.h>
#include "glext.h"
#include "wglext.h"

#include "WGL_ARB_extensions_string.h"

extern bool IsExtensionSupported(const char *extension);


/* WGL_ARB_extensions_string */
#ifdef WGL_ARB_extensions_string

PFNWGLGETEXTENSIONSSTRINGARBPROC wglGetExtensionsStringARB = NULL;

bool SetupARBExtensionsString()
{
	//get function pointer
	wglGetExtensionsStringARB			=	(PFNWGLGETEXTENSIONSSTRINGARBPROC)
											wglGetProcAddress("wglGetExtensionsStringARB");
	if(wglGetExtensionsStringARB==NULL)
		return false;
	
	return true;
}

#endif // WGL_ARB_extensions_string