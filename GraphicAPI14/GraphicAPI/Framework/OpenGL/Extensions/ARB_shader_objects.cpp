////////////////////////////////////////////////////////////////////////////////////////////////
//	ARB_shader_objects.h 
//	ARB_shader_objects Extension Setup
//	Created: 6th August 2006
//	
////////////////////////////////////////////////////////////////////////////////////////////////

#include <windows.h>
#include <gl/gl.h>
#include "glext.h"

#include "ARB_shader_objects.h"

extern bool IsExtensionSupported(const char *extension);


/* GL_ARB_shader_objects */
#ifdef GL_ARB_shader_objects

// The function pointers for shaders
PFNGLDELETEOBJECTARBPROC		glDeleteObjectARB	 	= NULL;
PFNGLGETHANDLEARBPROC			glGetHandleARB	 		= NULL;
PFNGLDETACHOBJECTARBPROC		glDetachObjectARB	 	= NULL;
PFNGLCREATESHADEROBJECTARBPROC	glCreateShaderObjectARB	= NULL;
PFNGLSHADERSOURCEARBPROC		glShaderSourceARB	 	= NULL;
PFNGLCOMPILESHADERARBPROC		glCompileShaderARB	 	= NULL;
PFNGLCREATEPROGRAMOBJECTARBPROC glCreateProgramObjectARB= NULL;
PFNGLATTACHOBJECTARBPROC		glAttachObjectARB	 	= NULL;
PFNGLLINKPROGRAMARBPROC			glLinkProgramARB	 	= NULL;
PFNGLUSEPROGRAMOBJECTARBPROC	glUseProgramObjectARB	= NULL;
PFNGLUSEPROGRAMOBJECTARBPROC	glValidateProgramARB	= NULL;

PFNGLUNIFORM1FARBPROC	glUniform1fARB	= NULL;
PFNGLUNIFORM2FARBPROC	glUniform2fARB	= NULL;
PFNGLUNIFORM3FARBPROC	glUniform3fARB	= NULL;
PFNGLUNIFORM4FARBPROC	glUniform4fARB	= NULL;
PFNGLUNIFORM1IARBPROC	glUniform1iARB	= NULL;
PFNGLUNIFORM2IARBPROC	glUniform2iARB	= NULL;
PFNGLUNIFORM3IARBPROC	glUniform3iARB	= NULL;
PFNGLUNIFORM4IARBPROC	glUniform4iARB	= NULL;
PFNGLUNIFORM1FVARBPROC	glUniform1fvARB	= NULL;
PFNGLUNIFORM2FVARBPROC	glUniform2fvARB	= NULL;
PFNGLUNIFORM3FVARBPROC	glUniform3fvARB	= NULL;
PFNGLUNIFORM4FVARBPROC	glUniform4fvARB	= NULL;
PFNGLUNIFORM1IVARBPROC	glUniform1ivARB	= NULL;
PFNGLUNIFORM2IVARBPROC	glUniform2ivARB	= NULL;
PFNGLUNIFORM3IVARBPROC	glUniform3ivARB	= NULL;
PFNGLUNIFORM4IVARBPROC	glUniform4ivARB	= NULL;

PFNGLUNIFORMMATRIX2FVARBPROC glUniformMatrix2fvARB	= NULL;
PFNGLUNIFORMMATRIX3FVARBPROC glUniformMatrix3fvARB	= NULL;
PFNGLUNIFORMMATRIX4FVARBPROC glUniformMatrix4fvARB	= NULL;

PFNGLGETOBJECTPARAMETERFVARBPROC	 glGetObjectParameterfvARB	= NULL;
PFNGLGETOBJECTPARAMETERIVARBPROC	 glGetObjectParameterivARB	= NULL;
PFNGLGETINFOLOGARBPROC				 glGetInfoLogARB			= NULL;
PFNGLGETATTACHEDOBJECTSARBPROC		 glGetAttachedObjectsARB	= NULL;
PFNGLGETUNIFORMLOCATIONARBPROC		 glGetUniformLocationARB	= NULL;
PFNGLGETACTIVEUNIFORMARBPROC		 glGetActiveUniformARB		= NULL;
PFNGLGETUNIFORMFVARBPROC			 glGetUniformfvARB			= NULL;
PFNGLGETUNIFORMIVARBPROC			 glGetUniformivARB			= NULL;
PFNGLGETSHADERSOURCEARBPROC			 glGetShaderSourceARB		= NULL; 

bool SetupARBShaderObjects()
{
	if(!IsExtensionSupported("GL_ARB_shader_objects"))
		return false;

		glDeleteObjectARB = (PFNGLDELETEOBJECTARBPROC) 
			wglGetProcAddress("glDeleteObjectARB");

		glGetHandleARB = (PFNGLGETHANDLEARBPROC) 
			wglGetProcAddress("glGetHandleARB");

		glDetachObjectARB = (PFNGLDETACHOBJECTARBPROC) 
			wglGetProcAddress("glDetachObjectARB");

		glCreateShaderObjectARB	= (PFNGLCREATESHADEROBJECTARBPROC) 
			wglGetProcAddress("glCreateShaderObjectARB");

		glShaderSourceARB = (PFNGLSHADERSOURCEARBPROC) 
			wglGetProcAddress("glShaderSourceARB");

		glCompileShaderARB = (PFNGLCOMPILESHADERARBPROC) 
			wglGetProcAddress("glCompileShaderARB");

		glCreateProgramObjectARB = (PFNGLCREATEPROGRAMOBJECTARBPROC) 
			wglGetProcAddress("glCreateProgramObjectARB");

		glAttachObjectARB = (PFNGLATTACHOBJECTARBPROC) 
			wglGetProcAddress("glAttachObjectARB");

		glLinkProgramARB = (PFNGLLINKPROGRAMARBPROC) 
			wglGetProcAddress("glLinkProgramARB");

		glUseProgramObjectARB = (PFNGLUSEPROGRAMOBJECTARBPROC) 
			wglGetProcAddress("glUseProgramObjectARB");

		glValidateProgramARB = (PFNGLUSEPROGRAMOBJECTARBPROC) 
			wglGetProcAddress("glValidateProgramARB");


	
		glUniform1fARB	= (PFNGLUNIFORM1FARBPROC) 
			wglGetProcAddress("glUniform1fARB");

		glUniform2fARB	= (PFNGLUNIFORM2FARBPROC) 
			wglGetProcAddress("glUniform2fARB");

		glUniform3fARB	= (PFNGLUNIFORM3FARBPROC) 
			wglGetProcAddress("glUniform3fARB");

		glUniform4fARB	= (PFNGLUNIFORM4FARBPROC) 
			wglGetProcAddress("glUniform4fARB");

		glUniform1iARB	= (PFNGLUNIFORM1IARBPROC) 
			wglGetProcAddress("glUniform1iARB");

		glUniform2iARB	= (PFNGLUNIFORM2IARBPROC) 
			wglGetProcAddress("glUniform2iARB");

		glUniform3iARB	= (PFNGLUNIFORM3IARBPROC) 
			wglGetProcAddress("glUniform3iARB");

		glUniform4iARB	= (PFNGLUNIFORM4IARBPROC) 
			wglGetProcAddress("glUniform4iARB");

		glUniform1fvARB	= (PFNGLUNIFORM1FVARBPROC) 
			wglGetProcAddress("glUniform1fvARB");

		glUniform2fvARB	= (PFNGLUNIFORM2FVARBPROC) 
			wglGetProcAddress("glUniform2fvARB");

		glUniform3fvARB	= (PFNGLUNIFORM3FVARBPROC) 
			wglGetProcAddress("glUniform3fvARB");

		glUniform4fvARB	= (PFNGLUNIFORM4FVARBPROC) 
			wglGetProcAddress("glUniform4fvARB");

		glUniform1ivARB	= (PFNGLUNIFORM1IVARBPROC) 
			wglGetProcAddress("glUniform1ivARB");

		glUniform2ivARB	= (PFNGLUNIFORM2IVARBPROC) 
			wglGetProcAddress("glUniform2ivARB");

		glUniform3ivARB	= (PFNGLUNIFORM3IVARBPROC) 
			wglGetProcAddress("glUniform3ivARB");

		glUniform4ivARB	= (PFNGLUNIFORM4IVARBPROC) 
			wglGetProcAddress("glUniform4ivARB");


		glUniformMatrix2fvARB	= (PFNGLUNIFORMMATRIX2FVARBPROC) 
			wglGetProcAddress("glUniformMatrix2fvARB");

		glUniformMatrix3fvARB	= (PFNGLUNIFORMMATRIX3FVARBPROC) 
			wglGetProcAddress("glUniformMatrix3fvARB");

		glUniformMatrix4fvARB	= (PFNGLUNIFORMMATRIX4FVARBPROC) 
			wglGetProcAddress("glUniformMatrix4fvARB");


		glGetObjectParameterfvARB	= (PFNGLGETOBJECTPARAMETERFVARBPROC) 
			wglGetProcAddress("glGetObjectParameterfvARB");

		glGetObjectParameterivARB	= (PFNGLGETOBJECTPARAMETERIVARBPROC) 
			wglGetProcAddress("glGetObjectParameterivARB");

		glGetInfoLogARB	= (PFNGLGETINFOLOGARBPROC) 
			wglGetProcAddress("glGetInfoLogARB");

		glGetAttachedObjectsARB	= (PFNGLGETATTACHEDOBJECTSARBPROC) 
			wglGetProcAddress("glGetAttachedObjectsARB");

		glGetUniformLocationARB = (PFNGLGETUNIFORMLOCATIONARBPROC) 
			 wglGetProcAddress("glGetUniformLocationARB");

		glGetActiveUniformARB = (PFNGLGETACTIVEUNIFORMARBPROC) 
			wglGetProcAddress("glGetActiveUniformARB");

		glGetUniformfvARB = (PFNGLGETUNIFORMFVARBPROC) 
			wglGetProcAddress("glGetUniformfvARB");

		glGetUniformivARB = (PFNGLGETUNIFORMIVARBPROC) 
			wglGetProcAddress("glGetUniformivARB");

		glGetShaderSourceARB = (PFNGLGETSHADERSOURCEARBPROC) 
			wglGetProcAddress("glGetShaderSourceARB");


	return true;
}

#endif //GL_ARB_shader_objects