////////////////////////////////////////////////////////////////////////////////////////////////
//	GL_version_2_0.cpp 
//	OpenGL 2.0 Extensions Setup
//	Created: 7th August 2006
//	
////////////////////////////////////////////////////////////////////////////////////////////////

#include <windows.h>
#include <gl/gl.h>
#include "glext.h"

#include "GL_version_2_0.h"

extern bool IsExtensionSupported(const char *extension);


#ifdef GL_VERSION_2_0

PFNGLATTACHSHADERPROC			 glAttachShader			 = NULL;
PFNGLBINDATTRIBLOCATIONPROC		 glBindAttribLocation	 = NULL;
PFNGLBLENDEQUATIONSEPARATEPROC	 glBlendEquationSeparate = NULL;
PFNGLCOMPILESHADERPROC			 glCompileShader		 = NULL;
PFNGLCREATEPROGRAMPROC			 glCreateProgram		 = NULL;
PFNGLCREATESHADERPROC			 glCreateShader			 = NULL;
PFNGLDELETEPROGRAMPROC			 glDeleteProgram		 = NULL;
PFNGLDELETESHADERPROC			 glDeleteShader			 = NULL;
PFNGLDETACHSHADERPROC			 glDetachShader			 = NULL;

PFNGLDISABLEVERTEXATTRIBARRAYPROC	 glDisableVertexAttribArray	 = NULL;
PFNGLDRAWBUFFERSPROC				 glDrawBuffers				 = NULL;
PFNGLENABLEVERTEXATTRIBARRAYPROC	 glEnableVertexAttribArray	 = NULL;
PFNGLGETACTIVEATTRIBPROC			 glGetActiveAttrib			 = NULL;
PFNGLGETACTIVEUNIFORMPROC			 glGetActiveUniform			 = NULL;
PFNGLGETATTACHEDSHADERSPROC			 glGetAttachedShaders		 = NULL;
PFNGLGETATTRIBLOCATIONPROC			 glGetAttribLocation		 = NULL;
PFNGLGETPROGRAMINFOLOGPROC			 glGetProgramInfoLog		 = NULL;
PFNGLGETPROGRAMIVPROC				 glGetProgramiv				 = NULL;
PFNGLGETSHADERINFOLOGPROC			 glGetShaderInfoLog			 = NULL;
PFNGLGETSHADERSOURCEPROC			 glGetShaderSource			 = NULL;
PFNGLGETSHADERIVPROC				 glGetShaderiv				 = NULL;
PFNGLGETUNIFORMLOCATIONPROC			 glGetUniformLocation		 = NULL;
PFNGLGETUNIFORMFVPROC				 glGetUniformfv				 = NULL;
PFNGLGETUNIFORMIVPROC				 glGetUniformiv				 = NULL;

PFNGLGETVERTEXATTRIBPOINTERVPROC	glGetVertexAttribPointerv	 = NULL;
PFNGLGETVERTEXATTRIBDVPROC			glGetVertexAttribdv			 = NULL;
PFNGLGETVERTEXATTRIBFVPROC			glGetVertexAttribfv			 = NULL;
PFNGLGETVERTEXATTRIBIVPROC			glGetVertexAttribiv			 = NULL;

PFNGLISPROGRAMPROC		 glIsProgram	 = NULL;
PFNGLISSHADERPROC		 glIsShader		 = NULL;
PFNGLLINKPROGRAMPROC	 glLinkProgram	 = NULL;
PFNGLSHADERSOURCEPROC	 glShaderSource	 = NULL;

PFNGLSTENCILFUNCSEPARATEPROC	glStencilFuncSeparate	 = NULL;
PFNGLSTENCILMASKSEPARATEPROC	glStencilMaskSeparate	 = NULL;
PFNGLSTENCILOPSEPARATEPROC		glStencilOpSeparate		 = NULL;

PFNGLUNIFORM1FPROC	 glUniform1f	 = NULL;
PFNGLUNIFORM1FVPROC	 glUniform1fv	 = NULL;
PFNGLUNIFORM1IPROC   glUniform1i	 = NULL;
PFNGLUNIFORM1IVPROC	 glUniform1iv	 = NULL;

PFNGLUNIFORM2FPROC	 glUniform2f	 = NULL;
PFNGLUNIFORM2FVPROC	 glUniform2fv	 = NULL;
PFNGLUNIFORM2IPROC	 glUniform2i	 = NULL;
PFNGLUNIFORM2IVPROC	 glUniform2iv	 = NULL;

PFNGLUNIFORM3FPROC	 glUniform3f	 = NULL;
PFNGLUNIFORM3FVPROC	 glUniform3fv	 = NULL;
PFNGLUNIFORM3IPROC	 glUniform3i	 = NULL;
PFNGLUNIFORM3IVPROC	 glUniform3iv	 = NULL;

PFNGLUNIFORM4FPROC		glUniform4f	 = NULL;
PFNGLUNIFORM4FVPROC		glUniform4fv = NULL;
PFNGLUNIFORM4IPROC		glUniform4i	 = NULL;
PFNGLUNIFORM4IVPROC		glUniform4iv = NULL;

PFNGLUNIFORMMATRIX2FVPROC glUniformMatrix2fv = NULL;
PFNGLUNIFORMMATRIX3FVPROC glUniformMatrix3fv = NULL;
PFNGLUNIFORMMATRIX4FVPROC glUniformMatrix4fv = NULL;

PFNGLUSEPROGRAMPROC glUseProgram = NULL;
PFNGLVALIDATEPROGRAMPROC glValidateProgram = NULL;

PFNGLVERTEXATTRIB1DPROC		glVertexAttrib1d	= NULL;
PFNGLVERTEXATTRIB1DVPROC	glVertexAttrib1dv	= NULL;
PFNGLVERTEXATTRIB1FPROC		glVertexAttrib1f	= NULL;
PFNGLVERTEXATTRIB1FVPROC	glVertexAttrib1fv	= NULL;
PFNGLVERTEXATTRIB1SPROC		glVertexAttrib1s	= NULL;
PFNGLVERTEXATTRIB1SVPROC	glVertexAttrib1sv	= NULL;

PFNGLVERTEXATTRIB2DPROC		glVertexAttrib2d	= NULL;
PFNGLVERTEXATTRIB2DVPROC	glVertexAttrib2dv	= NULL;
PFNGLVERTEXATTRIB2FPROC		glVertexAttrib2f	= NULL;
PFNGLVERTEXATTRIB2FVPROC	glVertexAttrib2fv	= NULL;
PFNGLVERTEXATTRIB2SPROC		glVertexAttrib2s	= NULL;
PFNGLVERTEXATTRIB2SVPROC	glVertexAttrib2sv	= NULL;

PFNGLVERTEXATTRIB3DPROC		glVertexAttrib3d	= NULL;
PFNGLVERTEXATTRIB3DVPROC	glVertexAttrib3dv	= NULL;
PFNGLVERTEXATTRIB3FPROC		glVertexAttrib3f	= NULL;
PFNGLVERTEXATTRIB3FVPROC	glVertexAttrib3fv	= NULL;
PFNGLVERTEXATTRIB3SPROC		glVertexAttrib3s	= NULL;
PFNGLVERTEXATTRIB3SVPROC	glVertexAttrib3sv	= NULL;

PFNGLVERTEXATTRIB4NBVPROC		glVertexAttrib4Nbv		= NULL;
PFNGLVERTEXATTRIB4NIVPROC		glVertexAttrib4Niv		= NULL;
PFNGLVERTEXATTRIB4NSVPROC		glVertexAttrib4Nsv		= NULL;
PFNGLVERTEXATTRIB4NUBPROC		glVertexAttrib4Nub		= NULL;
PFNGLVERTEXATTRIB4NUBVPROC		glVertexAttrib4Nubv		= NULL;
PFNGLVERTEXATTRIB4NUIVPROC		glVertexAttrib4Nuiv		= NULL;
PFNGLVERTEXATTRIB4NUSVPROC		glVertexAttrib4Nusv		= NULL;
PFNGLVERTEXATTRIB4BVPROC		glVertexAttrib4bv		= NULL;
PFNGLVERTEXATTRIB4DPROC			glVertexAttrib4d		= NULL;
PFNGLVERTEXATTRIB4DVPROC		glVertexAttrib4dv		= NULL;
PFNGLVERTEXATTRIB4FPROC			glVertexAttrib4f		= NULL;
PFNGLVERTEXATTRIB4FVPROC		glVertexAttrib4fv		= NULL;
PFNGLVERTEXATTRIB4IVPROC		glVertexAttrib4iv		= NULL;
PFNGLVERTEXATTRIB4SPROC			glVertexAttrib4s		= NULL;
PFNGLVERTEXATTRIB4SVPROC		glVertexAttrib4sv		= NULL;
PFNGLVERTEXATTRIB4UBVPROC		glVertexAttrib4ubv		= NULL;
PFNGLVERTEXATTRIB4UIVPROC		glVertexAttrib4uiv		= NULL;
PFNGLVERTEXATTRIB4USVPROC		glVertexAttrib4usv		= NULL;
PFNGLVERTEXATTRIBPOINTERPROC	glVertexAttribPointer	= NULL;

bool SetupGL20()
{
	if(!IsExtensionSupported("GL_ARB_shading_language_100"))
		return false;

	glAttachShader = (PFNGLATTACHSHADERPROC) 
		wglGetProcAddress("glAttachShader");

	glBindAttribLocation = (PFNGLBINDATTRIBLOCATIONPROC) 
		wglGetProcAddress("glBindAttribLocation");

	glBlendEquationSeparate = (PFNGLBLENDEQUATIONSEPARATEPROC) 
		wglGetProcAddress("glBlendEquationSeparate");

	glCompileShader = (PFNGLCOMPILESHADERPROC) 
		wglGetProcAddress("glCompileShader");

	glCreateProgram = (PFNGLCREATEPROGRAMPROC) 
		wglGetProcAddress("glCreateProgram");

	glCreateShader = (PFNGLCREATESHADERPROC) 
		wglGetProcAddress("glCreateShader");

	glDeleteProgram = (PFNGLDELETEPROGRAMPROC) 
		wglGetProcAddress("glDeleteProgram");

	glDeleteShader = (PFNGLDELETESHADERPROC) 
		wglGetProcAddress("glDeleteShader");

	glDetachShader = (PFNGLDETACHSHADERPROC) 
		wglGetProcAddress("glDetachShader");



	glDisableVertexAttribArray = (PFNGLDISABLEVERTEXATTRIBARRAYPROC) 
		wglGetProcAddress("glDisableVertexAttribArray");

	glDrawBuffers = (PFNGLDRAWBUFFERSPROC) 
		wglGetProcAddress("glDrawBuffers");

	glEnableVertexAttribArray = (PFNGLENABLEVERTEXATTRIBARRAYPROC) 
		wglGetProcAddress("glEnableVertexAttribArray");

	glGetActiveAttrib = (PFNGLGETACTIVEATTRIBPROC) 
		wglGetProcAddress("glGetActiveAttrib");

	glGetActiveUniform = (PFNGLGETACTIVEUNIFORMPROC) 
		wglGetProcAddress("glGetActiveUniform");

	glGetAttachedShaders = (PFNGLGETATTACHEDSHADERSPROC) 
		wglGetProcAddress("glGetAttachedShaders");

	glGetAttribLocation = (PFNGLGETATTRIBLOCATIONPROC) 
		wglGetProcAddress("glGetAttribLocation");

	glGetProgramInfoLog = (PFNGLGETPROGRAMINFOLOGPROC) 
		wglGetProcAddress("glGetProgramInfoLog");

	glGetProgramiv = (PFNGLGETPROGRAMIVPROC) 
		wglGetProcAddress("glGetProgramiv");

	glGetShaderInfoLog = (PFNGLGETSHADERINFOLOGPROC) 
		wglGetProcAddress("glGetShaderInfoLog");

	glGetShaderSource = (PFNGLGETSHADERSOURCEPROC) 
		wglGetProcAddress("glGetShaderSource");

	glGetShaderiv = (PFNGLGETSHADERIVPROC) 
		wglGetProcAddress("glGetShaderiv");

	glGetUniformLocation = (PFNGLGETUNIFORMLOCATIONPROC) 
		wglGetProcAddress("glGetUniformLocation");

	glGetUniformfv = (PFNGLGETUNIFORMFVPROC) 
		wglGetProcAddress("glGetUniformfv");

	glGetUniformiv = (PFNGLGETUNIFORMIVPROC) 
		wglGetProcAddress("glGetUniformiv");

	glGetVertexAttribPointerv = (PFNGLGETVERTEXATTRIBPOINTERVPROC) 
		wglGetProcAddress("glGetVertexAttribPointerv");

	glGetVertexAttribdv = (PFNGLGETVERTEXATTRIBDVPROC) 
		wglGetProcAddress("glGetVertexAttribdv");

	glGetVertexAttribfv = (PFNGLGETVERTEXATTRIBFVPROC) 
		wglGetProcAddress("glGetVertexAttribfv");

	glGetVertexAttribiv = (PFNGLGETVERTEXATTRIBIVPROC) 
		wglGetProcAddress("glGetVertexAttribiv");

	glIsProgram	= (PFNGLISPROGRAMPROC) 
		wglGetProcAddress("glIsProgram");

	glIsShader = (PFNGLISSHADERPROC) 
		wglGetProcAddress("glIsShader");

	glLinkProgram = (PFNGLLINKPROGRAMPROC) 
		wglGetProcAddress("glLinkProgram");

	glShaderSource = (PFNGLSHADERSOURCEPROC) 
		wglGetProcAddress("glShaderSource");



	glStencilFuncSeparate = (PFNGLSTENCILFUNCSEPARATEPROC) 
		wglGetProcAddress("glStencilFuncSeparate");

	glStencilMaskSeparate = (PFNGLSTENCILMASKSEPARATEPROC) 
		wglGetProcAddress("glStencilMaskSeparate");

	glStencilOpSeparate = (PFNGLSTENCILOPSEPARATEPROC) 
		wglGetProcAddress("glStencilOpSeparate");

	glUniform1f = (PFNGLUNIFORM1FPROC) 
		wglGetProcAddress("glUniform1f");

	glUniform1fv = (PFNGLUNIFORM1FVPROC) 
		wglGetProcAddress("glUniform1fv");

	glUniform1i = (PFNGLUNIFORM1IPROC) 
	   wglGetProcAddress("glUniform1i");

	glUniform1iv = (PFNGLUNIFORM1IVPROC) 
		wglGetProcAddress("glUniform1iv");


	glUniform2f	 = (PFNGLUNIFORM2FPROC) 
		wglGetProcAddress("glUniform2f");

	glUniform2fv = (PFNGLUNIFORM2FVPROC) 
		wglGetProcAddress("glUniform2fv");

	glUniform2i	= (PFNGLUNIFORM2IPROC) 
		wglGetProcAddress("glUniform2i");

	glUniform2iv = (PFNGLUNIFORM2IVPROC) 
		wglGetProcAddress("glUniform2iv");

	glUniform3f = (PFNGLUNIFORM3FPROC) 
		wglGetProcAddress("glUniform3f");

	glUniform3fv = (PFNGLUNIFORM3FVPROC) 
		wglGetProcAddress("glUniform3fv");

	glUniform3i = (PFNGLUNIFORM3IPROC) 
		wglGetProcAddress("glUniform3i");

	glUniform3iv = (PFNGLUNIFORM3IVPROC) 
		wglGetProcAddress("glUniform3iv");

	glUniform4f = (PFNGLUNIFORM4FPROC) 
		wglGetProcAddress("glUniform4f");

	glUniform4fv = (PFNGLUNIFORM4FVPROC) 
		wglGetProcAddress("glUniform4fv");

	glUniform4i = (PFNGLUNIFORM4IPROC) 
		wglGetProcAddress("glUniform4i");

	glUniform4iv = (PFNGLUNIFORM4IVPROC) 
		wglGetProcAddress("glUniform4iv");


	glUniformMatrix2fv = (PFNGLUNIFORMMATRIX2FVPROC) 
		wglGetProcAddress("glUniformMatrix2fv");

	glUniformMatrix3fv = (PFNGLUNIFORMMATRIX3FVPROC) 
		wglGetProcAddress("glUniformMatrix3fv");

	glUniformMatrix4fv = (PFNGLUNIFORMMATRIX4FVPROC) 
		wglGetProcAddress("glUniformMatrix4fv");



	glUseProgram = (PFNGLUSEPROGRAMPROC) 
		wglGetProcAddress("glUseProgram");

	glValidateProgram = (PFNGLVALIDATEPROGRAMPROC) 
		wglGetProcAddress("glValidateProgram");

	glVertexAttrib1d = (PFNGLVERTEXATTRIB1DPROC) 
		wglGetProcAddress("glVertexAttrib1d");

	glVertexAttrib1dv = (PFNGLVERTEXATTRIB1DVPROC) 
		wglGetProcAddress("glVertexAttrib1dv");

	glVertexAttrib1f = (PFNGLVERTEXATTRIB1FPROC) 
		wglGetProcAddress("glVertexAttrib1f");

	glVertexAttrib1fv = (PFNGLVERTEXATTRIB1FVPROC) 
		wglGetProcAddress("glVertexAttrib1fv");

	glVertexAttrib1s = (PFNGLVERTEXATTRIB1SPROC) 
		wglGetProcAddress("glVertexAttrib1s");

	glVertexAttrib1sv = (PFNGLVERTEXATTRIB1SVPROC) 
		wglGetProcAddress("glVertexAttrib1sv");


	glVertexAttrib2d = (PFNGLVERTEXATTRIB2DPROC) 
		wglGetProcAddress("glVertexAttrib2d");

	glVertexAttrib2dv = (PFNGLVERTEXATTRIB2DVPROC) 
		wglGetProcAddress("glVertexAttrib2dv");

	glVertexAttrib2f = (PFNGLVERTEXATTRIB2FPROC) 
		wglGetProcAddress("glVertexAttrib2f");

	glVertexAttrib2fv = (PFNGLVERTEXATTRIB2FVPROC) 
		wglGetProcAddress("glVertexAttrib2fv");

	glVertexAttrib2s = (PFNGLVERTEXATTRIB2SPROC) 
		wglGetProcAddress("glVertexAttrib2s");

	glVertexAttrib2sv = (PFNGLVERTEXATTRIB2SVPROC) 
		wglGetProcAddress("glVertexAttrib2sv");


	glVertexAttrib3d = (PFNGLVERTEXATTRIB3DPROC) 
		wglGetProcAddress("glVertexAttrib3d");

	glVertexAttrib3dv = (PFNGLVERTEXATTRIB3DVPROC) 
		wglGetProcAddress("glVertexAttrib3dv");

	glVertexAttrib3f = (PFNGLVERTEXATTRIB3FPROC) 
		wglGetProcAddress("glVertexAttrib3f");

	glVertexAttrib3fv = (PFNGLVERTEXATTRIB3FVPROC) 
		wglGetProcAddress("glVertexAttrib3fv");

	glVertexAttrib3s = (PFNGLVERTEXATTRIB3SPROC) 
		wglGetProcAddress("glVertexAttrib3s");

	glVertexAttrib3sv = (PFNGLVERTEXATTRIB3SVPROC) 
		wglGetProcAddress("glVertexAttrib3sv");


	glVertexAttrib4Nbv = (PFNGLVERTEXATTRIB4NBVPROC) 
		wglGetProcAddress("glVertexAttrib4Nbv");

	glVertexAttrib4Niv = (PFNGLVERTEXATTRIB4NIVPROC) 
		wglGetProcAddress("glVertexAttrib4Niv");

	glVertexAttrib4Nsv = (PFNGLVERTEXATTRIB4NSVPROC) 
		wglGetProcAddress("glVertexAttrib4Nsv");

	glVertexAttrib4Nub = (PFNGLVERTEXATTRIB4NUBPROC) 
		wglGetProcAddress("glVertexAttrib4Nub");

	glVertexAttrib4Nubv	= (PFNGLVERTEXATTRIB4NUBVPROC) 
		wglGetProcAddress("glVertexAttrib4Nubv");

	glVertexAttrib4Nuiv	= (PFNGLVERTEXATTRIB4NUIVPROC) 
		wglGetProcAddress("glVertexAttrib4Nuiv");

	glVertexAttrib4Nusv	= (PFNGLVERTEXATTRIB4NUSVPROC) 
		wglGetProcAddress("glVertexAttrib4Nusv");

	glVertexAttrib4bv = (PFNGLVERTEXATTRIB4BVPROC) 
		wglGetProcAddress("glVertexAttrib4bv");

	glVertexAttrib4d = (PFNGLVERTEXATTRIB4DPROC) 
		wglGetProcAddress("glVertexAttrib4d");

	glVertexAttrib4dv = (PFNGLVERTEXATTRIB4DVPROC) 
		wglGetProcAddress("glVertexAttrib4dv");

	glVertexAttrib4f = (PFNGLVERTEXATTRIB4FPROC) 
		wglGetProcAddress("glVertexAttrib4f");

	glVertexAttrib4fv = (PFNGLVERTEXATTRIB4FVPROC) 
		wglGetProcAddress("glVertexAttrib4fv");

	glVertexAttrib4iv = (PFNGLVERTEXATTRIB4IVPROC) 
		wglGetProcAddress("glVertexAttrib4iv");

	glVertexAttrib4s = (PFNGLVERTEXATTRIB4SPROC) 
		wglGetProcAddress("glVertexAttrib4s");

	glVertexAttrib4sv = (PFNGLVERTEXATTRIB4SVPROC) 
		wglGetProcAddress("glVertexAttrib4sv");

	glVertexAttrib4ubv = (PFNGLVERTEXATTRIB4UBVPROC) 
		wglGetProcAddress("glVertexAttrib4ubv");

	glVertexAttrib4uiv = (PFNGLVERTEXATTRIB4UIVPROC) 
		wglGetProcAddress("glVertexAttrib4uiv");

	glVertexAttrib4usv = (PFNGLVERTEXATTRIB4USVPROC) 
		wglGetProcAddress("glVertexAttrib4usv");

	glVertexAttribPointer = (PFNGLVERTEXATTRIBPOINTERPROC) 
		wglGetProcAddress("glVertexAttribPointer");


	return true;
}


#endif /* GL_VERSION_2_0 */ 
 