////////////////////////////////////////////////////////////////////////////////////////////////
//	ARB_matrix_palette.cpp 
//	ARB_matrix_palette Extension Setup
//	Created: 6th August 2006
//	
////////////////////////////////////////////////////////////////////////////////////////////////

#include <windows.h>
#include <gl/gl.h>
#include "glext.h"

#include "ARB_matrix_palette.h"

extern bool IsExtensionSupported(const char *extension);


/* GL_ARB_matrix_palette */
#ifdef GL_ARB_matrix_palette

PFNGLCURRENTPALETTEMATRIXARBPROC glCurrentPaletteMatrixARB = NULL;
PFNGLMATRIXINDEXUBVARBPROC glMatrixIndexubvARB = NULL;
PFNGLMATRIXINDEXUSVARBPROC glMatrixIndexusvARB = NULL; 
PFNGLMATRIXINDEXUIVARBPROC glMatrixIndexuivARB = NULL; 
PFNGLMATRIXINDEXPOINTERARBPROC glMatrixIndexPointerARB = NULL;

bool SetupARBMatrixPalette()
{
	if(!IsExtensionSupported("GL_ARB_matrix_palette"))
		return false;

	glCurrentPaletteMatrixARB = (PFNGLCURRENTPALETTEMATRIXARBPROC) 
		wglGetProcAddress("glCurrentPaletteMatrixARB");

	glMatrixIndexubvARB = (PFNGLMATRIXINDEXUBVARBPROC) 
		wglGetProcAddress("glMatrixIndexubvARB");

	glMatrixIndexusvARB = (PFNGLMATRIXINDEXUSVARBPROC) 
		wglGetProcAddress("glMatrixIndexusvARB");

	glMatrixIndexuivARB = (PFNGLMATRIXINDEXUIVARBPROC) 
		wglGetProcAddress("glMatrixIndexuivARB");

	glMatrixIndexPointerARB = (PFNGLMATRIXINDEXPOINTERARBPROC)
		wglGetProcAddress("glMatrixIndexPointerARB");

	return true;
}

#endif //GL_ARB_matrix_palette