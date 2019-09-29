/////////////////////////////////////////////////////////////////////////////////////////////
// GLDevInfo.cpp
//
// OpenGL Device Info Class
/////////////////////////////////////////////////////////////////////////////////////////////

#include <stdio.h>

#include "GLDevInfo.h"
#include "Utils/Logger/Logger.h"
#include "sys/getproc.h"
//#include "GLExtProc.h"
//#include "GLARBcoreProc.h"
#include "GLVerProc.h"
#include "GLArbProc.h"

#include <string>

/////////////////////////////////////////////////////////////////////////////////////////////
const char* CGLDevInfo::m_strExtensions = NULL;
const char* CGLDevInfo::m_strVendor = NULL;
const char* CGLDevInfo::m_strVersion = NULL;
const char* CGLDevInfo::m_strRender = NULL;
const char* CGLDevInfo::m_strVerGLSL = NULL;

CGLSupExtStruct CGLDevInfo::m_GLSupExt = CGLSupExtStruct();
//--CGLDevInfo::GLExtSupportedStruct CGLDevInfo::mExtSupInfo = GLExtSupportedStruct::GLExtSupportedStruct();

/////////////////////////////////////////////////////////////////////////////////////////////
const CGLSupExtStruct& CGLDevInfo::GetDevInfoStruct()
{
    return m_GLSupExt;
}

/////////////////////////////////////////////////////////////////////////////////////////////
void CGLDevInfo::InitExtensions()
{
    m_strExtensions = (const char*) glGetString(GL_EXTENSIONS);
    m_strVendor     = (const char*) glGetString(GL_VENDOR);
    m_strVersion    = (const char*) glGetString(GL_VERSION);
    m_strRender     = (const char*) glGetString(GL_RENDERER);
    m_strVerGLSL    = (const char*) glGetString(GL_SHADING_LANGUAGE_VERSION);

	std::string strglext(m_strExtensions);
	size_t spacepos = strglext.find(' ',0);
	while(spacepos != std::string::npos) {
		strglext.replace(spacepos,1,"\n");
		spacepos = strglext.find(' ',spacepos);
	}

    //Putting OpenGL Info
    int major = 0;
	int minor = 0;
    glGetIntegerv(GL_MAJOR_VERSION, &major);
    glGetIntegerv(GL_MINOR_VERSION, &minor);
    LOG_DEBUG(LOG_RENDER_INFO,"OpenGL render context information:\n"
        "  Renderer       : %s\n"
        "  Vendor         : %s\n"
        "  Version        : %s\n"
        "  GLSL version   : %s\n"
        "  OpenGL version : %d.%d\n"
        "  Extensions     : \n%s",
        m_strRender,
        m_strVendor,
        m_strVersion,
        m_strVerGLSL,
        major, minor,
		strglext.c_str()
        );

    GetGLSupExtInfo();
    SetupGLExtProc();
}

/////////////////////////////////////////////////////////////////////////////////////////////
uint32t CGLDevInfo::GetDevParam(GAPIDEVCAPSPARAM devparam)
{
    GLint iparam = 0;
    GLfloat fparam = 0.0f;


    switch(devparam)
    {
    case GAPI_DEVPARAM_MAX_LIGHTS:
        glGetIntegerv(GL_MAX_LIGHTS, &iparam);
        break;
    case GAPI_DEVPARAM_MAX_TEXTURE_SIZE:
        glGetIntegerv(GL_MAX_TEXTURE_SIZE, &iparam);
        break;
    case GAPI_DEVPARAM_MAX_TEXTURE_UNITS:
        glGetIntegerv(GL_MAX_TEXTURE_UNITS_ARB, &iparam);
        break;
    case GAPI_DEVPARAM_MAX_TEXTURE_ANISOTROPY:
        glGetIntegerv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &iparam);
        break;

    case GAPI_DEVPARAM_MIN_POINT_SIZE:
        glGetFloatv(GL_POINT_SIZE_MIN_ARB, &fparam);
        iparam = (GLint)fparam;
        break;
    case GAPI_DEVPARAM_MAX_POINT_SIZE:
        glGetFloatv(GL_POINT_SIZE_MAX_ARB, &fparam);
        iparam = (GLint)fparam;
        break;

    // Vertex Shader Params
    case GAPI_DEVPARAM_VERTEX_SHADER:
        iparam = m_GLSupExt.ARB_vertex_shader && m_GLSupExt.ARB_vertex_program;
        break;
    case GAPI_DEVPARAM_MAX_VERTEX_ATTRIBS:
        glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &iparam);////----
        break;
    case GAPI_DEVPARAM_MAX_VERTEX_UNIFORM_COMPONENTS:
        glGetIntegerv(GL_MAX_VERTEX_UNIFORM_COMPONENTS_ARB,&iparam);
        break;
    case GAPI_DEVPARAM_MAX_VERTEX_VARYING_COMPONENTS:
        glGetIntegerv(GL_MAX_VERTEX_VARYING_COMPONENTS_ARB,&iparam);
        break;
    case GAPI_DEVPARAM_MAX_VARYING_COMPONENTS:
        glGetIntegerv(GL_MAX_VARYING_COMPONENTS,&iparam);
        break;
    case GAPI_DEVPARAM_MAX_VERTEX_TEXTURE_UNITS:
        glGetIntegerv(GL_MAX_VERTEX_TEXTURE_IMAGE_UNITS_ARB,&iparam);
        break;
    case GAPI_DEVPARAM_MAX_VARYING_FLOATS:
        glGetIntegerv(GL_MAX_VARYING_FLOATS_ARB,&iparam);
        break;
    case GAPI_DEVPARAM_MAX_COMBINED_TEXTURE_UNITS:
        glGetIntegerv(GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS_ARB,&iparam);
        break;
            

    // Fragment Shader Params 
    case GAPI_DEVPARAM_PIXEL_SHADER:
        iparam = m_GLSupExt.ARB_fragment_program && m_GLSupExt.ARB_fragment_shader;
        break;
    case GAPI_DEVPARAM_MAX_FRAGMENT_UNIFORM_COMPONENTS:
        glGetIntegerv(GL_MAX_FRAGMENT_UNIFORM_COMPONENTS_ARB,&iparam);
        break;
    case GAPI_DEVPARAM_MAX_FRAGMENT_TEXTURE_UNITS:
        glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS,&iparam);
        break;

    // Geometry Shader Params
    case GAPI_DEVPARAM_GEOMETRY_SHADER:
        iparam = m_GLSupExt.ARB_geometry_shader4;
        break;

    case GAPI_DEVPARAM_MAX_GEOMETRY_OUTPUT_VERT:
        glGetIntegerv(GL_MAX_GEOMETRY_OUTPUT_VERTICES_EXT,&iparam);////----
        break;

    case GAPI_DEVPARAM_MAX_GEOMETRY_TEXTURE_UNITS:
        glGetIntegerv(GL_MAX_GEOMETRY_TEXTURE_IMAGE_UNITS_ARB,&iparam);
        break;

    case GAPI_DEVPARAM_MAX_GEOMETRY_VARYING_COMPONENTS:
        glGetIntegerv(GL_MAX_GEOMETRY_VARYING_COMPONENTS_ARB,&iparam);
        break;

    case GAPI_DEVPARAM_MAX_GEOMETRY_UNIFORM_COMPONENTS:
        glGetIntegerv(GL_MAX_GEOMETRY_UNIFORM_COMPONENTS_ARB,&iparam);
        break;

    case GAPI_DEVPARAM_MAX_GEOMETRY_TOTAL_OUTPUT_COMPONENTS:
        glGetIntegerv(GL_MAX_GEOMETRY_TOTAL_OUTPUT_COMPONENTS_ARB,&iparam);
        break;

    // Uniform Buffer Objects
    case GAPI_DEVPARAM_UNIFORM_BLOCKS:
        iparam = m_GLSupExt.ARB_uniform_buffer_object;
        break;

    case GAPI_DEVPARAM_MAX_VERTEX_UNIFORM_BLOCKS:
        glGetIntegerv(GL_MAX_VERTEX_UNIFORM_BLOCKS,&iparam);
        break;

    case GAPI_DEVPARAM_MAX_GEOMETRY_UNIFORM_BLOCKS:
        glGetIntegerv(GL_MAX_GEOMETRY_UNIFORM_BLOCKS,&iparam);
        break;

    case GAPI_DEVPARAM_MAX_FRAGMENT_UNIFORM_BLOCKS:
        glGetIntegerv(GL_MAX_FRAGMENT_UNIFORM_BLOCKS,&iparam);
        break;

    case GAPI_DEVPARAM_MAX_COMBINED_UNIFORM_BLOCKS:
        glGetIntegerv(GL_MAX_COMBINED_UNIFORM_BLOCKS,&iparam);
        break;

    case GAPI_DEVPARAM_MAX_UNIFORM_BUFFER_BINDINGS:
        glGetIntegerv(GL_MAX_UNIFORM_BUFFER_BINDINGS,&iparam);
        break;

    case GAPI_DEVPARAM_MAX_UNIFORM_BLOCK_SIZE:
        glGetIntegerv(GL_MAX_UNIFORM_BLOCK_SIZE,&iparam);
        break;

    case GAPI_DEVPARAM_MAX_COMBINED_VERTEX_UNIFORM_COMPONENTS:
        glGetIntegerv(GL_MAX_COMBINED_VERTEX_UNIFORM_COMPONENTS,&iparam);
        break;

    case GAPI_DEVPARAM_MAX_COMBINED_GEOMETRY_UNIFORM_COMPONENTS:
        glGetIntegerv(GL_MAX_COMBINED_GEOMETRY_UNIFORM_COMPONENTS,&iparam);
        break;

    case GAPI_DEVPARAM_MAX_COMBINED_FRAGMENT_UNIFORM_COMPONENTS:
        glGetIntegerv(GL_MAX_COMBINED_FRAGMENT_UNIFORM_COMPONENTS,&iparam);
        break;

    // Draw Buffers Params
    case GAPI_DEVPARAM_MAX_DRAW_BUFFERS:
        glGetIntegerv(GL_MAX_DRAW_BUFFERS,&iparam);
        break;

    // Clip Distances
    case GAPI_DEVPARAM_MAX_CLIP_DISTANCES:
        glGetIntegerv(GL_MAX_CLIP_DISTANCES,&iparam);
        break;

    // Transform Feedback
    case GAPI_DEVPARAM_MAX_TRANSFORM_FEEDBACK_SEPARATE_COMPONENTS:
        glGetIntegerv(GL_MAX_TRANSFORM_FEEDBACK_SEPARATE_COMPONENTS,&iparam);
        break;
    case GAPI_DEVPARAM_MAX_TRANSFORM_FEEDBACK_INTERLEAVED_COMPONENTS:
        glGetIntegerv(GL_MAX_TRANSFORM_FEEDBACK_INTERLEAVED_COMPONENTS,&iparam);
        break;
    case GAPI_DEVPARAM_MAX_TRANSFORM_FEEDBACK_SEPARATE_ATTRIBS:
        glGetIntegerv(GL_MAX_TRANSFORM_FEEDBACK_SEPARATE_ATTRIBS,&iparam);
        break;

    // Blend Func Extended
    case GAPI_DEVPARAM_MAX_DUAL_SOURCE_DRAW_BUFFERS:
        glGetIntegerv(GL_MAX_DUAL_SOURCE_DRAW_BUFFERS,&iparam);
        break;

    // Texture Layers
    case GAPI_DEVPARAM_MAX_ARRAY_TEXTURE_LAYERS:
        glGetIntegerv(GL_MAX_ARRAY_TEXTURE_LAYERS,&iparam);
        break;

    case GAPI_DEVPARAM_MIN_PROGRAM_TEXEL_OFFSET:
        glGetIntegerv(GL_MIN_PROGRAM_TEXEL_OFFSET,&iparam);
        break;

    case GAPI_DEVPARAM_MAX_PROGRAM_TEXEL_OFFSET:
        glGetIntegerv(GL_MAX_PROGRAM_TEXEL_OFFSET,&iparam);
        break;

    case GAPI_DEVPARAM_MAX_RECTANGLE_TEXTURE_SIZE:
        glGetIntegerv(GL_MAX_RECTANGLE_TEXTURE_SIZE,&iparam);
        break;
    
    ////////////////////////////////
    // Program Texture
    case GAPI_DEVPARAM_MIN_SAMPLE_SHADING_VALUE:
        glGetIntegerv(GL_MIN_SAMPLE_SHADING_VALUE,&iparam);
        break;

    case GAPI_DEVPARAM_MIN_PROGRAM_TEXTURE_GATHER_OFFSET:
        glGetIntegerv(GL_MIN_PROGRAM_TEXTURE_GATHER_OFFSET,&iparam);
        break;

    case GAPI_DEVPARAM_MAX_PROGRAM_TEXTURE_GATHER_OFFSET:
        glGetIntegerv(GL_MAX_PROGRAM_TEXTURE_GATHER_OFFSET,&iparam);
        break;

    case GAPI_DEVPARAM_MAX_PROGRAM_TEXTURE_GATHER_COMPONENTS:
        glGetIntegerv(GL_MAX_PROGRAM_TEXTURE_GATHER_COMPONENTS_ARB,&iparam);
        break;


    // Texture Cube Map
    case GAPI_DEVPARAM_MAX_CUBE_MAP_TEXTURE_SIZE:
        glGetIntegerv(GL_MAX_CUBE_MAP_TEXTURE_SIZE_ARB,&iparam);
        break;

    // Framebuffer Object
    case GAPI_DEVPARAM_MAX_RENDERBUFFER_SIZE:
        glGetIntegerv(GL_MAX_RENDERBUFFER_SIZE,&iparam);
        break;

    case GAPI_DEVPARAM_MAX_COLOR_ATTACHMENTS:
        glGetIntegerv(GL_MAX_COLOR_ATTACHMENTS,&iparam);
        break;

    case GAPI_DEVPARAM_MAX_SAMPLES:
        glGetIntegerv(GL_MAX_SAMPLES,&iparam);
        break;

    // Texture Buffer Object
    case GAPI_DEVPARAM_MAX_TEXTURE_BUFFER_SIZE:
        glGetIntegerv(GL_MAX_TEXTURE_BUFFER_SIZE,&iparam);
        break;
    
    // Texture Multisample 
    case GAPI_DEVPARAM_MAX_SAMPLE_MASK_WORDS:
        glGetIntegerv(GL_MAX_SAMPLE_MASK_WORDS,&iparam);
        break;

    case GAPI_DEVPARAM_MAX_COLOR_TEXTURE_SAMPLES:
        glGetIntegerv(GL_MAX_COLOR_TEXTURE_SAMPLES,&iparam);
        break;

    case GAPI_DEVPARAM_MAX_DEPTH_TEXTURE_SAMPLES:
        glGetIntegerv(GL_MAX_DEPTH_TEXTURE_SAMPLES,&iparam);
        break;

    case GAPI_DEVPARAM_MAX_INTEGER_SAMPLES:
        glGetIntegerv(GL_MAX_INTEGER_SAMPLES,&iparam);
        break;

  
    // GPU Shader 5
    case GAPI_DEVPARAM_MAX_GEOMETRY_SHADER_INVOCATIONS:
        glGetIntegerv(GL_MAX_GEOMETRY_SHADER_INVOCATIONS,&iparam);
        break;

    case GAPI_DEVPARAM_MIN_FRAGMENT_INTERPOLATION_OFFSET:
        glGetIntegerv(GL_MIN_FRAGMENT_INTERPOLATION_OFFSET,&iparam);
        break;

    case GAPI_DEVPARAM_MAX_FRAGMENT_INTERPOLATION_OFFSET:
        glGetIntegerv(GL_MAX_FRAGMENT_INTERPOLATION_OFFSET,&iparam);
        break;

    case GAPI_DEVPARAM_FRAGMENT_INTERPOLATION_OFFSET_BITS:
        glGetIntegerv(GL_FRAGMENT_INTERPOLATION_OFFSET_BITS,&iparam);
        break;

    // Tessellation Shader
    case GAPI_DEVPARAM_MAX_PATCH_VERTICES:
        glGetIntegerv(GL_MAX_PATCH_VERTICES,&iparam);
        break;

    case GAPI_DEVPARAM_MAX_TESS_GEN_LEVEL:
        glGetIntegerv(GL_MAX_TESS_GEN_LEVEL,&iparam);
        break;

    case GAPI_DEVPARAM_MAX_TESS_CONTROL_UNIFORM_COMPONENTS:
        glGetIntegerv(GL_MAX_TESS_CONTROL_UNIFORM_COMPONENTS,&iparam);
        break;

    case GAPI_DEVPARAM_MAX_TESS_EVALUATION_UNIFORM_COMPONENTS:
        glGetIntegerv(GL_MAX_TESS_EVALUATION_UNIFORM_COMPONENTS,&iparam);
        break;

    case GAPI_DEVPARAM_MAX_TESS_CONTROL_TEXTURE_IMAGE_UNITS:
        glGetIntegerv(GL_MAX_TESS_CONTROL_TEXTURE_IMAGE_UNITS,&iparam);
        break;

    case GAPI_DEVPARAM_MAX_TESS_EVALUATION_TEXTURE_IMAGE_UNITS:
        glGetIntegerv(GL_MAX_TESS_EVALUATION_TEXTURE_IMAGE_UNITS,&iparam);
        break;

    case GAPI_DEVPARAM_MAX_TESS_CONTROL_OUTPUT_COMPONENTS:
        glGetIntegerv(GL_MAX_TESS_CONTROL_OUTPUT_COMPONENTS,&iparam);
        break;

    case GAPI_DEVPARAM_MAX_TESS_PATCH_COMPONENTS:
        glGetIntegerv(GL_MAX_TESS_PATCH_COMPONENTS,&iparam);
        break;

    case GAPI_DEVPARAM_MAX_TESS_CONTROL_TOTAL_OUTPUT_COMPONENTS:
        glGetIntegerv(GL_MAX_TESS_CONTROL_TOTAL_OUTPUT_COMPONENTS,&iparam);
        break;

    case GAPI_DEVPARAM_MAX_TESS_EVALUATION_OUTPUT_COMPONENTS:
        glGetIntegerv(GL_MAX_TESS_EVALUATION_OUTPUT_COMPONENTS,&iparam);
        break;

    case GAPI_DEVPARAM_MAX_TESS_CONTROL_UNIFORM_BLOCKS:
        glGetIntegerv(GL_MAX_TESS_CONTROL_UNIFORM_BLOCKS,&iparam);
        break;

    case GAPI_DEVPARAM_MAX_TESS_EVALUATION_UNIFORM_BLOCKS:
        glGetIntegerv(GL_MAX_TESS_EVALUATION_UNIFORM_BLOCKS,&iparam);
        break;

    case GAPI_DEVPARAM_MAX_TESS_CONTROL_INPUT_COMPONENTS:
        glGetIntegerv(GL_MAX_TESS_CONTROL_INPUT_COMPONENTS,&iparam);
        break;

    case GAPI_DEVPARAM_MAX_TESS_EVALUATION_INPUT_COMPONENTS:
        glGetIntegerv(GL_MAX_TESS_EVALUATION_INPUT_COMPONENTS,&iparam);
        break;

    case GAPI_DEVPARAM_MAX_COMBINED_TESS_CONTROL_UNIFORM_COMPONENTS:
        glGetIntegerv(GL_MAX_COMBINED_TESS_CONTROL_UNIFORM_COMPONENTS,&iparam);
        break;

    case GAPI_DEVPARAM_MAX_COMBINED_TESS_EVALUATION_UNIFORM_COMPONENTS:
        glGetIntegerv(GL_MAX_COMBINED_TESS_EVALUATION_UNIFORM_COMPONENTS,&iparam);
        break;


    // Transform Feedback 3
    case GAPI_DEVPARAM_MAX_TRANSFORM_FEEDBACK_BUFFERS:
        glGetIntegerv(GL_MAX_TRANSFORM_FEEDBACK_BUFFERS,&iparam);
        break;

    case GAPI_DEVPARAM_MAX_VERTEX_STREAMS:
        glGetIntegerv(GL_MAX_VERTEX_STREAMS,&iparam);
        break;

    // Viewport Array
    case GAPI_DEVPARAM_MAX_VIEWPORTS:
        glGetIntegerv(GL_MAX_VIEWPORTS,&iparam);
        break;
    
    // Debug Output
    case GAPI_DEVPARAM_MAX_DEBUG_MESSAGE_LENGTH:
        glGetIntegerv(GL_MAX_DEBUG_MESSAGE_LENGTH_ARB,&iparam);
        break;

    case GAPI_DEVPARAM_MAX_DEBUG_LOGGED_MESSAGES:
        glGetIntegerv(GL_MAX_DEBUG_LOGGED_MESSAGES,&iparam);
        break;

    // Map Buffer Alignment
    case GAPI_DEVPARAM_MIN_MAP_BUFFER_ALIGNMENT:
        glGetIntegerv(GL_MIN_MAP_BUFFER_ALIGNMENT,&iparam);
        break;
    
    // Shader Atomic Counters
    case GAPI_DEVPARAM_MAX_VERTEX_ATOMIC_COUNTER_BUFFERS:
        glGetIntegerv(GL_MAX_VERTEX_ATOMIC_COUNTER_BUFFERS,&iparam);
        break;

    case GAPI_DEVPARAM_MAX_TESS_CONTROL_ATOMIC_COUNTER_BUFFERS:
        glGetIntegerv(GL_MAX_TESS_CONTROL_ATOMIC_COUNTER_BUFFERS,&iparam);
        break;

    case GAPI_DEVPARAM_MAX_TESS_EVALUATION_ATOMIC_COUNTER_BUFFERS:
        glGetIntegerv(GL_MAX_TESS_EVALUATION_ATOMIC_COUNTER_BUFFERS,&iparam);
        break;

    case GAPI_DEVPARAM_MAX_GEOMETRY_ATOMIC_COUNTER_BUFFERS:
        glGetIntegerv(GL_MAX_GEOMETRY_ATOMIC_COUNTER_BUFFERS,&iparam);
        break;

    case GAPI_DEVPARAM_MAX_FRAGMENT_ATOMIC_COUNTER_BUFFERS:
        glGetIntegerv(GL_MAX_FRAGMENT_ATOMIC_COUNTER_BUFFERS,&iparam);
        break;

    case GAPI_DEVPARAM_MAX_COMBINED_ATOMIC_COUNTER_BUFFERS:
        glGetIntegerv(GL_MAX_COMBINED_ATOMIC_COUNTER_BUFFERS,&iparam);
        break;

    case GAPI_DEVPARAM_MAX_VERTEX_ATOMIC_COUNTERS:
        glGetIntegerv(GL_MAX_VERTEX_ATOMIC_COUNTERS,&iparam);
        break;

    case GAPI_DEVPARAM_MAX_TESS_CONTROL_ATOMIC_COUNTERS:
        glGetIntegerv(GL_MAX_TESS_CONTROL_ATOMIC_COUNTERS,&iparam);
        break;

    case GAPI_DEVPARAM_MAX_TESS_EVALUATION_ATOMIC_COUNTERS:
        glGetIntegerv(GL_MAX_TESS_EVALUATION_ATOMIC_COUNTERS,&iparam);
        break;

    case GAPI_DEVPARAM_MAX_GEOMETRY_ATOMIC_COUNTERS:
        glGetIntegerv(GL_MAX_GEOMETRY_ATOMIC_COUNTERS,&iparam);
        break;

    case GAPI_DEVPARAM_MAX_FRAGMENT_ATOMIC_COUNTERS:
        glGetIntegerv(GL_MAX_FRAGMENT_ATOMIC_COUNTERS,&iparam);
        break;

    case GAPI_DEVPARAM_MAX_COMBINED_ATOMIC_COUNTERS:
        glGetIntegerv(GL_MAX_COMBINED_ATOMIC_COUNTERS,&iparam);
        break;

    case GAPI_DEVPARAM_MAX_ATOMIC_COUNTER_BUFFER_SIZE:
        glGetIntegerv(GL_MAX_ATOMIC_COUNTER_BUFFER_SIZE,&iparam);
        break;

    case GAPI_DEVPARAM_MAX_ATOMIC_COUNTER_BUFFER_BINDINGS:
        glGetIntegerv(GL_MAX_ATOMIC_COUNTER_BUFFER_BINDINGS,&iparam);
        break;


    // Shader Image Load Store
    case GAPI_DEVPARAM_MAX_IMAGE_UNITS:
        glGetIntegerv(GL_MAX_IMAGE_UNITS,&iparam);
        break;

    case GAPI_DEVPARAM_MAX_COMBINED_IMAGE_UNITS_AND_FRAGMENT_OUTPUTS:
        glGetIntegerv(GL_MAX_COMBINED_IMAGE_UNITS_AND_FRAGMENT_OUTPUTS,&iparam);
        break;

    case GAPI_DEVPARAM_MAX_IMAGE_SAMPLES:
        glGetIntegerv(GL_MAX_IMAGE_SAMPLES,&iparam);
        break;

    case GAPI_DEVPARAM_MAX_VERTEX_IMAGE_UNIFORMS:
        glGetIntegerv(GL_MAX_VERTEX_IMAGE_UNIFORMS,&iparam);
        break;

    case GAPI_DEVPARAM_MAX_TESS_EVALUATION_IMAGE_UNIFORMS:
        glGetIntegerv(GL_MAX_TESS_EVALUATION_IMAGE_UNIFORMS,&iparam);
        break;

    case GAPI_DEVPARAM_MAX_GEOMETRY_IMAGE_UNIFORMS:
        glGetIntegerv(GL_MAX_GEOMETRY_IMAGE_UNIFORMS,&iparam);
        break;

    case GAPI_DEVPARAM_MAX_FRAGMENT_IMAGE_UNIFORMS:
        glGetIntegerv(GL_MAX_FRAGMENT_IMAGE_UNIFORMS,&iparam);
        break;

    case GAPI_DEVPARAM_MAX_COMBINED_IMAGE_UNIFORMS:
        glGetIntegerv(GL_MAX_COMBINED_IMAGE_UNIFORMS,&iparam);
        break;

    }
    

    return (uint32t)iparam;
}

/////////////////////////////////////////////////////////////////////////////////////////////
void CGLDevInfo::GetGLSupExtInfo()
{
    std::string strglext(m_strExtensions);

    m_GLSupExt.AMDX_debug_output = OPENGL_GET_SUP_EXT(strglext,AMDX_debug_output);
    m_GLSupExt.AMDX_vertex_shader_tessellator = OPENGL_GET_SUP_EXT(strglext,AMDX_vertex_shader_tessellator);

    m_GLSupExt.AMD_conservative_depth = OPENGL_GET_SUP_EXT(strglext,AMD_conservative_depth);
    m_GLSupExt.AMD_debug_output = OPENGL_GET_SUP_EXT(strglext,AMD_debug_output);
    m_GLSupExt.AMD_depth_clamp_separate = OPENGL_GET_SUP_EXT(strglext,AMD_depth_clamp_separate);
    m_GLSupExt.AMD_draw_buffers_blend = OPENGL_GET_SUP_EXT(strglext,AMD_draw_buffers_blend);
    m_GLSupExt.AMD_multi_draw_indirect = OPENGL_GET_SUP_EXT(strglext,AMD_multi_draw_indirect);
    m_GLSupExt.AMD_name_gen_delete = OPENGL_GET_SUP_EXT(strglext,AMD_name_gen_delete);
    m_GLSupExt.AMD_performance_monitor = OPENGL_GET_SUP_EXT(strglext,AMD_performance_monitor);
    m_GLSupExt.AMD_pinned_memory = OPENGL_GET_SUP_EXT(strglext,AMD_pinned_memory);
    m_GLSupExt.AMD_query_buffer_object = OPENGL_GET_SUP_EXT(strglext,AMD_query_buffer_object);
    m_GLSupExt.AMD_sample_positions = OPENGL_GET_SUP_EXT(strglext,AMD_sample_positions);
    m_GLSupExt.AMD_seamless_cubemap_per_texture = OPENGL_GET_SUP_EXT(strglext,AMD_seamless_cubemap_per_texture);
    m_GLSupExt.AMD_shader_stencil_export = OPENGL_GET_SUP_EXT(strglext,AMD_shader_stencil_export);
    m_GLSupExt.AMD_shader_trace = OPENGL_GET_SUP_EXT(strglext,AMD_shader_trace);
    m_GLSupExt.AMD_texture_cube_map_array = OPENGL_GET_SUP_EXT(strglext,AMD_texture_cube_map_array);
    m_GLSupExt.AMD_texture_texture4 = OPENGL_GET_SUP_EXT(strglext,AMD_texture_texture4);
    m_GLSupExt.AMD_transform_feedback3_lines_triangles = OPENGL_GET_SUP_EXT(strglext,AMD_transform_feedback3_lines_triangles);
    m_GLSupExt.AMD_vertex_shader_layer = OPENGL_GET_SUP_EXT(strglext,AMD_vertex_shader_layer);
    m_GLSupExt.AMD_vertex_shader_tessellator = OPENGL_GET_SUP_EXT(strglext,AMD_vertex_shader_tessellator);
    m_GLSupExt.AMD_vertex_shader_viewport_index = OPENGL_GET_SUP_EXT(strglext,AMD_vertex_shader_viewport_index);

    m_GLSupExt.ARB_ES2_compatibility = OPENGL_GET_SUP_EXT(strglext,ARB_ES2_compatibility);
    m_GLSupExt.ARB_base_instance = OPENGL_GET_SUP_EXT(strglext,ARB_base_instance);
    m_GLSupExt.ARB_blend_func_extended = OPENGL_GET_SUP_EXT(strglext,ARB_blend_func_extended);
    m_GLSupExt.ARB_clear_buffer_object = OPENGL_GET_SUP_EXT(strglext,ARB_clear_buffer_object);
    m_GLSupExt.ARB_color_buffer_float = OPENGL_GET_SUP_EXT(strglext,ARB_color_buffer_float);
    m_GLSupExt.ARB_compressed_texture_pixel_storage = OPENGL_GET_SUP_EXT(strglext,ARB_compressed_texture_pixel_storage);
    m_GLSupExt.ARB_compute_shader = OPENGL_GET_SUP_EXT(strglext,ARB_compute_shader);
    m_GLSupExt.ARB_conservative_depth = OPENGL_GET_SUP_EXT(strglext,ARB_conservative_depth);
    m_GLSupExt.ARB_copy_buffer = OPENGL_GET_SUP_EXT(strglext,ARB_copy_buffer);
    m_GLSupExt.ARB_copy_image = OPENGL_GET_SUP_EXT(strglext,ARB_copy_image);
    m_GLSupExt.ARB_debug_output = OPENGL_GET_SUP_EXT(strglext,ARB_debug_output);
    m_GLSupExt.ARB_depth_buffer_float = OPENGL_GET_SUP_EXT(strglext,ARB_depth_buffer_float);
    m_GLSupExt.ARB_depth_clamp = OPENGL_GET_SUP_EXT(strglext,ARB_depth_clamp); 
    m_GLSupExt.ARB_depth_texture = OPENGL_GET_SUP_EXT(strglext,ARB_depth_texture);
    m_GLSupExt.ARB_draw_buffers = OPENGL_GET_SUP_EXT(strglext,ARB_draw_buffers); 
    m_GLSupExt.ARB_draw_buffers_blend = OPENGL_GET_SUP_EXT(strglext,ARB_draw_buffers_blend);
    m_GLSupExt.ARB_draw_elements_base_vertex = OPENGL_GET_SUP_EXT(strglext,ARB_draw_elements_base_vertex); 
    m_GLSupExt.ARB_draw_indirect = OPENGL_GET_SUP_EXT(strglext,ARB_draw_indirect);
    m_GLSupExt.ARB_draw_instanced = OPENGL_GET_SUP_EXT(strglext,ARB_draw_instanced); 
    m_GLSupExt.ARB_explicit_attrib_location = OPENGL_GET_SUP_EXT(strglext,ARB_explicit_attrib_location);
    m_GLSupExt.ARB_fragment_coord_conventions = OPENGL_GET_SUP_EXT(strglext,ARB_fragment_coord_conventions);
    m_GLSupExt.ARB_fragment_program = OPENGL_GET_SUP_EXT(strglext,ARB_fragment_program);
    m_GLSupExt.ARB_fragment_program_shadow = OPENGL_GET_SUP_EXT(strglext,ARB_fragment_program_shadow);
    m_GLSupExt.ARB_fragment_shader = OPENGL_GET_SUP_EXT(strglext,ARB_fragment_shader);
    m_GLSupExt.ARB_framebuffer_no_attachments = OPENGL_GET_SUP_EXT(strglext,ARB_framebuffer_no_attachments);
    m_GLSupExt.ARB_framebuffer_object = OPENGL_GET_SUP_EXT(strglext,ARB_framebuffer_object);
    m_GLSupExt.ARB_framebuffer_sRGB = OPENGL_GET_SUP_EXT(strglext,ARB_framebuffer_sRGB);
    m_GLSupExt.ARB_geometry_shader4 = OPENGL_GET_SUP_EXT(strglext,ARB_geometry_shader4);
    m_GLSupExt.ARB_get_program_binary = OPENGL_GET_SUP_EXT(strglext,ARB_get_program_binary);
    m_GLSupExt.ARB_gpu_shader5 = OPENGL_GET_SUP_EXT(strglext,ARB_gpu_shader5);
    m_GLSupExt.ARB_gpu_shader_fp64 = OPENGL_GET_SUP_EXT(strglext,ARB_gpu_shader_fp64);
    m_GLSupExt.ARB_half_float_pixel = OPENGL_GET_SUP_EXT(strglext,ARB_half_float_pixel);
    m_GLSupExt.ARB_half_float_vertex = OPENGL_GET_SUP_EXT(strglext,ARB_half_float_vertex);
    m_GLSupExt.ARB_imaging = OPENGL_GET_SUP_EXT(strglext,ARB_imaging);
    m_GLSupExt.ARB_instanced_arrays = OPENGL_GET_SUP_EXT(strglext,ARB_instanced_arrays);
    m_GLSupExt.ARB_internalformat_query = OPENGL_GET_SUP_EXT(strglext,ARB_internalformat_query);
    m_GLSupExt.ARB_internalformat_query2 = OPENGL_GET_SUP_EXT(strglext,ARB_internalformat_query2);
    m_GLSupExt.ARB_invalidate_subdata = OPENGL_GET_SUP_EXT(strglext,ARB_invalidate_subdata);
    m_GLSupExt.ARB_map_buffer_alignment = OPENGL_GET_SUP_EXT(strglext,ARB_map_buffer_alignment);
    m_GLSupExt.ARB_map_buffer_range = OPENGL_GET_SUP_EXT(strglext,ARB_map_buffer_range);
    m_GLSupExt.ARB_multi_draw_indirect = OPENGL_GET_SUP_EXT(strglext,ARB_multi_draw_indirect);
    m_GLSupExt.ARB_multisample = OPENGL_GET_SUP_EXT(strglext,ARB_multisample); 
    m_GLSupExt.ARB_multitexture = OPENGL_GET_SUP_EXT(strglext,ARB_multitexture);
    m_GLSupExt.ARB_occlusion_query = OPENGL_GET_SUP_EXT(strglext,ARB_occlusion_query); 
    m_GLSupExt.ARB_occlusion_query2 = OPENGL_GET_SUP_EXT(strglext,ARB_occlusion_query2);
    m_GLSupExt.ARB_pixel_buffer_object = OPENGL_GET_SUP_EXT(strglext,ARB_pixel_buffer_object);
    m_GLSupExt.ARB_point_parameters = OPENGL_GET_SUP_EXT(strglext,ARB_point_parameters);
    m_GLSupExt.ARB_point_sprite = OPENGL_GET_SUP_EXT(strglext,ARB_point_sprite);
    m_GLSupExt.ARB_program_interface_query = OPENGL_GET_SUP_EXT(strglext,ARB_program_interface_query);
    m_GLSupExt.ARB_provoking_vertex = OPENGL_GET_SUP_EXT(strglext,ARB_provoking_vertex); 
    m_GLSupExt.ARB_sample_shading = OPENGL_GET_SUP_EXT(strglext,ARB_sample_shading);
    m_GLSupExt.ARB_sampler_objects = OPENGL_GET_SUP_EXT(strglext,ARB_sampler_objects);
    m_GLSupExt.ARB_seamless_cube_map = OPENGL_GET_SUP_EXT(strglext,ARB_seamless_cube_map);
    m_GLSupExt.ARB_separate_shader_objects = OPENGL_GET_SUP_EXT(strglext,ARB_separate_shader_objects); 
    m_GLSupExt.ARB_shader_atomic_counters = OPENGL_GET_SUP_EXT(strglext,ARB_shader_atomic_counters);
    m_GLSupExt.ARB_shader_bit_encoding = OPENGL_GET_SUP_EXT(strglext,ARB_shader_bit_encoding);
    m_GLSupExt.ARB_shader_image_load_store = OPENGL_GET_SUP_EXT(strglext,ARB_shader_image_load_store);
    m_GLSupExt.ARB_shader_objects = OPENGL_GET_SUP_EXT(strglext,ARB_shader_objects);
    m_GLSupExt.ARB_shader_precision = OPENGL_GET_SUP_EXT(strglext,ARB_shader_precision);
    m_GLSupExt.ARB_shader_stencil_export = OPENGL_GET_SUP_EXT(strglext,ARB_shader_stencil_export);
    m_GLSupExt.ARB_shader_storage_buffer_object = OPENGL_GET_SUP_EXT(strglext,ARB_shader_storage_buffer_object);
    m_GLSupExt.ARB_shader_subroutine = OPENGL_GET_SUP_EXT(strglext,ARB_shader_subroutine); 
    m_GLSupExt.ARB_shader_texture_lod = OPENGL_GET_SUP_EXT(strglext,ARB_shader_texture_lod);
    m_GLSupExt.ARB_shading_language_100 = OPENGL_GET_SUP_EXT(strglext,ARB_shading_language_100); 
    m_GLSupExt.ARB_shading_language_420pack = OPENGL_GET_SUP_EXT(strglext,ARB_shading_language_420pack); 
    m_GLSupExt.ARB_shading_language_packing = OPENGL_GET_SUP_EXT(strglext,ARB_shading_language_packing); 
    m_GLSupExt.ARB_shadow = OPENGL_GET_SUP_EXT(strglext,ARB_shadow);
    m_GLSupExt.ARB_shadow_ambient = OPENGL_GET_SUP_EXT(strglext,ARB_shadow_ambient); 
    m_GLSupExt.ARB_sync = OPENGL_GET_SUP_EXT(strglext,ARB_sync);
    m_GLSupExt.ARB_tessellation_shader = OPENGL_GET_SUP_EXT(strglext,ARB_tessellation_shader);
    m_GLSupExt.ARB_texture_border_clamp = OPENGL_GET_SUP_EXT(strglext,ARB_texture_border_clamp);
    m_GLSupExt.ARB_texture_buffer_object = OPENGL_GET_SUP_EXT(strglext,ARB_texture_buffer_object);
    m_GLSupExt.ARB_texture_buffer_object_rgb32 = OPENGL_GET_SUP_EXT(strglext,ARB_texture_buffer_object_rgb32);
    m_GLSupExt.ARB_texture_buffer_range = OPENGL_GET_SUP_EXT(strglext,ARB_texture_buffer_range);
    m_GLSupExt.ARB_texture_compression = OPENGL_GET_SUP_EXT(strglext,ARB_texture_compression);
    m_GLSupExt.ARB_texture_compression_bptc = OPENGL_GET_SUP_EXT(strglext,ARB_texture_compression_bptc);
    m_GLSupExt.ARB_texture_compression_rgtc = OPENGL_GET_SUP_EXT(strglext,ARB_texture_compression_rgtc);
    m_GLSupExt.ARB_texture_cube_map = OPENGL_GET_SUP_EXT(strglext,ARB_texture_cube_map);
    m_GLSupExt.ARB_texture_cube_map_array = OPENGL_GET_SUP_EXT(strglext,ARB_texture_cube_map_array);
    m_GLSupExt.ARB_texture_env_add = OPENGL_GET_SUP_EXT(strglext,ARB_texture_env_add);
    m_GLSupExt.ARB_texture_env_combine = OPENGL_GET_SUP_EXT (strglext,ARB_texture_env_combine);
    m_GLSupExt.ARB_texture_env_crossbar = OPENGL_GET_SUP_EXT (strglext,ARB_texture_env_crossbar);
    m_GLSupExt.ARB_texture_env_dot3 = OPENGL_GET_SUP_EXT(strglext,ARB_texture_env_dot3);
    m_GLSupExt.ARB_texture_float = OPENGL_GET_SUP_EXT(strglext,ARB_texture_float);
    m_GLSupExt.ARB_texture_gather = OPENGL_GET_SUP_EXT(strglext,ARB_texture_gather);
    m_GLSupExt.ARB_texture_mirrored_repeat = OPENGL_GET_SUP_EXT(strglext,ARB_texture_mirrored_repeat);
    m_GLSupExt.ARB_texture_multisample = OPENGL_GET_SUP_EXT(strglext,ARB_texture_multisample);
    m_GLSupExt.ARB_texture_non_power_of_two = OPENGL_GET_SUP_EXT (strglext,ARB_texture_non_power_of_two);
    m_GLSupExt.ARB_texture_query_lod = OPENGL_GET_SUP_EXT(strglext,ARB_texture_query_lod);
    m_GLSupExt.ARB_texture_rectangle = OPENGL_GET_SUP_EXT(strglext,ARB_texture_rectangle);
    m_GLSupExt.ARB_texture_rg = OPENGL_GET_SUP_EXT(strglext,ARB_texture_rg);
    m_GLSupExt.ARB_texture_rgb10_a2ui = OPENGL_GET_SUP_EXT(strglext,ARB_texture_rgb10_a2ui);
    m_GLSupExt.ARB_texture_snorm = OPENGL_GET_SUP_EXT(strglext,ARB_texture_snorm);
    m_GLSupExt.ARB_texture_storage = OPENGL_GET_SUP_EXT(strglext,ARB_texture_storage);
    m_GLSupExt.ARB_texture_storage_multisample = OPENGL_GET_SUP_EXT(strglext,ARB_texture_storage_multisample);
    m_GLSupExt.ARB_texture_view = OPENGL_GET_SUP_EXT(strglext,ARB_texture_view);
    m_GLSupExt.ARB_timer_query = OPENGL_GET_SUP_EXT(strglext,ARB_timer_query);
    m_GLSupExt.ARB_transform_feedback2 = OPENGL_GET_SUP_EXT(strglext,ARB_transform_feedback2);
    m_GLSupExt.ARB_transform_feedback3 = OPENGL_GET_SUP_EXT(strglext,ARB_transform_feedback3);
    m_GLSupExt.ARB_transform_feedback_instanced = OPENGL_GET_SUP_EXT(strglext,ARB_transform_feedback_instanced);
    m_GLSupExt.ARB_transpose_matrix = OPENGL_GET_SUP_EXT(strglext,ARB_transpose_matrix);
    m_GLSupExt.ARB_uniform_buffer_object = OPENGL_GET_SUP_EXT(strglext,ARB_uniform_buffer_object);
    m_GLSupExt.ARB_vertex_array_bgra = OPENGL_GET_SUP_EXT(strglext,ARB_vertex_array_bgra);
    m_GLSupExt.ARB_vertex_array_object = OPENGL_GET_SUP_EXT(strglext,ARB_vertex_array_object);
    m_GLSupExt.ARB_vertex_attrib_64bit = OPENGL_GET_SUP_EXT(strglext,ARB_vertex_attrib_binding);
    m_GLSupExt.ARB_vertex_attrib_binding = OPENGL_GET_SUP_EXT(strglext,OPENGL_GET_SUP_EXT)
    m_GLSupExt.ARB_vertex_buffer_object = OPENGL_GET_SUP_EXT(strglext,ARB_vertex_buffer_object);
    m_GLSupExt.ARB_vertex_program = OPENGL_GET_SUP_EXT(strglext,ARB_vertex_program);
    m_GLSupExt.ARB_vertex_shader = OPENGL_GET_SUP_EXT(strglext,ARB_vertex_shader);
    m_GLSupExt.ARB_vertex_type_2_10_10_10_rev = OPENGL_GET_SUP_EXT(strglext,ARB_vertex_type_2_10_10_10_rev);
    m_GLSupExt.ARB_viewport_array = OPENGL_GET_SUP_EXT(strglext,ARB_viewport_array);
    m_GLSupExt.ARB_window_pos = OPENGL_GET_SUP_EXT(strglext,ARB_window_pos);


    m_GLSupExt.ATI_draw_buffers = OPENGL_GET_SUP_EXT(strglext,ATI_draw_buffers);
    m_GLSupExt.ATI_envmap_bumpmap = OPENGL_GET_SUP_EXT(strglext,ATI_envmap_bumpmap);
    m_GLSupExt.ATI_fragment_shader = OPENGL_GET_SUP_EXT(strglext,ATI_fragment_shader);
    m_GLSupExt.ATI_meminfo = OPENGL_GET_SUP_EXT(strglext,ATI_meminfo);
    m_GLSupExt.ATI_separate_stencil = OPENGL_GET_SUP_EXT(strglext,ATI_separate_stencil); 
    m_GLSupExt.ATI_texture_compression_3dc = OPENGL_GET_SUP_EXT(strglext,ATI_texture_compression_3dc); 
    m_GLSupExt.ATI_texture_env_combine3 = OPENGL_GET_SUP_EXT(strglext,ATI_texture_env_combine3);
    m_GLSupExt.ATI_texture_float = OPENGL_GET_SUP_EXT(strglext,ATI_texture_float);
    m_GLSupExt.ATI_texture_mirror_once = OPENGL_GET_SUP_EXT(strglext,ATI_texture_mirror_once);


    m_GLSupExt.EXT_abgr = OPENGL_GET_SUP_EXT(strglext,EXT_abgr);
    m_GLSupExt.EXT_bgra = OPENGL_GET_SUP_EXT(strglext,EXT_bgra);
    m_GLSupExt.EXT_bindable_uniform = OPENGL_GET_SUP_EXT(strglext,EXT_bindable_uniform);
    m_GLSupExt.EXT_blend_color = OPENGL_GET_SUP_EXT(strglext,EXT_blend_color);
    m_GLSupExt.EXT_blend_equation_separate = OPENGL_GET_SUP_EXT(strglext,EXT_blend_equation_separate);
    m_GLSupExt.EXT_blend_func_separate = OPENGL_GET_SUP_EXT(strglext,EXT_blend_func_separate);
    m_GLSupExt.EXT_blend_minmax = OPENGL_GET_SUP_EXT(strglext,EXT_blend_minmax); 
    m_GLSupExt.EXT_blend_subtract = OPENGL_GET_SUP_EXT(strglext,EXT_blend_subtract);
    m_GLSupExt.EXT_compiled_vertex_array = OPENGL_GET_SUP_EXT(strglext,EXT_compiled_vertex_array);
    m_GLSupExt.EXT_copy_buffer = OPENGL_GET_SUP_EXT(strglext,EXT_copy_buffer);
    m_GLSupExt.EXT_copy_texture = OPENGL_GET_SUP_EXT(strglext,EXT_copy_texture);
    m_GLSupExt.EXT_direct_state_access = OPENGL_GET_SUP_EXT(strglext,EXT_direct_state_access);
    m_GLSupExt.EXT_draw_buffers2 = OPENGL_GET_SUP_EXT(strglext,EXT_draw_buffers2); 
    m_GLSupExt.EXT_draw_instanced = OPENGL_GET_SUP_EXT(strglext,EXT_draw_instanced); 
    m_GLSupExt.EXT_draw_range_elements = OPENGL_GET_SUP_EXT(strglext,EXT_draw_range_elements);
    m_GLSupExt.EXT_fog_coord = OPENGL_GET_SUP_EXT(strglext,EXT_fog_coord); 
    m_GLSupExt.EXT_framebuffer_blit = OPENGL_GET_SUP_EXT(strglext,EXT_framebuffer_blit); 
    m_GLSupExt.EXT_framebuffer_multisample = OPENGL_GET_SUP_EXT(strglext,EXT_framebuffer_multisample); 
    m_GLSupExt.EXT_framebuffer_object = OPENGL_GET_SUP_EXT(strglext,EXT_framebuffer_object);
    m_GLSupExt.EXT_framebuffer_sRGB = OPENGL_GET_SUP_EXT(strglext,EXT_framebuffer_sRGB);
    m_GLSupExt.EXT_geometry_shader4 = OPENGL_GET_SUP_EXT(strglext,EXT_geometry_shader4); 
    m_GLSupExt.EXT_gpu_program_parameters = OPENGL_GET_SUP_EXT(strglext,EXT_gpu_program_parameters); 
    m_GLSupExt.EXT_gpu_shader4 = OPENGL_GET_SUP_EXT(strglext,EXT_gpu_shader4); 
    m_GLSupExt.EXT_histogram = OPENGL_GET_SUP_EXT(strglext,EXT_histogram);
    m_GLSupExt.EXT_multi_draw_arrays = OPENGL_GET_SUP_EXT(strglext,EXT_multi_draw_arrays); 
    m_GLSupExt.EXT_packed_depth_stencil = OPENGL_GET_SUP_EXT(strglext,EXT_packed_depth_stencil); 
    m_GLSupExt.EXT_packed_float = OPENGL_GET_SUP_EXT(strglext,EXT_packed_float);
    m_GLSupExt.EXT_packed_pixels = OPENGL_GET_SUP_EXT(strglext,EXT_packed_pixels);
    m_GLSupExt.EXT_pixel_buffer_object = OPENGL_GET_SUP_EXT(strglext,EXT_pixel_buffer_object); 
    m_GLSupExt.EXT_point_parameters = OPENGL_GET_SUP_EXT(strglext,EXT_point_parameters); 
    m_GLSupExt.EXT_provoking_vertex = OPENGL_GET_SUP_EXT(strglext,EXT_provoking_vertex); 
    m_GLSupExt.EXT_rescale_normal = OPENGL_GET_SUP_EXT(strglext,EXT_rescale_normal); 
    m_GLSupExt.EXT_secondary_color = OPENGL_GET_SUP_EXT(strglext,EXT_secondary_color); 
    m_GLSupExt.EXT_separate_specular_color = OPENGL_GET_SUP_EXT(strglext,EXT_separate_specular_color); 
    m_GLSupExt.EXT_shader_image_load_store = OPENGL_GET_SUP_EXT(strglext,EXT_shader_image_load_store); 
    m_GLSupExt.EXT_shadow_funcs = OPENGL_GET_SUP_EXT(strglext,EXT_shadow_funcs); 
    m_GLSupExt.EXT_stencil_wrap = OPENGL_GET_SUP_EXT(strglext,EXT_stencil_wrap); 
    m_GLSupExt.EXT_subtexture = OPENGL_GET_SUP_EXT(strglext,EXT_subtexture); 
    m_GLSupExt.EXT_texgen_reflection = OPENGL_GET_SUP_EXT(strglext,EXT_texgen_reflection);
    m_GLSupExt.EXT_texture3D = OPENGL_GET_SUP_EXT(strglext,EXT_texture3D); 
    m_GLSupExt.EXT_texture_array = OPENGL_GET_SUP_EXT(strglext,EXT_texture_array); 
    m_GLSupExt.EXT_texture_buffer_object = OPENGL_GET_SUP_EXT(strglext,EXT_texture_buffer_object);
    m_GLSupExt.EXT_texture_compression_bptc = OPENGL_GET_SUP_EXT(strglext,EXT_texture_compression_bptc); 
    m_GLSupExt.EXT_texture_compression_latc = OPENGL_GET_SUP_EXT(strglext,EXT_texture_compression_latc); 
    m_GLSupExt.EXT_texture_compression_rgtc = OPENGL_GET_SUP_EXT(strglext,EXT_texture_compression_rgtc); 
    m_GLSupExt.EXT_texture_compression_s3tc = OPENGL_GET_SUP_EXT(strglext,EXT_texture_compression_s3tc);
    m_GLSupExt.EXT_texture_cube_map = OPENGL_GET_SUP_EXT(strglext,EXT_texture_cube_map); 
    m_GLSupExt.EXT_texture_edge_clamp = OPENGL_GET_SUP_EXT(strglext,EXT_texture_edge_clamp); 
    m_GLSupExt.EXT_texture_env_add = OPENGL_GET_SUP_EXT(strglext,EXT_texture_env_add); 
    m_GLSupExt.EXT_texture_env_combine = OPENGL_GET_SUP_EXT(strglext,EXT_texture_env_combine); 
    m_GLSupExt.EXT_texture_env_dot3 = OPENGL_GET_SUP_EXT(strglext,EXT_texture_env_dot3); 
    m_GLSupExt.EXT_texture_filter_anisotropic = OPENGL_GET_SUP_EXT(strglext,EXT_texture_filter_anisotropic); 
    m_GLSupExt.EXT_texture_integer = OPENGL_GET_SUP_EXT(strglext,EXT_texture_integer);
    m_GLSupExt.EXT_texture_lod = OPENGL_GET_SUP_EXT(strglext,EXT_texture_lod); 
    m_GLSupExt.EXT_texture_lod_bias = OPENGL_GET_SUP_EXT(strglext,EXT_texture_lod_bias);
    m_GLSupExt.EXT_texture_mirror_clamp = OPENGL_GET_SUP_EXT(strglext,EXT_texture_mirror_clamp); 
    m_GLSupExt.EXT_texture_object = OPENGL_GET_SUP_EXT(strglext,EXT_texture_object);
    m_GLSupExt.EXT_texture_rectangle = OPENGL_GET_SUP_EXT(strglext,EXT_texture_rectangle);
    m_GLSupExt.EXT_texture_sRGB = OPENGL_GET_SUP_EXT(strglext,EXT_texture_sRGB); 
    m_GLSupExt.EXT_texture_sRGB_decode = OPENGL_GET_SUP_EXT(strglext,EXT_texture_sRGB_decode); 
    m_GLSupExt.EXT_texture_shared_exponent = OPENGL_GET_SUP_EXT(strglext,EXT_texture_shared_exponent); 
    m_GLSupExt.EXT_texture_snorm = OPENGL_GET_SUP_EXT(strglext,EXT_texture_snorm);
    m_GLSupExt.EXT_texture_storage = OPENGL_GET_SUP_EXT(strglext,EXT_texture_storage); 
    m_GLSupExt.EXT_texture_swizzle = OPENGL_GET_SUP_EXT(strglext,EXT_texture_swizzle); 
    m_GLSupExt.EXT_timer_query = OPENGL_GET_SUP_EXT(strglext,EXT_timer_query); 
    m_GLSupExt.EXT_transform_feedback = OPENGL_GET_SUP_EXT(strglext,EXT_transform_feedback); 
    m_GLSupExt.EXT_vertex_array = OPENGL_GET_SUP_EXT(strglext,EXT_vertex_array); 
    m_GLSupExt.EXT_vertex_array_bgra = OPENGL_GET_SUP_EXT(strglext,EXT_vertex_array_bgra); 
    m_GLSupExt.EXT_vertex_attrib_64bit = OPENGL_GET_SUP_EXT(strglext,EXT_vertex_attrib_64bit); 


    m_GLSupExt.IBM_texture_mirrored_repeat = OPENGL_GET_SUP_EXT(strglext,IBM_texture_mirrored_repeat);

    m_GLSupExt.KHR_debug = OPENGL_GET_SUP_EXT(strglext,KHR_debug);

    m_GLSupExt.KTX_buffer_region = OPENGL_GET_SUP_EXT(strglext,KTX_buffer_region); 

    m_GLSupExt.NV_blend_square = OPENGL_GET_SUP_EXT(strglext,NV_blend_square); 
    m_GLSupExt.NV_conditional_render = OPENGL_GET_SUP_EXT(strglext,NV_conditional_render); 
    m_GLSupExt.NV_copy_depth_to_color = OPENGL_GET_SUP_EXT(strglext,NV_copy_depth_to_color); 
    m_GLSupExt.NV_copy_image = OPENGL_GET_SUP_EXT(strglext,NV_copy_image);
    m_GLSupExt.NV_explicit_multisample = OPENGL_GET_SUP_EXT(strglext,NV_explicit_multisample);
    m_GLSupExt.NV_float_buffer = OPENGL_GET_SUP_EXT(strglext,NV_float_buffer); 
    m_GLSupExt.NV_half_float = OPENGL_GET_SUP_EXT(strglext,NV_half_float); 
    m_GLSupExt.NV_primitive_restart = OPENGL_GET_SUP_EXT(strglext,NV_primitive_restart);
    m_GLSupExt.NV_texgen_reflection = OPENGL_GET_SUP_EXT(strglext,NV_texgen_reflection);
    m_GLSupExt.NV_texture_barrier = OPENGL_GET_SUP_EXT(strglext,NV_texture_barrier); 

    m_GLSupExt.SGIS_generate_mipmap = OPENGL_GET_SUP_EXT(strglext,SGIS_generate_mipmap);
    m_GLSupExt.SGIS_texture_edge_clamp = OPENGL_GET_SUP_EXT(strglext,SGIS_texture_edge_clamp); 
    m_GLSupExt.SGIS_texture_lod = OPENGL_GET_SUP_EXT(strglext,SGIS_texture_lod); 

    m_GLSupExt.SUN_multi_draw_arrays = OPENGL_GET_SUP_EXT(strglext,SUN_multi_draw_arrays);
}


#include "GLVerProc.cpp"
#include "GLArbProc.cpp"
#include "GLExtProc.cpp"
#include "GLVerProcAddr.cpp"
#include "GLArbProcAddr.cpp"
#include "GLExtProcAddr.cpp"


/////////////////////////////////////////////////////////////////////////////////////////////
void CGLDevInfo::SetupGLExtProc()
{
    CGLDevInfo::GetVerProcAddr();
    CGLDevInfo::GetArbProcAddr();
}

/////////////////////////////////////////////////////////////////////////////////////////////