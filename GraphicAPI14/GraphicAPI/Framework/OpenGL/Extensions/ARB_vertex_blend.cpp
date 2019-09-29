////////////////////////////////////////////////////////////////////////////////////////////////
//	ARB_vertex_blend.cpp 
//	ARB_vertex_blend Extension Setup
//	Created: 6th August 2006
//	
////////////////////////////////////////////////////////////////////////////////////////////////

#include <windows.h>
#include <gl/gl.h>
#include "glext.h"

#include "ARB_vertex_blend.h"

extern bool IsExtensionSupported(const char *extension);

/* GL_ARB_vertex_blend */
#ifdef GL_ARB_vertex_blend
PFNGLWEIGHTBVARBPROC glWeightbvARB = NULL;
PFNGLWEIGHTSVARBPROC glWeightsvARB = NULL;
PFNGLWEIGHTIVARBPROC glWeightivARB = NULL;
PFNGLWEIGHTFVARBPROC glWeightfvARB = NULL;
PFNGLWEIGHTDVARBPROC glWeightdvARB = NULL;
PFNGLWEIGHTUBVARBPROC glWeightubvARB = NULL;
PFNGLWEIGHTUSVARBPROC glWeightusvARB = NULL;
PFNGLWEIGHTUIVARBPROC glWeightuivARB = NULL;
PFNGLWEIGHTPOINTERARBPROC glWeightPointerARB = NULL;
PFNGLVERTEXBLENDARBPROC glVertexBlendARB = NULL;

bool SetupARBVertexBlend()
{
	if(!IsExtensionSupported("GL_ARB_vertex_blend"))
		return false;

	 glWeightbvARB = (PFNGLWEIGHTBVARBPROC) wglGetProcAddress("glWeightbvARB");
	 glWeightsvARB = (PFNGLWEIGHTSVARBPROC) wglGetProcAddress("glWeightsvARB");
	 glWeightivARB = (PFNGLWEIGHTIVARBPROC) wglGetProcAddress("glWeightivARB");
	 glWeightfvARB = (PFNGLWEIGHTFVARBPROC) wglGetProcAddress("glWeightfvARB");
	 glWeightdvARB = (PFNGLWEIGHTDVARBPROC) wglGetProcAddress("glWeightdvARB");
	 glWeightubvARB = (PFNGLWEIGHTUBVARBPROC) wglGetProcAddress("glWeightubvARB");
	 glWeightusvARB = (PFNGLWEIGHTUSVARBPROC) wglGetProcAddress("glWeightusvARB");
	 glWeightuivARB = (PFNGLWEIGHTUIVARBPROC) wglGetProcAddress("glWeightuivARB");
	 glWeightPointerARB = (PFNGLWEIGHTPOINTERARBPROC) wglGetProcAddress("glWeightPointerARB");
	 glVertexBlendARB = (PFNGLVERTEXBLENDARBPROC) wglGetProcAddress("glVertexBlendARB");

	 return true;
}


#endif //GL_ARB_vertex_blend
