////////////////////////////////////////////////////////////////////////////////////////////////
//	EXT_framebuffer_object.cpp
//	EXT_framebuffer_object Extension Setup 
//	Created: 6th August 2006
//	
////////////////////////////////////////////////////////////////////////////////////////////////

#include <windows.h>
#include <gl/gl.h>
#include "glext.h"
#include "wglext.h"

#include "EXT_framebuffer_object.h"

extern bool IsExtensionSupported(const char *extension);

/* GL_EXT_framebuffer_object */
#ifdef GL_EXT_framebuffer_object


PFNGLISRENDERBUFFEREXTPROC      glIsRenderbufferEXT = NULL;
PFNGLBINDRENDERBUFFEREXTPROC    glBindRenderbufferEXT = NULL;
PFNGLDELETERENDERBUFFERSEXTPROC glDeleteRenderbuffersEXT = NULL;
PFNGLGENRENDERBUFFERSEXTPROC    glGenRenderbuffersEXT = NULL;
PFNGLRENDERBUFFERSTORAGEEXTPROC glRenderbufferStorageEXT = NULL;
PFNGLGETRENDERBUFFERPARAMETERIVEXTPROC glGetRenderbufferParameterivEXT = NULL;
PFNGLISFRAMEBUFFEREXTPROC      glIsFramebufferEXT = NULL;
PFNGLBINDFRAMEBUFFEREXTPROC    glBindFramebufferEXT = NULL;
PFNGLDELETEFRAMEBUFFERSEXTPROC glDeleteFramebuffersEXT = NULL;
PFNGLGENFRAMEBUFFERSEXTPROC    glGenFramebuffersEXT = NULL;
PFNGLCHECKFRAMEBUFFERSTATUSEXTPROC  glCheckFramebufferStatusEXT = NULL;
PFNGLFRAMEBUFFERTEXTURE1DEXTPROC    glFramebufferTexture1DEXT = NULL;
PFNGLFRAMEBUFFERTEXTURE2DEXTPROC    glFramebufferTexture2DEXT = NULL;
PFNGLFRAMEBUFFERTEXTURE3DEXTPROC    glFramebufferTexture3DEXT = NULL;
PFNGLFRAMEBUFFERRENDERBUFFEREXTPROC glFramebufferRenderbufferEXT = NULL;
PFNGLGETFRAMEBUFFERATTACHMENTPARAMETERIVEXTPROC glGetFramebufferAttachmentParameterivEXT = NULL;
PFNGLGENERATEMIPMAPEXTPROC glGenerateMipmapEXT = NULL;


bool SetupEXTFramebufferObject()
{
	if(!IsExtensionSupported("GL_EXT_framebuffer_object"))
		return false;

	glIsRenderbufferEXT = (PFNGLISRENDERBUFFEREXTPROC) 
		wglGetProcAddress("glIsRenderbufferEXT");

    glBindRenderbufferEXT = (PFNGLBINDRENDERBUFFEREXTPROC) 
		wglGetProcAddress("glBindRenderbufferEXT");

	glDeleteRenderbuffersEXT = (PFNGLDELETERENDERBUFFERSEXTPROC) 
		wglGetProcAddress("glDeleteRenderbuffersEXT");

    glGenRenderbuffersEXT = (PFNGLGENRENDERBUFFERSEXTPROC) 
		wglGetProcAddress("glGenRenderbuffersEXT");

	glRenderbufferStorageEXT = (PFNGLRENDERBUFFERSTORAGEEXTPROC) 
		wglGetProcAddress("glRenderbufferStorageEXT");

	glGetRenderbufferParameterivEXT = (PFNGLGETRENDERBUFFERPARAMETERIVEXTPROC) 
		wglGetProcAddress("glGetRenderbufferParameterivEXT");

    glIsFramebufferEXT = (PFNGLISFRAMEBUFFEREXTPROC) 
		wglGetProcAddress("glIsFramebufferEXT");

	glBindFramebufferEXT = (PFNGLBINDFRAMEBUFFEREXTPROC) 
		wglGetProcAddress("glBindFramebufferEXT");

	glDeleteFramebuffersEXT = (PFNGLDELETEFRAMEBUFFERSEXTPROC) 
		wglGetProcAddress("glDeleteFramebuffersEXT");

    glGenFramebuffersEXT = (PFNGLGENFRAMEBUFFERSEXTPROC) 
		wglGetProcAddress("glGenFramebuffersEXT");

	glCheckFramebufferStatusEXT = (PFNGLCHECKFRAMEBUFFERSTATUSEXTPROC) 
		wglGetProcAddress("glCheckFramebufferStatusEXT");

    glFramebufferTexture1DEXT = (PFNGLFRAMEBUFFERTEXTURE1DEXTPROC) 
		wglGetProcAddress("glFramebufferTexture1DEXT");

    glFramebufferTexture2DEXT = (PFNGLFRAMEBUFFERTEXTURE2DEXTPROC) 
		wglGetProcAddress("glFramebufferTexture2DEXT");

    glFramebufferTexture3DEXT = (PFNGLFRAMEBUFFERTEXTURE3DEXTPROC) 
		wglGetProcAddress("glFramebufferTexture3DEXT");

	glFramebufferRenderbufferEXT = (PFNGLFRAMEBUFFERRENDERBUFFEREXTPROC)
		wglGetProcAddress("glFramebufferRenderbufferEXT");

	glGetFramebufferAttachmentParameterivEXT = (PFNGLGETFRAMEBUFFERATTACHMENTPARAMETERIVEXTPROC) 
		wglGetProcAddress("glGetFramebufferAttachmentParameterivEXT");

	glGenerateMipmapEXT = (PFNGLGENERATEMIPMAPEXTPROC) 
		wglGetProcAddress("glGenerateMipmapEXT");

	return true;
}

#endif // GL_EXT_framebuffer_object