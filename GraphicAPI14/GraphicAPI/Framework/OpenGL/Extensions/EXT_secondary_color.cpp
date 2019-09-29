////////////////////////////////////////////////////////////////////////////////////////////////
//	EXT_secondary_color.cpp
//	EXT_secondary_color Extension Setup
//	Created: 1st August 2006
//	
////////////////////////////////////////////////////////////////////////////////////////////////

#include <windows.h>
#include <gl/gl.h>
#include "glext.h"

#include "ARB_program.h"

extern bool IsExtensionSupported(const char *extension);

/* GL_EXT_secondary_color */
#ifdef GL_EXT_secondary_color 
PFNGLSECONDARYCOLOR3BEXTPROC glSecondaryColor3bEXT = NULL;
PFNGLSECONDARYCOLOR3BVEXTPROC glSecondaryColor3bvEXT = NULL;
PFNGLSECONDARYCOLOR3DEXTPROC glSecondaryColor3dEXT = NULL;
PFNGLSECONDARYCOLOR3DVEXTPROC glSecondaryColor3dvEXT = NULL;
PFNGLSECONDARYCOLOR3FEXTPROC glSecondaryColor3fEXT = NULL;
PFNGLSECONDARYCOLOR3FVEXTPROC glSecondaryColor3fvEXT = NULL;
PFNGLSECONDARYCOLOR3IEXTPROC glSecondaryColor3iEXT = NULL;
PFNGLSECONDARYCOLOR3IVEXTPROC glSecondaryColor3ivEXT = NULL;
PFNGLSECONDARYCOLOR3SEXTPROC glSecondaryColor3sEXT = NULL;
PFNGLSECONDARYCOLOR3SVEXTPROC glSecondaryColor3svEXT = NULL;
PFNGLSECONDARYCOLOR3UBEXTPROC glSecondaryColor3ubEXT = NULL;
PFNGLSECONDARYCOLOR3UBVEXTPROC glSecondaryColor3ubvEXT = NULL;
PFNGLSECONDARYCOLOR3UIEXTPROC glSecondaryColor3uiEXT = NULL;
PFNGLSECONDARYCOLOR3UIVEXTPROC glSecondaryColor3uivEXT = NULL;
PFNGLSECONDARYCOLOR3USEXTPROC glSecondaryColor3usEXT = NULL;
PFNGLSECONDARYCOLOR3USVEXTPROC glSecondaryColor3usvEXT = NULL;
PFNGLSECONDARYCOLORPOINTEREXTPROC glSecondaryColorPointerEXT = NULL;



bool SetupEXTSecondaryColor()
{
	if(!IsExtensionSupported("GL_EXT_secondary_color"))
		return false;

	glSecondaryColor3bEXT = (PFNGLSECONDARYCOLOR3BEXTPROC)
		wglGetProcAddress("glSecondaryColor3bEXT");

	glSecondaryColor3bvEXT = (PFNGLSECONDARYCOLOR3BVEXTPROC)
		wglGetProcAddress("glSecondaryColor3bvEXT");

	glSecondaryColor3dEXT = (PFNGLSECONDARYCOLOR3DEXTPROC)
		wglGetProcAddress("glSecondaryColor3dEXT");

	glSecondaryColor3dvEXT = (PFNGLSECONDARYCOLOR3DVEXTPROC)
		wglGetProcAddress("glSecondaryColor3dvEXT");

	glSecondaryColor3fEXT = (PFNGLSECONDARYCOLOR3FEXTPROC)
		wglGetProcAddress("glSecondaryColor3fEXT");

	glSecondaryColor3fvEXT = (PFNGLSECONDARYCOLOR3FVEXTPROC)
		wglGetProcAddress("glSecondaryColor3fvEXT");

	glSecondaryColor3iEXT = (PFNGLSECONDARYCOLOR3IEXTPROC)
		wglGetProcAddress("glSecondaryColor3iEXT");

	glSecondaryColor3ivEXT = (PFNGLSECONDARYCOLOR3IVEXTPROC)
		wglGetProcAddress("glSecondaryColor3ivEXT");

	glSecondaryColor3sEXT = (PFNGLSECONDARYCOLOR3SEXTPROC)
		wglGetProcAddress("glSecondaryColor3sEXT");

	glSecondaryColor3svEXT = (PFNGLSECONDARYCOLOR3SVEXTPROC)
		wglGetProcAddress("glSecondaryColor3svEXT");

	glSecondaryColor3ubEXT = (PFNGLSECONDARYCOLOR3UBEXTPROC)
		wglGetProcAddress("glSecondaryColor3ubEXT");

	glSecondaryColor3ubvEXT = (PFNGLSECONDARYCOLOR3UBVEXTPROC)
		wglGetProcAddress("glSecondaryColor3ubvEXT");

	glSecondaryColor3uiEXT = (PFNGLSECONDARYCOLOR3UIEXTPROC)
		wglGetProcAddress("glSecondaryColor3uiEXT");

	glSecondaryColor3uivEXT = (PFNGLSECONDARYCOLOR3UIVEXTPROC)
		wglGetProcAddress("glSecondaryColor3uivEXT");

	glSecondaryColor3usEXT = (PFNGLSECONDARYCOLOR3USEXTPROC)
		wglGetProcAddress("glSecondaryColor3usEXT");

	glSecondaryColor3usvEXT = (PFNGLSECONDARYCOLOR3USVEXTPROC)
		wglGetProcAddress("glSecondaryColor3usvEXT");

	glSecondaryColorPointerEXT = (PFNGLSECONDARYCOLORPOINTEREXTPROC)
		wglGetProcAddress("glSecondaryColorPointerEXT");

	return true;
}

#endif //GL_EXT_secondary_color
