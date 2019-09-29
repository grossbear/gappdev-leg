////////////////////////////////////////////////////////////////////////////////////////////////
//	EXT_multi_draw_arrays.cpp 
//	EXT_multi_draw_arrays Extension Setup
//	Created: 16th August 2006
//	
////////////////////////////////////////////////////////////////////////////////////////////////

#include <windows.h>
#include <gl/gl.h>
#include "glext.h"

#include "EXT_multi_draw_arrays.h"

extern bool IsExtensionSupported(const char *extension);

/* GL_EXT_multi_draw_arrays */
#ifdef GL_EXT_multi_draw_arrays

//function pointers
PFNGLMULTIDRAWARRAYSEXTPROC				glMultiDrawArraysEXT	=	NULL;
PFNGLMULTIDRAWELEMENTSEXTPROC			glMultiDrawElementsEXT	=	NULL;

bool SetupEXTMultiDrawArrays()
{
	if(!IsExtensionSupported("GL_EXT_multi_draw_array"))
		return false;
		
	//get function pointers
	glMultiDrawArraysEXT	=	(PFNGLMULTIDRAWARRAYSEXTPROC)
				wglGetProcAddress("glMultiDrawArraysEXT");

	glMultiDrawElementsEXT	=	(PFNGLMULTIDRAWELEMENTSEXTPROC)
				wglGetProcAddress("glMultiDrawElementsEXT");

	return true;
}

#endif //GL_EXT_multi_draw_arrays