////////////////////////////////////////////////////////////////////////////////////////////////
//	NV_register_combiners.cpp
//	NV_register_combiners Extension Setup
//	Created: 6th August 2006
//	
////////////////////////////////////////////////////////////////////////////////////////////////

#include <windows.h>
#include <gl/gl.h>
#include "glext.h"

#include "NV_register_combiners.h"

extern bool IsExtensionSupported(const char *extension);

/* GL_NV_register_combiners  */
#ifdef GL_NV_register_combiners

PFNGLCOMBINERPARAMETERFVNVPROC				glCombinerParameterfvNV				 = NULL;
PFNGLCOMBINERPARAMETERIVNVPROC				glCombinerParameterivNV				 = NULL;
PFNGLCOMBINERPARAMETERFNVPROC				glCombinerParameterfNV				 = NULL;
PFNGLCOMBINERPARAMETERINVPROC				glCombinerParameteriNV				 = NULL;

PFNGLCOMBINERINPUTNVPROC					glCombinerInputNV					 = NULL;
PFNGLCOMBINEROUTPUTNVPROC					glCombinerOutputNV					 = NULL;

PFNGLFINALCOMBINERINPUTNVPROC				glFinalCombinerInputNV				 = NULL;

PFNGLGETCOMBINERINPUTPARAMETERFVNVPROC		glGetCombinerInputParameterfvNV		 = NULL;
PFNGLGETCOMBINERINPUTPARAMETERIVNVPROC		glGetCombinerInputParameterivNV		 = NULL;

PFNGLGETCOMBINEROUTPUTPARAMETERFVNVPROC		glGetCombinerOutputParameterfvNV	 = NULL;
PFNGLGETCOMBINEROUTPUTPARAMETERIVNVPROC		glGetCombinerOutputParameterivNV	 = NULL;

PFNGLGETFINALCOMBINERINPUTPARAMETERFVNVPROC	glGetFinalCombinerInputParameterfvNV = NULL;
PFNGLGETFINALCOMBINERINPUTPARAMETERIVNVPROC	glGetFinalCombinerInputParameterivNV = NULL;


bool SetupNVRegisterCombiners()
{
	if(!IsExtensionSupported("GL_NV_register_combiners"))
		return false;

	//get function pointers
	glCombinerParameterfvNV	= (PFNGLCOMBINERPARAMETERFVNVPROC)
			wglGetProcAddress("glCombinerParameterfvNV");

	glCombinerParameterivNV	= (PFNGLCOMBINERPARAMETERIVNVPROC)
			wglGetProcAddress("glCombinerParameterivNV");

	glCombinerParameterfNV = (PFNGLCOMBINERPARAMETERFNVPROC)
			wglGetProcAddress("glCombinerParameterfNV");

	glCombinerParameteriNV = (PFNGLCOMBINERPARAMETERINVPROC)
			wglGetProcAddress("glCombinerParameteriNV");

	glCombinerInputNV =	(PFNGLCOMBINERINPUTNVPROC)
			wglGetProcAddress("glCombinerInputNV");

	glCombinerOutputNV = (PFNGLCOMBINEROUTPUTNVPROC)
			wglGetProcAddress("glCombinerOutputNV");

	glFinalCombinerInputNV =	(PFNGLFINALCOMBINERINPUTNVPROC)
			wglGetProcAddress("glFinalCombinerInputNV");

	glGetCombinerInputParameterfvNV	= (PFNGLGETCOMBINERINPUTPARAMETERFVNVPROC)
			wglGetProcAddress("glGetCombinerInputParameterfvNV");

	glGetCombinerInputParameterivNV	= (PFNGLGETCOMBINERINPUTPARAMETERIVNVPROC)
			wglGetProcAddress("glGetCombinerInputParameterivNV");

	glGetCombinerOutputParameterfvNV = (PFNGLGETCOMBINEROUTPUTPARAMETERFVNVPROC)
			wglGetProcAddress("glGetCombinerOutputParameterfvNV");

	glGetCombinerOutputParameterivNV = (PFNGLGETCOMBINEROUTPUTPARAMETERIVNVPROC)
			wglGetProcAddress("glGetCombinerOutputParameterivNV");

	glGetFinalCombinerInputParameterfvNV = (PFNGLGETFINALCOMBINERINPUTPARAMETERFVNVPROC)
			wglGetProcAddress("glGetFinalCombinerInputParameterfvNV");

	glGetFinalCombinerInputParameterivNV = (PFNGLGETFINALCOMBINERINPUTPARAMETERIVNVPROC)
			wglGetProcAddress("glGetFinalCombinerInputParameterivNV");

	return true;

}

#endif //GL_NV_register_combiners 