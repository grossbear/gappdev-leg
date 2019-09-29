/////////////////////////////////////////////////////////////////////////////////////////////
// GLSahderSystem.h
//
// OpenGL Shaders Managment Class
/////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _GLSHADERSYSTEM_H_
#define _GLSHADERSYSTEM_H_

#include "Renderapi.h"
#include "Utils/ArrayMap.h"
#include "Utils/BitArray.h"
#include "Utils/BlockArray.h"
#include <list>
#include <vector>
#include <string>

#define PARAM_VALS_BLOCK_SIZE   64
#define SHADER_OBJECTS_COUNT    64
#define PARAMS_DESC_BLOCK_SIZE  64
#define UNIFORM_BUFFER_COUNT    64
#define UNIFORM_BLOCK_DESC_SIZE 64
#define PIPELINE_OBJECTS_COUNT  64

#define SHADER_PARAMS_INFO_COUNT  8

#define VERTEX_SHADER_SOURCE


/////////////////////////////////////////////////////////////////////////////////////////////
class CGLShaderSystem
{
public:
    static CGLShaderSystem* GetSingleton();
    ~CGLShaderSystem();

    uint32t CreateShader(const char *strshader, bool bseparate=false);
    bool    DeleteShader(uint32t shaderidx);
    bool    SetShader(uint32t shaderIdx);
    bool    IsShaderActive() const {return m_bShaderActive;}

    uint32t CreateShaderParamsGroup(uint32t reservesize=0);
    bool    DeleteShaderParamsGroup(uint32t groupidx);
    uint32t AddShaderParam(uint32t groupidx, const char *name, GAPIUNIFORMTYPE type, const void *data);
    bool    UpdateShaderParam(uint32t groupidx, uint32t paramidx, const void *data);
    bool    SetShaderParams(uint32t shaderidx, uint32t groupidx, uint32t commgroupidx=0);
    uint32t GetShaderParamIndex(uint32t groupidx, const char *name);

    // Uniform Buffer Objects Management
    uint32t CreateUniformBuffer(GAPIBUFFERUSAGE usage, uint32t bufsize, const void *data);
    bool    DeleteUniformBuffer(uint32t bufidx);
    bool    UpdateUniformBuffer(uint32t bufidx, uint32t offset, uint32t size, const void *data);
    bool    AddUniformBlocks(uint32t shaderidx, uint32t *bufidx, const char **names, uint32t count);

    // Setting Uniforms And Uniform Buffers Directly
    bool    SetUniform(GAPIUNIFORMTYPE type, const char *name, const void *data);
    bool    SetProgramUniform(uint32t shaderidx, GAPIUNIFORMTYPE type, const char *name, const void *data);
    bool    SetUniforms(GAPIUNIFORMDATA *data, uint32t count);
    bool    SetProgramUniforms(uint32t shaderidx, GAPIUNIFORMDATA *data, uint32t count);

    uint32t GetUniformBlockSize(uint32t shaderidx, const char *blockname);
    bool    GetActiveUniformInfo(uint32t shaderidx, GAPIACTIVEUNIFORMTYPE type, const char **names, uint32t *pvalues, uint32t count);
    bool    SetUniformBlockBinding(uint32t shaderidx, const char *blockname, uint32t bindingpoint);

    uint32t GetAttribLocation(uint32t shaderidx, const char *name);
    /*--------------------------------------
    uint32t GetUniformBufferBindingPoint(uint32t bufidx);
    --------------------------------------*/
    bool    SetUniformBufferBindingPoint(uint32t bufidx, uint32t bindpoint);


    // Separate Shader Objects Handling
    uint32t CreateProgramPipeline(uint32t *shadersidx, uint32t count);
    bool    DeleteProgramPipeline(uint32t pipelineidx);
    bool    SetProgramPipeline(uint32t pipelineidx);////????
    bool    SetSeparateShadersParams(uint32t *pshadersidx, uint32t *pgroupsidx, uint32t count);

    GLhandleARB GetShaderProgram(uint32t shaderIdx);
    GLhandleARB GetActiveShaderProgram() const;

    // Retrieving Ids From Shader 
    GLint GetVertexAttribLocation(uint32t shaderidx, const char *attribname);
    GLint GetVertexAttribLocation(const char *attribname); 

    // Fragment Data Location
    void SetFragDataLocation(uint32t shaderidx, uint32t colornumber, const char *name);
    void SetFragDataLocation(uint32t shaderidx, uint32t *colorsnumbers, const char **names, uint32t count);
    GLint GetFragDataLocation(uint32t shaderidx, const char *name);

    // Blend Func Extended
    void SetFragDataLocationIndexed(uint32t shaderidx, uint32t colornumber, uint32t index, const char *name);
    void SetFragDataLocationIndexed(uint32t shaderidx, uint32t *colorsnumbers, uint32t *indices, const char **names, uint32t count);
    GLint GetFragDataIndex(uint32t shaderidx, const char *name);
    

    //Shader Uniform Params Map
    struct ShaderInputInfo
    {
        std::map<std::string,GLint> UniformNamesMap;
        std::map<std::string,GLuint> UniformBlockNamesMap;
        std::map<std::string,GLuint> InputNamesMap;
    };

protected:
    CGLShaderSystem();

    static CGLShaderSystem *m_pInstance;

    typedef struct ShaderObject
    {
        GLhandleARB hVertrexShader;
        GLhandleARB hFragmentShader;
        GLhandleARB hTessCtrlShader;
        GLhandleARB hTessEvalShader;
        GLhandleARB hGeometryShader;
        GLhandleARB hProgramObject;
        GLbitfield  ShadersMask;
        uint32t     UniformBlocksDescIdx;
        uint32t     ShaderInputInfoIdx;
    };

    struct ShaderParamDesc
    {
        uint32t         ParamIdx;
        uint32t         NameIdx;
        GAPIUNIFORMTYPE ParamType;

        ShaderParamDesc():ParamIdx(0),NameIdx(0),ParamType((GAPIUNIFORMTYPE)0){}
        bool operator == (const ShaderParamDesc &o) {return ParamIdx==o.ParamIdx && NameIdx==o.NameIdx && ParamType==o.ParamType;}
    };

    struct StringsVecData
    {
        std::vector<std::string> NamesVec;
        std::vector<uint32t> RefCounter;
    };

    typedef std::vector<CGLShaderSystem::ShaderParamDesc> tparamsvec;

    struct UniformBufferBlock
    {
        GLuint  UniformBufferID;
        GLenum  UniformBufferUsage;
        GLuint  UniformBufferSize;
        //--GLuint  BindingPoint;
    };

    struct UniformBlockDesc
    {
        GLuint  UniformBlockIndex;
        GLuint  BlockBindingPoint;
    };

    typedef std::vector<CGLShaderSystem::UniformBlockDesc> tblockdescvec;


    enum ShaderType
    {
        TYPE_VERTEX_SHADER,
        TYPE_PIXEL_SHADER,
        TYPE_GEOMETRY_SHADER,
        TYPE_TESS_CONTROL_SHADER,
        TYPE_TESS_EVALUATION_SHADER,
    };

    static char toupper(char c);
    void GetShaderSource(const std::string &source, std::string &strout, CGLShaderSystem::ShaderType shd_type);
    void GetGeomtryShaderParams(const std::string &source,GLint &input_type, GLint &output_type, GLint &vert_num);
    GLenum GetGeometryType(const std::string &strgeotype);

    GLhandleARB CompileShader(const std::string &src_code, GLenum shaderType);

    void SetShaderInfoLog(GLhandleARB hShader);
    void SetProgramInfoLog(GLhandleARB hProgram);
    void ReleaseShaderObject(CGLShaderSystem::ShaderObject *shaderobj);
    void ReleaseShaders();

    GLhandleARB CreateSeparateShaderProgram(const std::string &source, GLenum glshadertype);

    inline GLbitfield  ReleaseShaderSeparate(GLhandleARB hProgram, GLhandleARB &hShader, ShaderType shdtype);

    bool    CopyParamData(GAPIUNIFORMTYPE ParamType, uint32t idx, const void *data);
    uint32t CreateParamData(GAPIUNIFORMTYPE ParamType);
    void    ReleaseParamData(GAPIUNIFORMTYPE ParamType, uint32t idx);
    void    SetParam(GLhandleARB hProgramObject, GAPIUNIFORMTYPE ParamType, uint32t idx, const char* name);

    void    SetProgramParam(GLhandleARB hProgramObject, GAPIUNIFORMTYPE ParamType, uint32t idx, const char* name);

    // Setting Shader Uniforms
    void    SetUniform(GAPIUNIFORMTYPE ParamType, GLint uniloc, const void *data);
    void    SetProgramUniform(GLhandleARB hProgramObject, GAPIUNIFORMTYPE ParamType, GLint uniloc, const void *data);

    /*--------------------------------------
    GLuint  GetBindingPoint();
    inline void ReleaseBindingPoint(GLuint bindingpoint);
    --------------------------------------*/

    //////////////////////////////////////
    //Reading Shader Source Code Function And Extracting Uniform Data And Vertex Input Data
    void    ExtractUniformNames(const char *shadersrc, std::map<std::string,GLint> &unimap);
    void    ExtractUniformBlockNames(const char *shadersrc, std::map<std::string,GLuint> &uniblockmap);
    void    ExtractInputNames(const std::string &shadersrc, std::map<std::string,GLuint> &inputmap);

    bool    IsNameCommented(const std::string &source, size_t pos);
    std::string GetSingleName(const std::string &str_name);
    int     GetArrayElemCount(const std::string &str_name);

    void    GetUniformLoc(GLhandleARB hProgramObject, std::map<std::string,GLint> &unimap);
    void    GetUniformBlockIndices(GLhandleARB hProgramObject, std::map<std::string,GLuint> &uniblockmap);
    void    GetInputIndices(GLhandleARB hProgramObject, std::map<std::string,GLuint> &inputmap);
    //////////////////////////////////////
    
    // Shader Objects Array
    CArrayMap<CGLShaderSystem::ShaderObject,SHADER_OBJECTS_COUNT> m_ShaderObjects;
    //////////////////////////////////////

    // Pipeline Of Separate Shader Objects
    CArrayMap<GLuint, PIPELINE_OBJECTS_COUNT> m_PipelineObjects;

    std::string m_strShaderInfoLog;
    std::string m_strProgramInfoLog;
    std::string m_strPipelineInfoLog;

    //////////////////////////////////////
    //Shader Params
    //--CGLShaderSystem::ParamsDescList m_ShaderParamsDesc;
    CArrayMap<std::vector<ShaderParamDesc>,PARAMS_DESC_BLOCK_SIZE> m_ShaderParamsDescMap;
    CGLShaderSystem::StringsVecData m_ParamNames;

    CArrayMap<CGLShaderSystem::UniformBufferBlock,UNIFORM_BUFFER_COUNT> m_UniformBuffersMap;
    CArrayMap<tblockdescvec,UNIFORM_BLOCK_DESC_SIZE> m_UniformBlocksDescMap;
    /*--------------------------------------
    CBitArray m_UniformBindings;
    --------------------------------------*/
    //////////////////////////////////////
    CBlockArray<CGLShaderSystem::ShaderInputInfo,SHADER_PARAMS_INFO_COUNT> m_ShaderInputInfoMaps;
    //////////////////////////////////////
    

    CArrayMap<float,PARAM_VALS_BLOCK_SIZE> m_FloatParams;
    CArrayMap<float[2],PARAM_VALS_BLOCK_SIZE> m_Vec2fParams;
    CArrayMap<float[3],PARAM_VALS_BLOCK_SIZE> m_Vec3fParams;
    CArrayMap<float[4],PARAM_VALS_BLOCK_SIZE> m_Vec4fParams;
    CArrayMap<double,PARAM_VALS_BLOCK_SIZE> m_DoubleParams;
    CArrayMap<double[2],PARAM_VALS_BLOCK_SIZE> m_Vec2dParams;
    CArrayMap<double[3],PARAM_VALS_BLOCK_SIZE> m_Vec3dParams;
    CArrayMap<double[4],PARAM_VALS_BLOCK_SIZE> m_Vec4dParams;
    CArrayMap<int32t,PARAM_VALS_BLOCK_SIZE> m_IntParams;
    CArrayMap<uint32t,PARAM_VALS_BLOCK_SIZE> m_UIntParams;

    //////////////////////////////////////
    
    GLhandleARB m_ActiveProgram;
    ShaderInputInfo *m_pActiveShaderInInfo;
    bool m_bShaderActive;
    bool m_bProgramPipelineActive;

};
/////////////////////////////////////////////////////////////////////////////////////////////


#endif // _GLSHADERSYSTEM_H_
