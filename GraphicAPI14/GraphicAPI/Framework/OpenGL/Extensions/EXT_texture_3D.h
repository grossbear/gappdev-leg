////////////////////////////////////////////////////////////////////////////////////////////////
//	EXT_texture3D.h 
//	EXT_texture3D Extension Setup 
//	Created: 6th August 2006
//	
////////////////////////////////////////////////////////////////////////////////////////////////

/* GL_EXT_texture_3D */
//#ifdef GL_EXT_texture_3D

extern PFNGLTEXIMAGE3DEXTPROC glTexImage3DEXT;
extern PFNGLTEXSUBIMAGE3DEXTPROC glTexSubImage3DEXT;
extern PFNGLCOPYTEXSUBIMAGE3DPROC glCopyTexSubImage3DEXT;

bool SetupEXTtexture3D();

//#endif //GL_EXT_texture_3D