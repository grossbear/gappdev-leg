////////////////////////////////////////////////////////////////////////////////////////////////
//	ATI_separate_stencil.h 
//	ATI_separate_stencil Extension Setup
//	Created: 6th August 2006
//	
////////////////////////////////////////////////////////////////////////////////////////////////

/* GL_ATI_separate_stencil */
#ifdef GL_ATI_separate_stencil

extern PFNGLSTENCILOPSEPARATEATIPROC	glStencilOpSeparateATI;
extern PFNGLSTENCILFUNCSEPARATEATIPROC	glStencilFuncSeparateATI;

bool SetupATISeparateStencil();

#endif //GL_ATI_separate_stencil