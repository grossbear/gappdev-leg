////////////////////////////////////////////////////////////////////////////////////////////////
//	EXT_compiled_vertex_array.cpp 
//	EXT_compiled_vertex_array Extension Setup
//	Created: 16th August 2006
//	
////////////////////////////////////////////////////////////////////////////////////////////////

#include <windows.h>
#include <gl/gl.h>
#include "glext.h"

#include "EXT_compiled_vertex_array.h"

extern bool IsExtensionSupported(const char *extension);


/* GL_EXT_compiled_vertex_array */
#ifdef GL_EXT_compiled_vertex_array

//function pointers
PFNGLLOCKARRAYSEXTPROC		glLockArraysEXT		= NULL;
PFNGLUNLOCKARRAYSEXTPROC	glUnlockArraysEXT	= NULL;

bool SetupEXTCompiledVertexArray()
{
	if(!IsExtensionSupported("GL_EXT_compiled_vertex_array"))
		return false;

	//get function pointers
	glLockArraysEXT	= (PFNGLLOCKARRAYSEXTPROC)
			wglGetProcAddress("glLockArraysEXT");

	glUnlockArraysEXT =	(PFNGLUNLOCKARRAYSEXTPROC)
			wglGetProcAddress("glUnlockArraysEXT");

	return true;

}

#endif //GL_EXT_compiled_vertex_array