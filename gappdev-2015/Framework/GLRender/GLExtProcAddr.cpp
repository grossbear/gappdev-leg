/////////////////////////////////////////////////////////////////////////////////////////////
// GLExtProcAddr.cpp
//
// Getting OpenGL EXT Procedure Address
/////////////////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////////////////
void CGLDevInfo::GetExtProcAddr()
{
    //GL_EXT_direct_state_access
    if(m_GLSupExt.EXT_direct_state_access)
    {
        OPENGL_GET_PROC(PFNGLISRENDERBUFFERPROC, glIsRenderbuffer);

        //GL_EXT_direct_state_access
        OPENGL_GET_PROC(PFNGLCLIENTATTRIBDEFAULTEXTPROC, glClientAttribDefaultEXT);
        OPENGL_GET_PROC(PFNGLPUSHCLIENTATTRIBDEFAULTEXTPROC, glPushClientAttribDefaultEXT);
        OPENGL_GET_PROC(PFNGLMATRIXLOADFEXTPROC, glMatrixLoadfEXT);
        OPENGL_GET_PROC(PFNGLMATRIXLOADDEXTPROC, glMatrixLoaddEXT);
        OPENGL_GET_PROC(PFNGLMATRIXMULTFEXTPROC, glMatrixMultfEXT);
        OPENGL_GET_PROC(PFNGLMATRIXMULTDEXTPROC, glMatrixMultdEXT);
        OPENGL_GET_PROC(PFNGLMATRIXLOADIDENTITYEXTPROC, glMatrixLoadIdentityEXT);
        OPENGL_GET_PROC(PFNGLMATRIXROTATEFEXTPROC, glMatrixRotatefEXT);
        OPENGL_GET_PROC(PFNGLMATRIXROTATEDEXTPROC, glMatrixRotatedEXT);
        OPENGL_GET_PROC(PFNGLMATRIXSCALEFEXTPROC, glMatrixScalefEXT);
        OPENGL_GET_PROC(PFNGLMATRIXSCALEDEXTPROC, glMatrixScaledEXT);
        OPENGL_GET_PROC(PFNGLMATRIXTRANSLATEFEXTPROC, glMatrixTranslatefEXT);
        OPENGL_GET_PROC(PFNGLMATRIXTRANSLATEDEXTPROC, glMatrixTranslatedEXT);
        OPENGL_GET_PROC(PFNGLMATRIXFRUSTUMEXTPROC, glMatrixFrustumEXT);
        OPENGL_GET_PROC(PFNGLMATRIXORTHOEXTPROC, glMatrixOrthoEXT);
        OPENGL_GET_PROC(PFNGLMATRIXPOPEXTPROC, glMatrixPopEXT);
        OPENGL_GET_PROC(PFNGLMATRIXPUSHEXTPROC, glMatrixPushEXT);
        OPENGL_GET_PROC(PFNGLMATRIXLOADTRANSPOSEFEXTPROC, glMatrixLoadTransposefEXT);
        OPENGL_GET_PROC(PFNGLMATRIXLOADTRANSPOSEDEXTPROC, glMatrixLoadTransposedEXT);
        OPENGL_GET_PROC(PFNGLMATRIXMULTTRANSPOSEFEXTPROC, glMatrixMultTransposefEXT);
        OPENGL_GET_PROC(PFNGLMATRIXMULTTRANSPOSEDEXTPROC, glMatrixMultTransposedEXT);

        OPENGL_GET_PROC(PFNGLCLIENTATTRIBDEFAULTEXTPROC, glClientAttribDefaultEXT);

        OPENGL_GET_PROC(PFNGLTEXTUREPARAMETERFEXTPROC, glTextureParameterfEXT);
        OPENGL_GET_PROC(PFNGLTEXTUREPARAMETERFVEXTPROC, glTextureParameterfvEXT);
        OPENGL_GET_PROC(PFNGLTEXTUREPARAMETERIEXTPROC, glTextureParameteriEXT);
        OPENGL_GET_PROC(PFNGLTEXTUREPARAMETERIVEXTPROC, glTextureParameterivEXT);
        OPENGL_GET_PROC(PFNGLTEXTUREIMAGE1DEXTPROC, glTextureImage1DEXT);
        OPENGL_GET_PROC(PFNGLTEXTUREIMAGE2DEXTPROC, glTextureImage2DEXT);
        OPENGL_GET_PROC(PFNGLTEXTURESUBIMAGE1DEXTPROC, glTextureSubImage1DEXT);
        OPENGL_GET_PROC(PFNGLTEXTURESUBIMAGE2DEXTPROC, glTextureSubImage2DEXT);
        OPENGL_GET_PROC(PFNGLCOPYTEXTUREIMAGE1DEXTPROC, glCopyTextureImage1DEXT);
        OPENGL_GET_PROC(PFNGLCOPYTEXTUREIMAGE2DEXTPROC, glCopyTextureImage2DEXT);
        OPENGL_GET_PROC(PFNGLCOPYTEXTURESUBIMAGE1DEXTPROC, glCopyTextureSubImage1DEXT);
        OPENGL_GET_PROC(PFNGLCOPYTEXTURESUBIMAGE2DEXTPROC, glCopyTextureSubImage2DEXT);
        OPENGL_GET_PROC(PFNGLGETTEXTUREIMAGEEXTPROC, glGetTextureImageEXT);
        OPENGL_GET_PROC(PFNGLGETTEXTUREPARAMETERFVEXTPROC, glGetTextureParameterfvEXT);
        OPENGL_GET_PROC(PFNGLGETTEXTUREPARAMETERIVEXTPROC, glGetTextureParameterivEXT);
        OPENGL_GET_PROC(PFNGLGETTEXTURELEVELPARAMETERFVEXTPROC, glGetTextureLevelParameterfvEXT);
        OPENGL_GET_PROC(PFNGLGETTEXTURELEVELPARAMETERIVEXTPROC, glGetTextureLevelParameterivEXT);
        OPENGL_GET_PROC(PFNGLTEXTUREIMAGE3DEXTPROC, glTextureImage3DEXT);
        OPENGL_GET_PROC(PFNGLTEXTURESUBIMAGE3DEXTPROC, glTextureSubImage3DEXT);
        OPENGL_GET_PROC(PFNGLCOPYTEXTURESUBIMAGE3DEXTPROC, glCopyTextureSubImage3DEXT);
        OPENGL_GET_PROC(PFNGLMULTITEXPARAMETERFEXTPROC, glMultiTexParameterfEXT);
        OPENGL_GET_PROC(PFNGLMULTITEXPARAMETERFVEXTPROC, glMultiTexParameterfvEXT);
        OPENGL_GET_PROC(PFNGLMULTITEXPARAMETERIEXTPROC, glMultiTexParameteriEXT);
        OPENGL_GET_PROC(PFNGLMULTITEXPARAMETERIVEXTPROC, glMultiTexParameterivEXT);
        OPENGL_GET_PROC(PFNGLMULTITEXIMAGE1DEXTPROC, glMultiTexImage1DEXT);
        OPENGL_GET_PROC(PFNGLMULTITEXIMAGE2DEXTPROC, glMultiTexImage2DEXT);
        OPENGL_GET_PROC(PFNGLMULTITEXSUBIMAGE1DEXTPROC, glMultiTexSubImage1DEXT);
        OPENGL_GET_PROC(PFNGLMULTITEXSUBIMAGE2DEXTPROC, glMultiTexSubImage2DEXT);
        OPENGL_GET_PROC(PFNGLCOPYMULTITEXIMAGE1DEXTPROC, glCopyMultiTexImage1DEXT);
        OPENGL_GET_PROC(PFNGLCOPYMULTITEXIMAGE2DEXTPROC, glCopyMultiTexImage2DEXT);
        OPENGL_GET_PROC(PFNGLCOPYMULTITEXSUBIMAGE1DEXTPROC, glCopyMultiTexSubImage1DEXT);
        OPENGL_GET_PROC(PFNGLCOPYMULTITEXSUBIMAGE2DEXTPROC, glCopyMultiTexSubImage2DEXT);


        OPENGL_GET_PROC(PFNGLGETMULTITEXIMAGEEXTPROC, glGetMultiTexImageEXT);
        OPENGL_GET_PROC(PFNGLGETMULTITEXPARAMETERFVEXTPROC, glGetMultiTexParameterfvEXT);
        OPENGL_GET_PROC(PFNGLGETMULTITEXPARAMETERIVEXTPROC, glGetMultiTexParameterivEXT);
        OPENGL_GET_PROC(PFNGLGETMULTITEXLEVELPARAMETERFVEXTPROC, glGetMultiTexLevelParameterfvEXT);
        OPENGL_GET_PROC(PFNGLGETMULTITEXLEVELPARAMETERIVEXTPROC, glGetMultiTexLevelParameterivEXT);
        OPENGL_GET_PROC(PFNGLMULTITEXIMAGE3DEXTPROC, glMultiTexImage3DEXT);
        OPENGL_GET_PROC(PFNGLMULTITEXSUBIMAGE3DEXTPROC, glMultiTexSubImage3DEXT);
        OPENGL_GET_PROC(PFNGLCOPYMULTITEXSUBIMAGE3DEXTPROC, glCopyMultiTexSubImage3DEXT);
        OPENGL_GET_PROC(PFNGLBINDMULTITEXTUREEXTPROC, glBindMultiTextureEXT);
        OPENGL_GET_PROC(PFNGLENABLECLIENTSTATEINDEXEDEXTPROC, glEnableClientStateIndexedEXT);
        OPENGL_GET_PROC(PFNGLDISABLECLIENTSTATEINDEXEDEXTPROC, glDisableClientStateIndexedEXT);
        OPENGL_GET_PROC(PFNGLMULTITEXCOORDPOINTEREXTPROC, glMultiTexCoordPointerEXT);
        OPENGL_GET_PROC(PFNGLMULTITEXENVFEXTPROC, glMultiTexEnvfEXT);
        OPENGL_GET_PROC(PFNGLMULTITEXENVFVEXTPROC, glMultiTexEnvfvEXT);
        OPENGL_GET_PROC(PFNGLMULTITEXENVIEXTPROC, glMultiTexEnviEXT);
        OPENGL_GET_PROC(PFNGLMULTITEXENVIVEXTPROC, glMultiTexEnvivEXT);
        OPENGL_GET_PROC(PFNGLMULTITEXGENDEXTPROC, glMultiTexGendEXT);
        OPENGL_GET_PROC(PFNGLMULTITEXGENDVEXTPROC, glMultiTexGendvEXT);
        OPENGL_GET_PROC(PFNGLMULTITEXGENFEXTPROC, glMultiTexGenfEXT);
        OPENGL_GET_PROC(PFNGLMULTITEXGENFVEXTPROC, glMultiTexGenfvEXT);
        OPENGL_GET_PROC(PFNGLMULTITEXGENIEXTPROC, glMultiTexGeniEXT);
        OPENGL_GET_PROC(PFNGLMULTITEXGENIVEXTPROC, glMultiTexGenivEXT);
        OPENGL_GET_PROC(PFNGLGETMULTITEXENVFVEXTPROC, glGetMultiTexEnvfvEXT);
        OPENGL_GET_PROC(PFNGLGETMULTITEXENVIVEXTPROC, glGetMultiTexEnvivEXT);
        OPENGL_GET_PROC(PFNGLGETMULTITEXGENDVEXTPROC, glGetMultiTexGendvEXT);
        OPENGL_GET_PROC(PFNGLGETMULTITEXGENFVEXTPROC, glGetMultiTexGenfvEXT);
        OPENGL_GET_PROC(PFNGLGETMULTITEXGENIVEXTPROC, glGetMultiTexGenivEXT);
        OPENGL_GET_PROC(PFNGLGETFLOATINDEXEDVEXTPROC, glGetFloatIndexedvEXT);
        OPENGL_GET_PROC(PFNGLGETDOUBLEINDEXEDVEXTPROC, glGetDoubleIndexedvEXT);
        OPENGL_GET_PROC(PFNGLGETPOINTERINDEXEDVEXTPROC, glGetPointerIndexedvEXT);


        OPENGL_GET_PROC(PFNGLCOMPRESSEDTEXTUREIMAGE3DEXTPROC, glCompressedTextureImage3DEXT);
        OPENGL_GET_PROC(PFNGLCOMPRESSEDTEXTUREIMAGE2DEXTPROC, glCompressedTextureImage2DEXT);
        OPENGL_GET_PROC(PFNGLCOMPRESSEDTEXTUREIMAGE1DEXTPROC, glCompressedTextureImage1DEXT);
        OPENGL_GET_PROC(PFNGLCOMPRESSEDTEXTURESUBIMAGE3DEXTPROC, glCompressedTextureSubImage3DEXT);
        OPENGL_GET_PROC(PFNGLCOMPRESSEDTEXTURESUBIMAGE2DEXTPROC, glCompressedTextureSubImage2DEXT);
        OPENGL_GET_PROC(PFNGLCOMPRESSEDTEXTURESUBIMAGE1DEXTPROC, glCompressedTextureSubImage1DEXT);
        OPENGL_GET_PROC(PFNGLGETCOMPRESSEDTEXTUREIMAGEEXTPROC, glGetCompressedTextureImageEXT);
        OPENGL_GET_PROC(PFNGLCOMPRESSEDMULTITEXIMAGE3DEXTPROC, glCompressedMultiTexImage3DEXT);
        OPENGL_GET_PROC(PFNGLCOMPRESSEDMULTITEXIMAGE2DEXTPROC, glCompressedMultiTexImage2DEXT);
        OPENGL_GET_PROC(PFNGLCOMPRESSEDMULTITEXIMAGE1DEXTPROC, glCompressedMultiTexImage1DEXT);
        OPENGL_GET_PROC(PFNGLCOMPRESSEDMULTITEXSUBIMAGE3DEXTPROC, glCompressedMultiTexSubImage3DEXT);
        OPENGL_GET_PROC(PFNGLCOMPRESSEDMULTITEXSUBIMAGE2DEXTPROC, glCompressedMultiTexSubImage2DEXT);
        OPENGL_GET_PROC(PFNGLCOMPRESSEDMULTITEXSUBIMAGE1DEXTPROC, glCompressedMultiTexSubImage1DEXT);
        OPENGL_GET_PROC(PFNGLGETCOMPRESSEDMULTITEXIMAGEEXTPROC, glGetCompressedMultiTexImageEXT);

        OPENGL_GET_PROC(PFNGLNAMEDPROGRAMSTRINGEXTPROC, glNamedProgramStringEXT);
        OPENGL_GET_PROC(PFNGLNAMEDPROGRAMLOCALPARAMETER4DEXTPROC, glNamedProgramLocalParameter4dEXT);
        OPENGL_GET_PROC(PFNGLNAMEDPROGRAMLOCALPARAMETER4DVEXTPROC, glNamedProgramLocalParameter4dvEXT);
        OPENGL_GET_PROC(PFNGLNAMEDPROGRAMLOCALPARAMETER4FEXTPROC, glNamedProgramLocalParameter4fEXT);
        OPENGL_GET_PROC(PFNGLNAMEDPROGRAMLOCALPARAMETER4FVEXTPROC, glNamedProgramLocalParameter4fvEXT);
        OPENGL_GET_PROC(PFNGLGETNAMEDPROGRAMLOCALPARAMETERDVEXTPROC, glGetNamedProgramLocalParameterdvEXT);
        OPENGL_GET_PROC(PFNGLGETNAMEDPROGRAMLOCALPARAMETERFVEXTPROC, glGetNamedProgramLocalParameterfvEXT);
        OPENGL_GET_PROC(PFNGLGETNAMEDPROGRAMIVEXTPROC, glGetNamedProgramivEXT);
        OPENGL_GET_PROC(PFNGLGETNAMEDPROGRAMSTRINGEXTPROC, glGetNamedProgramStringEXT);
        OPENGL_GET_PROC(PFNGLNAMEDPROGRAMLOCALPARAMETERS4FVEXTPROC, glNamedProgramLocalParameters4fvEXT);
        OPENGL_GET_PROC(PFNGLNAMEDPROGRAMLOCALPARAMETERI4IEXTPROC, glNamedProgramLocalParameterI4iEXT);
        OPENGL_GET_PROC(PFNGLNAMEDPROGRAMLOCALPARAMETERI4IVEXTPROC, glNamedProgramLocalParameterI4ivEXT);
        OPENGL_GET_PROC(PFNGLNAMEDPROGRAMLOCALPARAMETERSI4IVEXTPROC, glNamedProgramLocalParametersI4ivEXT);
        OPENGL_GET_PROC(PFNGLNAMEDPROGRAMLOCALPARAMETERI4UIEXTPROC, glNamedProgramLocalParameterI4uiEXT);
        OPENGL_GET_PROC(PFNGLNAMEDPROGRAMLOCALPARAMETERI4UIVEXTPROC, glNamedProgramLocalParameterI4uivEXT);
        OPENGL_GET_PROC(PFNGLNAMEDPROGRAMLOCALPARAMETERSI4UIVEXTPROC, glNamedProgramLocalParametersI4uivEXT);
        OPENGL_GET_PROC(PFNGLGETNAMEDPROGRAMLOCALPARAMETERIIVEXTPROC, glGetNamedProgramLocalParameterIivEXT);
        OPENGL_GET_PROC(PFNGLGETNAMEDPROGRAMLOCALPARAMETERIUIVEXTPROC, glGetNamedProgramLocalParameterIuivEXT);
        OPENGL_GET_PROC(PFNGLTEXTUREPARAMETERIIVEXTPROC, glTextureParameterIivEXT);
        OPENGL_GET_PROC(PFNGLTEXTUREPARAMETERIUIVEXTPROC, glTextureParameterIuivEXT);
        OPENGL_GET_PROC(PFNGLGETTEXTUREPARAMETERIIVEXTPROC, glGetTextureParameterIivEXT);
        OPENGL_GET_PROC(PFNGLGETTEXTUREPARAMETERIUIVEXTPROC, glGetTextureParameterIuivEXT);
        OPENGL_GET_PROC(PFNGLMULTITEXPARAMETERIIVEXTPROC, glMultiTexParameterIivEXT);
        OPENGL_GET_PROC(PFNGLMULTITEXPARAMETERIUIVEXTPROC, glMultiTexParameterIuivEXT);
        OPENGL_GET_PROC(PFNGLGETMULTITEXPARAMETERIIVEXTPROC, glGetMultiTexParameterIivEXT);
        OPENGL_GET_PROC(PFNGLGETMULTITEXPARAMETERIUIVEXTPROC, glGetMultiTexParameterIuivEXT);


        OPENGL_GET_PROC(PFNGLPROGRAMUNIFORM1FEXTPROC, glProgramUniform1fEXT);
        OPENGL_GET_PROC(PFNGLPROGRAMUNIFORM2FEXTPROC, glProgramUniform2fEXT);
        OPENGL_GET_PROC(PFNGLPROGRAMUNIFORM3FEXTPROC, glProgramUniform3fEXT);
        OPENGL_GET_PROC(PFNGLPROGRAMUNIFORM4FEXTPROC, glProgramUniform4fEXT);
        OPENGL_GET_PROC(PFNGLPROGRAMUNIFORM1IEXTPROC, glProgramUniform1iEXT);
        OPENGL_GET_PROC(PFNGLPROGRAMUNIFORM2IEXTPROC, glProgramUniform2iEXT);
        OPENGL_GET_PROC(PFNGLPROGRAMUNIFORM3IEXTPROC, glProgramUniform3iEXT);
        OPENGL_GET_PROC(PFNGLPROGRAMUNIFORM4IEXTPROC, glProgramUniform4iEXT);
        OPENGL_GET_PROC(PFNGLPROGRAMUNIFORM1FVEXTPROC, glProgramUniform1fvEXT);
        OPENGL_GET_PROC(PFNGLPROGRAMUNIFORM2FVEXTPROC, glProgramUniform2fvEXT);
        OPENGL_GET_PROC(PFNGLPROGRAMUNIFORM3FVEXTPROC, glProgramUniform3fvEXT);
        OPENGL_GET_PROC(PFNGLPROGRAMUNIFORM4FVEXTPROC, glProgramUniform4fvEXT);
        OPENGL_GET_PROC(PFNGLPROGRAMUNIFORM1IVEXTPROC, glProgramUniform1ivEXT);
        OPENGL_GET_PROC(PFNGLPROGRAMUNIFORM2IVEXTPROC, glProgramUniform2ivEXT);
        OPENGL_GET_PROC(PFNGLPROGRAMUNIFORM3IVEXTPROC, glProgramUniform3ivEXT);
        OPENGL_GET_PROC(PFNGLPROGRAMUNIFORM4IVEXTPROC, glProgramUniform4ivEXT);
        OPENGL_GET_PROC(PFNGLPROGRAMUNIFORMMATRIX2FVEXTPROC, glProgramUniformMatrix2fvEXT);
        OPENGL_GET_PROC(PFNGLPROGRAMUNIFORMMATRIX3FVEXTPROC, glProgramUniformMatrix3fvEXT);
        OPENGL_GET_PROC(PFNGLPROGRAMUNIFORMMATRIX4FVEXTPROC, glProgramUniformMatrix4fvEXT);
        OPENGL_GET_PROC(PFNGLPROGRAMUNIFORMMATRIX2X3FVEXTPROC, glProgramUniformMatrix2x3fvEXT);
        OPENGL_GET_PROC(PFNGLPROGRAMUNIFORMMATRIX3X2FVEXTPROC, glProgramUniformMatrix3x2fvEXT);
        OPENGL_GET_PROC(PFNGLPROGRAMUNIFORMMATRIX2X4FVEXTPROC, glProgramUniformMatrix2x4fvEXT);
        OPENGL_GET_PROC(PFNGLPROGRAMUNIFORMMATRIX4X2FVEXTPROC, glProgramUniformMatrix4x2fvEXT);
        OPENGL_GET_PROC(PFNGLPROGRAMUNIFORMMATRIX3X4FVEXTPROC, glProgramUniformMatrix3x4fvEXT);
        OPENGL_GET_PROC(PFNGLPROGRAMUNIFORMMATRIX4X3FVEXTPROC, glProgramUniformMatrix4x3fvEXT);
        OPENGL_GET_PROC(PFNGLPROGRAMUNIFORM1UIEXTPROC, glProgramUniform1uiEXT);
        OPENGL_GET_PROC(PFNGLPROGRAMUNIFORM2UIEXTPROC, glProgramUniform2uiEXT);
        OPENGL_GET_PROC(PFNGLPROGRAMUNIFORM3UIEXTPROC, glProgramUniform3uiEXT);
        OPENGL_GET_PROC(PFNGLPROGRAMUNIFORM4UIEXTPROC, glProgramUniform4uiEXT);
        OPENGL_GET_PROC(PFNGLPROGRAMUNIFORM1UIVEXTPROC, glProgramUniform1uivEXT);
        OPENGL_GET_PROC(PFNGLPROGRAMUNIFORM2UIVEXTPROC, glProgramUniform2uivEXT);
        OPENGL_GET_PROC(PFNGLPROGRAMUNIFORM3UIVEXTPROC, glProgramUniform3uivEXT);
        OPENGL_GET_PROC(PFNGLPROGRAMUNIFORM4UIVEXTPROC, glProgramUniform4uivEXT);
        OPENGL_GET_PROC(PFNGLNAMEDBUFFERDATAEXTPROC, glNamedBufferDataEXT);
        OPENGL_GET_PROC(PFNGLNAMEDBUFFERSUBDATAEXTPROC, glNamedBufferSubDataEXT);

        OPENGL_GET_PROC(PFNGLMAPNAMEDBUFFEREXTPROC, glMapNamedBufferEXT);
        OPENGL_GET_PROC(PFNGLUNMAPNAMEDBUFFEREXTPROC, glUnmapNamedBufferEXT);
        OPENGL_GET_PROC(PFNGLMAPNAMEDBUFFERRANGEEXTPROC, glMapNamedBufferRangeEXT);

        OPENGL_GET_PROC(PFNGLFLUSHMAPPEDNAMEDBUFFERRANGEEXTPROC, glFlushMappedNamedBufferRangeEXT);
        OPENGL_GET_PROC(PFNGLNAMEDCOPYBUFFERSUBDATAEXTPROC, glNamedCopyBufferSubDataEXT);
        OPENGL_GET_PROC(PFNGLGETNAMEDBUFFERPARAMETERIVEXTPROC, glGetNamedBufferParameterivEXT);
        OPENGL_GET_PROC(PFNGLGETNAMEDBUFFERPOINTERVEXTPROC, glGetNamedBufferPointervEXT);
        OPENGL_GET_PROC(PFNGLGETNAMEDBUFFERSUBDATAEXTPROC, glGetNamedBufferSubDataEXT);
        OPENGL_GET_PROC(PFNGLTEXTUREBUFFEREXTPROC, glTextureBufferEXT);
        OPENGL_GET_PROC(PFNGLMULTITEXBUFFEREXTPROC, glMultiTexBufferEXT);
        OPENGL_GET_PROC(PFNGLNAMEDRENDERBUFFERSTORAGEEXTPROC, glNamedRenderbufferStorageEXT);
        OPENGL_GET_PROC(PFNGLGETNAMEDRENDERBUFFERPARAMETERIVEXTPROC, glGetNamedRenderbufferParameterivEXT);
        OPENGL_GET_PROC(PFNGLCHECKNAMEDFRAMEBUFFERSTATUSEXTPROC, glCheckNamedFramebufferStatusEXT);

        OPENGL_GET_PROC(PFNGLNAMEDFRAMEBUFFERTEXTURE1DEXTPROC, glNamedFramebufferTexture1DEXT);
        OPENGL_GET_PROC(PFNGLNAMEDFRAMEBUFFERTEXTURE2DEXTPROC, glNamedFramebufferTexture2DEXT);
        OPENGL_GET_PROC(PFNGLNAMEDFRAMEBUFFERTEXTURE3DEXTPROC, glNamedFramebufferTexture3DEXT);
        OPENGL_GET_PROC(PFNGLNAMEDFRAMEBUFFERRENDERBUFFEREXTPROC, glNamedFramebufferRenderbufferEXT);
        OPENGL_GET_PROC(PFNGLGETNAMEDFRAMEBUFFERATTACHMENTPARAMETERIVEXTPROC, glGetNamedFramebufferAttachmentParameterivEXT);
        OPENGL_GET_PROC(PFNGLGENERATETEXTUREMIPMAPEXTPROC, glGenerateTextureMipmapEXT);
        OPENGL_GET_PROC(PFNGLGENERATEMULTITEXMIPMAPEXTPROC, glGenerateMultiTexMipmapEXT);
        OPENGL_GET_PROC(PFNGLFRAMEBUFFERDRAWBUFFEREXTPROC, glFramebufferDrawBufferEXT);
        OPENGL_GET_PROC(PFNGLFRAMEBUFFERDRAWBUFFERSEXTPROC, glFramebufferDrawBuffersEXT);
        OPENGL_GET_PROC(PFNGLFRAMEBUFFERREADBUFFEREXTPROC, glFramebufferReadBufferEXT);
        OPENGL_GET_PROC(PFNGLGETFRAMEBUFFERPARAMETERIVEXTPROC, glGetFramebufferParameterivEXT);
        OPENGL_GET_PROC(PFNGLNAMEDRENDERBUFFERSTORAGEMULTISAMPLEEXTPROC, glNamedRenderbufferStorageMultisampleEXT);
        OPENGL_GET_PROC(PFNGLNAMEDRENDERBUFFERSTORAGEMULTISAMPLECOVERAGEEXTPROC, glNamedRenderbufferStorageMultisampleCoverageEXT);
        OPENGL_GET_PROC(PFNGLNAMEDFRAMEBUFFERTEXTUREEXTPROC, glNamedFramebufferTextureEXT);
        OPENGL_GET_PROC(PFNGLNAMEDFRAMEBUFFERTEXTURELAYEREXTPROC, glNamedFramebufferTextureLayerEXT);
        OPENGL_GET_PROC(PFNGLNAMEDFRAMEBUFFERTEXTUREFACEEXTPROC, glNamedFramebufferTextureFaceEXT);
        OPENGL_GET_PROC(PFNGLTEXTURERENDERBUFFEREXTPROC, glTextureRenderbufferEXT);
        OPENGL_GET_PROC(PFNGLMULTITEXRENDERBUFFEREXTPROC, glMultiTexRenderbufferEXT);
        OPENGL_GET_PROC(PFNGLPROGRAMUNIFORM1DEXTPROC, glProgramUniform1dEXT);
        OPENGL_GET_PROC(PFNGLPROGRAMUNIFORM2DEXTPROC, glProgramUniform2dEXT);
        OPENGL_GET_PROC(PFNGLPROGRAMUNIFORM3DEXTPROC, glProgramUniform3dEXT);
        OPENGL_GET_PROC(PFNGLPROGRAMUNIFORM4DEXTPROC, glProgramUniform4dEXT);
        OPENGL_GET_PROC(PFNGLPROGRAMUNIFORM1DVEXTPROC, glProgramUniform1dvEXT);
        OPENGL_GET_PROC(PFNGLPROGRAMUNIFORM2DVEXTPROC, glProgramUniform2dvEXT);
        OPENGL_GET_PROC(PFNGLPROGRAMUNIFORM3DVEXTPROC, glProgramUniform3dvEXT);
        OPENGL_GET_PROC(PFNGLPROGRAMUNIFORM4DVEXTPROC, glProgramUniform4dvEXT);
        OPENGL_GET_PROC(PFNGLPROGRAMUNIFORMMATRIX2DVEXTPROC, glProgramUniformMatrix2dvEXT);
        OPENGL_GET_PROC(PFNGLPROGRAMUNIFORMMATRIX3DVEXTPROC, glProgramUniformMatrix3dvEXT);
        OPENGL_GET_PROC(PFNGLPROGRAMUNIFORMMATRIX4DVEXTPROC, glProgramUniformMatrix4dvEXT);
        OPENGL_GET_PROC(PFNGLPROGRAMUNIFORMMATRIX2X3DVEXTPROC, glProgramUniformMatrix2x3dvEXT);
        OPENGL_GET_PROC(PFNGLPROGRAMUNIFORMMATRIX2X4DVEXTPROC, glProgramUniformMatrix2x4dvEXT);
        OPENGL_GET_PROC(PFNGLPROGRAMUNIFORMMATRIX3X2DVEXTPROC, glProgramUniformMatrix3x2dvEXT);
        OPENGL_GET_PROC(PFNGLPROGRAMUNIFORMMATRIX3X4DVEXTPROC, glProgramUniformMatrix3x4dvEXT);
        OPENGL_GET_PROC(PFNGLPROGRAMUNIFORMMATRIX4X2DVEXTPROC, glProgramUniformMatrix4x2dvEXT);
        OPENGL_GET_PROC(PFNGLPROGRAMUNIFORMMATRIX4X3DVEXTPROC, glProgramUniformMatrix4x3dvEXT);


    }
}
/////////////////////////////////////////////////////////////////////////////////////////////