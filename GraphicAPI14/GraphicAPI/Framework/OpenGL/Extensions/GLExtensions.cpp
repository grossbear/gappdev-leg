////////////////////////////////////////////////////////////////////////////////////////////////
//	GLExtensions.h 
//	OpenGL Extensions Setup
//	Created: 1st August 2006
//	
////////////////////////////////////////////////////////////////////////////////////////////////



#include <windows.h>
#include <gl/gl.h>
#include <stdio.h> 

#include "glext.h"
#include "wglext.h"
#include "../GLExtStruct.h"
#include "GLExtensions.h"

// Check OpenGL Driver Version
void getGlVersion( int *major, int *minor )
{
    const char* verstr = (const char*)glGetString( GL_VERSION );
    if( (verstr == NULL) || (sscanf( verstr, "%d.%d", major, minor ) != 2) )
    {
        *major = *minor = 0;
        fprintf( stderr, "Invalid GL_VERSION format!!!\n" );
    }
}

// Checking If Driver Supports OpenGL Extension
bool IsExtensionSupported(const char *extension)
{
   static const char *extensions = NULL;
   const char *start;
   char *where, *terminator;
   
   where = (char *) strchr(extension, ' ');
   if ((where) || (*extension == '\0'))
      return FALSE;
   
   if (!extensions)
      extensions = (const char *) glGetString(GL_EXTENSIONS);

   //FILE *file = fopen("extensions.txt","wb");
   //fwrite(extensions,strlen(extensions),1,file);
   //fclose(file);
   
   start = extensions;
   for (;;) 
      {
      where = (char *) strstr((const char *) start, extension);
      if (!where)
        break;

      terminator = where + strlen(extension);
      if (where == start || *(where - 1) == ' ') 
         {
         if (*terminator == ' ' || *terminator == '\0') 
            {
            return TRUE;
            }
         }

      start = terminator;
      }

   return FALSE;
}

//Checking WGL ARB Extensions
bool IsWGLARBExtensionSupported(const char *extension,HDC hDC)
{
   static const char *extensions = NULL;
   const char *start;
   char *where, *terminator;
   
   where = (char *) strchr(extension, ' ');
   if ((where) || (*extension == '\0'))
      return FALSE;
   
   if (!extensions)
      extensions = (const char *) wglGetExtensionsStringARB(hDC);

   //FILE *file = fopen("WGLARBextensions.txt","wb");
   //fwrite(extensions,strlen(extensions),1,file);
   //fclose(file);
   
   start = extensions;
   for (;;) 
      {
      where = (char *) strstr((const char *) start, extension);
      if (!where)
        break;

      terminator = where + strlen(extension);
      if (where == start || *(where - 1) == ' ') 
         {
         if (*terminator == ' ' || *terminator == '\0') 
            {
            return TRUE;
            }
         }

      start = terminator;
      }

   return FALSE;
}


// Initializing All Necessary OpenGL Extensions
void InitGLExtensions(sGLExtensions &ExtStruct, HDC hDC)
{
	memset(&ExtStruct,0,sizeof(sGLExtensions));

	if(SetupARBMultitexture())
		ExtStruct.GL_ARB_multitexture_flag = true;

	if(IsExtensionSupported("GL_ARB_texture_cube_map"))
		ExtStruct.GL_ARB_texture_cube_map_flag = true;

	if(SetupARBTransposeMatrix())
		ExtStruct.GL_ARB_transpose_matrix_flag = true;

	if(SetupARBVertexBufferObject())
		ExtStruct.GL_ARB_vertex_buffer_object_flag = true;

	if(SetupARBProgram())
	{
		ExtStruct.GL_ARB_vertex_program_flag = true;
		ExtStruct.GL_ARB_fragment_program_flag = true;
	}

	if(SetupARBShaderObjects())
	{
		ExtStruct.GL_ARB_fragment_shader_flag = true;
		ExtStruct.GL_ARB_vertex_shader_flag = true;
	}

	if(SetupARBExtensionsString())
		ExtStruct.WGL_ARB_extensions_string_flag = true;
	if(SetupWGLARBPBuffer(hDC))
		ExtStruct.WGL_ARB_pbuffer_flag = true;
	if(SetupWGLARBPixelFormat(hDC))
		ExtStruct.WGL_ARB_pixel_format_flag = true;
	if(SetupWGLARBRenderTexture(hDC))
		ExtStruct.WGL_ARB_render_texture_flag = true;

	if(IsExtensionSupported("GL_ARB_depth_texture"))
		ExtStruct.GL_ARB_depth_texture_flag = true;

	if(IsExtensionSupported("GL_ARB_shadow"))
		ExtStruct.GL_ARB_shadow_flag = true;


	if(SetupATISeparateStencil())
		ExtStruct.GL_ATI_separate_stencil_flag = true;

	if(SetupEXTFogCoord())
		ExtStruct.GL_EXT_fog_coord_flag = true;

	if(SetupEXTFramebufferObject())
		ExtStruct.GL_EXT_framebuffer_object_flag = true;

	if(SetupEXTSecondaryColor())
		ExtStruct.GL_EXT_secondary_color_flag = true;

	if(SetupEXTStencilTwoSide())
		ExtStruct.GL_EXT_stencil_two_side_flag = true;

	if(IsExtensionSupported("GL_EXT_stencil_wrap"))
		ExtStruct.GL_EXT_stencil_wrap_flag = true;

	if(SetupEXTtexture3D())
		ExtStruct.GL_EXT_texture3D_flag = true;


	if(IsExtensionSupported("GL_SGIS_generate_mipmap"))
		ExtStruct.GL_SGIS_generate_mipmap_flag = true;

	if(IsExtensionSupported("GL_SGIX_depth_texture"))
		ExtStruct.GL_SGIX_depth_texture_flag = true;

	if(IsExtensionSupported("GL_SGIX_shadow"))
		ExtStruct.GL_SGIX_shadow_flag = true;


	if(SetupWGLARBPBuffer(hDC))
		ExtStruct.WGL_ARB_extensions_string_flag = true;

	if(SetupWGLARBPixelFormat(hDC))
		ExtStruct.WGL_ARB_pixel_format_flag = true;

	if(SetupWGLARBRenderTexture(hDC))
		ExtStruct.WGL_ARB_render_texture_flag = true;

	if(ExtStruct.WGL_ARB_extensions_string_flag)
		if(IsWGLARBExtensionSupported("WGL_ARB_multisample",hDC))
			ExtStruct.WGL_ARB_multisample_flag = true;

	int major,minor;
	getGlVersion(&major, &minor);
	if(!(major<2))
	{
		ExtStruct.GL_20_supported_flag = true;
		SetupGL20();
	}
}