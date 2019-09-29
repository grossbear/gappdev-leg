/////////////////////////////////////////////////////////////////////////////////////////////
// GLSahderSystem.cpp
//
// OpenGL Shaders Managment Class
/////////////////////////////////////////////////////////////////////////////////////////////

#define _CRT_SECURE_NO_WARNINGS

#include <Windows.h>
#include "GL/gl.h"
#include "GL/glext.h"
#include "GLDevInfo.h"
#include "GLConvFunc.h"
#include "GLShaderSystem.h"
#include "GLVerProc.h"
#include "GLArbProc.h"

#include <string>
#include <algorithm>
#include <memory.h>


/////////////////////////////////////////////////////////////////////////////////////////////
CGLShaderSystem* CGLShaderSystem::m_pInstance = NULL;

/////////////////////////////////////////////////////////////////////////////////////////////
char CGLShaderSystem::toupper(char c)
{
    if(c >= 'a' && c <= 'z')
        return c - 32;

    return c;
}

/////////////////////////////////////////////////////////////////////////////////////////////
CGLShaderSystem* CGLShaderSystem::GetSingleton()
{
    if(!m_pInstance)
        m_pInstance = new CGLShaderSystem();

    return m_pInstance;
}
/////////////////////////////////////////////////////////////////////////////////////////////
CGLShaderSystem::CGLShaderSystem():m_bShaderActive(false),m_bProgramPipelineActive(false),m_pActiveShaderInInfo(0)
{

}

/////////////////////////////////////////////////////////////////////////////////////////////
CGLShaderSystem::~CGLShaderSystem()
{
    ReleaseShaders();
    m_ShaderParamsDescMap.Clear();
    m_ParamNames.NamesVec.clear();
    m_ParamNames.RefCounter.clear();
}
/////////////////////////////////////////////////////////////////////////////////////////////
uint32t CGLShaderSystem::CreateShader(const char *strshader, bool bseparate)
{
    if(strshader==NULL)
        return 0;

    // Parsing Shader
    std::string source_code(strshader);

    // Extracting Shaders
    std::string src_shaders[5];
    GetShaderSource(source_code,src_shaders[TYPE_VERTEX_SHADER],TYPE_VERTEX_SHADER);
    GetShaderSource(source_code,src_shaders[TYPE_PIXEL_SHADER],TYPE_PIXEL_SHADER);
    GetShaderSource(source_code,src_shaders[TYPE_GEOMETRY_SHADER],TYPE_GEOMETRY_SHADER);
    GetShaderSource(source_code,src_shaders[TYPE_TESS_EVALUATION_SHADER],TYPE_TESS_EVALUATION_SHADER);
    GetShaderSource(source_code,src_shaders[TYPE_TESS_CONTROL_SHADER],TYPE_TESS_CONTROL_SHADER);

    if(src_shaders[TYPE_VERTEX_SHADER].empty() || src_shaders[TYPE_PIXEL_SHADER].empty())
        if(!bseparate)
            return 0;

    uint32t objidx = m_ShaderObjects.Reserve(1);
    CGLShaderSystem::ShaderObject *shaderobj = m_ShaderObjects.GetItem(objidx);
    if(!shaderobj)
        return 0;
    memset(shaderobj,0,sizeof(CGLShaderSystem::ShaderObject));

    // Compile Vertex Shader
    shaderobj->hVertrexShader = CompileShader(src_shaders[TYPE_VERTEX_SHADER],GL_VERTEX_SHADER);
    GLint vert_status = 0;
    glGetShaderiv(shaderobj->hVertrexShader, GL_COMPILE_STATUS, &vert_status);
    if(!shaderobj->hVertrexShader || vert_status==GL_FALSE)
    {
        SetShaderInfoLog(shaderobj->hVertrexShader);
        ReleaseShaderObject(shaderobj);
        m_ShaderObjects.Release(objidx);
        return 0;
    }
    
    // Compile Pixel Shader
    shaderobj->hFragmentShader = CompileShader(src_shaders[TYPE_PIXEL_SHADER],GL_FRAGMENT_SHADER);
    GLint frag_status = 0;
    glGetShaderiv(shaderobj->hFragmentShader, GL_COMPILE_STATUS, &frag_status);
    if(!shaderobj->hFragmentShader || frag_status==GL_FALSE)
    {
        SetShaderInfoLog(shaderobj->hFragmentShader);
        ReleaseShaderObject(shaderobj);
        m_ShaderObjects.Release(objidx);
        return 0;
    }

    // Setting Geometry Shader
    if(!src_shaders[TYPE_GEOMETRY_SHADER].empty()) 
    {
        // Compile Geometry Shader
        shaderobj->hGeometryShader = CompileShader(src_shaders[TYPE_GEOMETRY_SHADER],GL_GEOMETRY_SHADER);
        GLint geom_status = 0;
        glGetShaderiv(shaderobj->hGeometryShader, GL_COMPILE_STATUS, &geom_status);
        if(!shaderobj->hGeometryShader || geom_status==GL_FALSE)
        {
            SetShaderInfoLog(shaderobj->hGeometryShader);
            ReleaseShaderObject(shaderobj);
            m_ShaderObjects.Release(objidx);
            return 0;
        }
    }

    // Setting Tessellation Control Shader
    if(!src_shaders[TYPE_TESS_CONTROL_SHADER].empty()) 
    {
        // Compile Tessellation Control Shader
        shaderobj->hTessCtrlShader = CompileShader(src_shaders[TYPE_TESS_CONTROL_SHADER],GL_TESS_CONTROL_SHADER);
        GLint tess_status = 0;
        glGetShaderiv(shaderobj->hTessCtrlShader, GL_COMPILE_STATUS, &tess_status);
        if(!shaderobj->hTessCtrlShader || tess_status==GL_FALSE)
        {
            SetShaderInfoLog(shaderobj->hTessCtrlShader);
            ReleaseShaderObject(shaderobj);
            m_ShaderObjects.Release(objidx);
            return 0;
        }
    }

    // Setting Tessellation Evaluation Shader
    if(!src_shaders[TYPE_TESS_EVALUATION_SHADER].empty()) 
    {
        // Compile Tessellation Evaluation Shader
        shaderobj->hTessEvalShader = CompileShader(src_shaders[TYPE_TESS_EVALUATION_SHADER],GL_TESS_EVALUATION_SHADER);
        GLint tess_status = 0;
        glGetShaderiv(shaderobj->hTessEvalShader, GL_COMPILE_STATUS, &tess_status);
        if(!shaderobj->hTessEvalShader || tess_status==GL_FALSE)
        {
            SetShaderInfoLog(shaderobj->hTessEvalShader);
            ReleaseShaderObject(shaderobj);
            m_ShaderObjects.Release(objidx);
            return 0;
        }
    }


    //Create A Program Object And Attach Compiled Shaders
    shaderobj->hProgramObject = glCreateProgram();
    //--GLint separate_val = (bseparate)?(GL_TRUE):(GL_FALSE);
    //--glProgramParameteri(shaderobj->hProgramObject,GL_PROGRAM_SEPARABLE, separate_val);

    // Vertex Shader
    if(shaderobj->hVertrexShader)
        glAttachShader(shaderobj->hProgramObject, shaderobj->hVertrexShader);
    // Fragment Shader
    if(shaderobj->hFragmentShader)
        glAttachShader(shaderobj->hProgramObject, shaderobj->hFragmentShader);
    

    //Tessellation Control Shader
    if(shaderobj->hTessCtrlShader)
        glAttachShader(shaderobj->hProgramObject, shaderobj->hTessCtrlShader);
    //Tessellation Evaluation Shader
    if(shaderobj->hTessEvalShader)
        glAttachShader(shaderobj->hProgramObject, shaderobj->hTessEvalShader);

    
    if(shaderobj->hGeometryShader)
    {
        glAttachShader(shaderobj->hProgramObject, shaderobj->hGeometryShader);

        GLint input_type,output_type,max_vert;
        GetGeomtryShaderParams(source_code,input_type,output_type,max_vert);
        if(input_type==0 || output_type==0)
        {
            m_strShaderInfoLog = "No input and output types for geometry shader";
            ReleaseShaderObject(shaderobj);
            m_ShaderObjects.Release(objidx);
            return 0;
        }
        GLint outvertnum = 0;
        glGetIntegerv(GL_MAX_GEOMETRY_OUTPUT_VERTICES_EXT,&outvertnum);
        max_vert = (max_vert<=0 || max_vert >= outvertnum) ? (outvertnum) : (max_vert);
        glProgramParameteri(shaderobj->hProgramObject,GL_GEOMETRY_INPUT_TYPE_EXT,input_type);
        glProgramParameteri(shaderobj->hProgramObject,GL_GEOMETRY_OUTPUT_TYPE_EXT,output_type);
        glProgramParameteri(shaderobj->hProgramObject,GL_GEOMETRY_VERTICES_OUT_EXT,max_vert);
    }

    //--glProgramParameteri ( shaderobj->hProgramObject, GL_PROGRAM_BINARY_RETRIEVABLE_HINT, GL_TRUE );

    //Link The Program Object
    glLinkProgram(shaderobj->hProgramObject);

    //Check Link Status
    GLint linked;
    glGetProgramiv(shaderobj->hProgramObject, GL_LINK_STATUS, &linked);
    SetProgramInfoLog(shaderobj->hProgramObject);
    if(!linked)
    {
        ReleaseShaderObject(shaderobj);
        m_ShaderObjects.Release(objidx);
        return 0;
    }

    //Retrieving Shader Uniform Names And Locations
    unsigned int infoidx = m_ShaderInputInfoMaps.Reserve(1);
    ShaderInputInfo *ininfo = m_ShaderInputInfoMaps.GetItem(infoidx);
    
    ExtractUniformNames(strshader,ininfo->UniformNamesMap);
    ExtractUniformBlockNames(strshader,ininfo->UniformBlockNamesMap);
    ExtractInputNames(src_shaders[TYPE_VERTEX_SHADER],ininfo->InputNamesMap);

    GetUniformLoc(shaderobj->hProgramObject,ininfo->UniformNamesMap);
    GetUniformBlockIndices(shaderobj->hProgramObject,ininfo->UniformBlockNamesMap);
    GetInputIndices(shaderobj->hProgramObject,ininfo->InputNamesMap);

    shaderobj->ShaderInputInfoIdx = infoidx;

    if(bseparate)
    {
        // Detach All Shaders
        shaderobj->ShadersMask |= ReleaseShaderSeparate(shaderobj->hProgramObject,shaderobj->hVertrexShader,TYPE_VERTEX_SHADER);
        shaderobj->ShadersMask |= ReleaseShaderSeparate(shaderobj->hProgramObject,shaderobj->hFragmentShader,TYPE_PIXEL_SHADER);
        shaderobj->ShadersMask |= ReleaseShaderSeparate(shaderobj->hProgramObject,shaderobj->hGeometryShader,TYPE_GEOMETRY_SHADER);
        shaderobj->ShadersMask |= ReleaseShaderSeparate(shaderobj->hProgramObject,shaderobj->hTessCtrlShader,TYPE_TESS_CONTROL_SHADER);
        shaderobj->ShadersMask |= ReleaseShaderSeparate(shaderobj->hProgramObject,shaderobj->hTessEvalShader,TYPE_TESS_EVALUATION_SHADER);
    }


    //
    //#define GL_TESS_EVALUATION_SHADER         0x8E87
    //#define GL_TESS_CONTROL_SHADER            0x8E88
    //

    //GLint attachedShaders=0;
    //glGetProgramiv(shaderobj->hProgramObject, GL_ATTACHED_SHADERS, &attachedShaders);

    //GLint fmtNum = 0;
    //glGetProgramiv(shaderobj->hProgramObject,GL_NUM_PROGRAM_BINARY_FORMATS,&fmtNum);

    //GLint binaryLength = 0;
    //glGetProgramiv(shaderobj->hProgramObject, GL_PROGRAM_BINARY_LENGTH, &binaryLength);

    //GLsizei ilength = 0;
    //GLenum binaryFormat = 0;
    //GLchar *binaryCode = new GLchar[binaryLength];
   // glGetProgramBinary(shaderobj->hProgramObject, binaryLength, &ilength, &binaryFormat, binaryCode);
    //

    //shd_obj.hVertrexShader

    return objidx;
}
/////////////////////////////////////////////////////////////////////////////////////////////
bool    CGLShaderSystem::DeleteShader(uint32t shaderidx)
{
    CGLShaderSystem::ShaderObject *shaderobj = m_ShaderObjects.GetItem(shaderidx);
    if(!shaderobj)
        return false;

    ReleaseShaderObject(shaderobj);
    m_ShaderObjects.Release(shaderidx);

    return true;
}
/////////////////////////////////////////////////////////////////////////////////////////////
bool    CGLShaderSystem::SetShader(uint32t shaderidx)
{
    if(shaderidx==0)
    {
        glUseProgram(0);
        m_ActiveProgram = 0;
        m_pActiveShaderInInfo = NULL;
        m_bShaderActive = false;
        return true;
    }

    CGLShaderSystem::ShaderObject *shaderobj = m_ShaderObjects.GetItem(shaderidx);
    if(!shaderobj)
        return false;

    glUseProgram(shaderobj->hProgramObject);
    m_bShaderActive = true;
    m_ActiveProgram = shaderobj->hProgramObject;
    m_pActiveShaderInInfo = m_ShaderInputInfoMaps.GetItem(shaderobj->ShaderInputInfoIdx);

    return true;
}
/////////////////////////////////////////////////////////////////////////////////////////////
uint32t CGLShaderSystem::CreateShaderParamsGroup(uint32t reservesize)
{
    //find empty
    uint32t objidx = m_ShaderParamsDescMap.Reserve(1);
    if(objidx==0)
        return 0;

    std::vector<ShaderParamDesc> &params_desc = *m_ShaderParamsDescMap.GetItem(objidx);
    params_desc.reserve(reservesize);

    return objidx;
}
/////////////////////////////////////////////////////////////////////////////////////////////
bool    CGLShaderSystem::DeleteShaderParamsGroup(uint32t groupidx)
{
    tparamsvec *ptr = m_ShaderParamsDescMap.GetItem(groupidx);
    if(ptr==0)
        return false;

    tparamsvec &param_desc = *ptr;
    tparamsvec::iterator desc_iter = param_desc.begin();
    while(desc_iter != param_desc.end())
    {
        ReleaseParamData(desc_iter->ParamType,desc_iter->ParamIdx);
        uint32t name_idx = desc_iter->NameIdx;
        m_ParamNames.RefCounter[name_idx] -= 1;
        if(m_ParamNames.RefCounter[name_idx]==0)
            m_ParamNames.NamesVec.clear();

        desc_iter++;
    }

    m_ShaderParamsDescMap.Release(groupidx);

    return true;
}
/////////////////////////////////////////////////////////////////////////////////////////////
uint32t CGLShaderSystem::AddShaderParam(uint32t groupidx, const char *name, GAPIUNIFORMTYPE type, const void *data)
{
    if(!name)
        return 0;
    tparamsvec *ptr = m_ShaderParamsDescMap.GetItem(groupidx);
    if(ptr==0)
        return 0;

    tparamsvec &paramsdesc = *ptr;
    uint32t param_size = (uint32t)paramsdesc.size();

    CGLShaderSystem::ShaderParamDesc emptydesc;
    tparamsvec::iterator iter = std::find(paramsdesc.begin(),paramsdesc.end(),emptydesc);
    uint32t paramidx = 0;
    if(iter == paramsdesc.end())
    {
        paramsdesc.resize(param_size+1);
        paramidx = param_size;
    }
    else
        paramidx = iter - paramsdesc.begin();

    CGLShaderSystem::ShaderParamDesc &new_desc = paramsdesc[paramidx];
    
    // find name
    std::string strname(name);
    
    std::vector<std::string>::iterator name_iter = std::find(m_ParamNames.NamesVec.begin(),m_ParamNames.NamesVec.end(),strname);
    uint32t nameidx = (name_iter != m_ParamNames.NamesVec.end()) ? (name_iter - m_ParamNames.NamesVec.begin()) : (0);
    if(name_iter == m_ParamNames.NamesVec.end())
    {
        std::vector<uint32t>::iterator ref_iter = std::find(m_ParamNames.RefCounter.begin(),m_ParamNames.RefCounter.end(),0);
        if(ref_iter!=m_ParamNames.RefCounter.end()) {
            nameidx = ref_iter - m_ParamNames.RefCounter.begin();
            m_ParamNames.RefCounter[nameidx] = 0;
            m_ParamNames.NamesVec[nameidx] = strname;
        }
        else {
            nameidx = m_ParamNames.NamesVec.size();
            m_ParamNames.NamesVec.push_back(strname);
            m_ParamNames.RefCounter.push_back(0);
        }   
    }

    m_ParamNames.RefCounter[nameidx] += 1;

    new_desc.NameIdx = nameidx;
    new_desc.ParamType = type;
    new_desc.ParamIdx = CreateParamData(type);
    CopyParamData(type,new_desc.ParamIdx,data);

    return paramidx;
}


/////////////////////////////////////////////////////////////////////////////////////////////
bool    CGLShaderSystem::UpdateShaderParam(uint32t groupidx, uint32t paramidx, const void *data)
{
    tparamsvec *ptr_param = m_ShaderParamsDescMap.GetItem(groupidx);
    if(!ptr_param)
        return false;

    tparamsvec &shaderparams = *ptr_param;
    uint32t params_count = (uint32t)shaderparams.size();
    if(paramidx >= params_count)
        return false;

    CGLShaderSystem::ShaderParamDesc &param_desc = shaderparams[paramidx];
    CopyParamData(param_desc.ParamType,param_desc.ParamIdx,data);

    return true;
}
/////////////////////////////////////////////////////////////////////////////////////////////
bool CGLShaderSystem::SetShaderParams(uint32t shaderidx, uint32t groupidx, uint32t commgroupidx)
{
    ShaderObject *shd_obj = m_ShaderObjects.GetItem(shaderidx);
    tparamsvec *ptr_desc = m_ShaderParamsDescMap.GetItem(groupidx);
    if(!shd_obj || !ptr_desc)
        return false;

    tparamsvec &desc_params = *ptr_desc;
    for(uint32t i = 0; i < desc_params.size(); i++)
    {
        uint32t nameidx = desc_params[i].NameIdx;
        std::string &param_name = m_ParamNames.NamesVec[nameidx];
        SetParam(shd_obj->hProgramObject,desc_params[i].ParamType,desc_params[i].ParamIdx,param_name.c_str());
    }

    // Setting Common Shader Params 
    if(commgroupidx)
    {
        tparamsvec *pdesc2 = m_ShaderParamsDescMap.GetItem(commgroupidx);
        if(pdesc2)
        {
            tparamsvec &comm_desc = *pdesc2;
            for(uint32t i = 0; i < comm_desc.size(); i++)
            {
                uint32t nameidx = comm_desc[i].NameIdx;
                std::string &param_name = m_ParamNames.NamesVec[nameidx];
                SetParam(shd_obj->hProgramObject,comm_desc[i].ParamType,comm_desc[i].ParamIdx,param_name.c_str());
            }
        }
    }

    return true;
}
/////////////////////////////////////////////////////////////////////////////////////////////
uint32t CGLShaderSystem::GetShaderParamIndex(uint32t groupidx, const char *name)
{
    tparamsvec *ptr_param = m_ShaderParamsDescMap.GetItem(groupidx);
    if(!ptr_param)
        return 0xffffffff;

    if(name==NULL)
        return 0xffffffff;

    std::string param_name(name);
    std::vector<std::string>::iterator name_iter = std::find(m_ParamNames.NamesVec.begin(),m_ParamNames.NamesVec.end(),param_name);
    if(name_iter==m_ParamNames.NamesVec.end())
        return 0xffffffff;

    uint32t nameidx = name_iter - m_ParamNames.NamesVec.begin();

    return nameidx;
}

/////////////////////////////////////////////////////////////////////////////////////////////
uint32t CGLShaderSystem::CreateUniformBuffer(GAPIBUFFERUSAGE usage, uint32t bufsize, const void *data)
{
    // Reserving Uniform Buffer Block
    uint32t bufidx = m_UniformBuffersMap.Reserve(1);
    CGLShaderSystem::UniformBufferBlock *bufblock = m_UniformBuffersMap.GetItem(bufidx);

    if(!bufblock) return 0;

    bufblock->UniformBufferSize = bufsize;
    bufblock->UniformBufferUsage = CGLConvFunc::GetGLBufferDataUsage(usage);

    // Generating Uniform Buffer Object
    glGenBuffers(1,&bufblock->UniformBufferID);
    if(!bufblock->UniformBufferID) // Failed To Create Buffer ?
    {
        GLenum error = glGetError();
        m_UniformBuffersMap.Release(bufidx,1);
        return 0;
    }

    /*
    // Getting Binding Point For Uniform Block
    bufblock->BindingPoint = this->GetBindingPoint();
    */

    // Bind Buffer And Copy Data
    glBindBuffer(GL_UNIFORM_BUFFER, bufblock->UniformBufferID);
    glBufferData(GL_UNIFORM_BUFFER, bufsize, data, bufblock->UniformBufferUsage);

    /*
    // Setting Binding Point For Uniform Buffer Object 
    glBindBufferBase(GL_UNIFORM_BUFFER, bufblock->BindingPoint, bufblock->UniformBufferID);
    */
    // Unbinding Uniform Buffer Object
    glBindBuffer(GL_UNIFORM_BUFFER, 0);

    return bufidx;
}


/////////////////////////////////////////////////////////////////////////////////////////////
bool CGLShaderSystem::DeleteUniformBuffer(uint32t bufidx)
{
    UniformBufferBlock *unibuf = m_UniformBuffersMap.GetItem(bufidx);
    if(!unibuf)
        return false;

    glDeleteBuffers(1,&unibuf->UniformBufferID);
    //--ReleaseBindingPoint(unibuf->BindingPoint);
    memset(unibuf,0,sizeof(UniformBufferBlock));
    m_UniformBuffersMap.Release(bufidx,1);
    
    return true;
}
/////////////////////////////////////////////////////////////////////////////////////////////
bool CGLShaderSystem::UpdateUniformBuffer(uint32t bufidx, uint32t offset, uint32t size, const void *data)
{
    UniformBufferBlock *unibuf = m_UniformBuffersMap.GetItem(bufidx);
    if(!unibuf)
        return false;

    if(size > unibuf->UniformBufferSize)
        return false;

    glBindBuffer(GL_UNIFORM_BUFFER,unibuf->UniformBufferID);
    glBufferSubData(GL_UNIFORM_BUFFER,offset,size,data);
    glBindBuffer(GL_UNIFORM_BUFFER,0);

    return true;
}

/////////////////////////////////////////////////////////////////////////////////////////////

//Binding point cut out. Could not work!!!
bool CGLShaderSystem::AddUniformBlocks(uint32t shaderidx, uint32t *bufidx, const char **names, uint32t count)
{
    CGLShaderSystem::ShaderObject *shd_obj = m_ShaderObjects.GetItem(shaderidx);
    if(!shd_obj)
        return false;

    // Allocating Uniform Blocks Desc Vector 
    if(!shd_obj->UniformBlocksDescIdx)
    {
        uint32t descidx = m_UniformBlocksDescMap.Reserve(1);
        if(!descidx) return false;

        shd_obj->UniformBlocksDescIdx = descidx;
        tblockdescvec *pvec = m_UniformBlocksDescMap.GetItem(descidx);
        pvec->reserve(count);
    }

    uint32t descidx = shd_obj->UniformBlocksDescIdx;
    tblockdescvec *pvec = m_UniformBlocksDescMap.GetItem(descidx);

    // Getting Uniform Blocks
    for(uint32t i = 0; i < count; i++)
    {
        UniformBufferBlock *unibuf = m_UniformBuffersMap.GetItem(bufidx[i]);
        if(!unibuf) // Oops! Wrong Buffer Index!
            return false;

        const char *pname = names[i];
        GLuint blockindex = glGetUniformBlockIndex(shd_obj->hProgramObject,pname);
        GLenum error1 = glGetError();
        GLint blocksize = 0;
        // Getting Uniform Block Size
        glGetActiveUniformBlockiv(shd_obj->hProgramObject, blockindex, GL_UNIFORM_BLOCK_DATA_SIZE, &blocksize);
        GLenum error2 = glGetError();
        if(blocksize != (GLint)unibuf->UniformBufferSize) // Oops! Wrong Buffer Sizes!
            return false;

        /*
        GLuint bindpoint = unibuf->BindingPoint; 
        glUniformBlockBinding(shd_obj->hProgramObject, blockindex, bindpoint);
        */

        UniformBlockDesc blockdesc;
        /*
        //--blockdesc.BlockBindingPoint = bindpoint;
        */
        blockdesc.UniformBlockIndex = blockindex;
        pvec->push_back(blockdesc);
    }
    
    return true;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Setting Uniforms And Uniform Buffers Directly
bool CGLShaderSystem::SetUniform(GAPIUNIFORMTYPE type, const char *name, const void *data)
{
    if(m_bProgramPipelineActive || !m_bShaderActive)
        return false;

    if(!m_pActiveShaderInInfo->UniformNamesMap.count(name))
        return false;

    GLint uniloc = m_pActiveShaderInInfo->UniformNamesMap[name];
    //If not fount
    if(uniloc==-1)
        uniloc = glGetUniformLocation(m_ActiveProgram,name);
    SetUniform(type,uniloc,data);

    return true;
    /*
        GLhandleARB m_ActiveProgram;
    bool m_bShaderActive;
    bool m_bProgramPipelineActive;
    */
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool CGLShaderSystem::SetProgramUniform(uint32t shaderidx, GAPIUNIFORMTYPE type, const char *name, const void *data)
{
    if(!m_bProgramPipelineActive)
        return false;

    ShaderObject *shaderobj = m_ShaderObjects.GetItem(shaderidx);
    if(!shaderobj) return false;

    ShaderInputInfo *ininfo = m_ShaderInputInfoMaps.GetItem(shaderobj->ShaderInputInfoIdx);
    GLint uniloc = ininfo->UniformNamesMap[name];
    //If not fount
    if(uniloc==-1) 
        uniloc = glGetUniformLocation(shaderobj->hProgramObject,name);
    SetProgramUniform(shaderobj->hProgramObject,type,uniloc,data);
    return true;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool CGLShaderSystem::SetUniforms(GAPIUNIFORMDATA *data, uint32t count)
{
    if(m_bProgramPipelineActive || !m_bShaderActive)
        return false;

    for(uint32t i = 0; i < count; i++)
    {
        if(!m_pActiveShaderInInfo->UniformNamesMap.count(data[i].Name))
            return false;

        GLint uniloc = m_pActiveShaderInInfo->UniformNamesMap[data[i].Name];
        //If not fount
        if(uniloc==-1)
            uniloc = glGetUniformLocation(m_ActiveProgram,data[i].Name);
        SetUniform(data[i].Type,uniloc,data[i].Data);
    }
    return true;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool CGLShaderSystem::SetProgramUniforms(uint32t shaderidx, GAPIUNIFORMDATA *data, uint32t count)
{
    if(!m_bProgramPipelineActive)
        return false;

    ShaderObject *shaderobj = m_ShaderObjects.GetItem(shaderidx);
    if(!shaderobj) return false;

    ShaderInputInfo *ininfo = m_ShaderInputInfoMaps.GetItem(shaderobj->ShaderInputInfoIdx);

    for(uint32t i = 0; i < count; i++)
    {
        GLint uniloc = ininfo->UniformNamesMap[data[i].Name];
        //If not fount
        if(uniloc==-1)
            uniloc = glGetUniformLocation(m_ActiveProgram,data[i].Name);
        SetProgramUniform(shaderobj->hProgramObject,data[i].Type,uniloc,data[i].Data);
    }

    return true;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
uint32t CGLShaderSystem::GetUniformBlockSize(uint32t shaderidx, const char *blockname)
{
    ShaderObject *shaderobj = m_ShaderObjects.GetItem(shaderidx);
    ShaderInputInfo *ininfo = m_ShaderInputInfoMaps.GetItem(shaderobj->ShaderInputInfoIdx);
    GLint uniblocksize = 0;

    if(!ininfo->UniformBlockNamesMap.count(blockname)) 
        return 0;

    GLuint uniblockindex = ininfo->UniformBlockNamesMap[blockname];

    //Getting UniformBlock Size
    glGetActiveUniformBlockiv(shaderobj->hProgramObject,uniblockindex,GL_UNIFORM_BLOCK_DATA_SIZE, &uniblocksize);

    return (uint32t)uniblocksize;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool CGLShaderSystem::GetActiveUniformInfo(uint32t shaderidx, GAPIACTIVEUNIFORMTYPE type, const char **names, uint32t *pvalues, uint32t count)
{
    /*
    GAPI_UNIFORM_SIZE,
    GAPI_UNIFORM_OFFSET,
    */
    GLenum ActiveUniTypeTab[]={GL_UNIFORM_SIZE,GL_UNIFORM_OFFSET};
    ShaderObject *shaderobj = m_ShaderObjects.GetItem(shaderidx);
    ShaderInputInfo *ininfo = m_ShaderInputInfoMaps.GetItem(shaderobj->ShaderInputInfoIdx);

    std::vector<GLuint> indices;
    indices.resize(count);

    // Getting Uniform Indices
    glGetUniformIndices(shaderobj->hProgramObject,count,names,&indices[0]);

    // Getting Active Uniforms Sizes
    GLenum gltype = ActiveUniTypeTab[type];
    glGetActiveUniformsiv(shaderobj->hProgramObject,count,&indices[0],gltype,(GLint*)pvalues);

    return true;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool CGLShaderSystem::SetUniformBlockBinding(uint32t shaderidx, const char *blockname, uint32t bindingpoint)
{
    ShaderObject *shaderobj = m_ShaderObjects.GetItem(shaderidx);
    if(!shaderobj)
        return false;

    ShaderInputInfo *ininfo = m_ShaderInputInfoMaps.GetItem(shaderobj->ShaderInputInfoIdx);

    if(!ininfo->UniformBlockNamesMap.count(blockname))
        return false;

    GLuint uniblockindex = ininfo->UniformBlockNamesMap[blockname];

    glUniformBlockBinding (shaderobj->hProgramObject, uniblockindex, (GLuint)bindingpoint);

    return true;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
uint32t CGLShaderSystem::GetAttribLocation(uint32t shaderidx, const char *name)
{
    ShaderObject *shaderobj = m_ShaderObjects.GetItem(shaderidx);
    if(!shaderobj)
        return (-1);

    ShaderInputInfo *ininfo = m_ShaderInputInfoMaps.GetItem(shaderobj->ShaderInputInfoIdx);
    if(!ininfo->InputNamesMap.count(name)) {
        return (-1);
    }

    GLuint attribloc = ininfo->InputNamesMap[name];

    return attribloc;
}

/////////////////////////////////////////////////////////////////////////////////////////////
bool CGLShaderSystem::SetUniformBufferBindingPoint(uint32t bufidx, uint32t bindpoint)
{
    CGLShaderSystem::UniformBufferBlock *bufblock = m_UniformBuffersMap.GetItem(bufidx);
    if(!bufblock) return false;

    // Binding Uniform Buffer
    glBindBuffer(GL_UNIFORM_BUFFER, bufblock->UniformBufferID);

    // Setting Binding Point For Uniform Buffer Object 
    glBindBufferBase(GL_UNIFORM_BUFFER, (GLuint)bindpoint, bufblock->UniformBufferID);

    // Unbinding Uniform Buffer Object
    glBindBuffer(GL_UNIFORM_BUFFER, 0);

    return true;
}

/////////////////////////////////////////////////////////////////////////////////////////////
/*--------------------------------------
uint32t CGLShaderSystem::GetUniformBufferBindingPoint(uint32t bufidx)
{
    CGLShaderSystem::UniformBufferBlock *bufblock = m_UniformBuffersMap.GetItem(bufidx);
    if(!bufblock) return (-1);

    return bufblock->BindingPoint;
}
--------------------------------------*/

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Separate Shader Objects Management

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
uint32t CGLShaderSystem::CreateProgramPipeline(uint32t *pshadersidx, uint32t count)
{
    if(!pshadersidx || !count)
        return 0;

    // Generating Program Pipeline
    GLuint pipelineid = 0;
    glGenProgramPipelines(1,&pipelineid);
    if(!pipelineid) 
        return 0;

    // Binding Program Pipeline
    glBindProgramPipeline(pipelineid);
    GLbitfield progmask = 0;

    // Set Program Stages
    for(uint32t sidx = 0; sidx < count; sidx++)
    {
        // Retrieving Shader Object That Should Separate Shader Object
        uint32t shader_idx = pshadersidx[sidx];
        ShaderObject *shaderobj = m_ShaderObjects.GetItem(shader_idx);
        if(!shaderobj) {
            glBindProgramPipeline(0);
            glDeleteProgramPipelines(1,&pipelineid);
            return 0;
        }
        if(!shaderobj->ShadersMask) {
            glBindProgramPipeline(0);
            glDeleteProgramPipelines(1,&pipelineid);
            return 0;
        }
        
        if(!(progmask & shaderobj->ShadersMask)) {
            glBindProgramPipeline(0);
            glDeleteProgramPipelines(1,&pipelineid);
            return 0;
        }

        // Setting Program Stages
        glUseProgramStages(pipelineid,shaderobj->ShadersMask,shaderobj->hProgramObject);
        progmask |= shaderobj->ShadersMask;
    }

    // Validating Program Pipeline
    GLint status = 0;
    glValidateProgramPipeline( pipelineid );
    glGetProgramPipelineiv( pipelineid, GL_VALIDATE_STATUS, &status );

    GLint loglen = 0;
    GLint charswritten = 0;
    glGetProgramPipelineiv(pipelineid, GL_INFO_LOG_LENGTH, &loglen );

    m_strPipelineInfoLog.clear();
    m_strPipelineInfoLog.resize(loglen);

    // Getting Pipeline Info Log
    glGetProgramPipelineInfoLog(pipelineid, loglen, &charswritten, (GLchar*)m_strPipelineInfoLog.c_str());

    glBindProgramPipeline(0);

    // Delete Program Pipeline If Failed
    if(status != 0) {
        glDeleteProgramPipelines(1,&pipelineid);
        return 0;
    }

    // Copy Pipeline ID To Array Map
    uint32t objidx = m_PipelineObjects.Reserve(1);
    GLuint *ptr = m_PipelineObjects.GetItem(objidx);
    *ptr = pipelineid;

    return objidx;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool CGLShaderSystem::DeleteProgramPipeline(uint32t pipelineidx)
{
    GLuint *pipelineid = m_PipelineObjects.GetItem(pipelineidx);
    if(!pipelineid)
        return false;

    glDeleteProgramPipelines(1,pipelineid);
    m_PipelineObjects.Release(pipelineidx,1);
    
    return true;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool CGLShaderSystem::SetProgramPipeline(uint32t pipelineidx)
{
    if(pipelineidx==0)
    {
        glBindProgramPipeline(0);
        m_bProgramPipelineActive = false;
        return true;
    }

    GLuint *pipeline_ptr = m_PipelineObjects.GetItem(pipelineidx);
    if(!pipeline_ptr)
        return false;

    GLuint pipeline = *pipeline_ptr;
    glBindProgramPipeline(pipeline);
    m_bProgramPipelineActive = true;

    return true;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool CGLShaderSystem::SetSeparateShadersParams(uint32t *pshadersidx, uint32t *pgroupsidx, uint32t count)
{
    if(!pshadersidx || !pgroupsidx || !count)
        return false;

    for(uint32t i = 0; i < count; i++)
    {
        if(pgroupsidx[i]==0)
            continue;
        ShaderObject *shd_obj = m_ShaderObjects.GetItem(pshadersidx[i]);
        tparamsvec *ptr_desc = m_ShaderParamsDescMap.GetItem(pgroupsidx[i]);
        if(!shd_obj || !ptr_desc)
        return false;

        tparamsvec &desc_params = *ptr_desc;
        for(uint32t i = 0; i < desc_params.size(); i++)
        {
            uint32t nameidx = desc_params[i].NameIdx;
            std::string &param_name = m_ParamNames.NamesVec[nameidx];
            SetProgramParam(shd_obj->hProgramObject,desc_params[i].ParamType,desc_params[i].ParamIdx,param_name.c_str());
        }
        
    }

    return true;
}

/////////////////////////////////////////////////////////////////////////////////////////////
GLhandleARB CGLShaderSystem::GetShaderProgram(uint32t shaderidx)
{
    CGLShaderSystem::ShaderObject *shaderobj = m_ShaderObjects.GetItem(shaderidx);
    if(!shaderobj)
        return 0;

    return shaderobj->hProgramObject;
}

/////////////////////////////////////////////////////////////////////////////////////////////
GLhandleARB CGLShaderSystem::GetActiveShaderProgram() const
{
    return m_ActiveProgram;
}


/////////////////////////////////////////////////////////////////////////////////////////////
// Retrieving Ids From Shader 
GLint CGLShaderSystem::GetVertexAttribLocation(uint32t shaderidx, const char *attribname)
{
    GLhandleARB hProgram = this->GetShaderProgram(shaderidx);
    GLint loc = glGetAttribLocation(hProgram,attribname);
    return loc;
}
/////////////////////////////////////////////////////////////////////////////////////////////
GLint CGLShaderSystem::GetVertexAttribLocation(const char *attribname)
{
    GLhandleARB hProgram = this->GetActiveShaderProgram();
    GLint loc = glGetAttribLocation(hProgram,attribname);
    return loc;
}

/////////////////////////////////////////////////////////////////////////////////////////////
// Fragment Data Location 
void CGLShaderSystem::SetFragDataLocation(uint32t shaderidx, uint32t colornumber, const char *name)
{
    GLhandleARB hProgram = this->GetShaderProgram(shaderidx);
    glBindFragDataLocation(hProgram,colornumber,name);
}

/////////////////////////////////////////////////////////////////////////////////////////////    
void CGLShaderSystem::SetFragDataLocation(uint32t shaderidx, uint32t *colorsnumbers, const char **names, uint32t count)
{
    GLhandleARB hProgram = this->GetShaderProgram(shaderidx);
    for(uint32t i = 0; i < count; i++) {
        glBindFragDataLocation(hProgram,colorsnumbers[i],names[i]);
    }
}
/////////////////////////////////////////////////////////////////////////////////////////////
GLint CGLShaderSystem::GetFragDataLocation(uint32t shaderidx, const char *name)
{
    GLhandleARB hProgram = this->GetShaderProgram(shaderidx);
    GLint loc = glGetFragDataLocation(hProgram,name);
    return loc;
}

/////////////////////////////////////////////////////////////////////////////////////////////
// Blend Func Extended
void CGLShaderSystem::SetFragDataLocationIndexed(uint32t shaderidx, uint32t colornumber, uint32t index, const char *name)
{
    GLhandleARB hProgram = this->GetShaderProgram(shaderidx);
    glBindFragDataLocationIndexed(hProgram,colornumber,index,name);
}

/////////////////////////////////////////////////////////////////////////////////////////////
void CGLShaderSystem::SetFragDataLocationIndexed(uint32t shaderidx, uint32t *colorsnumbers, uint32t *indices, const char **names, uint32t count)
{
    GLhandleARB hProgram = this->GetShaderProgram(shaderidx);
    for(uint32t i = 0; i < count; i++) {
        glBindFragDataLocationIndexed(hProgram,colorsnumbers[i],indices[i],names[i]);
    }
}

/////////////////////////////////////////////////////////////////////////////////////////////
GLint CGLShaderSystem::GetFragDataIndex(uint32t shaderidx, const char *name)
{
    GLhandleARB hProgram = this->GetShaderProgram(shaderidx);
    GLint index = glGetFragDataIndex(hProgram,name);
    return index;
}


/////////////////////////////////////////////////////////////////////////////////////////////
// Protected Methods

/////////////////////////////////////////////////////////////////////////////////////////////
void CGLShaderSystem::GetShaderSource(const std::string &source, std::string &strout, CGLShaderSystem::ShaderType shd_type)
{
    const char shader_types[5][9]={{"VERTEX"},{"PIXEL"},{"GEOMETRY"},{"TESS"},{"TESS"}};
    const char shader_sec_type[5][11]={{""},{""},{""},{"EVALUATION"},{"CONTROL"}};
    const char shader_word[]={"SHADER"};

    size_t type_pos1 = source.find("[[");
    size_t type_pos2 = 0;
    while(type_pos1 != std::string::npos)
    {
        type_pos2 = source.find("]]",type_pos1);
        if(type_pos2==std::string::npos)//error no correct shader name
            return ;

        type_pos1 += 2;
        std::string shader_name = source.substr(type_pos1,type_pos2-type_pos1);
        std::transform(shader_name.begin(),shader_name.end(),shader_name.begin(),CGLShaderSystem::toupper);

        size_t npos1 = shader_name.find(shader_types[shd_type]);
        size_t npos2 = shader_name.find(shader_word);
        size_t npos3 = shader_name.find(shader_sec_type[shd_type]);

        bool bnext = (npos1 == std::string::npos) || (npos2 == std::string::npos);
        bnext = bnext || ((shd_type>TYPE_GEOMETRY_SHADER)?(npos3 == std::string::npos):(false));
        if(bnext)
        {
            type_pos1 = source.find("[[",type_pos2+2);
            continue;
        }
        
        size_t spos1 = type_pos2 + 2;
        size_t spos2 = source.find("[[",spos1);
        spos2 = (spos2 != std::string::npos) ? (spos2) : (source.size());
        
        // Getting Shader Source Code
        strout = source.substr(spos1,spos2-spos1);
        return ;
    }

}

/////////////////////////////////////////////////////////////////////////////////////////////
void CGLShaderSystem::GetGeomtryShaderParams(const std::string &source,GLint &input_type, GLint &output_type, GLint &vert_num)
{
    size_t type_pos1 = source.find("[[");
    size_t type_pos2 = 0;
    while(type_pos1 != std::string::npos)
    {
        type_pos2 = source.find("]]",type_pos1);
        if(type_pos2==std::string::npos)
            return ;

        type_pos1 += 2;
        std::string str_info = source.substr(type_pos1,type_pos2-type_pos1);
        std::transform(str_info.begin(),str_info.end(),str_info.begin(),CGLShaderSystem::toupper);

        size_t next_pos = source.find("[[");
        next_pos = (next_pos==std::string::npos)?(source.size()):(next_pos);

        if(str_info.find("INPUT") != std::string::npos) {
            if(str_info.find("GEOMETRY") != std::string::npos) {
                std::string str_type = source.substr(type_pos2+2,next_pos-type_pos2);
                std::transform(str_type.begin(),str_type.end(),str_type.begin(),CGLShaderSystem::toupper);
                input_type = GetGeometryType(str_type);
            }
        }

        if(str_info.find("OUTPUT") != std::string::npos) {
            if(str_info.find("GEOMETRY") != std::string::npos) {
                std::string str_type = source.substr(type_pos2+2,next_pos-type_pos2);
                std::transform(str_type.begin(),str_type.end(),str_type.begin(),CGLShaderSystem::toupper);
                output_type = GetGeometryType(str_type);
            }
        }

        if(str_info.find("VERT") != std::string::npos) {
            if(str_info.find("NUM") != std::string::npos) {
                if(str_info.find("GEOMETRY") != std::string::npos) {
                    std::string str_num = source.substr(type_pos2+2,next_pos-type_pos2);
                    std::transform(str_num.begin(),str_num.end(),str_num.begin(),CGLShaderSystem::toupper);
                    if(str_num.find("MAX_VERTICES")!=std::string::npos)
                        vert_num = 0;
                    else
                        vert_num = atoi(str_num.c_str());
                }
            }
        }
    }
}

/////////////////////////////////////////////////////////////////////////////////////////////
GLenum CGLShaderSystem::GetGeometryType(const std::string &strgeotype)
{
    GLenum geometry_type = 0;
    if(strgeotype.find("LINE")!=std::string::npos)
    {
        bool badj = strgeotype.find("ADJ") != std::string::npos;
        bool bloop = strgeotype.find("LOOP") != std::string::npos;
        bool bstrip = strgeotype.find("STRIP") != std::string::npos;
        if(bloop)
            geometry_type = GL_LINE_LOOP;
        else if(bstrip)
            geometry_type = (badj) ? (GL_LINE_STRIP_ADJACENCY_ARB) : (GL_LINE_STRIP);
        else 
            geometry_type = (badj) ? (GL_LINES_ADJACENCY_ARB) : (GL_LINES);
    }
    else if(strgeotype.find("TRAINGLE")!=std::string::npos)
    {
        bool badj = strgeotype.find("ADJ") != std::string::npos;
        bool bfan = strgeotype.find("FAN") != std::string::npos;
        bool bstrip = strgeotype.find("STRIP") != std::string::npos;

        if(bfan)
            geometry_type = GL_TRIANGLE_FAN;
        else if(bstrip)
            geometry_type = (badj)?(GL_TRIANGLE_STRIP_ADJACENCY_ARB):(GL_TRIANGLE_STRIP);
        else 
            geometry_type = (badj)?(GL_TRIANGLES_ADJACENCY_ARB):(GL_TRIANGLES);

    }
    else if(strgeotype.find("POINT")!=std::string::npos)
    {
        geometry_type = GL_POINTS;
    }
   
    return geometry_type;
}

/////////////////////////////////////////////////////////////////////////////////////////////
GLhandleARB CGLShaderSystem::CompileShader(const std::string &src_code, GLenum shaderType)
{
    // Creating Shader Object
    GLhandleARB shader_handle = glCreateShader(shaderType);
    if(!shader_handle) // Failed To Create Shader Object
        return 0;

    // Loading Source Code Into Shader 
    GLint code_len = (GLint)src_code.length();
    const char *str_code = src_code.c_str();
    glShaderSource(shader_handle,1,&str_code,&code_len);

    // Compile Shader From Source
    glCompileShader(shader_handle);

    return shader_handle;
}

/////////////////////////////////////////////////////////////////////////////////////////////
void CGLShaderSystem::SetShaderInfoLog(GLhandleARB hShader)
{
    GLint loglen = 0;
    GLint charswritten = 0;
    glGetShaderiv(hShader, GL_INFO_LOG_LENGTH, &loglen);
    if(loglen==0)
        return ;

    m_strShaderInfoLog.clear();
    m_strShaderInfoLog.resize(loglen);
    glGetShaderInfoLog(hShader, loglen, &charswritten, (GLchar *)m_strShaderInfoLog.c_str());
}
/////////////////////////////////////////////////////////////////////////////////////////////
void CGLShaderSystem::SetProgramInfoLog(GLhandleARB hProgram)
{
    GLint loglen = 0;
    GLint charswritten = 0;
    glGetProgramiv(hProgram, GL_INFO_LOG_LENGTH, &loglen);
    if(loglen==0)
        return ;

    m_strShaderInfoLog.clear();
    m_strShaderInfoLog.resize(loglen);
    glGetProgramInfoLog(hProgram, loglen, &charswritten, (GLchar *)m_strProgramInfoLog.c_str());
}
/////////////////////////////////////////////////////////////////////////////////////////////
void CGLShaderSystem::ReleaseShaderObject(CGLShaderSystem::ShaderObject *shaderobj)
{
    /*
    if(shaderobj->bSeparateShaders)
    {
        if(shaderobj->hVertrexShader)
            glDeleteProgram(shaderobj->hVertrexShader);
        if(shaderobj->hFragmentShader)
            glDeleteProgram(shaderobj->hFragmentShader);
        if(shaderobj->hTessCtrlShader)
            glDeleteProgram(shaderobj->hTessCtrlShader);
        if(shaderobj->hTessEvalShader)
            glDeleteProgram(shaderobj->hTessEvalShader);
        if(shaderobj->hGeometryShader)
            glDeleteProgram(shaderobj->hGeometryShader);
        return ;
    }
    */
    // Deleting Vertex Shader Object
    if(shaderobj->hVertrexShader)
    {
        GLint shader_type;
        glGetShaderiv(shaderobj->hVertrexShader,GL_SHADER_TYPE,&shader_type);

        glDetachShader(shaderobj->hProgramObject, shaderobj->hVertrexShader);
        glDeleteShader(shaderobj->hVertrexShader);
        shaderobj->hVertrexShader = 0;
    }

    // Deleting Fragment Shader Object
    if(shaderobj->hFragmentShader)
    {
        glDetachShader(shaderobj->hProgramObject, shaderobj->hFragmentShader);
        glDeleteShader(shaderobj->hFragmentShader);
        shaderobj->hVertrexShader = 0;
    }

    //Deleting Geometry Shader Object
    if(shaderobj->hGeometryShader)
    {
        glDetachShader(shaderobj->hProgramObject, shaderobj->hGeometryShader);
        glDeleteShader(shaderobj->hGeometryShader);
        shaderobj->hGeometryShader = 0;
    }

    //Deleting Tessellation Control Shader Object
    if(shaderobj->hTessCtrlShader)
    {
        glDetachShader(shaderobj->hProgramObject, shaderobj->hTessCtrlShader);
        glDeleteShader(shaderobj->hTessCtrlShader);
        shaderobj->hTessCtrlShader = 0;
    }

    //Deleting Tessellation Evaluation Shader Object
    if(shaderobj->hTessEvalShader)
    {
        glDetachShader(shaderobj->hProgramObject, shaderobj->hTessEvalShader);
        glDeleteShader(shaderobj->hTessEvalShader);
        shaderobj->hTessCtrlShader = 0;
    }

    //Deleting Program Object
    if(shaderobj->hProgramObject)
    {
        glDeleteProgram(shaderobj->hProgramObject);
        shaderobj->hProgramObject = 0;
    }

}

////////////////////////////////////////////////////////////////////////////////////////////////
void CGLShaderSystem::ReleaseShaders()
{
    //CArrayMap<CGLShaderSystem::ShaderObject,SHADER_OBJECTS_COUNT> m_ShaderObjects;
    unsigned objsize = m_ShaderObjects.Size();
    for(unsigned i = 0; i < objsize; i++)
    {
        CGLShaderSystem::ShaderObject *shaderobj = m_ShaderObjects.GetItem(i);
        if(shaderobj->hProgramObject)
            ReleaseShaderObject(shaderobj);
    }
    m_ShaderObjects.Clear();
}

////////////////////////////////////////////////////////////////////////////////////////////////
// Creating Separate Shader Object
GLhandleARB CGLShaderSystem::CreateSeparateShaderProgram(const std::string &strsource, GLenum glshadertype)
{
    GLhandleARB hObject = glCreateShaderProgramv(glshadertype,1,(const GLchar* const *)strsource.c_str());
    //Check Link Status
    GLint linked = 0;
    glGetProgramiv(hObject, GL_LINK_STATUS, &linked);
    if(!linked)
    {
        SetProgramInfoLog(hObject);
        glDeleteProgram( hObject );
        return 0;
    }

    return hObject;
}

////////////////////////////////////////////////////////////////////////////////////////////////
GLbitfield CGLShaderSystem::ReleaseShaderSeparate(GLhandleARB hProgram, GLhandleARB &hShader, ShaderType shdtype)
{
    static const GLbitfield shd_bitfields[] = {GL_VERTEX_SHADER_BIT,GL_FRAGMENT_SHADER_BIT,GL_GEOMETRY_SHADER_BIT,
                                        GL_TESS_CONTROL_SHADER_BIT,GL_TESS_EVALUATION_SHADER_BIT};

    if(!hShader || !hProgram) return 0;

    glDetachShader(hProgram,hShader);
    glDeleteShader(hShader);
    hShader = 0;

    return shd_bitfields[shdtype];
}


////////////////////////////////////////////////////////////////////////////////////////////////
uint32t CGLShaderSystem::CreateParamData(GAPIUNIFORMTYPE ParamType)
{
    uint32t uniformidx = 0;
    switch(ParamType)
    {
    case GAPI_UNIFORM_INT:
        uniformidx = m_IntParams.Reserve(1);
        break;
    case GAPI_UNIFORM_UINT:
        uniformidx = m_UIntParams.Reserve(1);
        break;

    case GAPI_UNIFORM_FLOAT:
        uniformidx = m_FloatParams.Reserve(1);
        break;

    case GAPI_UNIFORM_DOUBLE:
        uniformidx = m_DoubleParams.Reserve(1);
        break;

    case GAPI_UNIFORM_VECTOR2I:
        uniformidx = m_IntParams.Reserve(2);
        break;
    case GAPI_UNIFORM_VECTOR3I:
        uniformidx = m_IntParams.Reserve(3);
        break;
    case GAPI_UNIFORM_VECTOR4I:
        uniformidx = m_IntParams.Reserve(4);
        break;

    case GAPI_UNIFORM_VECTOR2UI:
        uniformidx = m_UIntParams.Reserve(2);
        break;
    case GAPI_UNIFORM_VECTOR3UI:
        uniformidx = m_UIntParams.Reserve(3);
        break;
    case GAPI_UNIFORM_VECTOR4UI:
        uniformidx = m_UIntParams.Reserve(4);
        break;

    case GAPI_UNIFORM_VECTOR2F:
        uniformidx = m_Vec2fParams.Reserve(1);
        break;
    case GAPI_UNIFORM_VECTOR3F:
        uniformidx = m_Vec3fParams.Reserve(1);
        break;
    case GAPI_UNIFORM_VECTOR4F:
        uniformidx = m_Vec4fParams.Reserve(1);
        break;
    case GAPI_UNIFORM_VECTOR2D:
        uniformidx = m_Vec2dParams.Reserve(1);
        break;
    case GAPI_UNIFORM_VECTOR3D:
        uniformidx = m_Vec2dParams.Reserve(1);
        break;
    case GAPI_UNIFORM_VECTOR4D:
        uniformidx = m_Vec2dParams.Reserve(1);
        break;

    case GAPI_UNIFORM_MTX33F:
        uniformidx = m_Vec3fParams.Reserve(3);
        break;
    case GAPI_UNIFORM_MTX44F:
        uniformidx = m_Vec4fParams.Reserve(4);
        break;
    case GAPI_UNIFORM_MTX33D:
        uniformidx = m_Vec3fParams.Reserve(3);
        break;
    case GAPI_UNIFORM_MTX44D:
        uniformidx = m_Vec4fParams.Reserve(4);
        break;
    }

    return uniformidx;
}

////////////////////////////////////////////////////////////////////////////////////////////////
bool    CGLShaderSystem::CopyParamData(GAPIUNIFORMTYPE ParamType, uint32t idx, const void *data)
{
    if(idx==0)
        return false;

    int size = 0;
    void *dest = NULL;

    switch(ParamType)
    {
    case GAPI_UNIFORM_INT:
        dest = m_IntParams.GetItem(idx);
        size = sizeof(int32t);
        break;

    case GAPI_UNIFORM_UINT:
        dest = m_UIntParams.GetItem(idx);
        size = sizeof(int32t);
        break;

    case GAPI_UNIFORM_FLOAT:
        dest = m_FloatParams.GetItem(idx);
        size = sizeof(float);
        break;

    case GAPI_UNIFORM_DOUBLE:
        dest = m_DoubleParams.GetItem(idx);
        size = sizeof(float);
        break;

    case GAPI_UNIFORM_VECTOR2I:
        dest = m_IntParams.GetItem(idx);
        size = sizeof(float)*2;
        break;

    case GAPI_UNIFORM_VECTOR3I:
        dest = m_IntParams.GetItem(idx);
        size = sizeof(float)*3;
        break;

    case GAPI_UNIFORM_VECTOR4I:
        dest = m_IntParams.GetItem(idx);
        size = sizeof(float)*4;
        break;

    case GAPI_UNIFORM_VECTOR2UI:
        dest = m_UIntParams.GetItem(idx);
        size = sizeof(float)*2;
        break;

    case GAPI_UNIFORM_VECTOR3UI:
        dest = m_UIntParams.GetItem(idx);
        size = sizeof(float)*3;
        break;

    case GAPI_UNIFORM_VECTOR4UI:
        dest = m_UIntParams.GetItem(idx);
        size = sizeof(float)*4;
        break;


    case GAPI_UNIFORM_VECTOR2F:
        dest = m_Vec2fParams.GetItem(idx);
        size = sizeof(float)*2;
        break;

    case GAPI_UNIFORM_VECTOR3F:
        dest = m_Vec3fParams.GetItem(idx);
        size = sizeof(float)*3;
        break;

    case GAPI_UNIFORM_VECTOR4F:
        dest = m_Vec4fParams.GetItem(idx);
        size = sizeof(float)*4;
        break;

    case GAPI_UNIFORM_VECTOR2D:
        dest = m_Vec2dParams.GetItem(idx);
        size = sizeof(float)*2;
        break;

    case GAPI_UNIFORM_VECTOR3D:
        dest = m_Vec3dParams.GetItem(idx);
        size = sizeof(float)*3;
        break;

    case GAPI_UNIFORM_VECTOR4D:
        dest = m_Vec4dParams.GetItem(idx);
        size = sizeof(float)*4;
        break;



    case GAPI_UNIFORM_MTX33F:
        dest = m_Vec3fParams.GetItem(idx);
        size = sizeof(float)*9;
        break;

    case GAPI_UNIFORM_MTX44F:
        dest = m_Vec4fParams.GetItem(idx);
        size = sizeof(float)*16;
        break;

    case GAPI_UNIFORM_MTX33D:
        dest = m_Vec3dParams.GetItem(idx);
        size = sizeof(float)*9;
        break;

    case GAPI_UNIFORM_MTX44D:
        dest = m_Vec4dParams.GetItem(idx);
        size = sizeof(float)*16;
        break;
    }

    if(!dest)
        return false;

    if(data)
        memcpy(dest,data,size);
    else
        memset(dest,0,size);

    return true;
}

////////////////////////////////////////////////////////////////////////////////////////////////
void    CGLShaderSystem::ReleaseParamData(GAPIUNIFORMTYPE ParamType, uint32t idx)
{
    if(idx == 0)
    return ;

    switch(ParamType)
    {
    case GAPI_UNIFORM_INT:
        m_IntParams.Release(idx,1);
        break;

    case GAPI_UNIFORM_UINT:
        m_UIntParams.Release(idx,1);
        break;

    case GAPI_UNIFORM_FLOAT:
        m_FloatParams.Release(idx,1);
        break;

    case GAPI_UNIFORM_DOUBLE:
        m_DoubleParams.Release(idx,1);
        break;

    case GAPI_UNIFORM_VECTOR2I:
        m_IntParams.Release(idx,2);
        break;
    case GAPI_UNIFORM_VECTOR3I:
        m_IntParams.Release(idx,3);
        break;
    case GAPI_UNIFORM_VECTOR4I:
        m_IntParams.Release(idx,4);
        break;

    case GAPI_UNIFORM_VECTOR2UI:
        m_UIntParams.Release(idx,2);
        break;
    case GAPI_UNIFORM_VECTOR3UI:
        m_UIntParams.Release(idx,3);
        break;
    case GAPI_UNIFORM_VECTOR4UI:
        m_UIntParams.Release(idx,4);
        break;

    case GAPI_UNIFORM_VECTOR2F:
        m_Vec2fParams.Release(idx,1);
        break;
    case GAPI_UNIFORM_VECTOR3F:
        m_Vec3fParams.Release(idx,1);
        break;
    case GAPI_UNIFORM_VECTOR4F:
        m_Vec4fParams.Release(idx,1);
        break;

    case GAPI_UNIFORM_VECTOR2D:
        m_Vec2dParams.Release(idx,1);
        break;
    case GAPI_UNIFORM_VECTOR3D:
        m_Vec3dParams.Release(idx,1);
        break;
    case GAPI_UNIFORM_VECTOR4D:
        m_Vec4dParams.Release(idx,1);
        break;

    case GAPI_UNIFORM_MTX33F:
        m_Vec3fParams.Release(idx,3);
        break;
    case GAPI_UNIFORM_MTX44F:
        m_Vec4fParams.Release(idx,4);
        break;

    case GAPI_UNIFORM_MTX33D:
        m_Vec3dParams.Release(idx,3);
        break;
    case GAPI_UNIFORM_MTX44D:
        m_Vec4dParams.Release(idx,4);
        break;
    }
}


////////////////////////////////////////////////////////////////////////////////////////////////
void CGLShaderSystem::SetParam(GLhandleARB hProgramObject, GAPIUNIFORMTYPE ParamType, uint32t idx, const char* name)
{
    GLint uniloc = glGetUniformLocation(hProgramObject,name);
    if(uniloc == -1)
        return ;

    switch(ParamType)
    {
    case GAPI_UNIFORM_INT:
        glUniform1iv(uniloc,1,(GLint*)m_IntParams.GetItem(idx));
        break;
    case GAPI_UNIFORM_UINT:
        glUniform1uiv(uniloc,1,(GLuint*)m_UIntParams.GetItem(idx));
        break;
    case GAPI_UNIFORM_FLOAT:
        glUniform1fv(uniloc,1,(GLfloat*)m_FloatParams.GetItem(idx));
        break;
    case GAPI_UNIFORM_DOUBLE:
        glUniform1dv(uniloc,1,(GLdouble*)m_DoubleParams.GetItem(idx));
        break;

    case GAPI_UNIFORM_VECTOR2I:
        glUniform2iv(uniloc,1,(GLint*)m_IntParams.GetItem(idx));
        break;
    case GAPI_UNIFORM_VECTOR3I:
        glUniform3iv(uniloc,1,(GLint*)m_IntParams.GetItem(idx));
        break;
    case GAPI_UNIFORM_VECTOR4I:
        glUniform4iv(uniloc,1,(GLint*)m_IntParams.GetItem(idx));
        break;

    case GAPI_UNIFORM_VECTOR2UI:
        glUniform2uiv(uniloc,1,(GLuint*)m_UIntParams.GetItem(idx));
        break;
    case GAPI_UNIFORM_VECTOR3UI:
        glUniform3uiv(uniloc,1,(GLuint*)m_UIntParams.GetItem(idx));
        break;
    case GAPI_UNIFORM_VECTOR4UI:
        glUniform4uiv(uniloc,1,(GLuint*)m_UIntParams.GetItem(idx));
        break;

    case GAPI_UNIFORM_VECTOR2F:
        glUniform2fv(uniloc,1,(GLfloat*)m_Vec2fParams.GetItem(idx));
        break;
    case GAPI_UNIFORM_VECTOR3F:
        glUniform3fv(uniloc,1,(GLfloat*)m_Vec3fParams.GetItem(idx));
        break;
    case GAPI_UNIFORM_VECTOR4F:
        glUniform4fv(uniloc,1,(GLfloat*)m_Vec4fParams.GetItem(idx));
        break;
    case GAPI_UNIFORM_MTX33F:
        glUniformMatrix3fv(uniloc,1,false,(GLfloat*)m_Vec3fParams.GetItem(idx));
        break;
    case GAPI_UNIFORM_MTX44F:
        glUniformMatrix4fv(uniloc,1,false,(GLfloat*)m_Vec4fParams.GetItem(idx));
        break;

    case GAPI_UNIFORM_VECTOR2D:
        glUniform2dv(uniloc,1,(GLdouble*)m_Vec2dParams.GetItem(idx));
        break;
    case GAPI_UNIFORM_VECTOR3D:
        glUniform3dv(uniloc,1,(GLdouble*)m_Vec3dParams.GetItem(idx));
        break;
    case GAPI_UNIFORM_VECTOR4D:
        glUniform4dv(uniloc,1,(GLdouble*)m_Vec4dParams.GetItem(idx));
        break;

    case GAPI_UNIFORM_MTX33D:
        glUniformMatrix3dv(uniloc,1,false,(GLdouble*)m_Vec3dParams.GetItem(idx));
        break;
    case GAPI_UNIFORM_MTX44D:
        glUniformMatrix4dv(uniloc,1,false,(GLdouble*)m_Vec4dParams.GetItem(idx));
        break;
    }
}

/////////////////////////////////////////////////////////////////////////////////////////////
void CGLShaderSystem::SetProgramParam(GLhandleARB hProgramObject, GAPIUNIFORMTYPE ParamType, uint32t idx, const char* name)
{
    GLint uniloc = glGetUniformLocation(hProgramObject,name);
    if(uniloc == -1)
        return ;

    switch(ParamType)
    {
    case GAPI_UNIFORM_INT:
        glProgramUniform1iv(hProgramObject,uniloc,1,(GLint*)m_IntParams.GetItem(idx));
        break;
    case GAPI_UNIFORM_UINT:
        glProgramUniform1uiv(hProgramObject,uniloc,1,(GLuint*)m_UIntParams.GetItem(idx));
        break;
    case GAPI_UNIFORM_FLOAT:
        glProgramUniform1fv(hProgramObject,uniloc,1,(GLfloat*)m_FloatParams.GetItem(idx));
        break;
    case GAPI_UNIFORM_DOUBLE:
        glProgramUniform1dv(hProgramObject,uniloc,1,(GLdouble*)m_DoubleParams.GetItem(idx));
        break;

    case GAPI_UNIFORM_VECTOR2I:
        glProgramUniform2iv(hProgramObject,uniloc,1,(GLint*)m_IntParams.GetItem(idx));
        break;
    case GAPI_UNIFORM_VECTOR3I:
        glProgramUniform3iv(hProgramObject,uniloc,1,(GLint*)m_IntParams.GetItem(idx));
        break;
    case GAPI_UNIFORM_VECTOR4I:
        glProgramUniform4iv(hProgramObject,uniloc,1,(GLint*)m_IntParams.GetItem(idx));
        break;

    case GAPI_UNIFORM_VECTOR2UI:
        glProgramUniform2uiv(hProgramObject,uniloc,1,(GLuint*)m_UIntParams.GetItem(idx));
        break;
    case GAPI_UNIFORM_VECTOR3UI:
        glProgramUniform3uiv(hProgramObject,uniloc,1,(GLuint*)m_UIntParams.GetItem(idx));
        break;
    case GAPI_UNIFORM_VECTOR4UI:
        glProgramUniform4uiv(hProgramObject,uniloc,1,(GLuint*)m_UIntParams.GetItem(idx));
        break;

    case GAPI_UNIFORM_VECTOR2F:
        glProgramUniform2fv(hProgramObject,uniloc,1,(GLfloat*)m_Vec2fParams.GetItem(idx));
        break;
    case GAPI_UNIFORM_VECTOR3F:
        glProgramUniform3fv(hProgramObject,uniloc,1,(GLfloat*)m_Vec3fParams.GetItem(idx));
        break;
    case GAPI_UNIFORM_VECTOR4F:
        glProgramUniform4fv(hProgramObject,uniloc,1,(GLfloat*)m_Vec4fParams.GetItem(idx));
        break;

    case GAPI_UNIFORM_MTX33F:
        glProgramUniformMatrix3fv(hProgramObject,uniloc,1,false,(GLfloat*)m_Vec3fParams.GetItem(idx));
        break;
    case GAPI_UNIFORM_MTX44F:
        glProgramUniformMatrix4fv(hProgramObject,uniloc,1,false,(GLfloat*)m_Vec4fParams.GetItem(idx));
        break;


    case GAPI_UNIFORM_VECTOR2D:
        glProgramUniform2dv(hProgramObject,uniloc,1,(GLdouble*)m_Vec2dParams.GetItem(idx));
        break;
    case GAPI_UNIFORM_VECTOR3D:
        glProgramUniform3dv(hProgramObject,uniloc,1,(GLdouble*)m_Vec3dParams.GetItem(idx));
        break;
    case GAPI_UNIFORM_VECTOR4D:
        glProgramUniform4dv(hProgramObject,uniloc,1,(GLdouble*)m_Vec4dParams.GetItem(idx));
        break;


    case GAPI_UNIFORM_MTX33D:
        glProgramUniformMatrix3dv(hProgramObject,uniloc,1,false,(GLdouble*)m_Vec3dParams.GetItem(idx));
        break;
    case GAPI_UNIFORM_MTX44D:
        glProgramUniformMatrix4dv(hProgramObject,uniloc,1,false,(GLdouble*)m_Vec4dParams.GetItem(idx));
        break;
    }
}

/////////////////////////////////////////////////////////////////////////////////////////////
void CGLShaderSystem::SetUniform(GAPIUNIFORMTYPE ParamType, GLint uniloc, const void *data)
{
    if(uniloc == -1)
        return ;

    switch(ParamType)
    {
    case GAPI_UNIFORM_INT:
        glUniform1iv(uniloc,1,(GLint*)data);
        break;
    case GAPI_UNIFORM_UINT:
        glUniform1uiv(uniloc,1,(GLuint*)data);
        break;
    case GAPI_UNIFORM_FLOAT:
        glUniform1fv(uniloc,1,(GLfloat*)data);
        break;
    case GAPI_UNIFORM_DOUBLE:
        glUniform1dv(uniloc,1,(GLdouble*)data);
        break;

    case GAPI_UNIFORM_VECTOR2I:
        glUniform2iv(uniloc,1,(GLint*)data);
        break;
    case GAPI_UNIFORM_VECTOR3I:
        glUniform3iv(uniloc,1,(GLint*)data);
        break;
    case GAPI_UNIFORM_VECTOR4I:
        glUniform4iv(uniloc,1,(GLint*)data);
        break;

    case GAPI_UNIFORM_VECTOR2UI:
        glUniform2uiv(uniloc,1,(GLuint*)data);
        break;
    case GAPI_UNIFORM_VECTOR3UI:
        glUniform3uiv(uniloc,1,(GLuint*)data);
        break;
    case GAPI_UNIFORM_VECTOR4UI:
        glUniform4uiv(uniloc,1,(GLuint*)data);
        break;

    case GAPI_UNIFORM_VECTOR2F:
        glUniform2fv(uniloc,1,(GLfloat*)data);
        break;
    case GAPI_UNIFORM_VECTOR3F:
        glUniform3fv(uniloc,1,(GLfloat*)data);
        break;
    case GAPI_UNIFORM_VECTOR4F:
        glUniform4fv(uniloc,1,(GLfloat*)data);
        break;
    case GAPI_UNIFORM_MTX33F:
        glUniformMatrix3fv(uniloc,1,false,(GLfloat*)data);
        break;
    case GAPI_UNIFORM_MTX44F:
        glUniformMatrix4fv(uniloc,1,false,(GLfloat*)data);
        break;

    case GAPI_UNIFORM_VECTOR2D:
        glUniform2dv(uniloc,1,(GLdouble*)data);
        break;
    case GAPI_UNIFORM_VECTOR3D:
        glUniform3dv(uniloc,1,(GLdouble*)data);
        break;
    case GAPI_UNIFORM_VECTOR4D:
        glUniform4dv(uniloc,1,(GLdouble*)data);
        break;

    case GAPI_UNIFORM_MTX33D:
        glUniformMatrix3dv(uniloc,1,false,(GLdouble*)data);
        break;
    case GAPI_UNIFORM_MTX44D:
        glUniformMatrix4dv(uniloc,1,false,(GLdouble*)data);
        break;
    }
}

/////////////////////////////////////////////////////////////////////////////////////////////
void CGLShaderSystem::SetProgramUniform(GLhandleARB hProgramObject, GAPIUNIFORMTYPE ParamType, GLint uniloc, const void *data)
{
    if(uniloc == -1)
        return ;

    switch(ParamType)
    {
    case GAPI_UNIFORM_INT:
        glProgramUniform1iv(hProgramObject,uniloc,1,(GLint*)data);
        break;
    case GAPI_UNIFORM_UINT:
        glProgramUniform1uiv(hProgramObject,uniloc,1,(GLuint*)data);
        break;
    case GAPI_UNIFORM_FLOAT:
        glProgramUniform1fv(hProgramObject,uniloc,1,(GLfloat*)data);
        break;
    case GAPI_UNIFORM_DOUBLE:
        glProgramUniform1dv(hProgramObject,uniloc,1,(GLdouble*)data);
        break;

    case GAPI_UNIFORM_VECTOR2I:
        glProgramUniform2iv(hProgramObject,uniloc,1,(GLint*)data);
        break;
    case GAPI_UNIFORM_VECTOR3I:
        glProgramUniform3iv(hProgramObject,uniloc,1,(GLint*)data);
        break;
    case GAPI_UNIFORM_VECTOR4I:
        glProgramUniform4iv(hProgramObject,uniloc,1,(GLint*)data);
        break;

    case GAPI_UNIFORM_VECTOR2UI:
        glProgramUniform2uiv(hProgramObject,uniloc,1,(GLuint*)data);
        break;
    case GAPI_UNIFORM_VECTOR3UI:
        glProgramUniform3uiv(hProgramObject,uniloc,1,(GLuint*)data);
        break;
    case GAPI_UNIFORM_VECTOR4UI:
        glProgramUniform4uiv(hProgramObject,uniloc,1,(GLuint*)data);
        break;

    case GAPI_UNIFORM_VECTOR2F:
        glProgramUniform2fv(hProgramObject,uniloc,1,(GLfloat*)data);
        break;
    case GAPI_UNIFORM_VECTOR3F:
        glProgramUniform3fv(hProgramObject,uniloc,1,(GLfloat*)data);
        break;
    case GAPI_UNIFORM_VECTOR4F:
        glProgramUniform4fv(hProgramObject,uniloc,1,(GLfloat*)data);
        break;

    case GAPI_UNIFORM_MTX33F:
        glProgramUniformMatrix3fv(hProgramObject,uniloc,1,false,(GLfloat*)data);
        break;
    case GAPI_UNIFORM_MTX44F:
        glProgramUniformMatrix4fv(hProgramObject,uniloc,1,false,(GLfloat*)data);
        break;


    case GAPI_UNIFORM_VECTOR2D:
        glProgramUniform2dv(hProgramObject,uniloc,1,(GLdouble*)data);
        break;
    case GAPI_UNIFORM_VECTOR3D:
        glProgramUniform3dv(hProgramObject,uniloc,1,(GLdouble*)data);
        break;
    case GAPI_UNIFORM_VECTOR4D:
        glProgramUniform4dv(hProgramObject,uniloc,1,(GLdouble*)data);
        break;


    case GAPI_UNIFORM_MTX33D:
        glProgramUniformMatrix3dv(hProgramObject,uniloc,1,false,(GLdouble*)data);
        break;
    case GAPI_UNIFORM_MTX44D:
        glProgramUniformMatrix4dv(hProgramObject,uniloc,1,false,(GLdouble*)data);
        break;
    }
}

/////////////////////////////////////////////////////////////////////////////////////////////
/*--------------------------------------
GLuint CGLShaderSystem::GetBindingPoint()
{
    GLuint bindpoint = -1;
    unsigned idx = 0;
    for(idx = 0; idx < m_UniformBindings.Size(); idx++)
        if(!m_UniformBindings[idx])
            break;

    if(idx != m_UniformBindings.Size())
        bindpoint = (GLuint)idx;

    if(bindpoint == -1)
    {
        m_UniformBindings.AddElems(1);
        bindpoint = (GLuint)(m_UniformBindings.Size() - 1);
    }
    m_UniformBindings.SetBit(bindpoint);

    return bindpoint;
}
--------------------------------------*/

/////////////////////////////////////////////////////////////////////////////////////////////
/*--------------------------------------
void CGLShaderSystem::ReleaseBindingPoint(GLuint bindingpoint)
{
    if(bindingpoint >= m_UniformBindings.Size()) return;
    m_UniformBindings.ClearBit(bindingpoint);
}
--------------------------------------*/

/////////////////////////////////////////////////////////////////////////////////////////////
//Reading Shader Source Code Function And Extracting Uniform Data And Verte Input Data
void CGLShaderSystem::ExtractUniformNames(const char *shader_src, std::map<std::string,GLint> &uni_map)
{
    std::string strshader(shader_src);
    size_t keypos = strshader.find("uniform ");
    while(keypos != std::string::npos)
    {
        size_t endpos = strshader.find(';',keypos);
        size_t spos = strshader.find('{',keypos);
        if(spos < endpos) {
            keypos = strshader.find("uniform ",endpos);
            continue;
        }

        if(IsNameCommented(strshader,keypos)) {
            keypos = strshader.find("uniform ",endpos);
            continue; 
        }

        endpos = strshader.find_last_not_of("\r\n ;",endpos);
        size_t namepos = strshader.find_last_of(' ',endpos);
        std::string strname = strshader.substr(namepos+1,endpos-namepos);

        //Handle Uniform Array
        int elemcount = GetArrayElemCount(strname);
        
        //Get Single Name In Case If We Working With Uniform Array
        strname = GetSingleName(strname);
        uni_map.insert(std::pair<std::string,GLint>(strname,-1));
        keypos = strshader.find("uniform ",endpos);

        for(int i = 0; i < elemcount; i++)
        {
            char buf[32]={0};
            _itoa(i,buf,10);
            std::string fullname = strname;
            fullname.push_back('[');
            fullname.append(buf);
            fullname.push_back(']');
            uni_map.insert(std::pair<std::string,GLint>(fullname,-1));
        }
    }
}
/////////////////////////////////////////////////////////////////////////////////////////////
void CGLShaderSystem::ExtractUniformBlockNames(const char *shader_src, std::map<std::string,GLuint> &uniblock_map)
{
    std::string strshader(shader_src);
    size_t keypos = strshader.find("uniform ");
    while(keypos != std::string::npos)
    {
        size_t endpos = strshader.find(';',keypos);
        size_t spos = strshader.find('{',keypos);
        if(spos > endpos) {
            keypos = strshader.find("uniform ",endpos);
            continue;
        }

        if(IsNameCommented(strshader,keypos)) {
            keypos = strshader.find("uniform ",endpos);
            continue; 
        }

        //Getting Block Name
        spos = strshader.find_first_of(' ',keypos);
        spos = strshader.find_first_not_of(' ',spos);
        size_t namepos = spos;
        endpos = strshader.find_first_of("\r\n ",namepos);
        std::string strname = strshader.substr(namepos,endpos-namepos);
        uniblock_map.insert(std::pair<std::string,GLuint>(strname,-1));

        keypos = strshader.find("uniform ",endpos);

        //Searching For Instance Name
        size_t pos0 = strshader.find('}',endpos);
        size_t pos1 = strshader.find(';',pos0);
        if(pos0 != std::string::npos) 
        {
            std::string instname = strshader.substr(pos0+1,pos1-pos0-1);
            if(instname.empty()) continue;

            size_t npos0 = instname.find_first_not_of("\r\n\t ");
            if(npos0 > 0)
                instname = instname.substr(npos0);
            size_t lp0 = instname.find_last_of("\r\n ");
            size_t lp1 = instname.find_last_not_of("\r\n ");
            if(lp0 != std::string::npos && lp0 > lp1)
                instname.erase(lp0);

            int arrcount = GetArrayElemCount(instname);

            instname = GetSingleName(instname);

            for(int i = 0; i < arrcount; i++)
            {
                char buf[32]={0};
                _itoa(i,buf,10);
                std::string fullname = instname;
                fullname.push_back('[');
                fullname.append(buf);
                fullname.push_back(']');
                uniblock_map.insert(std::pair<std::string,GLuint>(strname,-1));
            }
        }

        
    }
}
/////////////////////////////////////////////////////////////////////////////////////////////
void CGLShaderSystem::ExtractInputNames(const std::string &str_shader, std::map<std::string,GLuint> &input_map)
{
    size_t attribpos = str_shader.find("attribute ");
    size_t inpos = str_shader.find("in ");
    size_t keypos = (attribpos > inpos) ? (inpos) : (attribpos);

    while(keypos != std::string::npos)
    {
        size_t endpos = str_shader.find(';',keypos);
        size_t spos = str_shader.find_last_not_of("\r\n ;",endpos);
        size_t namepos = str_shader.find_last_of(' ',spos);

        if(!IsNameCommented(str_shader,keypos)) 
        {
            std::string strname = str_shader.substr(namepos+1,endpos-namepos-1);
            input_map.insert(std::pair<std::string,GLuint>(strname,-1));
        }

        attribpos = str_shader.find("attribute ",endpos);
        inpos = str_shader.find("in ",endpos);
        keypos = (attribpos > inpos) ? (inpos) : (attribpos);
    }
}
/////////////////////////////////////////////////////////////////////////////////////////////
bool CGLShaderSystem::IsNameCommented(const std::string &source, size_t str_pos)
{
  
    //Searching first pattern: "//"
    size_t cmtpos = source.rfind("//",str_pos);
    if(cmtpos != std::string::npos) {
        size_t nlpos = source.find_first_of("\r\n",cmtpos);
        if(nlpos != std::string::npos) {
            if(nlpos < str_pos)
                return false;
        }
    }

    //searching for a another pattern: "/*" "*/"
    size_t patpos0 = source.rfind("/*",str_pos);
    size_t patpos1 = source.rfind("*/",str_pos);
    if(patpos0 == std::string::npos && patpos1 == std::string::npos)
        return false;
    if(patpos0 < patpos1 && patpos1 < str_pos)
        return true;

    return false;
}

/////////////////////////////////////////////////////////////////////////////////////////////
std::string CGLShaderSystem::GetSingleName(const std::string &str_name)
{
    size_t arrpos = str_name.find("[");
    if(arrpos == std::string::npos) {
        size_t pos = str_name.find("\r\n ");
        std::string outname = str_name;
        if(pos != std::string::npos)
            outname.erase(pos);
        return outname;
    }


    std::string subname = str_name.substr(0,arrpos);
    size_t pos = subname.find("\r\n ");
    if(pos != std::string::npos)
        subname.erase(pos);

    return subname;
}
/////////////////////////////////////////////////////////////////////////////////////////////
int CGLShaderSystem::GetArrayElemCount(const std::string &str_name)
{
    size_t arrpos = str_name.find('[');
    if(arrpos == std::string::npos)
        return 0;

    size_t arrpos2 = str_name.find(']');
    std::string strcnt = str_name.substr(arrpos+1,arrpos2-arrpos-1);

    int icount = atoi(strcnt.c_str());

    return icount;
}

/////////////////////////////////////////////////////////////////////////////////////////////
void CGLShaderSystem::GetUniformLoc(GLhandleARB hProgramObject, std::map<std::string,GLint> &uni_map)
{
    // Getting Uniform Locations
    std::map<std::string,GLint>::iterator it = uni_map.begin();
    while(it!=uni_map.end())
    {
        it->second = glGetUniformLocation(hProgramObject,it->first.c_str());
        GLenum err = glGetError();
        it++;
    }
}
/////////////////////////////////////////////////////////////////////////////////////////////
void CGLShaderSystem::GetUniformBlockIndices(GLhandleARB hProgramObject, std::map<std::string,GLuint> &uniblock_map)
{
    // Getting Block Indices
    std::map<std::string,GLuint>::iterator it = uniblock_map.begin();
    while(it!=uniblock_map.end())
    {
        it->second = glGetUniformBlockIndex(hProgramObject,it->first.c_str());
        it++;
    }
}
/////////////////////////////////////////////////////////////////////////////////////////////
void CGLShaderSystem::GetInputIndices(GLhandleARB hProgramObject, std::map<std::string,GLuint> &input_map)
{
    // Getting Vertex Attribute Location
    std::map<std::string,GLuint>::iterator it = input_map.begin();
    while(it!=input_map.end())
    {
        it->second = glGetAttribLocation(hProgramObject,it->first.c_str());
        it++;
    }
    
}
/////////////////////////////////////////////////////////////////////////////////////////////

// Uniform Block Array
