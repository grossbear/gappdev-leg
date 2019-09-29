////////////////////////////////////////////////////////////////////////////////////////////////
//	ARB_transpose_matrix.h 
//	ARB_transpose_matrix Extension Setup
//	Created: 1st August 2006
//	
////////////////////////////////////////////////////////////////////////////////////////////////


#ifdef GL_ARB_transpose_matrix
/* Transpose Matrix */
extern PFNGLLOADTRANSPOSEMATRIXFPROC glLoadTransposeMatrixf;
extern PFNGLLOADTRANSPOSEMATRIXDPROC glLoadTransposeMatrixd;
extern PFNGLMULTTRANSPOSEMATRIXFPROC glMultTransposeMatrixf;
extern PFNGLMULTTRANSPOSEMATRIXDPROC glMultTransposeMatrixd;

bool SetupARBTransposeMatrix();

#endif //GL_ARB_transpose_matrix


