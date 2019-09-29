////////////////////////////////////////////////////////////////////////////////////////////////
//	EXT_fog_coord.cpp 
//	EXT_fog_coord Extension Setup
//	Created: 6th August 2006
//	
////////////////////////////////////////////////////////////////////////////////////////////////

#include <windows.h>
#include <gl/gl.h>
#include "glext.h"

#include "EXT_fog_coord.h"

extern bool IsExtensionSupported(const char *extension);


/* GL_EXT_fog_coord */
#ifdef GL_EXT_fog_coord

PFNGLFOGCOORDFEXTPROC glFogCoordfEXT = NULL;
PFNGLFOGCOORDDEXTPROC glFogCoorddEXT = NULL;
PFNGLFOGCOORDFVEXTPROC glFogCoordfvEXT = NULL;
PFNGLFOGCOORDDVEXTPROC glFogCoorddvEXT = NULL;
PFNGLFOGCOORDPOINTEREXTPROC glFogCoordPointerEXT = NULL; 

bool SetupEXTFogCoord()
{
	if(!IsExtensionSupported("GL_EXT_fog_coord"))
		return false;

	glFogCoordfEXT = (PFNGLFOGCOORDFEXTPROC) wglGetProcAddress("glFogCoordfEXT");
	glFogCoorddEXT = (PFNGLFOGCOORDDEXTPROC) wglGetProcAddress("glFogCoorddEXT");
	glFogCoordfvEXT = (PFNGLFOGCOORDFVEXTPROC) wglGetProcAddress("glFogCoordfvEXT");
	glFogCoorddvEXT = (PFNGLFOGCOORDDVEXTPROC) wglGetProcAddress("glFogCoorddvEXT");
	glFogCoordPointerEXT = (PFNGLFOGCOORDPOINTEREXTPROC) 
								wglGetProcAddress("glFogCoordPointerEXT");

	return true;
}

#endif // GL_EXT_fog_coord