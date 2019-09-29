////////////////////////////////////////////////////////////////////////////////////////////////
//	WGL_ARB_pbuffer.h 
//	WGL_ARB_pbuffer Extension Setup 
//	Created: 6th August 2006
//	
////////////////////////////////////////////////////////////////////////////////////////////////

//function pointers
extern PFNWGLCREATEPBUFFERARBPROC		wglCreatePbufferARB;
extern PFNWGLGETPBUFFERDCARBPROC		wglGetPbufferDCARB;
extern PFNWGLRELEASEPBUFFERDCARBPROC	wglReleasePbufferDCARB;
extern PFNWGLDESTROYPBUFFERARBPROC		wglDestroyPbufferARB;
extern PFNWGLQUERYPBUFFERARBPROC		wglQueryPbufferARB;

bool SetupWGLARBPBuffer(HDC hDC);
