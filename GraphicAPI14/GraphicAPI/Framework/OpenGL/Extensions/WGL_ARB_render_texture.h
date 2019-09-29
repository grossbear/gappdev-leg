////////////////////////////////////////////////////////////////////////////////////////////////
//	WGL_ARB_render_texture.h 
//	WGL_ARB_render_texture Extension Setup
//	Created: 6th August 2006
//	
////////////////////////////////////////////////////////////////////////////////////////////////


//function pointers
extern PFNWGLBINDTEXIMAGEARBPROC		wglBindTexImageARB;
extern PFNWGLRELEASETEXIMAGEARBPROC		wglReleaseTexImageARB;
extern PFNWGLSETPBUFFERATTRIBARBPROC	wglSetPbufferAttribARB;
	
bool SetupWGLARBRenderTexture(HDC hDC);
