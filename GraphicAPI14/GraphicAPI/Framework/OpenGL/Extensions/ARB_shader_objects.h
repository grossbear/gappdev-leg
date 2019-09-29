////////////////////////////////////////////////////////////////////////////////////////////////
//	ARB_shader_objects.h 
//	ARB_shader_objects Extension Setup
//	Created: 6th August 2006
//	
////////////////////////////////////////////////////////////////////////////////////////////////

/* GL_ARB_shader_objects */
#ifdef GL_ARB_shader_objects

// The function pointers for shaders
extern PFNGLDELETEOBJECTARBPROC			glDeleteObjectARB;
extern PFNGLGETHANDLEARBPROC			glGetHandleARB;
extern PFNGLDETACHOBJECTARBPROC			glDetachObjectARB;
extern PFNGLCREATESHADEROBJECTARBPROC	glCreateShaderObjectARB;
extern PFNGLSHADERSOURCEARBPROC			glShaderSourceARB;
extern PFNGLCOMPILESHADERARBPROC		glCompileShaderARB;
extern PFNGLCREATEPROGRAMOBJECTARBPROC	glCreateProgramObjectARB;
extern PFNGLATTACHOBJECTARBPROC			glAttachObjectARB;
extern PFNGLLINKPROGRAMARBPROC			glLinkProgramARB;
extern PFNGLUSEPROGRAMOBJECTARBPROC		glUseProgramObjectARB;
extern PFNGLUSEPROGRAMOBJECTARBPROC		glValidateProgramARB;

extern PFNGLUNIFORM1FARBPROC	glUniform1fARB;
extern PFNGLUNIFORM2FARBPROC	glUniform2fARB;
extern PFNGLUNIFORM3FARBPROC	glUniform3fARB;
extern PFNGLUNIFORM4FARBPROC	glUniform4fARB;
extern PFNGLUNIFORM1IARBPROC	glUniform1iARB;
extern PFNGLUNIFORM2IARBPROC	glUniform2iARB;
extern PFNGLUNIFORM3IARBPROC	glUniform3iARB;
extern PFNGLUNIFORM4IARBPROC	glUniform4iARB;
extern PFNGLUNIFORM1FVARBPROC	glUniform1fvARB;
extern PFNGLUNIFORM2FVARBPROC	glUniform2fvARB;
extern PFNGLUNIFORM3FVARBPROC	glUniform3fvARB;
extern PFNGLUNIFORM4FVARBPROC	glUniform4fvARB;
extern PFNGLUNIFORM1IVARBPROC	glUniform1ivARB;
extern PFNGLUNIFORM2IVARBPROC	glUniform2ivARB;
extern PFNGLUNIFORM3IVARBPROC	glUniform3ivARB;
extern PFNGLUNIFORM4IVARBPROC	glUniform4ivARB;

extern PFNGLUNIFORMMATRIX2FVARBPROC glUniformMatrix2fvARB;
extern PFNGLUNIFORMMATRIX3FVARBPROC glUniformMatrix3fvARB;
extern PFNGLUNIFORMMATRIX4FVARBPROC glUniformMatrix4fvARB;

extern PFNGLGETOBJECTPARAMETERFVARBPROC	 glGetObjectParameterfvARB;
extern PFNGLGETOBJECTPARAMETERIVARBPROC	 glGetObjectParameterivARB;
extern PFNGLGETINFOLOGARBPROC			 glGetInfoLogARB;
extern PFNGLGETATTACHEDOBJECTSARBPROC	 glGetAttachedObjectsARB;
extern PFNGLGETUNIFORMLOCATIONARBPROC	 glGetUniformLocationARB;
extern PFNGLGETACTIVEUNIFORMARBPROC		 glGetActiveUniformARB;
extern PFNGLGETUNIFORMFVARBPROC			 glGetUniformfvARB;
extern PFNGLGETUNIFORMIVARBPROC			 glGetUniformivARB;
extern PFNGLGETSHADERSOURCEARBPROC		 glGetShaderSourceARB;

bool SetupARBShaderObjects();

#endif //GL_ARB_shader_objects