/////////////////////////////////////////////////////////////////////////////////////////////
// GLArbProcAddr.cpp
//
// Getting OpenGL ARB Procedure Address
/////////////////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////////////////
void CGLDevInfo::GetArbProcAddr()
{
    // GL_ARB_framebuffer_object
    if(m_GLSupExt.ARB_framebuffer_object)
    {
        OPENGL_GET_PROC(PFNGLISRENDERBUFFERPROC, glIsRenderbuffer);
        OPENGL_GET_PROC(PFNGLBINDRENDERBUFFERPROC, glBindRenderbuffer);
        OPENGL_GET_PROC(PFNGLDELETERENDERBUFFERSPROC, glDeleteRenderbuffers);
        OPENGL_GET_PROC(PFNGLGENRENDERBUFFERSPROC, glGenRenderbuffers);
        OPENGL_GET_PROC(PFNGLRENDERBUFFERSTORAGEPROC, glRenderbufferStorage);
        OPENGL_GET_PROC(PFNGLGETRENDERBUFFERPARAMETERIVPROC, glGetRenderbufferParameteriv);
        OPENGL_GET_PROC(PFNGLISFRAMEBUFFERPROC, glIsFramebuffer);
        OPENGL_GET_PROC(PFNGLBINDFRAMEBUFFERPROC, glBindFramebuffer);
        OPENGL_GET_PROC(PFNGLDELETEFRAMEBUFFERSPROC, glDeleteFramebuffers);
        OPENGL_GET_PROC(PFNGLGENFRAMEBUFFERSPROC, glGenFramebuffers);
        OPENGL_GET_PROC(PFNGLCHECKFRAMEBUFFERSTATUSPROC, glCheckFramebufferStatus);
        OPENGL_GET_PROC(PFNGLFRAMEBUFFERTEXTURE1DPROC, glFramebufferTexture1D);
        OPENGL_GET_PROC(PFNGLFRAMEBUFFERTEXTURE2DPROC, glFramebufferTexture2D);
        OPENGL_GET_PROC(PFNGLFRAMEBUFFERTEXTURE3DPROC, glFramebufferTexture3D);
        OPENGL_GET_PROC(PFNGLFRAMEBUFFERRENDERBUFFERPROC, glFramebufferRenderbuffer);
        OPENGL_GET_PROC(PFNGLGETFRAMEBUFFERATTACHMENTPARAMETERIVPROC, glGetFramebufferAttachmentParameteriv);
        OPENGL_GET_PROC(PFNGLGENERATEMIPMAPPROC, glGenerateMipmap);
        OPENGL_GET_PROC(PFNGLBLITFRAMEBUFFERPROC, glBlitFramebuffer);
        OPENGL_GET_PROC(PFNGLRENDERBUFFERSTORAGEMULTISAMPLEPROC, glRenderbufferStorageMultisample);
        OPENGL_GET_PROC(PFNGLFRAMEBUFFERTEXTURELAYERPROC, glFramebufferTextureLayer);
    }

    // GL_ARB_uniform_buffer_object 
    if(m_GLSupExt.ARB_uniform_buffer_object)
    {
        OPENGL_GET_PROC(PFNGLGETUNIFORMINDICESPROC, glGetUniformIndices);
        OPENGL_GET_PROC(PFNGLGETACTIVEUNIFORMSIVPROC, glGetActiveUniformsiv);
        OPENGL_GET_PROC(PFNGLGETACTIVEUNIFORMNAMEPROC, glGetActiveUniformName);
        OPENGL_GET_PROC(PFNGLGETUNIFORMBLOCKINDEXPROC, glGetUniformBlockIndex);
        OPENGL_GET_PROC(PFNGLGETACTIVEUNIFORMBLOCKIVPROC, glGetActiveUniformBlockiv);
        OPENGL_GET_PROC(PFNGLGETACTIVEUNIFORMBLOCKNAMEPROC, glGetActiveUniformBlockName);
        OPENGL_GET_PROC(PFNGLUNIFORMBLOCKBINDINGPROC, glUniformBlockBinding);
    }

    // GL_ARB_uniform_buffer_object 
    if(m_GLSupExt.ARB_uniform_buffer_object)
    {
        OPENGL_GET_PROC(PFNGLGETUNIFORMINDICESPROC, glGetUniformIndices);
        OPENGL_GET_PROC(PFNGLGETACTIVEUNIFORMSIVPROC, glGetActiveUniformsiv);
        OPENGL_GET_PROC(PFNGLGETACTIVEUNIFORMNAMEPROC, glGetActiveUniformName);
        OPENGL_GET_PROC(PFNGLGETUNIFORMBLOCKINDEXPROC, glGetUniformBlockIndex);
        OPENGL_GET_PROC(PFNGLGETACTIVEUNIFORMBLOCKIVPROC, glGetActiveUniformBlockiv);
        OPENGL_GET_PROC(PFNGLGETACTIVEUNIFORMBLOCKNAMEPROC, glGetActiveUniformBlockName);
        OPENGL_GET_PROC(PFNGLUNIFORMBLOCKBINDINGPROC, glUniformBlockBinding);
    }

    // GL_ARB_draw_elements_base_vertex 
    if(m_GLSupExt.ARB_draw_elements_base_vertex)
    {
        OPENGL_GET_PROC(PFNGLDRAWELEMENTSBASEVERTEXPROC, glDrawElementsBaseVertex);
        OPENGL_GET_PROC(PFNGLDRAWRANGEELEMENTSBASEVERTEXPROC, glDrawRangeElementsBaseVertex);
        OPENGL_GET_PROC(PFNGLDRAWELEMENTSINSTANCEDBASEVERTEXPROC, glDrawElementsInstancedBaseVertex);
        OPENGL_GET_PROC(PFNGLMULTIDRAWELEMENTSBASEVERTEXPROC, glMultiDrawElementsBaseVertex);
    }

    // GL_ARB_provoking_vertex
    if(m_GLSupExt.ARB_provoking_vertex)
    {
        OPENGL_GET_PROC(PFNGLPROVOKINGVERTEXPROC, glProvokingVertex);
    }

    // GL_ARB_sync 
    if(m_GLSupExt.ARB_sync)
    {
        OPENGL_GET_PROC(PFNGLFENCESYNCPROC, glFenceSync);
        OPENGL_GET_PROC(PFNGLISSYNCPROC, glIsSync);
        OPENGL_GET_PROC(PFNGLDELETESYNCPROC, glDeleteSync);
        OPENGL_GET_PROC(PFNGLCLIENTWAITSYNCPROC, glClientWaitSync);
        OPENGL_GET_PROC(PFNGLWAITSYNCPROC, glWaitSync);
        OPENGL_GET_PROC(PFNGLGETINTEGER64VPROC, glGetInteger64v);
        OPENGL_GET_PROC(PFNGLGETSYNCIVPROC, glGetSynciv);
    }

    // GL_ARB_texture_multisample
    if(m_GLSupExt.ARB_texture_multisample)
    {
        OPENGL_GET_PROC(PFNGLTEXIMAGE2DMULTISAMPLEPROC, glTexImage2DMultisample);
        OPENGL_GET_PROC(PFNGLTEXIMAGE3DMULTISAMPLEPROC, glTexImage3DMultisample);
        OPENGL_GET_PROC(PFNGLGETMULTISAMPLEFVPROC, glGetMultisamplefv);
        OPENGL_GET_PROC(PFNGLSAMPLEMASKIPROC, glSampleMaski);
    }

    // GL_ARB_blend_func_extended
    if(m_GLSupExt.ARB_blend_func_extended)
    {
        OPENGL_GET_PROC(PFNGLBINDFRAGDATALOCATIONINDEXEDPROC, glBindFragDataLocationIndexed);
        OPENGL_GET_PROC(PFNGLGETFRAGDATAINDEXPROC, glGetFragDataIndex);
    }

    // GL_ARB_sampler_objects 
    if(m_GLSupExt.ARB_sampler_objects)
    {
        OPENGL_GET_PROC(PFNGLGENSAMPLERSPROC, glGenSamplers);
        OPENGL_GET_PROC(PFNGLDELETESAMPLERSPROC, glDeleteSamplers);
        OPENGL_GET_PROC(PFNGLISSAMPLERPROC, glIsSampler);
        OPENGL_GET_PROC(PFNGLBINDSAMPLERPROC, glBindSampler);
        OPENGL_GET_PROC(PFNGLSAMPLERPARAMETERIPROC, glSamplerParameteri);
        OPENGL_GET_PROC(PFNGLSAMPLERPARAMETERIVPROC, glSamplerParameteriv);
        OPENGL_GET_PROC(PFNGLSAMPLERPARAMETERFPROC, glSamplerParameterf);
        OPENGL_GET_PROC(PFNGLSAMPLERPARAMETERFVPROC, glSamplerParameterfv);
        OPENGL_GET_PROC(PFNGLSAMPLERPARAMETERIIVPROC, glSamplerParameterIiv);
        OPENGL_GET_PROC(PFNGLSAMPLERPARAMETERIUIVPROC, glSamplerParameterIuiv);
        OPENGL_GET_PROC(PFNGLGETSAMPLERPARAMETERIVPROC, glGetSamplerParameteriv);
        OPENGL_GET_PROC(PFNGLGETSAMPLERPARAMETERIIVPROC, glGetSamplerParameterIiv);
        OPENGL_GET_PROC(PFNGLGETSAMPLERPARAMETERFVPROC, glGetSamplerParameterfv);
        OPENGL_GET_PROC(PFNGLGETSAMPLERPARAMETERIUIVPROC, glGetSamplerParameterIuiv);
    }

    // GL_ARB_timer_query
    if(m_GLSupExt.ARB_timer_query)
    {
        OPENGL_GET_PROC(PFNGLQUERYCOUNTERPROC, glQueryCounter);
        OPENGL_GET_PROC(PFNGLGETQUERYOBJECTI64VPROC, glGetQueryObjecti64v);
        OPENGL_GET_PROC(PFNGLGETQUERYOBJECTUI64VPROC, glGetQueryObjectui64v);
    }

    // GL_ARB_vertex_type_2_10_10_10_rev 

    // GL_ARB_draw_indirect
    if(m_GLSupExt.ARB_draw_indirect)
    {
        OPENGL_GET_PROC(PFNGLDRAWARRAYSINDIRECTPROC, glDrawArraysIndirect);
        OPENGL_GET_PROC(PFNGLDRAWELEMENTSINDIRECTPROC, glDrawElementsIndirect);
    }

    // GL_ARB_gpu_shader5
    // GL_ARB_gpu_shader_fp64
    if(m_GLSupExt.ARB_gpu_shader_fp64)
    {
        OPENGL_GET_PROC(PFNGLUNIFORM1DPROC, glUniform1d);
        OPENGL_GET_PROC(PFNGLUNIFORM2DPROC, glUniform2d);
        OPENGL_GET_PROC(PFNGLUNIFORM3DPROC, glUniform3d);
        OPENGL_GET_PROC(PFNGLUNIFORM4DPROC, glUniform4d);
        OPENGL_GET_PROC(PFNGLUNIFORM1DVPROC, glUniform1dv);
        OPENGL_GET_PROC(PFNGLUNIFORM2DVPROC, glUniform2dv);
        OPENGL_GET_PROC(PFNGLUNIFORM3DVPROC, glUniform3dv);
        OPENGL_GET_PROC(PFNGLUNIFORM4DVPROC, glUniform4dv);
        OPENGL_GET_PROC(PFNGLUNIFORMMATRIX2DVPROC, glUniformMatrix2dv);
        OPENGL_GET_PROC(PFNGLUNIFORMMATRIX3DVPROC, glUniformMatrix3dv);
        OPENGL_GET_PROC(PFNGLUNIFORMMATRIX4DVPROC, glUniformMatrix4dv);
        OPENGL_GET_PROC(PFNGLUNIFORMMATRIX2X3DVPROC, glUniformMatrix2x3dv);
        OPENGL_GET_PROC(PFNGLUNIFORMMATRIX2X4DVPROC, glUniformMatrix2x4dv);
        OPENGL_GET_PROC(PFNGLUNIFORMMATRIX3X2DVPROC, glUniformMatrix3x2dv);
        OPENGL_GET_PROC(PFNGLUNIFORMMATRIX3X4DVPROC, glUniformMatrix3x4dv);
        OPENGL_GET_PROC(PFNGLUNIFORMMATRIX4X2DVPROC, glUniformMatrix4x2dv);
        OPENGL_GET_PROC(PFNGLUNIFORMMATRIX4X3DVPROC, glUniformMatrix4x3dv);
        OPENGL_GET_PROC(PFNGLGETUNIFORMDVPROC, glGetUniformdv);
    }


    // GL_ARB_shader_subroutine 
    if(m_GLSupExt.ARB_shader_subroutine)
    {
        OPENGL_GET_PROC(PFNGLGETSUBROUTINEUNIFORMLOCATIONPROC, glGetSubroutineUniformLocation);
        OPENGL_GET_PROC(PFNGLGETSUBROUTINEINDEXPROC, glGetSubroutineIndex);
        OPENGL_GET_PROC(PFNGLGETACTIVESUBROUTINEUNIFORMIVPROC, glGetActiveSubroutineUniformiv);
        OPENGL_GET_PROC(PFNGLGETACTIVESUBROUTINEUNIFORMNAMEPROC, glGetActiveSubroutineUniformName);
        OPENGL_GET_PROC(PFNGLGETACTIVESUBROUTINENAMEPROC, glGetActiveSubroutineName);
        OPENGL_GET_PROC(PFNGLUNIFORMSUBROUTINESUIVPROC, glUniformSubroutinesuiv);
        OPENGL_GET_PROC(PFNGLGETUNIFORMSUBROUTINEUIVPROC, glGetUniformSubroutineuiv);
        OPENGL_GET_PROC(PFNGLGETPROGRAMSTAGEIVPROC, glGetProgramStageiv);
    }

    // GL_ARB_tessellation_shader
    if(m_GLSupExt.ARB_tessellation_shader)
    {
        OPENGL_GET_PROC(PFNGLPATCHPARAMETERIPROC, glPatchParameteri);
        OPENGL_GET_PROC(PFNGLPATCHPARAMETERFVPROC, glPatchParameterfv);
    }

    // GL_ARB_transform_feedback2
    if(m_GLSupExt.ARB_transform_feedback2)
    {
        OPENGL_GET_PROC(PFNGLBINDTRANSFORMFEEDBACKPROC, glBindTransformFeedback);
        OPENGL_GET_PROC(PFNGLDELETETRANSFORMFEEDBACKSPROC, glDeleteTransformFeedbacks);
        OPENGL_GET_PROC(PFNGLGENTRANSFORMFEEDBACKSPROC, glGenTransformFeedbacks);
        OPENGL_GET_PROC(PFNGLISTRANSFORMFEEDBACKPROC, glIsTransformFeedback);
        OPENGL_GET_PROC(PFNGLPAUSETRANSFORMFEEDBACKPROC, glPauseTransformFeedback);
        OPENGL_GET_PROC(PFNGLRESUMETRANSFORMFEEDBACKPROC, glResumeTransformFeedback);
        OPENGL_GET_PROC(PFNGLDRAWTRANSFORMFEEDBACKPROC, glDrawTransformFeedback);
    }

    // GL_ARB_transform_feedback3 
    if(m_GLSupExt.ARB_transform_feedback3)
    {
        OPENGL_GET_PROC(PFNGLDRAWTRANSFORMFEEDBACKSTREAMPROC, glDrawTransformFeedbackStream);
        OPENGL_GET_PROC(PFNGLBEGINQUERYINDEXEDPROC, glBeginQueryIndexed);
        OPENGL_GET_PROC(PFNGLENDQUERYINDEXEDPROC, glEndQueryIndexed);
        OPENGL_GET_PROC(PFNGLGETQUERYINDEXEDIVPROC, glGetQueryIndexediv);
    }

    // GL_ARB_ES2_compatibility
    if(m_GLSupExt.ARB_ES2_compatibility)
    {
        OPENGL_GET_PROC(PFNGLRELEASESHADERCOMPILERPROC, glReleaseShaderCompiler);
        OPENGL_GET_PROC(PFNGLSHADERBINARYPROC, glShaderBinary);
        OPENGL_GET_PROC(PFNGLGETSHADERPRECISIONFORMATPROC, glGetShaderPrecisionFormat);
        OPENGL_GET_PROC(PFNGLDEPTHRANGEFPROC, glDepthRangef);
        OPENGL_GET_PROC(PFNGLCLEARDEPTHFPROC, glClearDepthf);
    }

    // GL_ARB_get_program_binary 
    if(m_GLSupExt.ARB_get_program_binary)
    {
        OPENGL_GET_PROC(PFNGLGETPROGRAMBINARYPROC, glGetProgramBinary);
        OPENGL_GET_PROC(PFNGLPROGRAMBINARYPROC, glProgramBinary);
        OPENGL_GET_PROC(PFNGLPROGRAMPARAMETERIPROC, glProgramParameteri);
    }

    // GL_ARB_separate_shader_objects 
    if(m_GLSupExt.ARB_separate_shader_objects)
    {
        OPENGL_GET_PROC(PFNGLUSEPROGRAMSTAGESPROC, glUseProgramStages);
        OPENGL_GET_PROC(PFNGLACTIVESHADERPROGRAMPROC, glActiveShaderProgram);
        OPENGL_GET_PROC(PFNGLCREATESHADERPROGRAMVPROC, glCreateShaderProgramv);
        OPENGL_GET_PROC(PFNGLBINDPROGRAMPIPELINEPROC, glBindProgramPipeline);
        OPENGL_GET_PROC(PFNGLDELETEPROGRAMPIPELINESPROC, glDeleteProgramPipelines);
        OPENGL_GET_PROC(PFNGLGENPROGRAMPIPELINESPROC, glGenProgramPipelines);
        OPENGL_GET_PROC(PFNGLISPROGRAMPIPELINEPROC, glIsProgramPipeline);

        OPENGL_GET_PROC(PFNGLGETPROGRAMPIPELINEIVPROC, glGetProgramPipelineiv);
        OPENGL_GET_PROC(PFNGLPROGRAMUNIFORM1IPROC, glProgramUniform1i);
        OPENGL_GET_PROC(PFNGLPROGRAMUNIFORM1IVPROC, glProgramUniform1iv);
        OPENGL_GET_PROC(PFNGLPROGRAMUNIFORM1FPROC, glProgramUniform1f);
        OPENGL_GET_PROC(PFNGLPROGRAMUNIFORM1FVPROC, glProgramUniform1fv);
        OPENGL_GET_PROC(PFNGLPROGRAMUNIFORM1DPROC, glProgramUniform1d); 
        OPENGL_GET_PROC(PFNGLPROGRAMUNIFORM1DVPROC, glProgramUniform1dv); 
        OPENGL_GET_PROC(PFNGLPROGRAMUNIFORM1UIPROC, glProgramUniform1ui); 
        OPENGL_GET_PROC(PFNGLPROGRAMUNIFORM1UIVPROC, glProgramUniform1uiv);
        OPENGL_GET_PROC(PFNGLPROGRAMUNIFORM2IPROC, glProgramUniform2i); 
        OPENGL_GET_PROC(PFNGLPROGRAMUNIFORM2IVPROC, glProgramUniform2iv); 
        OPENGL_GET_PROC(PFNGLPROGRAMUNIFORM2FPROC, glProgramUniform2f);
        OPENGL_GET_PROC(PFNGLPROGRAMUNIFORM2FVPROC, glProgramUniform2fv);
        OPENGL_GET_PROC(PFNGLPROGRAMUNIFORM2DPROC, glProgramUniform2d);
        OPENGL_GET_PROC(PFNGLPROGRAMUNIFORM2DVPROC, glProgramUniform2dv);
        OPENGL_GET_PROC(PFNGLPROGRAMUNIFORM2UIPROC, glProgramUniform2ui);
        OPENGL_GET_PROC(PFNGLPROGRAMUNIFORM2UIVPROC, glProgramUniform2uiv);
        OPENGL_GET_PROC(PFNGLPROGRAMUNIFORM3IPROC, glProgramUniform3i);
        OPENGL_GET_PROC(PFNGLPROGRAMUNIFORM3IVPROC, glProgramUniform3iv);
        OPENGL_GET_PROC(PFNGLPROGRAMUNIFORM3FPROC, glProgramUniform3f);
        OPENGL_GET_PROC(PFNGLPROGRAMUNIFORM3FVPROC, glProgramUniform3fv);
        OPENGL_GET_PROC(PFNGLPROGRAMUNIFORM3DPROC, glProgramUniform3d);
        OPENGL_GET_PROC(PFNGLPROGRAMUNIFORM3DVPROC, glProgramUniform3dv);
        OPENGL_GET_PROC(PFNGLPROGRAMUNIFORM3UIPROC, glProgramUniform3ui);
        OPENGL_GET_PROC(PFNGLPROGRAMUNIFORM3UIVPROC, glProgramUniform3uiv);
        OPENGL_GET_PROC(PFNGLPROGRAMUNIFORM4IPROC, glProgramUniform4i);
        OPENGL_GET_PROC(PFNGLPROGRAMUNIFORM4IVPROC, glProgramUniform4iv);
        OPENGL_GET_PROC(PFNGLPROGRAMUNIFORM4FPROC, glProgramUniform4f);
        OPENGL_GET_PROC(PFNGLPROGRAMUNIFORM4FVPROC, glProgramUniform4fv);
        OPENGL_GET_PROC(PFNGLPROGRAMUNIFORM4DPROC, glProgramUniform4d);
        OPENGL_GET_PROC(PFNGLPROGRAMUNIFORM4DVPROC, glProgramUniform4dv);
        OPENGL_GET_PROC(PFNGLPROGRAMUNIFORM4UIPROC, glProgramUniform4ui);
        OPENGL_GET_PROC(PFNGLPROGRAMUNIFORM4UIVPROC, glProgramUniform4uiv);

        OPENGL_GET_PROC(PFNGLPROGRAMUNIFORMMATRIX2FVPROC, glProgramUniformMatrix2fv); 
        OPENGL_GET_PROC(PFNGLPROGRAMUNIFORMMATRIX3FVPROC, glProgramUniformMatrix3fv); 
        OPENGL_GET_PROC(PFNGLPROGRAMUNIFORMMATRIX4FVPROC, glProgramUniformMatrix4fv); 
        OPENGL_GET_PROC(PFNGLPROGRAMUNIFORMMATRIX2DVPROC, glProgramUniformMatrix2dv); 
        OPENGL_GET_PROC(PFNGLPROGRAMUNIFORMMATRIX3DVPROC, glProgramUniformMatrix3dv);
        OPENGL_GET_PROC(PFNGLPROGRAMUNIFORMMATRIX4DVPROC, glProgramUniformMatrix4dv);
        OPENGL_GET_PROC(PFNGLPROGRAMUNIFORMMATRIX2X3FVPROC, glProgramUniformMatrix2x3fv);
        OPENGL_GET_PROC(PFNGLPROGRAMUNIFORMMATRIX3X2FVPROC, glProgramUniformMatrix3x2fv);
        OPENGL_GET_PROC(PFNGLPROGRAMUNIFORMMATRIX2X4FVPROC, glProgramUniformMatrix2x4fv);
        OPENGL_GET_PROC(PFNGLPROGRAMUNIFORMMATRIX4X2FVPROC, glProgramUniformMatrix4x2fv);
        OPENGL_GET_PROC(PFNGLPROGRAMUNIFORMMATRIX3X4FVPROC, glProgramUniformMatrix3x4fv);
        OPENGL_GET_PROC(PFNGLPROGRAMUNIFORMMATRIX4X3FVPROC, glProgramUniformMatrix4x3fv);
        OPENGL_GET_PROC(PFNGLPROGRAMUNIFORMMATRIX2X3DVPROC, glProgramUniformMatrix2x3dv);
        OPENGL_GET_PROC(PFNGLPROGRAMUNIFORMMATRIX3X2DVPROC, glProgramUniformMatrix3x2dv);
        OPENGL_GET_PROC(PFNGLPROGRAMUNIFORMMATRIX2X4DVPROC, glProgramUniformMatrix2x4dv);
        OPENGL_GET_PROC(PFNGLPROGRAMUNIFORMMATRIX4X2DVPROC, glProgramUniformMatrix4x2dv);
        OPENGL_GET_PROC(PFNGLPROGRAMUNIFORMMATRIX3X4DVPROC, glProgramUniformMatrix3x4dv);
        OPENGL_GET_PROC(PFNGLPROGRAMUNIFORMMATRIX4X3DVPROC, glProgramUniformMatrix4x3dv);
        OPENGL_GET_PROC(PFNGLVALIDATEPROGRAMPIPELINEPROC, glValidateProgramPipeline);
        OPENGL_GET_PROC(PFNGLGETPROGRAMPIPELINEINFOLOGPROC, glGetProgramPipelineInfoLog);
    }

    // GL_ARB_vertex_attrib_64bit 
    if(m_GLSupExt.ARB_vertex_attrib_64bit)
    {
        OPENGL_GET_PROC(PFNGLVERTEXATTRIBL1DPROC, glVertexAttribL1d);
        OPENGL_GET_PROC(PFNGLVERTEXATTRIBL2DPROC, glVertexAttribL2d);
        OPENGL_GET_PROC(PFNGLVERTEXATTRIBL3DPROC, glVertexAttribL3d);
        OPENGL_GET_PROC(PFNGLVERTEXATTRIBL4DPROC, glVertexAttribL4d);
        OPENGL_GET_PROC(PFNGLVERTEXATTRIBL1DVPROC, glVertexAttribL1dv);
        OPENGL_GET_PROC(PFNGLVERTEXATTRIBL2DVPROC, glVertexAttribL2dv);
        OPENGL_GET_PROC(PFNGLVERTEXATTRIBL3DVPROC, glVertexAttribL3dv);
        OPENGL_GET_PROC(PFNGLVERTEXATTRIBL4DVPROC, glVertexAttribL4dv);
        OPENGL_GET_PROC(PFNGLVERTEXATTRIBLPOINTERPROC, glVertexAttribLPointer);
        OPENGL_GET_PROC(PFNGLGETVERTEXATTRIBLDVPROC, glGetVertexAttribLdv);
    }

    // GL_ARB_viewport_array
    if(m_GLSupExt.ARB_viewport_array)
    {
        OPENGL_GET_PROC(PFNGLVIEWPORTARRAYVPROC, glViewportArrayv);
        OPENGL_GET_PROC(PFNGLVIEWPORTINDEXEDFPROC, glViewportIndexedf);
        OPENGL_GET_PROC(PFNGLVIEWPORTINDEXEDFVPROC, glViewportIndexedfv); 
        OPENGL_GET_PROC(PFNGLSCISSORARRAYVPROC, glScissorArrayv);
        OPENGL_GET_PROC(PFNGLSCISSORINDEXEDPROC, glScissorIndexed); 
        OPENGL_GET_PROC(PFNGLSCISSORINDEXEDVPROC, glScissorIndexedv); 
        OPENGL_GET_PROC(PFNGLDEPTHRANGEARRAYVPROC, glDepthRangeArrayv); 
        OPENGL_GET_PROC(PFNGLDEPTHRANGEINDEXEDPROC, glDepthRangeIndexed); 
        OPENGL_GET_PROC(PFNGLGETFLOATI_VPROC, glGetFloati_v); 
        OPENGL_GET_PROC(PFNGLGETDOUBLEI_VPROC, glGetDoublei_v); 
    }

    // GL_ARB_base_instance
    if(m_GLSupExt.ARB_base_instance)
    {
        OPENGL_GET_PROC(PFNGLDRAWARRAYSINSTANCEDBASEINSTANCEPROC, glDrawArraysInstancedBaseInstance);
        OPENGL_GET_PROC(PFNGLDRAWELEMENTSINSTANCEDBASEINSTANCEPROC, glDrawElementsInstancedBaseInstance);
        OPENGL_GET_PROC(PFNGLDRAWELEMENTSINSTANCEDBASEVERTEXBASEINSTANCEPROC, glDrawElementsInstancedBaseVertexBaseInstance);
    }

    // GL_ARB_transform_feedback_instanced 
    if(m_GLSupExt.ARB_transform_feedback_instanced)
    {
        OPENGL_GET_PROC(PFNGLDRAWTRANSFORMFEEDBACKINSTANCEDPROC, glDrawTransformFeedbackInstanced);
        OPENGL_GET_PROC(PFNGLDRAWTRANSFORMFEEDBACKSTREAMINSTANCEDPROC, glDrawTransformFeedbackStreamInstanced);
    }

    // GL_ARB_internalformat_query
    if(m_GLSupExt.ARB_internalformat_query)
    {
        OPENGL_GET_PROC(PFNGLGETINTERNALFORMATIVPROC, glGetInternalformativ);
    }

    // GL_ARB_shader_atomic_counters 
    if(m_GLSupExt.ARB_shader_atomic_counters)
    {
        OPENGL_GET_PROC(PFNGLGETACTIVEATOMICCOUNTERBUFFERIVPROC, glGetActiveAtomicCounterBufferiv);
    }

    // GL_ARB_shader_image_load_store 
    if(m_GLSupExt.ARB_shader_image_load_store)
    {
        OPENGL_GET_PROC(PFNGLBINDIMAGETEXTUREPROC, glBindImageTexture);
        OPENGL_GET_PROC(PFNGLMEMORYBARRIERPROC, glMemoryBarrier);
    }

    // GL_ARB_texture_storage 
    if(m_GLSupExt.ARB_texture_storage)
    {
        OPENGL_GET_PROC(PFNGLTEXSTORAGE1DPROC, glTexStorage1D);
        OPENGL_GET_PROC(PFNGLTEXSTORAGE2DPROC, glTexStorage2D);
        OPENGL_GET_PROC(PFNGLTEXSTORAGE3DPROC, glTexStorage3D);
        OPENGL_GET_PROC(PFNGLTEXTURESTORAGE1DEXTPROC, glTextureStorage1DEXT);
        OPENGL_GET_PROC(PFNGLTEXTURESTORAGE2DEXTPROC, glTextureStorage2DEXT);
        OPENGL_GET_PROC(PFNGLTEXTURESTORAGE3DEXTPROC, glTextureStorage3DEXT);
    }

    // GL_KHR_debug
    if(m_GLSupExt.KHR_debug)
    {
        OPENGL_GET_PROC(PFNGLDEBUGMESSAGECONTROLPROC, glDebugMessageControl);
        OPENGL_GET_PROC(PFNGLDEBUGMESSAGEINSERTPROC, glDebugMessageInsert);
        OPENGL_GET_PROC(PFNGLDEBUGMESSAGECALLBACKPROC, glDebugMessageCallback);
        OPENGL_GET_PROC(PFNGLGETDEBUGMESSAGELOGPROC, glGetDebugMessageLog);
        OPENGL_GET_PROC(PFNGLPUSHDEBUGGROUPPROC, glPushDebugGroup);
        OPENGL_GET_PROC(PFNGLPOPDEBUGGROUPPROC, glPopDebugGroup);
        OPENGL_GET_PROC(PFNGLOBJECTLABELPROC, glObjectLabel);
        OPENGL_GET_PROC(PFNGLGETOBJECTLABELPROC, glGetObjectLabel);
        OPENGL_GET_PROC(PFNGLOBJECTPTRLABELPROC, glObjectPtrLabel);
        OPENGL_GET_PROC(PFNGLGETOBJECTPTRLABELPROC, glGetObjectPtrLabel);
    }
    
    // GL_ARB_clear_buffer_object
    if(m_GLSupExt.ARB_clear_buffer_object)
    {
        OPENGL_GET_PROC(PFNGLCLEARBUFFERDATAPROC, glClearBufferData);
        OPENGL_GET_PROC(PFNGLCLEARBUFFERSUBDATAPROC, glClearBufferSubData);
        OPENGL_GET_PROC(PFNGLCLEARNAMEDBUFFERDATAEXTPROC, glClearNamedBufferDataEXT);
        OPENGL_GET_PROC(PFNGLCLEARNAMEDBUFFERSUBDATAEXTPROC, glClearNamedBufferSubDataEXT);
    }

    // GL_ARB_copy_image
    if(m_GLSupExt.ARB_copy_image)
    {
        OPENGL_GET_PROC(PFNGLCOPYIMAGESUBDATAPROC, glCopyImageSubData);
    }

    // GL_ARB_texture_view
    if(m_GLSupExt.ARB_texture_view)
    {
        OPENGL_GET_PROC(PFNGLTEXTUREVIEWPROC, glTextureView);
    }

    // GL_ARB_vertex_attrib_binding
    if(m_GLSupExt.ARB_vertex_attrib_binding)
    {
        OPENGL_GET_PROC(PFNGLBINDVERTEXBUFFERPROC, glBindVertexBuffer);
        OPENGL_GET_PROC(PFNGLVERTEXATTRIBFORMATPROC, glVertexAttribFormat);
        OPENGL_GET_PROC(PFNGLVERTEXATTRIBIFORMATPROC, glVertexAttribIFormat);
        OPENGL_GET_PROC(PFNGLVERTEXATTRIBLFORMATPROC, glVertexAttribLFormat);
        OPENGL_GET_PROC(PFNGLVERTEXATTRIBBINDINGPROC, glVertexAttribBinding);
        OPENGL_GET_PROC(PFNGLVERTEXBINDINGDIVISORPROC, glVertexBindingDivisor);
        OPENGL_GET_PROC(PFNGLVERTEXARRAYBINDVERTEXBUFFEREXTPROC, glVertexArrayBindVertexBufferEXT);
        OPENGL_GET_PROC(PFNGLVERTEXARRAYVERTEXATTRIBFORMATEXTPROC, glVertexArrayVertexAttribFormatEXT);
        OPENGL_GET_PROC(PFNGLVERTEXARRAYVERTEXATTRIBIFORMATEXTPROC, glVertexArrayVertexAttribIFormatEXT);
        OPENGL_GET_PROC(PFNGLVERTEXARRAYVERTEXATTRIBLFORMATEXTPROC, glVertexArrayVertexAttribLFormatEXT);
        OPENGL_GET_PROC(PFNGLVERTEXARRAYVERTEXATTRIBBINDINGEXTPROC, glVertexArrayVertexAttribBindingEXT);
        OPENGL_GET_PROC(PFNGLVERTEXARRAYVERTEXBINDINGDIVISOREXTPROC, glVertexArrayVertexBindingDivisorEXT);
    }


    // GL_ARB_framebuffer_no_attachments
    if(m_GLSupExt.ARB_framebuffer_no_attachments)
    {
        OPENGL_GET_PROC(PFNGLFRAMEBUFFERPARAMETERIPROC, glFramebufferParameteri);
        OPENGL_GET_PROC(PFNGLGETFRAMEBUFFERPARAMETERIVPROC, glGetFramebufferParameteriv);
        OPENGL_GET_PROC(PFNGLNAMEDFRAMEBUFFERPARAMETERIEXTPROC, glNamedFramebufferParameteriEXT);
        OPENGL_GET_PROC(PFNGLGETNAMEDFRAMEBUFFERPARAMETERIVEXTPROC, glGetNamedFramebufferParameterivEXT);
    }

    // GL_ARB_internalformat_query2
    if(m_GLSupExt.ARB_internalformat_query2)
    {
        OPENGL_GET_PROC(PFNGLGETINTERNALFORMATI64VPROC, glGetInternalformati64v);
    }

    // GL_ARB_invalidate_subdata
    if(m_GLSupExt.ARB_invalidate_subdata)
    {
        OPENGL_GET_PROC(PFNGLINVALIDATETEXSUBIMAGEPROC, glInvalidateTexSubImage);
        OPENGL_GET_PROC(PFNGLINVALIDATETEXIMAGEPROC, glInvalidateTexImage);
        OPENGL_GET_PROC(PFNGLINVALIDATEBUFFERSUBDATAPROC, glInvalidateBufferSubData);
        OPENGL_GET_PROC(PFNGLINVALIDATEBUFFERDATAPROC, glInvalidateBufferData);
        OPENGL_GET_PROC(PFNGLINVALIDATEFRAMEBUFFERPROC, glInvalidateFramebuffer);
        OPENGL_GET_PROC(PFNGLINVALIDATESUBFRAMEBUFFERPROC, glInvalidateSubFramebuffer);
    }

    // GL_ARB_multi_draw_indirect
    if(m_GLSupExt.ARB_multi_draw_indirect)
    {
        OPENGL_GET_PROC(PFNGLMULTIDRAWARRAYSINDIRECTPROC, glMultiDrawArraysIndirect);
        OPENGL_GET_PROC(PFNGLMULTIDRAWELEMENTSINDIRECTPROC, glMultiDrawElementsIndirect);
    }

    // GL_ARB_program_interface_query
    if(m_GLSupExt.ARB_program_interface_query)
    {
        OPENGL_GET_PROC(PFNGLGETPROGRAMINTERFACEIVPROC, glGetProgramInterfaceiv);
        OPENGL_GET_PROC(PFNGLGETPROGRAMRESOURCEINDEXPROC, glGetProgramResourceIndex);
        OPENGL_GET_PROC(PFNGLGETPROGRAMRESOURCENAMEPROC, glGetProgramResourceName);
        OPENGL_GET_PROC(PFNGLGETPROGRAMRESOURCEIVPROC, glGetProgramResourceiv);
        OPENGL_GET_PROC(PFNGLGETPROGRAMRESOURCELOCATIONPROC, glGetProgramResourceLocation);
        OPENGL_GET_PROC(PFNGLGETPROGRAMRESOURCELOCATIONINDEXPROC, glGetProgramResourceLocationIndex);
    }

    if(m_GLSupExt.ARB_shader_storage_buffer_object)
    {
        OPENGL_GET_PROC(PFNGLSHADERSTORAGEBLOCKBINDINGPROC, glShaderStorageBlockBinding);
    }
    
    if(m_GLSupExt.ARB_texture_buffer_range)
    {
        OPENGL_GET_PROC(PFNGLTEXBUFFERRANGEPROC, glTexBufferRange); 
        OPENGL_GET_PROC(PFNGLTEXTUREBUFFERRANGEEXTPROC, glTextureBufferRangeEXT);
    }

    if(m_GLSupExt.ARB_texture_storage_multisample)
    {    
        OPENGL_GET_PROC(PFNGLTEXSTORAGE2DMULTISAMPLEPROC, glTexStorage2DMultisample);
        OPENGL_GET_PROC(PFNGLTEXSTORAGE3DMULTISAMPLEPROC, glTexStorage3DMultisample);
        OPENGL_GET_PROC(PFNGLTEXTURESTORAGE2DMULTISAMPLEEXTPROC, glTextureStorage2DMultisampleEXT);
        OPENGL_GET_PROC(PFNGLTEXTURESTORAGE3DMULTISAMPLEEXTPROC, glTextureStorage3DMultisampleEXT);
    
    }
}
/////////////////////////////////////////////////////////////////////////////////////////////