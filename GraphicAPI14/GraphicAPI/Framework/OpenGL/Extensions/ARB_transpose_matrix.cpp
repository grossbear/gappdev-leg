////////////////////////////////////////////////////////////////////////////////////////////////
//	ARB_transpose_matrix.cpp 
//	ARB_transpose_matrix Extension Setup
//	Created: 1st August 2006
//	
////////////////////////////////////////////////////////////////////////////////////////////////

#include <windows.h>
#include <gl/gl.h>
#include "glext.h"

#include "ARB_transpose_matrix.h"

extern bool IsExtensionSupported(const char *extension);

#ifdef GL_ARB_transpose_matrix
/* Transpose Matrix */
PFNGLLOADTRANSPOSEMATRIXFPROC glLoadTransposeMatrixf=NULL;
PFNGLLOADTRANSPOSEMATRIXDPROC glLoadTransposeMatrixd=NULL;
PFNGLMULTTRANSPOSEMATRIXFPROC glMultTransposeMatrixf=NULL;
PFNGLMULTTRANSPOSEMATRIXDPROC glMultTransposeMatrixd=NULL;

bool SetupARBTransposeMatrix()
{
	if(!IsExtensionSupported("GL_ARB_transpose_matrix"))
		return false;

	glLoadTransposeMatrixf = (PFNGLLOADTRANSPOSEMATRIXFPROC)
								wglGetProcAddress("glLoadTransposeMatrixf");

	glLoadTransposeMatrixd = (PFNGLLOADTRANSPOSEMATRIXDPROC)
								wglGetProcAddress("glLoadTransposeMatrixd");

	glMultTransposeMatrixf = (PFNGLMULTTRANSPOSEMATRIXFPROC) 
								wglGetProcAddress("glMultTransposeMatrixf"); 

	glMultTransposeMatrixd = (PFNGLMULTTRANSPOSEMATRIXDPROC) 
								wglGetProcAddress("glMultTransposeMatrixd"); 

	return true;
}

#endif //GL_ARB_transpose_matrix
