////////////////////////////////////////////////////////////////////////////////////////////////
//	EXT_draw_range_elements.cpp 
//	EXT_draw_range_elements Extension Setup
//	Created: 16th August 2006
//	
////////////////////////////////////////////////////////////////////////////////////////////////

#include <windows.h>
#include <gl/gl.h>
#include "glext.h"

#include "EXT_draw_range_elements.h"

extern bool IsExtensionSupported(const char *extension);


/* GL_EXT_draw_range_elements */
#ifdef GL_EXT_draw_range_elements

//function pointers
PFNGLDRAWRANGEELEMENTSEXTPROC	glDrawRangeElementsEXT = NULL;


bool SetupEXTDrawRangeElements()
{
	if(!IsExtensionSupported("GL_EXT_draw_range_elements"))
		return false;
	
	//get function pointers
	glDrawRangeElementsEXT = (PFNGLDRAWRANGEELEMENTSEXTPROC)
		wglGetProcAddress("glDrawRangeElementsEXT");

	return true;
}

#endif //GL_EXT_draw_range_elements