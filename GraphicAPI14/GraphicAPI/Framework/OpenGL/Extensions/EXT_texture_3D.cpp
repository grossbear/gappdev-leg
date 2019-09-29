////////////////////////////////////////////////////////////////////////////////////////////////
//	EXT_texture3D.cpp
//	EXT_texture3D Extension Setup 
//	Created: 6th August 2006
//	
////////////////////////////////////////////////////////////////////////////////////////////////

#include <windows.h>
#include <gl/gl.h>
#include "glext.h"

#include "EXT_texture_3D.h"

extern bool IsExtensionSupported(const char *extension);


/* GL_EXT_texture_3D */
//#ifdef GL_EXT_texture_3D

PFNGLTEXIMAGE3DEXTPROC glTexImage3DEXT				= NULL;	
PFNGLTEXSUBIMAGE3DEXTPROC glTexSubImage3DEXT		= NULL;
PFNGLCOPYTEXSUBIMAGE3DPROC glCopyTexSubImage3DEXT	= NULL;

bool SetupEXTtexture3D()
{
	if(!IsExtensionSupported("GL_EXT_texture3D"))
		return false;

	glTexImage3DEXT = (PFNGLTEXIMAGE3DEXTPROC) wglGetProcAddress("glTexImage3DEXT");
	glTexSubImage3DEXT = (PFNGLTEXSUBIMAGE3DEXTPROC) wglGetProcAddress("glTexSubImage3DEXT");
	glCopyTexSubImage3DEXT = (PFNGLCOPYTEXSUBIMAGE3DPROC)
								wglGetProcAddress("glCopyTexSubImage3DEXT");
	return true;
}

//#endif //GL_EXT_texture_3D