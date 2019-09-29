/////////////////////////////////////////////////////////////////////////////////////////////
// GLRender3.h
//
// OpenGL 3.x Rendering Core Class
/////////////////////////////////////////////////////////////////////////////////////////////

#define WIN32_LEAN_AND_MEAN 1
#include <windows.h>

#include "GL/gl.h"
#include "GL/glext.h"
#include "Renderapi.h"


/////////////////////////////////////////////////////////////////////////////////////////////
class CGLRender3 : public IGraphicApi
{
public:
    CGLRender3();
    virtual ~CGLRender3();

    void Clear();
    void Clear(bool color, bool depth, bool stencil);
    bool Create(const GAPICREATIONINFO *info);
    void Render();
    void Resize(int width, int height);
    bool SwapGraphicBuffers();

    //////////////////////////////////////////////////////////////////////////////////////////////
    // Device Caps
    uint32t  GetDevCaps(GAPIDEVCAPSPARAM devparam);

    //////////////////////////////////////////////////////////////////////////////////////////////
    // Scissor Setting Functions
    void EnableScissorTest(bool flag);
    bool IsScissorTestEnabled();
    void SetScissorRect(int x, int y, int width, int height);
    void GetScissorRect(int scissor_box[4]);

    //////////////////////////////////////////////////////////////////////////////////////////////
    // Depth Setting Functions
    void EnableDepthTest(bool flag);
    void EnableDepthWrite(bool flag);
    void SetDepthFunc(GAPIFUNC func);
    void SetClearDepth(float depth);

    bool IsDepthTestEnabled();
    bool IsDepthWriteEnabled();
    float GetClearDepth();
    GAPIFUNC GetDepthFunc();

    //////////////////////////////////////////////////////////////////////////////////////////////
    // Stencil Setting Functions
    void EnableStencilTest(bool flag);
    void SetStencilWriteMask(uint32t mask);
    bool IsStencilTestEnabled();
    void SetStencilFunc(GAPIFUNC func, uint32t ref, uint32t mask);
    void SetStencilOp(GAPIOP fail, GAPIOP zfail, GAPIOP pass);
    void EnableTwoSideStencil(bool flag);
    void SetClearStencil(uint32t val=0);
    bool IsTwoSideStencilEnabled();

    uint32t GetStencilWriteMask();
    GAPIFUNC GetStencilFunc();
    uint32t GetStencilBits();
    uint32t GetStencilClearVal();
    uint32t GetStencilFuncMask();
    uint32t GetStencilFuncRef();
    GAPIOP  GetStencilOpFail();
    GAPIOP  GetStencilOpDepthFail();
    GAPIOP  GetStencilOpDepthPass();

    void  SetStencilFuncSeparate(GAPIFUNC frontfunc, GAPIFUNC backfunc, uint32t ref, uint32t mask);
    void  SetStencilOpSeparate(bool frontface, GAPIOP fail, GAPIOP zfail, GAPIOP pass);
    void  SetStencilMaskSeparate(bool frontface, uint32t mask);

    //////////////////////////////////////////////////////////////////////////////////////////////
    // Raster Functions
    void SetRect(int32t x1, int32t y1, int32t x2, int32t y2);
    void SetRect(float x1, float y1, float x2, float y2);
    void SetRasterPos(int32t x, int32t y);
    void SetRasterPos(float x, float y);
    void SetColor(ubyte red, ubyte green, ubyte blue, ubyte alpha);
    void SetColor(ubyte color[4]);
    void SetColor(unsigned color);
    void DrawBitmap(int width, int height, float xorig, float yorig, float xmove, float ymove, const ubyte *bitmap);

    //////////////////////////////////////////////////////////////////////////////////////////////
    // Matrix Handling
    void SetProjMatrix(const float matrix[16]);

    //////////////////////////////////////////////////////////////////////////////////////////////
    // Viewport Setting
    void SetViewport(int x, int y, int width, int height);
    void GetViewport(int rect[4]);

    //////////////////////////////////////////////////////////////////////////////////////////////
    // Alpha Test
    void EnableAlphaTest(bool flag);
    bool IsAlphaTestEnabled();
    void SetAlphaFunc(GAPIFUNC func, float ref);
    GAPIFUNC GetAlphaTestFunc();
    float GetAlphaTestRef();

    //////////////////////////////////////////////////////////////////////////////////////////////
    // Alpha To Coverage Test
    void EnableSampleAlphaToMask(bool flag);
    bool IsSampleAlphaToMaskEnabled();
    void SetSampleCoverage(float val, bool invert);
    void GetSampleCoverage(float &val, bool &invert);

    //////////////////////////////////////////////////////////////////////////////////////////////
    // Color Blending
    void EnableColorBlend(bool flag);
    bool IsColorBlendEnabled();
    void SetBlendFunc(GAPIBLENDOP src, GAPIBLENDOP dest);
    GAPIBLENDOP GetSrcBlendFunc();
    GAPIBLENDOP GetDestBlendFunc();

    void SetBlendEquation(GAPIBLENDFUNC mode);
    void SetBlendEquationSeparate(GAPIBLENDFUNC modeRGB, GAPIBLENDFUNC modeAlpha);
    void SetBlendFuncSeparate(GAPIBLENDOP srcRGB, GAPIBLENDOP destRGB, GAPIBLENDOP srcAlpha, GAPIBLENDOP destAlpha);

    void SetBlendColor(float red, float green, float blue, float alpha);
    //////////////////////////////////////////////////////////////////////////////////////////////
    // Setting Framebuffer Clear Color
    void SetClearColor(float red, float green, float blue, float alpha);
    //////////////////////////////////////////////////////////////////////////////////////////////
    // Setting Polygon Fill Mode
    void SetFillMode(GAPIFILLMODE mode);
    //////////////////////////////////////////////////////////////////////////////////////////////
    // Setting Polygon Cull Mode
    void SetCullMode(bool CullFace,bool CCW);
    //////////////////////////////////////////////////////////////////////////////////////////////
    // Setting Shading Mode (Gouraud or Flat Shading)
    void SetShadingMode(bool bsmooth);

    //////////////////////////////////////////////////////////////////////////////////////////////
    // Occlusion Query Managment Functions
    uint32t CreateQueryObject();
    bool    DeleteQueryObject(uint32t objidx);
    void    BeginQuery(uint32t objidx);
    void    EndQuery();
    uint32t GetQueryObjects(uint32t objidx);
    bool    QueryStarted();

    //////////////////////////////////////////////////////////////////////////////////////////////
    // Texture Managment Functions
    uint32t CreateTexture(uint32t width, uint32t height, uint32t depth, uint32t flags, TEXFMT fmt, const void **data);
    bool    DeleteTexture(uint32t texidx);

    bool    UpdateTexture(uint32t texidx, uint32t miplevel, uint32t face, uint32t align, uint32t xoff, uint32t yoff, uint32t zoff, const void *data);

    bool    BindRenderTarget(uint32t texidx, uint32t face=0);
    bool    UnbindRenderTarget();

    bool    SetTexFilter(uint32t texidx, TEXFLT flt);
    bool    GetTexFilter(uint32t texidx, TEXFLT &flt);

    bool    SetTexAnisotropy(uint32t texidx, uint32t val);
    bool    GetTexAnisotropy(uint32t texidx, uint32t &val);

    bool    SetTexAddr(uint32t texidx, TEXADDR addrS, TEXADDR addrT, TEXADDR addrR);
    bool    GetTexAddr(uint32t texidx, TEXADDR &addrS, TEXADDR &addrT, TEXADDR &addrR);

    bool    GetTexWidth(uint32t texidx, uint32t &width);
    bool    GetTexHeight(uint32t texidx, uint32t &height);
    bool    GetTexDepth(uint32t texidx, uint32t &depth);

    bool    SetTexture(uint32t texIdx, uint32t tex_unit);
    bool    SetTexture(const uint32t *pTexIdx, uint32t tex_unit_num);
    void    DisableTextures();

    uint32t CreateTextureSampler();
    bool    DeleteTextureSampler(uint32t sampleridx);
    bool    SetTexSamplerFilter(uint32t sampleridx, TEXFLT flt);
    bool    SetTexSamplerMinFilter(uint32t sampleridx, TEXFLT flt);
    bool    SetTexSamplerMagFilter(uint32t sampleridx, TEXFLT flt);
    bool    SetTexSamplerAddr(uint32t sampleridx, TEXADDR wrap);
    bool    SetTexSamplerAddr(uint32t sampleridx, TEXADDR sWrap, TEXADDR tWrap, TEXADDR rWrap);
    bool    SetTexSamplerMinLod(uint32t sampleridx, int32t lod);
    bool    SetTexSamplerMaxLod(uint32t sampleridx, int32t lod);
    bool    SetTexSamplerMaxAnisotroty(uint32t sampleridx, int32t anisotropy);
    bool    SetTexSamplerCompare(uint32t sampleridx, GAPIFUNCTION func, bool modeon);

    bool    SetTextureSampler(uint32t texidx, uint32t texunit);
    bool    SetTextureSamplers(const uint32t *pTexIdx, uint32t texunitcnt);
    void    DisableTextureSamplers();

    void    EnableTexEnvStatesCheck(bool flag);
    
    //////////////////////////////////////////////////////////////////////////////////////////////
    // Shader Management Functions
    uint32t CreateShader(const char *strshader);
    bool    DeleteShader(uint32t shaderidx);

    uint32t CreateShaderParamsGroup(uint32t reservesize=0);
    bool    DeleteShaderParamsGroup(uint32t shaderidx);
    uint32t AddShaderParam(uint32t groupidx, const char *name, GAPIUNIFORMTYPE type, const void *data);
    bool    UpdateShaderParam(uint32t groupidx, uint32t paramidx, const void *data);
    bool    SetShaderParams(uint32t shaderidx, uint32t groupidx, uint32t commgroupidx=0);
    uint32t GetShaderParamIndex(uint32t groupidx, const char *name);

    uint32t CreateUniformBuffer(GAPIBUFFERUSAGE usage, uint32t bufsize, const void *data);
    bool    AddUniformBlocks(uint32t shaderidx, uint32t *bufidx, const char **names, uint32t count);

	bool	SetShader(uint32t shaderidx);

    // Shader Uniform Block Handling Functions
    bool    SetShaderUniform(GAPIUNIFORMTYPE type, const char *name, const void *data);
    bool    SetShaderProgramUniform(uint32t shaderidx, GAPIUNIFORMTYPE type, const char *name, const void *data);
    bool    SetShaderUniforms(GAPIUNIFORMDATA *data, uint32t count);
    bool    SetShaderProgramUniforms(uint32t shaderidx, GAPIUNIFORMDATA *data, uint32t count);

    uint32t GetUniformBlockSize(uint32t shaderidx, const char *blockname);
    bool    GetActiveUniformInfo(uint32t shaderidx, GAPIACTIVEUNIFORMTYPE type, const char **names, uint32t *pvalues, uint32t count);
    bool    SetUniformBlockBinding(uint32t shaderidx, const char *blockname, uint32t bindingpoint);
    //--uint32t GetUniformBufferBindingPoint(uint32t bufidx);
    bool    SetUniformBufferBinding(uint32t bufidx, uint32t bindpoint);

    //////////////////////////////////////////////////////////////////////////////////////////////
    // Fragment Data Location
    void SetFragDataLocation(uint32t shaderidx, uint32t colornumber, const char *name);
    void SetFragDataLocation(uint32t shaderidx, uint32t *colorsnumbers, const char **names, uint32t count);
    uint32t GetFragDataLocation(uint32t shaderidx, const char *name);

    //////////////////////////////////////////////////////////////////////////////////////////////
    // Blend Func Extended
    void SetFragDataLocationIndexed(uint32t shaderidx, uint32t colornumber, uint32t index, const char *name);
    void SetFragDataLocationIndexed(uint32t shaderidx, uint32t *colorsnumbers, uint32t *indices, const char **names, uint32t count);
    uint32t GetFragDataIndex(uint32t shaderidx, const char *name);

	//////////////////////////////////////////////////////////////////////////////////////////////
	// Direct Vertex Attribute Management Functions
	void    SetVertexAttribPointer(uint32t stream, uint32t size, GAPIDATATYPE type, bool normalize, uint32t stride, const void *pointer);
    void    SetVertexAttribBuffer(uint32t stream, uint32t size, GAPIDATATYPE type, bool normalize, uint32t stride, uint32t offset, uint32t bufidx);
    void    SetVertexAttribPointerI(uint32t stream, uint32t size, GAPIDATATYPE type, uint32t stride, const void *pointer);
    void    SetVertexAttribBufferI(uint32t stream, uint32t size, GAPIDATATYPE type, uint32t stride, uint32t offset, uint32t buf_idx);
    void    SetVertexAttribPointerL(uint32t stream, uint32t size, uint32t stride, const void *pointer);
    void    SetVertexAttribBufferL(uint32t stream, uint32t size, uint32t stride, uint32t offset, uint32t buf_idx);

    void    SetVertexAttribBuffers(GAPIVERTATTRIBDATA *attribbuf, uint32t count);
	uint32t	GetVertexAttribLocation(uint32t shaderidx, const char *attribname);

    void    SetIndexBuffer(uint32t buf_idx);

	void    DisableVertexAttribPointers();

    //////////////////////////////////////////////////////////////////////////////////////////////
    // Vertex Object Managment Functions
    uint32t CreateVertexBuffer(GAPIBUFFERUSAGE usage, uint32t bufsize, const void *data);
    bool UpdateVertexBuffer(uint32t buf_idx, uint32t first, uint32t size, const void *data);
    bool DeleteVertexBuffer(uint32t buf_idx);
    bool SetVertexBuffer(uint32t bufferid);

	//////////////////////////////////////////////////////////////////////////////////////////////
	// Direct Drawing Functions
	void DrawArrays(GAPIPRIMITIVETYPE mode, uint32t first, uint32t count);
	void DrawElements(GAPIPRIMITIVETYPE mode, uint32t count, GAPIDATATYPE type, const void *indices);

    void MultiDrawArrays(GAPIPRIMITIVETYPE mode, const uint32t *first, const uint32t *count, uint32t drawcount);
    void MultiDrawElements(GAPIPRIMITIVETYPE mode, const uint32t *count, GAPIDATATYPE type, const void *const *indices, uint32t drawcount);

    void DrawRangeElements(GAPIPRIMITIVETYPE mode, uint32t start, uint32t end, uint32t count, GAPIDATATYPE type, const void *indices);
    
    void DrawArraysInstanced(GAPIPRIMITIVETYPE mode, uint32t first, uint32t count, uint32t instance_count);
    void DrawElementsInstanced(GAPIPRIMITIVETYPE mode, uint32t count, GAPIDATATYPE type, const void *indices, uint32t instancecount);

    /*
    typedef struct {
          GLuint count;
          GLuint primCount;
          GLuint first;
          GLuint reservedMustBeZero;
        } DrawArraysIndirectCommand;
    */
    void DrawArraysInderict(GAPIPRIMITIVETYPE mode, const void *indirect);
    /*
    typedef struct {
          GLuint count;
          GLuint primCount;
          GLuint firstIndex;
          GLint  baseVertex;
          GLuint reservedMustBeZero;
        } DrawElementsIndirectCommand;
    */
    void DrawElementsIndirect(GAPIPRIMITIVETYPE mode, GAPIDATATYPE type, const void *indirect);

    void DrawArraysInstancedBaseInstance(GAPIPRIMITIVETYPE mode, uint32t first, uint32t count, uint32t instancecount, uint32t baseinstance);
    void MultiDrawArraysIndirect(GAPIPRIMITIVETYPE mode, const void *indirect, uint32t drawcount, uint32t stride);
    void DrawElementsInstancedBaseInstance(GAPIPRIMITIVETYPE mode, uint32t count, GAPIDATATYPE type, const void *indices, uint32t instancecount, uint32t baseinstance);

    void DrawElementsBaseVertex(GAPIPRIMITIVETYPE mode, uint32t count, GAPIDATATYPE type, const void *indices, int32t basevertex);
    void DrawRangeElementsBaseVertex(GAPIPRIMITIVETYPE mode, uint32t start, uint32t end, uint32t count, GAPIDATATYPE type, const void *indices, int32t basevertex);
    void DrawElementsInstancedBaseVertex(GAPIPRIMITIVETYPE mode, uint32t count, GAPIDATATYPE type, const void *indices, uint32t instancecount, int32t basevertex);
    void DrawElementsInstancedBaseVertexBaseInstance(GAPIPRIMITIVETYPE mode, uint32t count, GAPIDATATYPE type, const void *indices, uint32t instancecount, int32t basevertex, uint32t baseinstance);
    void MultiDrawElementsBaseVertex(GAPIPRIMITIVETYPE mode, const uint32t *count, GAPIDATATYPE type, const void *const *indices, uint32t drawcount, int32t *basevertex);



    
protected:

    void InitGLExtensions();
};

/////////////////////////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////////////////////////////