////////////////////////////////////////////////////////////////////////////////////////////////
//	GLExtensions.h 
//	OpenGL Extensions Setup Header 
//	Created: 1st August 2006
//	
////////////////////////////////////////////////////////////////////////////////////////////////

#include "ARB_matrix_palette.h"
#include "ARB_multitexture_extension.h"
#include "ARB_program.h"
#include "ARB_shader_objects.h"
#include "ARB_texture_compression.h"
#include "ARB_transpose_matrix.h"
#include "ARB_vertex_blend.h"
#include "ARB_vertex_buffer_object.h"

#include "ATI_separate_stencil.h"

#include "EXT_blend_minmax.h"
#include "EXT_multi_draw_arrays.h"
#include "EXT_draw_range_elements.h"
#include "EXT_compiled_vertex_array.h"
#include "EXT_fog_coord.h"
#include "EXT_framebuffer_object.h"
#include "EXT_secondary_color.h"
#include "EXT_stencil_two_side.h"
#include "EXT_texture_3D.h"

#include "NV_register_combiners.h"
#include "NV_register_combiners2.h"
#include "NV_vertex_program.h"

#include "WGL_ARB_extensions_string.h"
#include "WGL_ARB_pbuffer.h"
#include "WGL_ARB_pixel_format.h"
#include "WGL_ARB_render_texture.h"

#include "GL_version_2_0.h"

void InitGLExtensions(sGLExtensions &ExtStruct, HDC hDC);