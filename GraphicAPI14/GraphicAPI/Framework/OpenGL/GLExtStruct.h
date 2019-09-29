////////////////////////////////////////////////////////////////////////////////////////////////
//	GLExtStruct.h 
//	Structure Contains Flags Of Supported OpenGL Extensions 
//	Created: 16th August 2006
//	
////////////////////////////////////////////////////////////////////////////////////////////////


typedef struct GLExtensions
{
	bool GL_ARB_matrix_palette_flag;
	bool GL_ARB_multitexture_flag;
	bool GL_ARB_depth_texture_flag;
	bool GL_ARB_shadow_flag;
	bool GL_ARB_texture_env_combine_flag;
	bool GL_ARB_texture_env_dot3_flag;
	bool GL_ARB_vertex_program_flag;
	bool GL_ARB_vertex_shader_flag;
	bool GL_ARB_fragment_program_flag;
	bool GL_ARB_fragment_shader_flag;
	bool GL_ARB_texture_compression_flag;
	bool GL_ARB_texture_cube_map_flag;
	bool GL_ARB_texture_border_clamp_flag;
	bool GL_ARB_transpose_matrix_flag;
	bool GL_ARB_vertex_blend_flag;
	bool GL_ARB_vertex_buffer_object_flag;
	bool GL_ARB_shading_language_100_flag;
	bool GL_ARB_shader_objects_flag;

	bool GL_ATI_separate_stencil_flag;

	bool GL_EXT_compiled_vertex_array_flag;
	bool GL_EXT_texture_cube_map_flag;
	bool GL_EXT_texture_filter_anisotropic_flag;
	bool GL_EXT_texture_env_combine_flag;
	bool GL_EXT_fog_coord_flag;
	bool GL_EXT_framebuffer_object_flag;
	bool GL_EXT_secondary_color_flag;
	bool GL_EXT_stencil_two_side_flag;
	bool GL_EXT_stencil_wrap_flag;
	bool GL_EXT_blend_subtract_flag;
	bool GL_EXT_texture3D_flag;
	bool GL_EXT_texture_compression_s3tc_flag;
	bool GL_EXT_texture_edge_clamp_flag; 
	bool GL_EXT_texture_lod_bias_flag;
	bool GL_EXT_texture_rectangle_flag;
	bool GL_EXT_multi_draw_arrays_flag;
	bool GL_EXT_draw_range_elements_flag;

	bool GL_NV_register_combiners_flag;
	bool GL_NV_register_combiners2_flag;
	bool GL_NV_vertex_program_flag;
	bool GL_NV_texture_shader_flag;

	bool GL_SGIS_generate_mipmap_flag;
	bool GL_SGIX_shadow_flag;
	bool GL_SGIX_depth_texture_flag;

	bool WGL_ARB_extensions_string_flag;
	bool WGL_ARB_pbuffer_flag;
	bool WGL_ARB_pixel_format_flag;
	bool WGL_ARB_render_texture_flag;
	bool WGL_ARB_multisample_flag;

	bool GL_20_supported_flag;

}sGLExtensions;