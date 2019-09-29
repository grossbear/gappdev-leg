/////////////////////////////////////////////////////////////////////////////////////////////
// WGLSupExtStruct.h
//
// WGL Supported Extensions Structure
/////////////////////////////////////////////////////////////////////////////////////////////

#ifndef __WGLSUPEXTSTRUCT_H__
#define __WGLSUPEXTSTRUCT_H__


/////////////////////////////////////////////////////////////////////////////////////////////
 
struct WGLSupExtStruct
{
    uint32t ARB_extensions_string : 1;
    uint32t ARB_pixel_format : 1;
    uint32t ATI_pixel_format_float : 1;
    uint32t ARB_pixel_format_float : 1;
    uint32t ARB_multisample : 1;
    uint32t EXT_swap_control : 1;
    uint32t EXT_swap_control_tear : 1;
    uint32t ARB_pbuffer : 1;
    uint32t ARB_render_texture : 1;
    uint32t ARB_make_current_read : 1;
    uint32t EXT_extensions_string : 1;
    uint32t ARB_buffer_region : 1;
    uint32t EXT_framebuffer_sRGB : 1;
    uint32t ATI_render_texture_rectangle : 1;
    uint32t EXT_pixel_format_packed_float : 1;
    uint32t I3D_genlock : 1;
    uint32t NV_swap_group : 1;
    uint32t ARB_create_context : 1;
    uint32t AMD_gpu_association : 1;
    uint32t AMDX_gpu_association : 1;
    uint32t ARB_create_context_profile : 1;
    uint32t NV_float_buffer : 1;

    WGLSupExtStruct():
        ARB_extensions_string(0),
        ARB_pixel_format(0),
        ATI_pixel_format_float(0),
        ARB_pixel_format_float(0),
        ARB_multisample(0),
        EXT_swap_control(0),
        EXT_swap_control_tear(0),
        ARB_pbuffer(0),
        ARB_render_texture(0),
        ARB_make_current_read(0),
        EXT_extensions_string(0),
        ARB_buffer_region(0),
        EXT_framebuffer_sRGB(0),
        ATI_render_texture_rectangle(0),
        EXT_pixel_format_packed_float(0),
        I3D_genlock(0),
        NV_swap_group(0),
        ARB_create_context(0),
        AMD_gpu_association(0),
        AMDX_gpu_association(0),
        ARB_create_context_profile(0),
        NV_float_buffer(0)
    {}
};







/////////////////////////////////////////////////////////////////////////////////////////////


#endif //__WGLSUPEXTSTRUCT_H__