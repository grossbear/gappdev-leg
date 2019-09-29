////////////////////////////////////////////////////////////////////////////////////////////////
//	NV_register_combiners.h 
//	NV_register_combiners Extension Setup
//	Created: 6th August 2006
//	
////////////////////////////////////////////////////////////////////////////////////////////////

/* GL_NV_register_combiners  */
#ifdef GL_NV_register_combiners

extern PFNGLCOMBINERPARAMETERFVNVPROC				glCombinerParameterfvNV;
extern PFNGLCOMBINERPARAMETERIVNVPROC				glCombinerParameterivNV;
extern PFNGLCOMBINERPARAMETERFNVPROC				glCombinerParameterfNV;
extern PFNGLCOMBINERPARAMETERINVPROC				glCombinerParameteriNV;

extern PFNGLCOMBINERINPUTNVPROC						glCombinerInputNV;
extern PFNGLCOMBINEROUTPUTNVPROC					glCombinerOutputNV;

extern PFNGLFINALCOMBINERINPUTNVPROC				glFinalCombinerInputNV;

extern PFNGLGETCOMBINERINPUTPARAMETERFVNVPROC		glGetCombinerInputParameterfvNV;
extern PFNGLGETCOMBINERINPUTPARAMETERIVNVPROC		glGetCombinerInputParameterivNV;

extern PFNGLGETCOMBINEROUTPUTPARAMETERFVNVPROC		glGetCombinerOutputParameterfvNV;
extern PFNGLGETCOMBINEROUTPUTPARAMETERIVNVPROC		glGetCombinerOutputParameterivNV;

extern PFNGLGETFINALCOMBINERINPUTPARAMETERFVNVPROC	glGetFinalCombinerInputParameterfvNV;
extern PFNGLGETFINALCOMBINERINPUTPARAMETERIVNVPROC	glGetFinalCombinerInputParameterivNV;

bool SetupNVRegisterCombiners();

#endif // GL_NV_register_combiners
