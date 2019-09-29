////////////////////////////////////////////////////////////////////////////////////////////////
//	ARB_texture_compression.cpp 
//	ARB_texture_compression Extension Setup
//	Created: 6th August 2006
//	
////////////////////////////////////////////////////////////////////////////////////////////////

#include <windows.h>
#include <gl/gl.h>
#include "glext.h"

#include "ARB_texture_compression.h"

extern bool IsExtensionSupported(const char *extension);


/* GL_ARB_texture_compression */
#ifdef GL_ARB_texture_compression
PFNGLCOMPRESSEDTEXIMAGE3DARBPROC glCompressedTexImage3D = NULL;
PFNGLCOMPRESSEDTEXIMAGE2DARBPROC glCompressedTexImage2D = NULL;
PFNGLCOMPRESSEDTEXIMAGE1DARBPROC glCompressedTexImage1D = NULL;
PFNGLCOMPRESSEDTEXSUBIMAGE3DARBPROC glCompressedTexSubImage3D = NULL;
PFNGLCOMPRESSEDTEXSUBIMAGE2DARBPROC glCompressedTexSubImage2D = NULL;
PFNGLCOMPRESSEDTEXSUBIMAGE1DARBPROC glCompressedTexSubImage1D = NULL;
PFNGLGETCOMPRESSEDTEXIMAGEARBPROC glGetCompressedTexImage = NULL;

bool SetupARBTextureCompression()
{
	if(!IsExtensionSupported("GL_ARB_texture_compression"))
		return false;

	PFNGLCOMPRESSEDTEXIMAGE3DARBPROC glCompressedTexImage3D = 
		(PFNGLCOMPRESSEDTEXIMAGE3DARBPROC) wglGetProcAddress("glCompressedTexImage3D");

	PFNGLCOMPRESSEDTEXIMAGE2DARBPROC glCompressedTexImage2D =
		(PFNGLCOMPRESSEDTEXIMAGE2DARBPROC)wglGetProcAddress("glCompressedTexImage2D");

	PFNGLCOMPRESSEDTEXIMAGE1DARBPROC glCompressedTexImage1D =
		(PFNGLCOMPRESSEDTEXIMAGE1DARBPROC)wglGetProcAddress("glCompressedTexImage1D");

	PFNGLCOMPRESSEDTEXSUBIMAGE3DARBPROC glCompressedTexSubImage3D =
		(PFNGLCOMPRESSEDTEXSUBIMAGE3DARBPROC)wglGetProcAddress("glCompressedTexSubImage3D");

	PFNGLCOMPRESSEDTEXSUBIMAGE2DARBPROC glCompressedTexSubImage2D =
		(PFNGLCOMPRESSEDTEXSUBIMAGE2DARBPROC)wglGetProcAddress("glCompressedTexSubImage2D");

	PFNGLCOMPRESSEDTEXSUBIMAGE1DARBPROC glCompressedTexSubImage1D =
		(PFNGLCOMPRESSEDTEXSUBIMAGE1DARBPROC)wglGetProcAddress("glCompressedTexSubImage1D");

	PFNGLGETCOMPRESSEDTEXIMAGEARBPROC glGetCompressedTexImage =
		(PFNGLGETCOMPRESSEDTEXIMAGEARBPROC)wglGetProcAddress("glGetCompressedTexImage");

	return true;
}
#endif //GL_ARB_texture_compression
