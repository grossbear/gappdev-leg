////////////////////////////////////////////////////////////////////////////////////////////////
//	WGL_ARB_pixel_format.h 
//	WGL_ARB_pixel_format Extension Setup
//	Created: 6th August 2006
//	
////////////////////////////////////////////////////////////////////////////////////////////////


/* WGL_ARB_pixel_format */
#ifdef WGL_ARB_pixel_format

//function pointers
extern PFNWGLGETPIXELFORMATATTRIBIVARBPROC	wglGetPixelFormatAttribivARB;
extern PFNWGLGETPIXELFORMATATTRIBFVARBPROC	wglGetPixelFormatAttribfvARB;
extern PFNWGLCHOOSEPIXELFORMATARBPROC		wglChoosePixelFormatARB;

bool SetupWGLARBPixelFormat(HDC hDC);

#endif // WGL_ARB_pixel_format
