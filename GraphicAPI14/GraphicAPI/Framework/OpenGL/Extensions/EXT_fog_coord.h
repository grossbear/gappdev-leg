////////////////////////////////////////////////////////////////////////////////////////////////
//	EXT_fog_coord.h 
//	EXT_fog_coord Extension Setup
//	Created: 6th August 2006
//	
////////////////////////////////////////////////////////////////////////////////////////////////

/* GL_EXT_fog_coord */
#ifdef GL_EXT_fog_coord

extern PFNGLFOGCOORDFEXTPROC glFogCoordfEXT;
extern PFNGLFOGCOORDDEXTPROC glFogCoorddEXT;
extern PFNGLFOGCOORDFVEXTPROC glFogCoordfvEXT;
extern PFNGLFOGCOORDDVEXTPROC glFogCoorddvEXT;
extern PFNGLFOGCOORDPOINTEREXTPROC glFogCoordPointerEXT; 

bool SetupEXTFogCoord();

#endif // GL_EXT_fog_coord