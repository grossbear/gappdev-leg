////////////////////////////////////////////////////////////////////////////////////////////////
//	EXT_compiled_vertex_array.h 
//	EXT_compiled_vertex_array Extension Setup
//	Created: 16th August 2006
//	
////////////////////////////////////////////////////////////////////////////////////////////////

/* GL_EXT_compiled_vertex_array */
#ifdef GL_EXT_compiled_vertex_array


extern PFNGLLOCKARRAYSEXTPROC		glLockArraysEXT;
extern PFNGLUNLOCKARRAYSEXTPROC		glUnlockArraysEXT;

bool SetupEXTCompiledVertexArray();

#endif //GL_EXT_compiled_vertex_array