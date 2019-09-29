/////////////////////////////////////////////////////////////////////////////////////////////
// GLSupExtStruct.h
//
// OpenGL Supported Extensions Structure
/////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _GLSUPEXTSTRUCT_H_
#define _GLSUPEXTSTRUCT_H_

struct CGLSupExtStruct 
{
    uint32t AMDX_debug_output : 1;
    uint32t AMDX_vertex_shader_tessellator : 1;

    uint32t AMD_conservative_depth : 1;
    uint32t AMD_debug_output : 1;
    uint32t AMD_depth_clamp_separate : 1;
    uint32t AMD_draw_buffers_blend : 1;
    uint32t AMD_multi_draw_indirect : 1; 
    uint32t AMD_name_gen_delete : 1; 
    uint32t AMD_performance_monitor : 1; 
    uint32t AMD_pinned_memory : 1; 
    uint32t AMD_query_buffer_object : 1; 
    uint32t AMD_sample_positions : 1; 
    uint32t AMD_seamless_cubemap_per_texture : 1; 
    uint32t AMD_shader_stencil_export : 1; 
    uint32t AMD_shader_trace : 1;
    uint32t AMD_sparse_texture : 1;
    uint32t AMD_texture_cube_map_array : 1; 
    uint32t AMD_texture_texture4 : 1; 
    uint32t AMD_transform_feedback3_lines_triangles : 1; 
    uint32t AMD_vertex_shader_layer : 1; 
    uint32t AMD_vertex_shader_tessellator : 1; 
    uint32t AMD_vertex_shader_viewport_index : 1; 

    uint32t ARB_ES2_compatibility : 1; 
    uint32t ARB_base_instance : 1; 
    uint32t ARB_blend_func_extended : 1; 
    uint32t ARB_clear_buffer_object : 1;
    uint32t ARB_color_buffer_float : 1; 
    uint32t ARB_compressed_texture_pixel_storage : 1; 
    uint32t ARB_compute_shader : 1;
    uint32t ARB_conservative_depth : 1; 
    uint32t ARB_copy_buffer : 1;
    uint32t ARB_copy_image : 1;
    uint32t ARB_debug_output : 1;
    uint32t ARB_depth_buffer_float : 1; 
    uint32t ARB_depth_clamp : 1; 
    uint32t ARB_depth_texture : 1; 
    uint32t ARB_draw_buffers : 1; 
    uint32t ARB_draw_buffers_blend : 1; 
    uint32t ARB_draw_elements_base_vertex : 1; 
    uint32t ARB_draw_indirect : 1; 
    uint32t ARB_draw_instanced : 1; 
    uint32t ARB_explicit_attrib_location : 1; 
    uint32t ARB_fragment_coord_conventions : 1; 
    uint32t ARB_fragment_program : 1; 
    uint32t ARB_fragment_program_shadow : 1; 
    uint32t ARB_fragment_shader : 1; 
    uint32t ARB_framebuffer_no_attachments : 1;
    uint32t ARB_framebuffer_object : 1; 
    uint32t ARB_framebuffer_sRGB : 1; 
    uint32t ARB_geometry_shader4 : 1;
    uint32t ARB_get_program_binary : 1; 
    uint32t ARB_gpu_shader5 : 1; 
    uint32t ARB_gpu_shader_fp64 : 1; 
    uint32t ARB_half_float_pixel : 1; 
    uint32t ARB_half_float_vertex : 1; 
    uint32t ARB_imaging : 1;
    uint32t ARB_instanced_arrays : 1; 
    uint32t ARB_internalformat_query : 1; 
    uint32t ARB_internalformat_query2 : 1;
    uint32t ARB_invalidate_subdata : 1;
    uint32t ARB_map_buffer_alignment : 1; 
    uint32t ARB_map_buffer_range : 1;
    uint32t ARB_multi_draw_indirect : 1;
    uint32t ARB_multisample : 1; 
    uint32t ARB_multitexture : 1; 
    uint32t ARB_occlusion_query : 1; 
    uint32t ARB_occlusion_query2 : 1; 
    uint32t ARB_pixel_buffer_object : 1; 
    uint32t ARB_point_parameters : 1; 
    uint32t ARB_point_sprite : 1; 
    uint32t ARB_program_interface_query : 1;
    uint32t ARB_provoking_vertex : 1; 
    uint32t ARB_sample_shading : 1; 
    uint32t ARB_sampler_objects : 1; 
    uint32t ARB_seamless_cube_map : 1; 
    uint32t ARB_separate_shader_objects : 1; 
    uint32t ARB_shader_atomic_counters : 1; 
    uint32t ARB_shader_bit_encoding : 1; 
    uint32t ARB_shader_image_load_store : 1; 
    uint32t ARB_shader_objects : 1; 
    uint32t ARB_shader_precision : 1; 
    uint32t ARB_shader_stencil_export : 1; 
    uint32t ARB_shader_storage_buffer_object : 1;
    uint32t ARB_shader_subroutine : 1; 
    uint32t ARB_shader_texture_lod : 1; 
    uint32t ARB_shading_language_100 : 1; 
    uint32t ARB_shading_language_420pack : 1; 
    uint32t ARB_shading_language_packing : 1; 
    uint32t ARB_shadow : 1; 
    uint32t ARB_shadow_ambient : 1; 
    uint32t ARB_sync : 1; 
    uint32t ARB_tessellation_shader : 1; 
    uint32t ARB_texture_border_clamp : 1; 
    uint32t ARB_texture_buffer_object : 1;
    uint32t ARB_texture_buffer_object_rgb32 : 1;
    uint32t ARB_texture_buffer_range : 1;
    uint32t ARB_texture_compression : 1; 
    uint32t ARB_texture_compression_bptc : 1; 
    uint32t ARB_texture_compression_rgtc : 1; 
    uint32t ARB_texture_cube_map : 1; 
    uint32t ARB_texture_cube_map_array : 1; 
    uint32t ARB_texture_env_add : 1; 
    uint32t ARB_texture_env_combine : 1; 
    uint32t ARB_texture_env_crossbar : 1; 
    uint32t ARB_texture_env_dot3 : 1; 
    uint32t ARB_texture_float : 1; 
    uint32t ARB_texture_gather : 1; 
    uint32t ARB_texture_mirrored_repeat : 1; 
    uint32t ARB_texture_multisample : 1; 
    uint32t ARB_texture_non_power_of_two : 1; 
    uint32t ARB_texture_query_lod : 1; 
    uint32t ARB_texture_rectangle : 1; 
    uint32t ARB_texture_rg : 1; 
    uint32t ARB_texture_rgb10_a2ui : 1; 
    uint32t ARB_texture_snorm : 1;
    uint32t ARB_texture_storage : 1;
    uint32t ARB_texture_storage_multisample : 1;
    uint32t ARB_texture_view : 1;
    uint32t ARB_timer_query : 1; 
    uint32t ARB_transform_feedback2 : 1; 
    uint32t ARB_transform_feedback3 : 1; 
    uint32t ARB_transform_feedback_instanced : 1; 
    uint32t ARB_transpose_matrix : 1; 
    uint32t ARB_uniform_buffer_object : 1; 
    uint32t ARB_vertex_array_bgra : 1; 
    uint32t ARB_vertex_array_object : 1; 
    uint32t ARB_vertex_attrib_64bit : 1; 
    uint32t ARB_vertex_attrib_binding : 1;
    uint32t ARB_vertex_buffer_object : 1; 
    uint32t ARB_vertex_program : 1; 
    uint32t ARB_vertex_shader : 1; 
    uint32t ARB_vertex_type_2_10_10_10_rev : 1; 
    uint32t ARB_viewport_array : 1; 
    uint32t ARB_window_pos : 1; 

    uint32t ATI_draw_buffers : 1; 
    uint32t ATI_envmap_bumpmap : 1; 
    uint32t ATI_fragment_shader : 1; 
    uint32t ATI_meminfo : 1; 
    uint32t ATI_separate_stencil : 1; 
    uint32t ATI_texture_compression_3dc : 1; 
    uint32t ATI_texture_env_combine3 : 1; 
    uint32t ATI_texture_float : 1; 
    uint32t ATI_texture_mirror_once : 1; 

    uint32t EXT_abgr : 1; 
    uint32t EXT_bgra : 1; 
    uint32t EXT_bindable_uniform : 1; 
    uint32t EXT_blend_color : 1; 
    uint32t EXT_blend_equation_separate : 1; 
    uint32t EXT_blend_func_separate : 1; 
    uint32t EXT_blend_minmax : 1; 
    uint32t EXT_blend_subtract : 1; 
    uint32t EXT_compiled_vertex_array : 1; 
    uint32t EXT_copy_buffer : 1; 
    uint32t EXT_copy_texture : 1; 
    uint32t EXT_direct_state_access : 1; 
    uint32t EXT_draw_buffers2 : 1; 
    uint32t EXT_draw_instanced : 1; 
    uint32t EXT_draw_range_elements : 1; 
    uint32t EXT_fog_coord : 1; 
    uint32t EXT_framebuffer_blit : 1; 
    uint32t EXT_framebuffer_multisample : 1; 
    uint32t EXT_framebuffer_object : 1; 
    uint32t EXT_framebuffer_sRGB : 1; 
    uint32t EXT_geometry_shader4 : 1; 
    uint32t EXT_gpu_program_parameters : 1; 
    uint32t EXT_gpu_shader4 : 1; 
    uint32t EXT_histogram : 1; 
    uint32t EXT_multi_draw_arrays : 1; 
    uint32t EXT_packed_depth_stencil : 1; 
    uint32t EXT_packed_float : 1; 
    uint32t EXT_packed_pixels : 1; 
    uint32t EXT_pixel_buffer_object : 1; 
    uint32t EXT_point_parameters : 1; 
    uint32t EXT_provoking_vertex : 1; 
    uint32t EXT_rescale_normal : 1; 
    uint32t EXT_secondary_color : 1; 
    uint32t EXT_separate_specular_color : 1; 
    uint32t EXT_shader_image_load_store : 1; 
    uint32t EXT_shadow_funcs : 1; 
    uint32t EXT_stencil_wrap : 1; 
    uint32t EXT_subtexture : 1; 
    uint32t EXT_texgen_reflection : 1; 
    uint32t EXT_texture3D : 1; 
    uint32t EXT_texture_array : 1; 
    uint32t EXT_texture_buffer_object : 1; 
    uint32t EXT_texture_compression_bptc : 1; 
    uint32t EXT_texture_compression_latc : 1; 
    uint32t EXT_texture_compression_rgtc : 1; 
    uint32t EXT_texture_compression_s3tc : 1;
    uint32t EXT_texture_cube_map : 1; 
    uint32t EXT_texture_edge_clamp : 1; 
    uint32t EXT_texture_env_add : 1; 
    uint32t EXT_texture_env_combine : 1; 
    uint32t EXT_texture_env_dot3 : 1; 
    uint32t EXT_texture_filter_anisotropic : 1; 
    uint32t EXT_texture_integer : 1; 
    uint32t EXT_texture_lod : 1; 
    uint32t EXT_texture_lod_bias : 1; 
    uint32t EXT_texture_mirror_clamp : 1; 
    uint32t EXT_texture_object : 1; 
    uint32t EXT_texture_rectangle : 1; 
    uint32t EXT_texture_sRGB : 1; 
    uint32t EXT_texture_sRGB_decode : 1; 
    uint32t EXT_texture_shared_exponent : 1; 
    uint32t EXT_texture_snorm : 1; 
    uint32t EXT_texture_storage : 1; 
    uint32t EXT_texture_swizzle : 1; 
    uint32t EXT_timer_query : 1; 
    uint32t EXT_transform_feedback : 1; 
    uint32t EXT_vertex_array : 1; 
    uint32t EXT_vertex_array_bgra : 1; 
    uint32t EXT_vertex_attrib_64bit : 1; 

    uint32t IBM_texture_mirrored_repeat : 1;

    uint32t KHR_debug : 1;

    uint32t KTX_buffer_region : 1; 

    uint32t NV_blend_square : 1; 
    uint32t NV_conditional_render : 1; 
    uint32t NV_copy_depth_to_color : 1; 
    uint32t NV_copy_image : 1; 
    uint32t NV_explicit_multisample : 1;
    uint32t NV_float_buffer : 1; 
    uint32t NV_half_float : 1; 
    uint32t NV_primitive_restart : 1; 
    uint32t NV_texgen_reflection : 1; 
    uint32t NV_texture_barrier : 1; 

    uint32t SGIS_generate_mipmap : 1; 
    uint32t SGIS_texture_edge_clamp : 1; 
    uint32t SGIS_texture_lod : 1; 

    uint32t SUN_multi_draw_arrays : 1; 

    //uint32t WIN_swap_hint : 1; 
    //uint32t WGL_EXT_swap_control : 1;

    CGLSupExtStruct():
    AMDX_debug_output(0),
    AMDX_vertex_shader_tessellator(0),

    AMD_conservative_depth(0),
    AMD_debug_output(0),
    AMD_depth_clamp_separate(0),
    AMD_draw_buffers_blend(0),
    AMD_multi_draw_indirect(0),
    AMD_name_gen_delete(0),
    AMD_performance_monitor(0),
    AMD_pinned_memory(0),
    AMD_query_buffer_object(0), 
    AMD_sample_positions(0),
    AMD_seamless_cubemap_per_texture(0),
    AMD_shader_stencil_export(0),
    AMD_shader_trace(0),
    AMD_sparse_texture(0),
    AMD_texture_cube_map_array(0),
    AMD_texture_texture4(0),
    AMD_transform_feedback3_lines_triangles(0),
    AMD_vertex_shader_layer(0),
    AMD_vertex_shader_tessellator(0),
    AMD_vertex_shader_viewport_index(0),

    ARB_ES2_compatibility(0),
    ARB_base_instance(0),
    ARB_blend_func_extended(0),
    ARB_clear_buffer_object(0),
    ARB_color_buffer_float(0),
    ARB_compressed_texture_pixel_storage(0),
    ARB_compute_shader(0),
    ARB_conservative_depth(0), 
    ARB_copy_buffer(0),
    ARB_copy_image(0),
    ARB_debug_output(0),
    ARB_depth_buffer_float(0), 
    ARB_depth_clamp(0), 
    ARB_depth_texture(0), 
    ARB_draw_buffers(0), 
    ARB_draw_buffers_blend(0), 
    ARB_draw_elements_base_vertex(0), 
    ARB_draw_indirect(0),
    ARB_draw_instanced(0), 
    ARB_explicit_attrib_location(0), 
    ARB_fragment_coord_conventions(0), 
    ARB_fragment_program(0),
    ARB_fragment_program_shadow(0), 
    ARB_fragment_shader(0),
    ARB_framebuffer_no_attachments(0),
    ARB_framebuffer_object(0),
    ARB_framebuffer_sRGB(0),
    ARB_geometry_shader4(0),
    ARB_get_program_binary(0), 
    ARB_gpu_shader5(0), 
    ARB_gpu_shader_fp64(0), 
    ARB_half_float_pixel(0), 
    ARB_half_float_vertex(0), 
    ARB_imaging(0),
    ARB_instanced_arrays(0), 
    ARB_internalformat_query(0), 
    ARB_internalformat_query2(0),
    ARB_invalidate_subdata(0),
    ARB_map_buffer_alignment(0), 
    ARB_map_buffer_range(0), 
    ARB_multi_draw_indirect(0),
    ARB_multisample(0), 
    ARB_multitexture(0), 
    ARB_occlusion_query(0), 
    ARB_occlusion_query2(0), 
    ARB_pixel_buffer_object(0), 
    ARB_point_parameters(0), 
    ARB_point_sprite(0),
    ARB_program_interface_query(0),
    ARB_provoking_vertex(0), 
    ARB_sample_shading(0), 
    ARB_sampler_objects(0), 
    ARB_seamless_cube_map(0), 
    ARB_separate_shader_objects(0), 
    ARB_shader_atomic_counters(0), 
    ARB_shader_bit_encoding(0), 
    ARB_shader_image_load_store(0), 
    ARB_shader_objects(0), 
    ARB_shader_precision(0), 
    ARB_shader_stencil_export(0), 
    ARB_shader_storage_buffer_object(0),
    ARB_shader_subroutine(0), 
    ARB_shader_texture_lod(0), 
    ARB_shading_language_100(0), 
    ARB_shading_language_420pack(0), 
    ARB_shading_language_packing(0), 
    ARB_shadow(0), 
    ARB_shadow_ambient(0), 
    ARB_sync(0), 
    ARB_tessellation_shader(0), 
    ARB_texture_border_clamp(0), 
    ARB_texture_buffer_object(0),
    ARB_texture_buffer_object_rgb32(0), 
    ARB_texture_buffer_range(0),
    ARB_texture_compression(0), 
    ARB_texture_compression_bptc(0), 
    ARB_texture_compression_rgtc(0), 
    ARB_texture_cube_map(0), 
    ARB_texture_cube_map_array(0), 
    ARB_texture_env_add(0), 
    ARB_texture_env_combine(0), 
    ARB_texture_env_crossbar(0), 
    ARB_texture_env_dot3(0), 
    ARB_texture_float(0), 
    ARB_texture_gather(0),
    ARB_texture_mirrored_repeat(0), 
    ARB_texture_multisample(0),
    ARB_texture_non_power_of_two(0), 
    ARB_texture_query_lod(0), 
    ARB_texture_rectangle(0), 
    ARB_texture_rg(0), 
    ARB_texture_rgb10_a2ui(0), 
    ARB_texture_snorm(0),
    ARB_texture_storage(0), 
    ARB_texture_view(0),
    ARB_timer_query(0), 
    ARB_transform_feedback2(0), 
    ARB_transform_feedback3(0), 
    ARB_transform_feedback_instanced(0), 
    ARB_transpose_matrix(0), 
    ARB_uniform_buffer_object(0), 
    ARB_vertex_array_bgra(0), 
    ARB_vertex_array_object(0), 
    ARB_vertex_attrib_64bit(0), 
    ARB_vertex_attrib_binding(0),
    ARB_vertex_buffer_object(0), 
    ARB_vertex_program(0), 
    ARB_vertex_shader(0), 
    ARB_vertex_type_2_10_10_10_rev(0), 
    ARB_viewport_array(0), 
    ARB_window_pos(0),

    ATI_draw_buffers(0), 
    ATI_envmap_bumpmap(0), 
    ATI_fragment_shader(0), 
    ATI_meminfo(0), 
    ATI_separate_stencil(0), 
    ATI_texture_compression_3dc(0), 
    ATI_texture_env_combine3(0),
    ATI_texture_float(0),
    ATI_texture_mirror_once(0),

    EXT_abgr(0), 
    EXT_bgra(0), 
    EXT_bindable_uniform(0), 
    EXT_blend_color(0), 
    EXT_blend_equation_separate(0), 
    EXT_blend_func_separate(0), 
    EXT_blend_minmax(0), 
    EXT_blend_subtract(0), 
    EXT_compiled_vertex_array(0), 
    EXT_copy_buffer(0), 
    EXT_copy_texture(0), 
    EXT_direct_state_access(0), 
    EXT_draw_buffers2(0), 
    EXT_draw_instanced(0), 
    EXT_draw_range_elements(0), 
    EXT_fog_coord(0), 
    EXT_framebuffer_blit(0), 
    EXT_framebuffer_multisample(0), 
    EXT_framebuffer_object(0), 
    EXT_framebuffer_sRGB(0),
    EXT_geometry_shader4(0), 
    EXT_gpu_program_parameters(0), 
    EXT_gpu_shader4(0), 
    EXT_histogram(0), 
    EXT_multi_draw_arrays(0), 
    EXT_packed_depth_stencil(0), 
    EXT_packed_float(0),
    EXT_packed_pixels(0), 
    EXT_pixel_buffer_object(0), 
    EXT_point_parameters(0), 
    EXT_provoking_vertex(0), 
    EXT_rescale_normal(0), 
    EXT_secondary_color(0), 
    EXT_separate_specular_color(0), 
    EXT_shader_image_load_store(0), 
    EXT_shadow_funcs(0), 
    EXT_stencil_wrap(0), 
    EXT_subtexture(0), 
    EXT_texgen_reflection(0), 
    EXT_texture3D(0), 
    EXT_texture_array(0), 
    EXT_texture_buffer_object(0),
    EXT_texture_compression_bptc(0), 
    EXT_texture_compression_latc(0), 
    EXT_texture_compression_rgtc(0), 
    EXT_texture_compression_s3tc(0),
    EXT_texture_cube_map(0), 
    EXT_texture_edge_clamp(0), 
    EXT_texture_env_add(0), 
    EXT_texture_env_combine(0), 
    EXT_texture_env_dot3(0), 
    EXT_texture_filter_anisotropic(0), 
    EXT_texture_integer(0), 
    EXT_texture_lod(0), 
    EXT_texture_lod_bias(0), 
    EXT_texture_mirror_clamp(0), 
    EXT_texture_object(0), 
    EXT_texture_rectangle(0), 
    EXT_texture_sRGB(0), 
    EXT_texture_sRGB_decode(0), 
    EXT_texture_shared_exponent(0), 
    EXT_texture_snorm(0),
    EXT_texture_storage(0), 
    EXT_texture_swizzle(0), 
    EXT_timer_query(0), 
    EXT_transform_feedback(0), 
    EXT_vertex_array(0), 
    EXT_vertex_array_bgra(0), 
    EXT_vertex_attrib_64bit(0), 

    IBM_texture_mirrored_repeat(0),

    KHR_debug(0),

    KTX_buffer_region(0), 

    NV_blend_square(0), 
    NV_conditional_render(0), 
    NV_copy_depth_to_color(0), 
    NV_copy_image(0), 
    NV_explicit_multisample(0),
    NV_float_buffer(0), 
    NV_half_float(0), 
    NV_primitive_restart(0), 
    NV_texgen_reflection(0), 
    NV_texture_barrier(0), 

    SGIS_generate_mipmap(0), 
    SGIS_texture_edge_clamp(0), 
    SGIS_texture_lod(0), 

    SUN_multi_draw_arrays(0) 
    
    {
    }

};

#endif //_GLSUPEXTSTRUCT_H_