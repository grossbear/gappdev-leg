////////////////////////////////////////////////////////////////////////////////////////////////
//	ARB_vertex_blend.h 
//	ARB_vertex_blend Extension Setup
//	Created: 6th August 2006
//	
////////////////////////////////////////////////////////////////////////////////////////////////

/* GL_ARB_vertex_blend */
#ifdef GL_ARB_vertex_blend
extern PFNGLWEIGHTBVARBPROC glWeightbvARB;
extern PFNGLWEIGHTSVARBPROC glWeightsvARB;
extern PFNGLWEIGHTIVARBPROC glWeightivARB;
extern PFNGLWEIGHTFVARBPROC glWeightfvARB;
extern PFNGLWEIGHTDVARBPROC glWeightdvARB;
extern PFNGLWEIGHTUBVARBPROC glWeightubvARB;
extern PFNGLWEIGHTUSVARBPROC glWeightusvARB;
extern PFNGLWEIGHTUIVARBPROC glWeightuivARB;
extern PFNGLWEIGHTPOINTERARBPROC glWeightPointerARB;
extern PFNGLVERTEXBLENDARBPROC glVertexBlendARB;

bool SetupARBVertexBlend();

#endif //GL_ARB_vertex_blend
