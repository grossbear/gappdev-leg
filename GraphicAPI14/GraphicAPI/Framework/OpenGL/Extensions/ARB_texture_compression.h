////////////////////////////////////////////////////////////////////////////////////////////////
//	ARB_texture_compression.h 
//	ARB_texture_compression Extension Setup
//	Created: 6th August 2006
//	
////////////////////////////////////////////////////////////////////////////////////////////////

/* GL_ARB_texture_compression */
#ifdef GL_ARB_texture_compression
extern PFNGLCOMPRESSEDTEXIMAGE3DARBPROC glCompressedTexImage3D;
extern PFNGLCOMPRESSEDTEXIMAGE2DARBPROC glCompressedTexImage2D;
extern PFNGLCOMPRESSEDTEXIMAGE1DARBPROC glCompressedTexImage1D;
extern PFNGLCOMPRESSEDTEXSUBIMAGE3DARBPROC glCompressedTexSubImage3D;
extern PFNGLCOMPRESSEDTEXSUBIMAGE2DARBPROC glCompressedTexSubImage2D;
extern PFNGLCOMPRESSEDTEXSUBIMAGE1DARBPROC glCompressedTexSubImage1D;
extern PFNGLGETCOMPRESSEDTEXIMAGEARBPROC glGetCompressedTexImage;

bool SetupARBTextureCompression();

#endif //GL_ARB_texture_compression
