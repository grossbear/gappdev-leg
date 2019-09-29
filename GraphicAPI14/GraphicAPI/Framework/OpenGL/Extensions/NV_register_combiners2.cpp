////////////////////////////////////////////////////////////////////////////////////////////////
//	NV_register_combiners2.h 
//	NV_register_combiners2 Extension Setup
//	Created: 6th August 2006
//	
////////////////////////////////////////////////////////////////////////////////////////////////

#include <windows.h>
#include <gl/gl.h>
#include "glext.h"

#include "NV_register_combiners2.h"

extern bool IsExtensionSupported(const char *extension);


/* GL_NV_register_combiners2  */
#ifdef GL_NV_register_combiners2

PFNGLCOMBINERSTAGEPARAMETERFVNVPROC			glCombinerStageParameterfvNV	= NULL;
PFNGLGETCOMBINERSTAGEPARAMETERFVNVPROC		glGetCombinerStageParameterfvNV	= NULL;


bool SetupNVRegisterCombiners2()
{
	if(!IsExtensionSupported("GL_NV_register_combiners2"))
		return false;

	//get function pointers
	glCombinerStageParameterfvNV	=	(PFNGLCOMBINERSTAGEPARAMETERFVNVPROC)
			wglGetProcAddress("glCombinerStageParameterfvNV");

	glGetCombinerStageParameterfvNV	= (PFNGLGETCOMBINERSTAGEPARAMETERFVNVPROC)
			wglGetProcAddress("glGetCombinerStageParameterfvNV");

	return true;

}

#endif // GL_NV_register_combiners2