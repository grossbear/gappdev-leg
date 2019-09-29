////////////////////////////////////////////////////////////////////////////////////////////////
//	ARB_vertex_buffer_object.cpp
//	ARB_vertex_buffer_object Extension Setup 
//	Created: 1st August 2006
//	
////////////////////////////////////////////////////////////////////////////////////////////////

#include <windows.h>
#include <gl/gl.h>
#include "glext.h"

#include "ARB_vertex_buffer_object.h"

extern bool IsExtensionSupported(const char *extension);


/* GL_ARB_vertex_buffer_object */
#ifdef GL_ARB_vertex_buffer_object
PFNGLBINDBUFFERARBPROC glBindBufferARB = NULL;
PFNGLDELETEBUFFERSARBPROC glDeleteBuffersARB = NULL;
PFNGLGENBUFFERSARBPROC glGenBuffersARB = NULL;
PFNGLISBUFFERARBPROC glIsBufferARB = NULL;
PFNGLBUFFERDATAARBPROC glBufferDataARB = NULL;
PFNGLBUFFERSUBDATAARBPROC glBufferSubDataARB = NULL;
PFNGLGETBUFFERSUBDATAARBPROC glGetBufferSubDataARB = NULL;
PFNGLMAPBUFFERARBPROC glMapBufferARB = NULL;
PFNGLUNMAPBUFFERARBPROC glUnmapBufferARB = NULL;
PFNGLGETBUFFERPARAMETERIVARBPROC glGetBufferParameterivARB = NULL;
PFNGLGETBUFFERPOINTERVARBPROC glGetBufferPointervARB = NULL;


bool SetupARBVertexBufferObject()
{
	if(!IsExtensionSupported( "GL_ARB_vertex_buffer_object" ))
		return false;
	
	// Get Pointers To The GL Vertex Buffers Functions
	glGenBuffersARB = (PFNGLGENBUFFERSARBPROC) wglGetProcAddress("glGenBuffersARB");
	glBindBufferARB = (PFNGLBINDBUFFERARBPROC) wglGetProcAddress("glBindBufferARB");
	glBufferDataARB = (PFNGLBUFFERDATAARBPROC) wglGetProcAddress("glBufferDataARB");
	glDeleteBuffersARB = (PFNGLDELETEBUFFERSARBPROC) wglGetProcAddress("glDeleteBuffersARB");
	glBufferSubDataARB = (PFNGLBUFFERSUBDATAARBPROC) wglGetProcAddress("glBufferSubDataARB");
	glMapBufferARB = (PFNGLMAPBUFFERARBPROC) wglGetProcAddress("glMapBufferARB");
	glUnmapBufferARB = (PFNGLUNMAPBUFFERARBPROC) wglGetProcAddress("glUnmapBufferARB");
	glIsBufferARB = (PFNGLISBUFFERARBPROC) wglGetProcAddress("glIsBufferARB");

	glBufferSubDataARB = (PFNGLBUFFERSUBDATAARBPROC) 
							wglGetProcAddress("glBufferSubData");

	glGetBufferSubDataARB = (PFNGLGETBUFFERSUBDATAARBPROC)
							wglGetProcAddress("glGetBufferSubDataARB");

	glGetBufferParameterivARB = (PFNGLGETBUFFERPARAMETERIVARBPROC)
							wglGetProcAddress("glGetBufferParameterivARB");

	glGetBufferPointervARB = (PFNGLGETBUFFERPOINTERVARBPROC)
							wglGetProcAddress("glGetBufferPointervARB");

	return true;
}

#endif /* GL_ARB_vertex_buffer_object */