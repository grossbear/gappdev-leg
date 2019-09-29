////////////////////////////////////////////////////////////////////////////////////////////////
//	ATI_separate_stencil.h 
//	ATI_separate_stencil Extension Setup
//	Created: 6th August 2006
//	
////////////////////////////////////////////////////////////////////////////////////////////////

#include <windows.h>
#include <gl/gl.h>
#include "glext.h"

#include "ATI_separate_stencil.h"

extern bool IsExtensionSupported(const char *extension);


/* GL_ATI_separate_stencil */
#ifdef GL_ATI_separate_stencil

PFNGLSTENCILOPSEPARATEATIPROC	glStencilOpSeparateATI		= NULL;
PFNGLSTENCILFUNCSEPARATEATIPROC	glStencilFuncSeparateATI	= NULL;

bool SetupATISeparateStencil()
{
	if(!IsExtensionSupported("GL_ATI_separate_stencil"))
		return false;

	glStencilOpSeparateATI = (PFNGLSTENCILOPSEPARATEATIPROC)
								wglGetProcAddress("glStencilOpSeparateATI");

	glStencilFuncSeparateATI = (PFNGLSTENCILFUNCSEPARATEATIPROC)
								wglGetProcAddress("glStencilFuncSeparateATI");

	return true;
}

#endif //GL_ATI_separate_stencil