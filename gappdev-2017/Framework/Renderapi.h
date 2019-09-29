/////////////////////////////////////////////////////////////////////////////////////////////
// Renderapi.h
//
// NULL Renderer API
/////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _GRAPHICAPI_
#define _GRAPHICAPI_

#include "PlatformTypes.h"

enum GAPIFUNCTION 
{
    NEVER    = 0,
    LESS	 = 1,
    EQUAL	 = 2,
    LEQUAL	 = 3,
    GREATER	 = 4,
    NOTEQUAL = 5,
    GEQUAL	 = 6,
    ALWAYS	 = 7,
};


enum GAPIOPERATION 
{
    KEEP	= 0,
    ZERO	= 1,
    REPLACE	= 2,
    INCRSAT	= 3,
    DECRSAT	= 4,
    INVERT	= 5,
    INCR	= 6,
    DECR	= 7,
};

enum GAPIBLENDOPERATION 
{
    BLENDOP_ZERO				= 0,
    BLENDOP_ONE					= 1,				
    BLENDOP_DST_COLOR			= 2, //For Source-Blending Factor Only
    BLENDOP_ONE_MINUS_DST_COLOR	= 3, //For Source-Blending Factor Only
    BLENDOP_SRC_ALPHA			= 4,
    BLENDOP_ONE_MINUS_SRC_ALPHA	= 5,
    BLENDOP_DST_ALPHA			= 6,
    BLENDOP_ONE_MINUS_DST_ALPHA = 7,
    BLENDOP_SRC_ALPHA_SATURATE	= 8,
    BLENDOP_SRC_COLOR			= 9, //For Destination-Blending  Factor Only
    BLENDOP_ONE_MINUS_SRC_COLOR	= 10,//For Destination-Blending  Factor Only
};

enum GAPIBLENDFUNCTION
{
    BLENDFUNC_ADD               = 0,
    BLENDFUNC_SUBTRACT          = 1,
    BLENDFUNC_REVERSE_SUBTRACT  = 2,
    BLENDFUNC_MIN               = 3,
    BLENDFUNC_MAX               = 4,
};

typedef GAPIFUNCTION        GAPIFUNC;
typedef GAPIOPERATION       GAPIOP;
typedef GAPIBLENDOPERATION  GAPIBLENDOP;
typedef GAPIBLENDFUNCTION   GAPIBLENDFUNC;

//Fill Mode
enum GAPIFILLMODE
{
    GAPIFILL_NONE       = 0,
    GAPIFILL_POINT      = 1,
    GAPIFILL_WIREFRAME  = 2,
    GAPIFILL_SOLID      = 3,
};

//Texture Parameters
enum TEXTURETYPE 
{
    TEX_NONE,
    TEX_1D,
    TEX_2D,
    TEX_3D,
    TEX_CUBE,
};


enum TEXTUREFILTER 
{
    //Single Texture Filters
    TEXFLT_POINT,
    TEXFLT_LINEAR,

    //Mipmap Texture Filters
    TEXFLT_BILINEAR,
    TEXFLT_TRILINEAR,

    //Mipmap Texture Filters With Anisotropic
    TEXFLT_BILINEAR_ANISO,
    TEXFLT_TRILINEAR_ANISO,
};

#define TEXF TEXTUREFILTER

enum TEXTUREADDRESS {
    TEXADDR_REPEAT		=	1,
    TEXADDR_CLAMP		=	2,
    TEXADDR_BORDER		=	3,
    TEXADDR_MIRROR		=	4,
    TEXADDR_MIRRORONCE	=	5,
};

enum TEXTUREFORMAT
{
    TEXFMT_NONE, 
    TEXFMT_L8,
    TEXFMT_LA8,
    TEXFMT_RGB8,
    TEXFMT_RGBA8,
    TEXFMT_BGR8,
    TEXFMT_BGRA8,
    TEXFMT_L16,
    TEXFMT_LA16,
    TEXFMT_RG16,
    TEXFMT_RGB16,
    TEXFMT_RGBA16,
    TEXFMT_R16f,
    TEXFMT_RG16f,
    TEXFMT_RGB16f,
    TEXFMT_RGBA16f,
    TEXFMT_R32f,
    TEXFMT_RG32f,
    TEXFMT_RGB32f,
    TEXFMT_RGBA32f,

    TEXFMT_UV8,
    TEXFMT_UVWQ8,
    TEXFMT_UV16,
    TEXFMT_UVWQ16,

    TEXFMT_DEPTH16,
    TEXFMT_DEPTH24,
    TEXFMT_L4A4,
    TEXFMT_A4RGB4,
    TEXFMT_R3G3B2,
    TEXFMT_R5G6B5,
    TEXFMT_A1RGB5,
    TEXFMT_A2RGB10,
    TEXFMT_RG11B10,
    TEXFMT_RGB9E5,

    TEXFMT_UV5L6,
    TEXFMT_UVW10A2,

    TEXFMT_DXT1,
    TEXFMT_DXT3,
    TEXFMT_DXT5,
    TEXFMT_ATI1N,
    TEXFMT_ATI2N,
};


typedef TEXTURETYPE TEXTYPE;
typedef TEXTUREFORMAT TEXFMT;
typedef TEXTUREFILTER TEXFLT;
typedef TEXTUREADDRESS TEXADDR;


enum CUBEMAPFACE {
    FACE_POSITIVE_X = 0,
    FACE_NEGATIVE_X = 1,
    FACE_POSITIVE_Y = 2,
    FACE_NEGATIVE_Y = 3,
    FACE_POSITIVE_Z = 4,
    FACE_NEGATIVE_Z = 5,
};

// Rendering Primitive Type
enum GAPIPRIMITIVETYPE
{
    GAPI_POINTS,
    GAPI_LINES,
    GAPI_LINE_STRIP,
    GAPI_LINE_LOOP,
    GAPI_TRIANGLES,
    GAPI_TRIANGLE_STRIP, 
    GAPI_TRIANGLE_FAN,
    GAPI_QUAD,
    GAPI_QUAD_STRIP,
};

typedef GAPIPRIMITIVETYPE GAPIPRIMTYPE;

enum VERTEXELEM
{
    VERTELEM_POSITION,
    VERTELEM_NORMAL,
    VERTELEM_DIFFUSE,
    VERTELEM_SPECULAR,
    VERTELEM_TANGENT,
    VERTELEM_BINORMAL,
    VERTELEM_FOGCOORD,
    VERTELEM_POINTSIZE,
    VERTELEM_TEXCOORD,
    VERTELEM_ATTRIBUTE,
    VERTELEM_INDEX,
};


enum GAPIDATATYPE
{
    GAPI_NONE,
    GAPI_FLOAT,
    GAPI_DOUBLE,
    GAPI_HALF,
    GAPI_FIXED,
    GAPI_BYTE,
    GAPI_UBYTE,
    GAPI_INT16,
    GAPI_UINT16,
    GAPI_INT32,
    GAPI_UINT32,
};

enum GAPIMODEFUNC
{
    GAPI_LINEAR,
    GAPI_EXP,
    GAPI_EXP2,
};

enum GAPIBUFFERUSAGE 
{
    GAPI_BUFUSAGE_DYNAMIC,
    GAPI_BUFUSAGE_STATIC,
    GAPI_BUFUSAGE_STREAM,
};

struct GAPIVERTATTRIBDATA
{
    uint32t StreamIndex;
    char AttribName[64];
    uint32t Size;
    GAPIDATATYPE Type;
    uint32t Stride;
    uint32t Offset;
    bool Normalize;
    bool DirectFunc;
    uint32t BufferIdx;
};



enum GAPIUNIFORMTYPE
{
    GAPI_UNIFORM_INT,
    GAPI_UNIFORM_UINT,
    GAPI_UNIFORM_FLOAT,
    GAPI_UNIFORM_DOUBLE,

    GAPI_UNIFORM_VECTOR2I,
    GAPI_UNIFORM_VECTOR3I,
    GAPI_UNIFORM_VECTOR4I,

    GAPI_UNIFORM_VECTOR2UI,
    GAPI_UNIFORM_VECTOR3UI,
    GAPI_UNIFORM_VECTOR4UI,

    GAPI_UNIFORM_VECTOR2F,
    GAPI_UNIFORM_VECTOR3F,      
    GAPI_UNIFORM_VECTOR4F,
    GAPI_UNIFORM_VECTOR2D,
    GAPI_UNIFORM_VECTOR3D,
    GAPI_UNIFORM_VECTOR4D,
    GAPI_UNIFORM_MTX33F,
    GAPI_UNIFORM_MTX44F,
    GAPI_UNIFORM_MTX33D,
    GAPI_UNIFORM_MTX44D,
};

struct GAPIUNIFORMDATA
{
    GAPIUNIFORMTYPE Type;
    const char *Name;
    const void *Data;
};

enum GAPIACTIVEUNIFORMTYPE
{
    GAPI_UNIFORM_SIZE,
    GAPI_UNIFORM_OFFSET,
};

enum GAPIDEVCAPSPARAM
{
    GAPI_DEVPARAM_MULTISAMPLE_SAMPLES,
    GAPI_DEVPARAM_VERTICAL_SYNC,
    GAPI_DEVPARAM_VERTICAL_SYNC_TEAR,

    GAPI_DEVPARAM_MAX_LIGHTS,
    GAPI_DEVPARAM_MAX_TEXTURE_SIZE,
    GAPI_DEVPARAM_MAX_TEXTURE_UNITS,
    GAPI_DEVPARAM_MAX_TEXTURE_ANISOTROPY,

    GAPI_DEVPARAM_MIN_POINT_SIZE,
    GAPI_DEVPARAM_MAX_POINT_SIZE,

    // Vertex Shader Params
    GAPI_DEVPARAM_VERTEX_SHADER,
    GAPI_DEVPARAM_MAX_VERTEX_ATTRIBS,
    GAPI_DEVPARAM_MAX_VERTEX_UNIFORM_COMPONENTS,
    GAPI_DEVPARAM_MAX_VERTEX_VARYING_COMPONENTS,
    GAPI_DEVPARAM_MAX_VARYING_COMPONENTS,
    GAPI_DEVPARAM_MAX_VERTEX_TEXTURE_UNITS,
    GAPI_DEVPARAM_MAX_VARYING_FLOATS,
    GAPI_DEVPARAM_MAX_COMBINED_TEXTURE_UNITS,

    // Fragment Shader Params
    GAPI_DEVPARAM_PIXEL_SHADER,
    GAPI_DEVPARAM_MAX_FRAGMENT_UNIFORM_COMPONENTS,
    GAPI_DEVPARAM_MAX_FRAGMENT_TEXTURE_UNITS,

    // Geometry Shader Params
    GAPI_DEVPARAM_GEOMETRY_SHADER,
    GAPI_DEVPARAM_MAX_GEOMETRY_OUTPUT_VERT,
    GAPI_DEVPARAM_MAX_GEOMETRY_TEXTURE_UNITS,
    GAPI_DEVPARAM_MAX_GEOMETRY_VARYING_COMPONENTS,
    GAPI_DEVPARAM_MAX_GEOMETRY_UNIFORM_COMPONENTS,
    GAPI_DEVPARAM_MAX_GEOMETRY_TOTAL_OUTPUT_COMPONENTS,

    // Uniform Blocks Params
    GAPI_DEVPARAM_UNIFORM_BLOCKS,
    GAPI_DEVPARAM_MAX_VERTEX_UNIFORM_BLOCKS,
    GAPI_DEVPARAM_MAX_GEOMETRY_UNIFORM_BLOCKS,
    GAPI_DEVPARAM_MAX_FRAGMENT_UNIFORM_BLOCKS,
    GAPI_DEVPARAM_MAX_COMBINED_UNIFORM_BLOCKS,
    GAPI_DEVPARAM_MAX_UNIFORM_BUFFER_BINDINGS,
    GAPI_DEVPARAM_MAX_UNIFORM_BLOCK_SIZE,
    GAPI_DEVPARAM_MAX_COMBINED_VERTEX_UNIFORM_COMPONENTS,
    GAPI_DEVPARAM_MAX_COMBINED_GEOMETRY_UNIFORM_COMPONENTS,
    GAPI_DEVPARAM_MAX_COMBINED_FRAGMENT_UNIFORM_COMPONENTS,

    // Draw Buffers Params
    GAPI_DEVPARAM_MAX_DRAW_BUFFERS,

    // Clip Distances
    GAPI_DEVPARAM_MAX_CLIP_DISTANCES,

    // Transform Feedback
    GAPI_DEVPARAM_MAX_TRANSFORM_FEEDBACK_SEPARATE_COMPONENTS,
    GAPI_DEVPARAM_MAX_TRANSFORM_FEEDBACK_INTERLEAVED_COMPONENTS,
    GAPI_DEVPARAM_MAX_TRANSFORM_FEEDBACK_SEPARATE_ATTRIBS,

    // Blend Func Extended
    GAPI_DEVPARAM_MAX_DUAL_SOURCE_DRAW_BUFFERS,

    // Texture Layers
    GAPI_DEVPARAM_MAX_ARRAY_TEXTURE_LAYERS,
    GAPI_DEVPARAM_MIN_PROGRAM_TEXEL_OFFSET,
    GAPI_DEVPARAM_MAX_PROGRAM_TEXEL_OFFSET,

    // Texture Rectangle
    GAPI_DEVPARAM_MAX_RECTANGLE_TEXTURE_SIZE,

    // Program Texture
    GAPI_DEVPARAM_MIN_SAMPLE_SHADING_VALUE,
    GAPI_DEVPARAM_MIN_PROGRAM_TEXTURE_GATHER_OFFSET,
    GAPI_DEVPARAM_MAX_PROGRAM_TEXTURE_GATHER_OFFSET,
    GAPI_DEVPARAM_MAX_PROGRAM_TEXTURE_GATHER_COMPONENTS,

    // Texture Cube Map
    GAPI_DEVPARAM_MAX_CUBE_MAP_TEXTURE_SIZE,

    // Framebuffer Object
    GAPI_DEVPARAM_MAX_RENDERBUFFER_SIZE,
    GAPI_DEVPARAM_MAX_COLOR_ATTACHMENTS,
    GAPI_DEVPARAM_MAX_SAMPLES,

    // Texture Buffer Object
    GAPI_DEVPARAM_MAX_TEXTURE_BUFFER_SIZE,

    // Texture Multisample
    GAPI_DEVPARAM_MAX_SAMPLE_MASK_WORDS,
    GAPI_DEVPARAM_MAX_COLOR_TEXTURE_SAMPLES,
    GAPI_DEVPARAM_MAX_DEPTH_TEXTURE_SAMPLES,
    GAPI_DEVPARAM_MAX_INTEGER_SAMPLES,

    // GPU Shader5
    GAPI_DEVPARAM_MAX_GEOMETRY_SHADER_INVOCATIONS,
    GAPI_DEVPARAM_MIN_FRAGMENT_INTERPOLATION_OFFSET,
    GAPI_DEVPARAM_MAX_FRAGMENT_INTERPOLATION_OFFSET,
    GAPI_DEVPARAM_FRAGMENT_INTERPOLATION_OFFSET_BITS,
    
    // Tessellation Shader
    GAPI_DEVPARAM_MAX_PATCH_VERTICES, 
    GAPI_DEVPARAM_MAX_TESS_GEN_LEVEL,            
    GAPI_DEVPARAM_MAX_TESS_CONTROL_UNIFORM_COMPONENTS,
    GAPI_DEVPARAM_MAX_TESS_EVALUATION_UNIFORM_COMPONENTS,
    GAPI_DEVPARAM_MAX_TESS_CONTROL_TEXTURE_IMAGE_UNITS,
    GAPI_DEVPARAM_MAX_TESS_EVALUATION_TEXTURE_IMAGE_UNITS,
    GAPI_DEVPARAM_MAX_TESS_CONTROL_OUTPUT_COMPONENTS,
    GAPI_DEVPARAM_MAX_TESS_PATCH_COMPONENTS,
    GAPI_DEVPARAM_MAX_TESS_CONTROL_TOTAL_OUTPUT_COMPONENTS,
    GAPI_DEVPARAM_MAX_TESS_EVALUATION_OUTPUT_COMPONENTS,
    GAPI_DEVPARAM_MAX_TESS_CONTROL_UNIFORM_BLOCKS,
    GAPI_DEVPARAM_MAX_TESS_EVALUATION_UNIFORM_BLOCKS,
    GAPI_DEVPARAM_MAX_TESS_CONTROL_INPUT_COMPONENTS,
    GAPI_DEVPARAM_MAX_TESS_EVALUATION_INPUT_COMPONENTS,
    GAPI_DEVPARAM_MAX_COMBINED_TESS_CONTROL_UNIFORM_COMPONENTS,
    GAPI_DEVPARAM_MAX_COMBINED_TESS_EVALUATION_UNIFORM_COMPONENTS,

    // Transform Feedback3
    GAPI_DEVPARAM_MAX_TRANSFORM_FEEDBACK_BUFFERS,
    GAPI_DEVPARAM_MAX_VERTEX_STREAMS,

    // Viewport Array
    GAPI_DEVPARAM_MAX_VIEWPORTS,

    // Debug Output
    GAPI_DEVPARAM_MAX_DEBUG_MESSAGE_LENGTH,
    GAPI_DEVPARAM_MAX_DEBUG_LOGGED_MESSAGES,

    // Map Buffer Alignment
    GAPI_DEVPARAM_MIN_MAP_BUFFER_ALIGNMENT,

    // Shader Atomic Counters
    GAPI_DEVPARAM_MAX_VERTEX_ATOMIC_COUNTER_BUFFERS,
    GAPI_DEVPARAM_MAX_TESS_CONTROL_ATOMIC_COUNTER_BUFFERS,
    GAPI_DEVPARAM_MAX_TESS_EVALUATION_ATOMIC_COUNTER_BUFFERS,
    GAPI_DEVPARAM_MAX_GEOMETRY_ATOMIC_COUNTER_BUFFERS,
    GAPI_DEVPARAM_MAX_FRAGMENT_ATOMIC_COUNTER_BUFFERS,
    GAPI_DEVPARAM_MAX_COMBINED_ATOMIC_COUNTER_BUFFERS,
    GAPI_DEVPARAM_MAX_VERTEX_ATOMIC_COUNTERS,
    GAPI_DEVPARAM_MAX_TESS_CONTROL_ATOMIC_COUNTERS,
    GAPI_DEVPARAM_MAX_TESS_EVALUATION_ATOMIC_COUNTERS,
    GAPI_DEVPARAM_MAX_GEOMETRY_ATOMIC_COUNTERS,
    GAPI_DEVPARAM_MAX_FRAGMENT_ATOMIC_COUNTERS,
    GAPI_DEVPARAM_MAX_COMBINED_ATOMIC_COUNTERS,
    GAPI_DEVPARAM_MAX_ATOMIC_COUNTER_BUFFER_SIZE,
    GAPI_DEVPARAM_MAX_ATOMIC_COUNTER_BUFFER_BINDINGS,

    // Shader Image Load Store
    GAPI_DEVPARAM_MAX_IMAGE_UNITS,
    GAPI_DEVPARAM_MAX_COMBINED_IMAGE_UNITS_AND_FRAGMENT_OUTPUTS,

    GAPI_DEVPARAM_MAX_IMAGE_SAMPLES,
    GAPI_DEVPARAM_MAX_VERTEX_IMAGE_UNIFORMS,
    GAPI_DEVPARAM_MAX_TESS_CONTROL_IMAGE_UNIFORMS,
    GAPI_DEVPARAM_MAX_TESS_EVALUATION_IMAGE_UNIFORMS,
    GAPI_DEVPARAM_MAX_GEOMETRY_IMAGE_UNIFORMS,
    GAPI_DEVPARAM_MAX_FRAGMENT_IMAGE_UNIFORMS,
    GAPI_DEVPARAM_MAX_COMBINED_IMAGE_UNIFORMS,
    
};

#define GAPI_COORD_UPPER_LEFT 0
#define GAPI_COORD_LOWER_LEFT 1

#define GAPI_TEX_GENMIPMAPS_FLAG    0x1
#define GAPI_TEX_WORD_ALIGN_FLAG    0x2
#define GAPI_TEX_DWORD_ALIGN_FLAG   0x4
#define GAPI_TEX_CUBE_MAP_FLAG      0x8
#define GAPI_TEX_RECT_FLAG          0x10
#define GAPI_TEX_RENDER_TARGET_FLAG 0x20
#define GAPI_TEX_ARRAY_FLAG         0x40


#define GAPI_PARAMS_QUERY_FLAG      0x1


/*
enum GAPIDEVCAPSPARAM
{
GAPI_DEVPARAM_COLOR_BUFFER_FLOAT,
GAPI_DEVPARAM_DEPTH_BUFFER_FLOAT,
GAPI_DEVPARAM_HALF_FLOAT_PIXEL,
GAPI_DEVPARAM_HALF_FLOAT_VERTEX,
GAPI_DEVPARAM_POINT_SPRITE,
GAPI_DEVPARAM_TEXTURE_FLOAT,
GAPI_DEVPARAM_TEXTURE_NON_POWER_OF_TWO,
GAPI_DEVPARAM_TEXTURE_RECTANGLE,
GAPI_DEVPARAM_TEXTURE_3D,
GAPI_DEVPARAM_SEPARATE_STENCIL,
GAPI_DEVPARAM_STENCIL_TWO_SIDE,
GAPI_DEVPARAM_BLEND_EQUATION_SEPARATE,
GAPI_DEVPARAM_BLEND_FUNC_SEPARATE,
GAPI_DEVPARAM_BLEND_MINMAX,
GAPI_DEVPARAM_BLEND_COLOR,
GAPI_DEVPARAM_OCCLUSION_QUERY,
GAPI_DEVPARAM_TEXTURE_MIRROR_REPEAT,
GAPI_DEVPARAM_TEXTURE_MIRROR_ONCE,
GAPI_DEVPARAM_TEXTURE_BORDER_CLAMP,
GAPI_DEVPARAM_ABGR,
GAPI_DEVPARAM_BGRA,
GAPI_DEVPARAM_SECONDARY_COLOR,
GAPI_DEVPARAM_TEXTURE_MIRROR_CLAMP,
GAPI_DEVPARAM_TEXTURE_COMPRESSION_3DC,
GAPI_DEVPARAM_TEXTURE_COMPRESSION_S3TC,

GAPI_DEVPARAM_MULTISAMPLE,

GAPI_DEVPARAM_MAX_TEXTURE_SIZE,
GAPI_DEVPARAM_MAX_TEXTURE_UNITS,
GAPI_DEVPARAM_MAX_TEXTURE_ANISOTROPY,

GAPI_DEVPARAM_MIN_POINT_SIZE,
GAPI_DEVPARAM_MAX_POINT_SIZE,

GAPI_DEVPARAM_MAX_VERTEX_ATTRIBS,
GAPI_DEVPARAM_MAX_FRAGMENT_UNIFORM_COMPONENTS,
GAPI_DEVPARAM_MAX_VERTEX_UNIFORM_COMPONENTS,
GAPI_DEVPARAM_MAX_VERTEX_VARYING_COMPONENTS,
GAPI_DEVPARAM_MAX_VARYING_COMPONENTS,
GAPI_DEVPARAM_MAX_VERTEX_TEXTURE_UNITS,
GAPI_DEVPARAM_MAX_VARYING_FLOATS,

GAPI_DEVPARAM_GEOMETRY_SHADER,
GAPI_DEVPARAM_MAX_GEOMETRY_OUTPUT_VERT,
GAPI_DEVPARAM_MAX_GEOMETRY_TEXTURE_UNITS,
GAPI_DEVPARAM_MAX_GEOMETRY_VARYING_COMPONENTS,
GAPI_DEVPARAM_MAX_GEOMETRY_UNIFORM_COMPONENTS,
GAPI_DEVPARAM_MAX_GEOMETRY_TOTAL_OUTPUT_COMPONENTS,

};
*/


struct GAPICREATIONINFO
{
	uint32t iWidth;
	uint32t iHeight;
	bool	bFullScreen;
    int     iColorBits;
	int		iDepthBits;
	int		iStencilBits;
    int     iMultisample;
    bool    bVSync;     

    void    *hWindow;
};

/////////////////////////////////////////////////////////////////////////////////////////////
class IGraphicApi
{
public:
	IGraphicApi(){}
	virtual ~IGraphicApi(){}

	virtual bool Create(const GAPICREATIONINFO *info) {return false;}
	virtual void Render() {}
	virtual void Resize(int width, int height) {}
	virtual bool SwapGraphicBuffers() {return false;}

    virtual void Clear(){}
    virtual void Clear(bool color, bool depth, bool stencil) {}

    //////////////////////////////////////////////////////////////////////////////////////////////
    // Device Caps
    virtual uint32t  GetDevCaps(GAPIDEVCAPSPARAM devparam){return 0;}

    //////////////////////////////////////////////////////////////////////////////////////////////
    // Scissor Setting Functions
    virtual void EnableScissorTest(bool flag){}
    virtual bool IsScissorTestEnabled(){return false;}
    virtual void SetScissorRect(int x, int y, int width, int height){}
    virtual void GetScissorRect(int scissor_box[4]){}

    //////////////////////////////////////////////////////////////////////////////////////////////
    // Depth Setting Functions
    virtual void EnableDepthTest(bool flag){}
    virtual void EnableDepthWrite(bool flag){}
    virtual void SetDepthFunc(GAPIFUNC func){}
    virtual void SetClearDepth(float depth = 1.0f){}

    virtual bool IsDepthTestEnabled(){return false;}
    virtual bool IsDepthWriteEnabled(){return false;}
    virtual float GetClearDepth(){return 0.0f;}
    virtual GAPIFUNC GetDepthFunc(){return (GAPIFUNC)0;}

    //////////////////////////////////////////////////////////////////////////////////////////////
    // Stencil Setting Functions
    virtual void EnableStencilTest(bool flag){}
    virtual void SetStencilWriteMask(uint32t mask){}
    virtual bool IsStencilTestEnabled(){return false;}
    virtual void SetStencilFunc(GAPIFUNC func, uint32t ref, uint32t mask){}
    virtual void SetStencilOp(GAPIOP fail, GAPIOP zfail, GAPIOP pass){}
    virtual void EnableTwoSideStencil(bool flag){}
    virtual void SetClearStencil(uint32t val=0){}
    virtual bool IsTwoSideStencilEnabled(){return false;}

    virtual uint32t GetStencilWriteMask(){return 0;}
    virtual GAPIFUNC GetStencilFunc(){return (GAPIFUNC)0;}
    virtual uint32t GetStencilBits(){return 0;}
    virtual uint32t GetStencilClearVal(){return 0;}
    virtual uint32t GetStencilFuncMask(){return 0;}
    virtual uint32t GetStencilFuncRef(){return 0;}
    virtual GAPIOP  GetStencilOpFail(){return (GAPIOP)0;}
    virtual GAPIOP  GetStencilOpDepthFail(){return (GAPIOP)0;}
    virtual GAPIOP  GetStencilOpDepthPass(){return (GAPIOP)0;}

    virtual void SetStencilFuncSeparate(GAPIFUNC frontfunc, GAPIFUNC backfunc, uint32t ref, uint32t mask){}
    virtual void SetStencilOpSeparate(bool frontface, GAPIOP fail, GAPIOP zfail, GAPIOP pass){}
    virtual void SetStencilMaskSeparate(bool frontface, uint32t mask){}

    //////////////////////////////////////////////////////////////////////////////////////////////
    // Raster Functions
    virtual void SetRect(int32t x1, int32t y1, int32t x2, int32t y2){}
    virtual void SetRect(float x1, float y1, float x2, float y2){}
    virtual void SetRasterPos(int32t x, int32t y){}
    virtual void SetRasterPos(float x, float y){}
    virtual void SetColor(ubyte red, ubyte green, ubyte blue, ubyte alpha){}
    virtual void SetColor(ubyte color[4]){}
    virtual void SetColor(unsigned color){}
    virtual void DrawBitmap(int width, int height, float xorig, float yorig, float xmove, float ymove, const ubyte *bitmap){}

    //////////////////////////////////////////////////////////////////////////////////////////////
    // Matrix Handling
    virtual void SetProjMatrix(const float matrix[16]){}

    //////////////////////////////////////////////////////////////////////////////////////////////
    // Viewport Setting
    virtual void SetViewport(int x, int y, int width, int height){}
    virtual void GetViewport(int rect[4]){}
        

    //////////////////////////////////////////////////////////////////////////////////////////////
    // Alpha Test
    virtual void EnableAlphaTest(bool flag){}
    virtual bool IsAlphaTestEnabled(){return false;}
    virtual void SetAlphaFunc(GAPIFUNC func, float ref){}
    virtual GAPIFUNC GetAlphaTestFunc(){return (GAPIFUNC)0;}
    virtual float GetAlphaTestRef(){return 0.0f;}

    //////////////////////////////////////////////////////////////////////////////////////////////
    // Alpha To Coverage Test
    virtual void EnableSampleAlphaToMask(bool flag){}
    virtual bool IsSampleAlphaToMaskEnabled(){return false;}
    virtual void SetSampleCoverage(float val, bool invert){}
    virtual void GetSampleCoverage(float &val, bool &invert){}

    //////////////////////////////////////////////////////////////////////////////////////////////
    // Color Blending
    virtual void EnableColorBlend(bool flag){}
    virtual bool IsColorBlendEnabled(){return false;}
    virtual void SetBlendFunc(GAPIBLENDOP src, GAPIBLENDOP dest){}
    virtual GAPIBLENDOP GetSrcBlendFunc(){return (GAPIBLENDOP)0;}
    virtual GAPIBLENDOP GetDestBlendFunc(){return (GAPIBLENDOP)0;}

    virtual void SetBlendEquation(GAPIBLENDFUNC mode){}
    virtual void SetBlendEquationSeparate(GAPIBLENDFUNC modeRGB, GAPIBLENDFUNC modeAlpha){}
    virtual void SetBlendFuncSeparate(GAPIBLENDOP srcRGB, GAPIBLENDOP destRGB, GAPIBLENDOP srcAlpha, GAPIBLENDOP destAlpha){}

    virtual void SetBlendColor(float red, float green, float blue, float alpha){}

    //////////////////////////////////////////////////////////////////////////////////////////////
    // Setting Framebuffer Clear Color
    virtual void SetClearColor(float red, float green, float blue, float alpha){}

    //////////////////////////////////////////////////////////////////////////////////////////////
    // Setting Polygon Fill Mode
    virtual void SetFillMode(GAPIFILLMODE mode){}
    //////////////////////////////////////////////////////////////////////////////////////////////
    // Setting Polygon Cull Mode
    virtual void SetCullMode(bool CullFace,bool CCW){}
    //////////////////////////////////////////////////////////////////////////////////////////////
    // Setting Shading Mode (Gouraud or Flat Shading)
    virtual void SetShadingMode(bool bsmooth){}

    //////////////////////////////////////////////////////////////////////////////////////////////
    // Occlusion Query Managment Functions
    virtual uint32t CreateQueryObject(){return 0;}
    virtual bool    DeleteQueryObject(uint32t objidx){return false;}
    virtual void    BeginQuery(uint32t objidx){}
    virtual void    EndQuery(){}
    virtual uint32t GetQueryObjects(uint32t objidx){return 0;}
    virtual bool    QueryStarted(){return false;}

    //////////////////////////////////////////////////////////////////////////////////////////////
    // Texture Managment Functions
    virtual uint32t CreateTexture(uint32t width, uint32t height, uint32t depth, uint32t flags, TEXFMT fmt, const void **data){return 0;}
    virtual  bool   DeleteTexture(uint32t texidx){return false;}

    virtual bool    UpdateTexture(uint32t texidx, uint32t miplevel, uint32t face, uint32t align, uint32t xoff, uint32t yoff, uint32t zoff, const void *data){return false;}

    virtual bool    BindRenderTarget(uint32t texidx, uint32t face=0){return false;}
    virtual bool    UnbindRenderTarget(){return false;}

    virtual bool    SetTexFilter(uint32t texidx, TEXFLT flt){return false;}
   virtual  bool    GetTexFilter(uint32t texidx, TEXFLT &flt){return false;}

    virtual bool    SetTexAnisotropy(uint32t texidx, uint32t val){return false;}
    virtual bool    GetTexAnisotropy(uint32t texidx, uint32t &val){return false;}

    virtual bool    SetTexAddr(uint32t texidx, TEXADDR addrS, TEXADDR addrT, TEXADDR addrR){return false;}
    virtual bool    GetTexAddr(uint32t texidx, TEXADDR &addrS, TEXADDR &addrT, TEXADDR &addrR){return false;}

    virtual bool    GetTexWidth(uint32t texidx, uint32t &width){return false;}
    virtual bool    GetTexHeight(uint32t texidx, uint32t &height){return false;}
    virtual bool    GetTexDepth(uint32t texidx, uint32t &depth){return false;}

    virtual bool    SetTexture(uint32t texIdx, uint32t tex_unit){return false;}
    virtual bool    SetTexture(const uint32t *pTexIdx, uint32t tex_unit_num){return false;}
    virtual void    DisableTextures(){}

    virtual uint32t CreateTextureSampler(){return 0;}
    virtual bool    DeleteTextureSampler(uint32t sampleridx){return false;}
    virtual bool    SetTexSamplerFilter(uint32t sampleridx, TEXFLT flt){return false;}
    virtual bool    SetTexSamplerMinFilter(uint32t sampleridx, TEXFLT flt){return false;}
    virtual bool    SetTexSamplerMagFilter(uint32t sampleridx, TEXFLT flt){return false;}
    virtual bool    SetTexSamplerAddr(uint32t sampleridx, TEXADDR wrap){return false;}
    virtual bool    SetTexSamplerAddr(uint32t sampleridx, TEXADDR sWrap, TEXADDR tWrap, TEXADDR rWrap){return false;}
    virtual bool    SetTexSamplerMinLod(uint32t sampleridx, int32t lod){return false;}
    virtual bool    SetTexSamplerMaxLod(uint32t sampleridx, int32t lod){return false;}
    virtual bool    SetTexSamplerMaxAnisotroty(uint32t sampleridx, int32t anisotropy){return false;}
    virtual bool    SetTexSamplerCompare(uint32t sampleridx, GAPIFUNCTION func, bool modeon){return false;}

    virtual bool    SetTextureSampler(uint32t texidx, uint32t texunit){return false;}
    virtual bool    SetTextureSamplers(const uint32t *pTexIdx, uint32t texunitcnt){return false;}
    virtual void    DisableTextureSamplers(){}

    virtual void    EnableTexEnvStatesCheck(bool flag){}
    
    //////////////////////////////////////////////////////////////////////////////////////////////
    // Shaders Management Functions
    virtual uint32t CreateShader(const char *strshader){return 0;}
    virtual bool    DeleteShader(uint32t shaderidx){return false;}

    virtual uint32t CreateShaderParamsGroup(uint32t reservesize=0){return 0;}
    virtual bool    DeleteShaderParamsGroup(uint32t shaderidx){return false;}
    virtual uint32t AddShaderParam(uint32t groupidx, const char *name, GAPIUNIFORMTYPE type, const void *data){return 0;}
    virtual bool    UpdateShaderParam(uint32t groupidx, uint32t paramidx, const void *data){return false;}
    virtual bool    SetShaderParams(uint32t shaderidx, uint32t groupidx, uint32t commgroupidx=0){return false;}
    virtual uint32t GetShaderParamIndex(uint32t groupidx, const char *name){return 0;}

    virtual uint32t CreateUniformBuffer(GAPIBUFFERUSAGE usage, uint32t bufsize, const void *data){return 0;}
    virtual bool    AddUniformBlocks(uint32t shaderidx, uint32t *bufidx, const char **names, uint32t count){return false;}

	virtual bool	SetShader(uint32t shaderidx){return false;}


    virtual bool    SetShaderUniform(GAPIUNIFORMTYPE type, const char *name, const void *data){return false;}
    virtual bool    SetShaderProgramUniform(uint32t shaderidx, GAPIUNIFORMTYPE type, const char *name, const void *data){return false;}
    virtual bool    SetShaderUniforms(GAPIUNIFORMDATA *data, uint32t count){return false;}
    virtual bool    SetShaderProgramUniforms(uint32t shaderidx, GAPIUNIFORMDATA *data, uint32t count){return false;}

    virtual uint32t GetUniformBlockSize(uint32t shaderidx, const char *blockname){return 0;}
    virtual bool    GetActiveUniformInfo(uint32t shaderidx, GAPIACTIVEUNIFORMTYPE type, const char **names, uint32t *pvalues, uint32t count){return false;}
    virtual bool    SetUniformBlockBinding(uint32t shaderidx, const char *blockname, uint32t bindingpoint){return false;}
    //--virtual uint32t GetUniformBufferBindingPoint(uint32t bufidx){return 0xffffffff;}
    virtual bool    SetUniformBufferBinding(uint32t bufidx, uint32t bindpoint){return false;}

    //////////////////////////////////////////////////////////////////////////////////////////////
    // Fragment Data Location
    virtual void SetFragDataLocation(uint32t shaderidx, uint32t colornumber, const char *name){}
    virtual void SetFragDataLocation(uint32t shaderidx, uint32t *colorsnumbers, const char **names, uint32t count){}
    virtual uint32t GetFragDataLocation(uint32t shaderidx, const char *name){return 0xffffffff;}

    //////////////////////////////////////////////////////////////////////////////////////////////
    // Blend Func Extended
    virtual void SetFragDataLocationIndexed(uint32t shaderidx, uint32t colornumber, uint32t index, const char *name){}
    virtual void SetFragDataLocationIndexed(uint32t shaderidx, uint32t *colorsnumbers, uint32t *indices, const char **names, uint32t count){}
    virtual uint32t GetFragDataIndex(uint32t shaderidx, const char *name){return 0xffffffff;}

    //////////////////////////////////////////////////////////////////////////////////////////////
    // Vertex Object Managment Functions
    virtual uint32t CreateVertexBuffer(GAPIBUFFERUSAGE usage, uint32t bufsize, const void *data){return 0;}
    virtual bool UpdateVertexBuffer(uint32t bufid, uint32t first, uint32t size, const void *data){return false;}
    virtual bool DeleteVertexBuffer(uint32t bufid){return false;}
    virtual bool SetVertexBuffer(uint32t bufferid){return false;}

	//////////////////////////////////////////////////////////////////////////////////////////////
	// Direct Vertex Attribute Management Functions
	virtual void	SetVertexAttribPointer(uint32t index, uint32t size, GAPIDATATYPE type, bool normalize, uint32t stride, const void *pointer) {}
    virtual void    SetVertexAttribBuffer(uint32t attribidx, uint32t size, GAPIDATATYPE type, bool normalize, uint32t stride, uint32t offset, uint32t bufidx) {}
    virtual void    SetVertexAttribPointerI(uint32t stream, uint32t size, GAPIDATATYPE type, uint32t stride, const void *pointer){}
    virtual void    SetVertexAttribBufferI(uint32t stream, uint32t size, GAPIDATATYPE type, uint32t stride, uint32t offset, uint32t buf_idx){}
    virtual void    SetVertexAttribPointerL(uint32t stream, uint32t size, uint32t stride, const void *pointer){}
    virtual void    SetVertexAttribBufferL(uint32t stream, uint32t size, uint32t stride, uint32t offset, uint32t buf_idx){}
    virtual void    SetVertexAttribBuffers(GAPIVERTATTRIBDATA *attribbuf, uint32t count){}
	virtual uint32t	GetVertexAttribLocation(uint32t shaderidx, const char *attribname){return 0;}
    virtual void    SetIndexBuffer(uint32t buf_idx){}
	virtual void	DisableVertexAttribPointers(){}

	//////////////////////////////////////////////////////////////////////////////////////////////
	// Direct Drawing Functions
	virtual void	DrawArrays(GAPIPRIMITIVETYPE mode, uint32t first, uint32t count){}
	virtual void	DrawElements(GAPIPRIMITIVETYPE mode, uint32t count, GAPIDATATYPE type, const void *indices){}

    virtual void    MultiDrawArrays(GAPIPRIMITIVETYPE mode, const uint32t *first, const uint32t *count, uint32t drawcount){}
    virtual void    MultiDrawElements(GAPIPRIMITIVETYPE mode, const uint32t *count, GAPIDATATYPE type, const void *const *indices, uint32t drawcount){}

    virtual void    DrawRangeElements(GAPIPRIMITIVETYPE mode, uint32t start, uint32t end, uint32t count, GAPIDATATYPE type, const void *indices){}
    
    virtual void    DrawArraysInstanced(GAPIPRIMITIVETYPE mode, uint32t first, uint32t count, uint32t instance_count){}
    virtual void    DrawElementsInstanced(GAPIPRIMITIVETYPE mode, uint32t count, GAPIDATATYPE type, const void *indices, uint32t instancecount){}

    virtual void    DrawArraysInderict(GAPIPRIMITIVETYPE mode, const void *indirect){}
    virtual void    DrawElementsIndirect(GAPIPRIMITIVETYPE mode, GAPIDATATYPE type, const void *indirect){}

    virtual void    DrawArraysInstancedBaseInstance(GAPIPRIMITIVETYPE mode, uint32t first, uint32t count, uint32t instancecount, uint32t baseinstance){}
    virtual void    MultiDrawArraysIndirect(GAPIPRIMITIVETYPE mode, const void *indirect, uint32t drawcount, uint32t stride){}
    virtual void    DrawElementsInstancedBaseInstance(GAPIPRIMITIVETYPE mode, uint32t count, GAPIDATATYPE type, const void *indices, uint32t instancecount, uint32t baseinstance){}

    virtual void    DrawElementsBaseVertex(GAPIPRIMITIVETYPE mode, uint32t count, GAPIDATATYPE type, const void *indices, int32t basevertex){}
    virtual void    DrawRangeElementsBaseVertex(GAPIPRIMITIVETYPE mode, uint32t start, uint32t end, uint32t count, GAPIDATATYPE type, const void *indices, int32t basevertex){}
    virtual void    DrawElementsInstancedBaseVertex(GAPIPRIMITIVETYPE mode, uint32t count, GAPIDATATYPE type, const void *indices, uint32t instancecount, int32t basevertex){}
    virtual void    DrawElementsInstancedBaseVertexBaseInstance(GAPIPRIMITIVETYPE mode, uint32t count, GAPIDATATYPE type, const void *indices, uint32t instancecount, int32t basevertex, uint32t baseinstance){}
    virtual void    MultiDrawElementsBaseVertex(GAPIPRIMITIVETYPE mode, const uint32t *count, GAPIDATATYPE type, const void *const *indices, uint32t drawcount, int32t *basevertex){}

    

    
};

/////////////////////////////////////////////////////////////////////////////////////////////

#endif //_GRAPHICAPI_