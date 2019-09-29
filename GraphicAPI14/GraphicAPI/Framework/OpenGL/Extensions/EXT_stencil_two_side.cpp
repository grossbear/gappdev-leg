////////////////////////////////////////////////////////////////////////////////////////////////
//	EXT_stencil_two_side.h 
//	EXT_stencil_two_side Extension Setup 
//	Created: 6th August 2006
//	
////////////////////////////////////////////////////////////////////////////////////////////////

#include <windows.h>
#include <gl/gl.h>
#include "glext.h"

#include "EXT_stencil_two_side.h"

extern bool IsExtensionSupported(const char *extension);


/* GL_EXT_stencil_two_side */
#ifdef GL_EXT_stencil_two_side
PFNGLACTIVESTENCILFACEEXTPROC	glActiveStencilFaceEXT = NULL;

bool SetupEXTStencilTwoSide()
{
	if(!IsExtensionSupported("GL_EXT_stencil_two_side"))
		return false;

	//if(!IsExtensionSupported("GL_EXT_stencil_wrap"))
	//	return false;

	//Get Function Pointer
	glActiveStencilFaceEXT = (PFNGLACTIVESTENCILFACEEXTPROC)
								wglGetProcAddress("glActiveStencilFaceEXT");

	return true;
}

#endif //GL_EXT_stencil_two_side
