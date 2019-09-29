///////////////////////////////////////////////////////////////////////////////////////////////
//	GLConvFunc.h
//	Function For Converting Enum Types Of Null Render API To OpenGL Formats
//	Created: 12-09-2008
//	
////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _GL_CONV_FUNC_H_
#define _GL_CONV_FUNC_H_

#include "Renderapi.h"

#include <GL/gl.h>
#include <GL/glext.h>


#ifdef GL_ARB_ES2_compatibility
typedef int GLfixed;
#endif

#ifndef GL_FIXED 
#define GL_FIXED 0x140C
typedef int GLfixed;
#endif 


#ifndef GL_HALF_FLOAT
#define GL_HALF_FLOAT 0x140B
typedef unsigned short GLhalfARB;
#endif 

// Converting System Class
class CGLConvFunc
{
public:
    static inline GLenum        GetGLFunc(GAPIFUNC func);
    static inline GAPIFUNC      GetGAPIFunc(GLenum glfunc);

    static inline GLenum        GetGLOp(GAPIOP op);
    static inline GAPIOP        GetGAPIOp(GLenum glop);
    static inline GLenum        GetGLBlendOp(GAPIBLENDOP blendop);
    static inline GAPIBLENDOP   GetGAPIBlendOp(GLenum glblendop);
    static inline GLenum        GetGLBlendFunc(GAPIBLENDFUNC blendfunc);
    static inline GAPIBLENDFUNC GetGAPIBlendFunc(GLenum glblendfunc);
    static inline GLenum        GetGLFillMode(GAPIFILLMODE FillMode);
    static inline GAPIFILLMODE  GetGAPIFillMode(GLenum mode);
    static inline GLint         GetGLModeFunc(GAPIMODEFUNC modefunc);
    static inline GAPIMODEFUNC  GetGAPIModeFunc(GLint mode);
    static inline GLenum        GetGLPolyOffsetType(GLenum FillMode);
    static inline GLenum        GetGLBufferDataUsage(GAPIBUFFERUSAGE bufferUsage);
    static inline GLenum        GetGLPrimitiveType(GAPIPRIMITIVETYPE primitiveType);
    static inline GAPIPRIMTYPE  GetGAPIPrimitiveType(GLenum glprim);
    static inline GLenum        GetGLDataType(GAPIDATATYPE type);
    static inline GLint         GetGLDataTypeSize(GLenum type);

    static inline GLint         GetGLTexInterFmt(TEXFMT fmt);
    static inline GLenum        GetGLTexFmt(TEXFMT fmt);
    static inline GLenum        GetGLTexDataType(TEXFMT fmt);
    static inline TEXFMT        GetGAPITexFmt(GLint interfmt, GLenum fmt);

    static inline GLint         GetGLTexFlt(TEXFLT flt);
    static inline TEXFLT        GetGAPITexFlt(GLint flt,GLint aniso);

    static inline GLint         GetGLTexAddr(TEXADDR addr);
    static inline TEXADDR       GetGAPITexAddr(GLint addr);

    static inline GLuint        GetPixelSize(TEXFMT fmt);

    static inline GLint         GetGLComponentsNum(GLenum texfmt);

    static inline GLboolean     IsCompressedFormat(TEXFMT fmt);
    static inline GLint         GetBlockSize(TEXFMT fmt);

    static inline GLboolean     IsSignedFormat(TEXFMT fmt);
    static inline GLboolean     IsMixedFormat(TEXFMT fmt);
};


/*
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
*/

///////////////////////////////////////////////////////////////////////////////////////////////
//Convert From GAPIFUNC Enum Type To GLenum Type
inline GLenum CGLConvFunc::GetGLFunc(GAPIFUNC func)
{
    GLenum glfunc;

    switch(func)
    {
    case NEVER:
	    glfunc = GL_NEVER;
	    break;

    case LESS:
	    glfunc = GL_LESS;
	    break;

    case EQUAL:
	    glfunc = GL_EQUAL;
	    break;

    case LEQUAL:
	    glfunc = GL_LEQUAL;
	    break;

    case GREATER:
	    glfunc = GL_GREATER;
	    break;

    case NOTEQUAL:
	    glfunc = GL_NOTEQUAL;
	    break;

    case GEQUAL:
	    glfunc = GL_GEQUAL;
	    break;

    case ALWAYS:
	    glfunc = GL_ALWAYS;
	    break;
    }

    return glfunc;
}

////////////////////////////////////////////////////////////////////////////////////////////////
//Convert From GLenum Function Type To GAPIFUNC Enum Type
inline GAPIFUNC CGLConvFunc::GetGAPIFunc(GLenum glfunc)
{
    GAPIFUNC gapifunc;

    switch(glfunc)
    {
    case GL_NEVER:
        gapifunc = NEVER;
        break;

    case GL_LESS:
        gapifunc = LESS;
        break;

    case GL_EQUAL:
        gapifunc = EQUAL;
        break;

    case GL_LEQUAL:
        gapifunc = LEQUAL;
        break;

    case GL_GREATER:
        gapifunc = GREATER;
        break;

    case GL_NOTEQUAL:
        gapifunc = NOTEQUAL;
        break;

    case GL_GEQUAL:
        gapifunc = GEQUAL;
        break;

    case GL_ALWAYS:
        gapifunc = ALWAYS;
        break;

    default:
        gapifunc = NEVER;
        break;
    }

    return gapifunc;
}


////////////////////////////////////////////////////////////////////////////////////////////////
// Converting From GAPIOP Enum Type To GLenum Type
inline GLenum CGLConvFunc::GetGLOp(GAPIOP op)
{
    GLenum glop;

    switch(op)
    {
    case KEEP:
		glop = GL_KEEP;
		break;

	case ZERO:
		glop = GL_ZERO;
		break;

	case REPLACE:
		glop = GL_REPLACE;
		break;

	case INCRSAT:
		glop = GL_INCR;
		break;

	case DECRSAT:
		glop = GL_DECR;
		break;

	case INVERT:
		glop = GL_INVERT;
		break;

	case INCR:
		glop = GL_INCR_WRAP_EXT;
		break;

	case DECR:
		glop = GL_DECR_WRAP_EXT;
		break;
    }

    return glop;
}

////////////////////////////////////////////////////////////////////////////////////////////////
// Converting From GAPIOP Enum Type To GLenum Type
inline GAPIOP CGLConvFunc::GetGAPIOp(GLenum glop)
{
    GAPIOP gapiop;

    switch(glop)
    {
    case GL_KEEP:
		gapiop = KEEP;
		break;

	case GL_ZERO:
		gapiop = ZERO;
		break;

	case GL_REPLACE:
		gapiop = REPLACE;
		break;

	case GL_INCR:
		gapiop = INCRSAT;
		break;

	case GL_DECR:
		gapiop = DECRSAT;
		break;

	case GL_INVERT:
		gapiop = INVERT;
		break;

	case GL_INCR_WRAP_EXT:
		gapiop = INCR;
		break;

	case GL_DECR_WRAP_EXT:
		gapiop = DECR;
		break;

    default:
        gapiop = KEEP;
        break;
    }

    return gapiop;
}

////////////////////////////////////////////////////////////////////////////////////////////////
// Converting Blending Operation From GAPIBLENDOP Enum Type To GLenum Type
inline GLenum CGLConvFunc::GetGLBlendOp(GAPIBLENDOP blendop)
{
    GLenum glblendop;

    switch(blendop)
    {
    case BLENDOP_ZERO:
		glblendop = GL_ZERO;
		break;

	case BLENDOP_ONE:
		glblendop = GL_ONE;
		break;

	case BLENDOP_DST_COLOR:
		glblendop = GL_DST_COLOR;
		break;

	case BLENDOP_ONE_MINUS_DST_COLOR:
		glblendop = GL_ONE_MINUS_DST_COLOR;
		break;

	case BLENDOP_SRC_ALPHA:
		glblendop = GL_SRC_ALPHA;
		break;

	case BLENDOP_ONE_MINUS_SRC_ALPHA:
		glblendop = GL_ONE_MINUS_SRC_ALPHA;
		break;

	case BLENDOP_DST_ALPHA:
		glblendop = GL_DST_ALPHA;
		break;

	case BLENDOP_ONE_MINUS_DST_ALPHA:
		glblendop = GL_ONE_MINUS_DST_ALPHA;
		break;

	case BLENDOP_SRC_ALPHA_SATURATE:
		glblendop = GL_SRC_ALPHA_SATURATE;
		break;

	case BLENDOP_SRC_COLOR:
		glblendop = GL_SRC_COLOR;
		break;

	case BLENDOP_ONE_MINUS_SRC_COLOR:
		glblendop = GL_ONE_MINUS_SRC_COLOR;
		break; 
    }

    return glblendop;
}

////////////////////////////////////////////////////////////////////////////////////////////////
//Converting Blending Operation From GLenum Type To GAPIBLENDOP Enum Type
inline GAPIBLENDOP CGLConvFunc::GetGAPIBlendOp(GLenum glblendop)
{
    GAPIBLENDOP gapiblendop;

    switch(glblendop)
    {
    case GL_ZERO:
		gapiblendop = BLENDOP_ZERO; 
		break;

	case GL_ONE:
		gapiblendop = BLENDOP_ONE;
		break;

	case GL_DST_COLOR:
		gapiblendop = BLENDOP_DST_COLOR;
		break;

	case GL_ONE_MINUS_DST_COLOR:
		gapiblendop = BLENDOP_ONE_MINUS_DST_COLOR;
		break;

	case GL_SRC_ALPHA:
		gapiblendop = BLENDOP_SRC_ALPHA;
		break;

	case GL_ONE_MINUS_SRC_ALPHA:
		gapiblendop = BLENDOP_ONE_MINUS_SRC_ALPHA;
		break;

	case GL_DST_ALPHA:
		gapiblendop = BLENDOP_DST_ALPHA;
		break;

	case GL_ONE_MINUS_DST_ALPHA:
		gapiblendop = BLENDOP_ONE_MINUS_DST_ALPHA;
		break;

	case GL_SRC_ALPHA_SATURATE:
		gapiblendop = BLENDOP_SRC_ALPHA_SATURATE;
		break;

	case GL_SRC_COLOR:
		gapiblendop = BLENDOP_SRC_COLOR;
		break;

	case GL_ONE_MINUS_SRC_COLOR:
		gapiblendop = BLENDOP_ONE_MINUS_SRC_COLOR;
		break; 

    default:
        gapiblendop = BLENDOP_ZERO;
        break;
    }

    return gapiblendop;
}

////////////////////////////////////////////////////////////////////////////////////////////////
// Converting Blending Function From GAPIBLENDFUNC Type To GLenum Type
inline GLenum CGLConvFunc::GetGLBlendFunc(GAPIBLENDFUNC blendfunc)
{
    GLenum glblendfunc = GL_NONE;

    switch(blendfunc)
    {
    case BLENDFUNC_ADD:
        glblendfunc = GL_FUNC_ADD;
        break;

    case BLENDFUNC_SUBTRACT:
        glblendfunc = GL_FUNC_SUBTRACT;
        break;

    case BLENDFUNC_REVERSE_SUBTRACT:
        glblendfunc = GL_FUNC_REVERSE_SUBTRACT;
        break;

    case BLENDFUNC_MIN:
        glblendfunc = GL_MIN;
        break;

    case BLENDFUNC_MAX:
        glblendfunc = GL_MAX;
        break;
    }

    return glblendfunc;
}

////////////////////////////////////////////////////////////////////////////////////////////////
// Converting Blending Function From GLenum Type To GAPIBLENDFUNC Type
inline GAPIBLENDFUNC CGLConvFunc::GetGAPIBlendFunc(GLenum glblendfunc)
{
    GAPIBLENDFUNC blendfunc;
    switch(glblendfunc)
    {
    case GL_FUNC_ADD:
        blendfunc = BLENDFUNC_ADD;
        break;

    case GL_FUNC_SUBTRACT:
        blendfunc = BLENDFUNC_SUBTRACT;
        break;

    case GL_FUNC_REVERSE_SUBTRACT:
        blendfunc = BLENDFUNC_REVERSE_SUBTRACT;
        break;

    case GL_MIN:
        blendfunc = BLENDFUNC_MIN;
        break;

    case GL_MAX:
        blendfunc = BLENDFUNC_MAX;
        break;
    }

    return blendfunc;
}

////////////////////////////////////////////////////////////////////////////////////////////////
// Converting Fill Mode From GAPIFILLMODE Enum Type To GLenum Type
inline GLenum CGLConvFunc::GetGLFillMode(GAPIFILLMODE FillMode)
{
    switch(FillMode)
    {
    case GAPIFILL_POINT:
        return GL_POINT;
        
    case GAPIFILL_WIREFRAME:
        return GL_LINE;

    case GAPIFILL_SOLID:
        return GL_FILL;

    default:
        return GL_POINT;
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////
// Converting Fill Mode From GLenum Type To GAPIFILLMODE Enum Type
inline GAPIFILLMODE CGLConvFunc::GetGAPIFillMode(GLenum mode)
{
    switch(mode)
    {
    case GL_POINT:
        return GAPIFILL_POINT;

    case GL_LINE:
        return GAPIFILL_WIREFRAME;

    case GL_FILL:
        return GAPIFILL_SOLID;

    default:
        return GAPIFILL_NONE;
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////
// Converting GAPIMODEFUNC To GLint mode Type
inline GLint CGLConvFunc::GetGLModeFunc(GAPIMODEFUNC modefunc)
{
    switch(modefunc)
    {
    case GAPI_LINEAR:
        return GL_LINEAR;

    case GAPI_EXP:
        return GL_EXP;

    case GAPI_EXP2:
        return GL_EXP2;

    default:
        return GL_LINEAR;
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////
// Converting GLint mode Type To GAPIMODEFUNC
inline GAPIMODEFUNC CGLConvFunc::GetGAPIModeFunc(GLint mode)
{
    switch(mode)
    {
    case GL_LINEAR:
        return GAPI_LINEAR;

    case GL_EXP:
        return GAPI_EXP;

    case GL_EXP2:
        return GAPI_EXP2;

    default:
        return GAPI_LINEAR;
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////
// Getting GL Polygon Offset Type From Fill Mode
inline GLenum CGLConvFunc::GetGLPolyOffsetType(GLenum FillMode)
{
    switch(FillMode)
    {
    case GL_POINT:
        return  GL_POLYGON_OFFSET_POINT;
        
    case GL_LINE:
        return  GL_POLYGON_OFFSET_LINE;
        
    case GL_FILL:
        return  GL_POLYGON_OFFSET_FILL;
    }

    ASSERT(0);

    return GL_NONE;
}

////////////////////////////////////////////////////////////////////////////////////////////////
// Getting GL Vertex Buffer Data Usage Type
inline GLenum CGLConvFunc::GetGLBufferDataUsage(GAPIBUFFERUSAGE bufferUsage)
{
    GLenum glusage;
    switch(bufferUsage)
    {
    case GAPI_BUFUSAGE_DYNAMIC:
        glusage = GL_DYNAMIC_DRAW_ARB;
        break;

    case GAPI_BUFUSAGE_STATIC:
        glusage = GL_STATIC_DRAW_ARB;
        break;

    case GAPI_BUFUSAGE_STREAM:
        glusage = GL_STREAM_DRAW_ARB;
        break;
    }

    return glusage;
}

////////////////////////////////////////////////////////////////////////////////////////////////
// Getting GL Type Of Primitive to Render
inline GLenum CGLConvFunc::GetGLPrimitiveType(GAPIPRIMITIVETYPE primitiveType)
{
    static const GLenum PrimTypeTab[]={GL_POINTS,GL_LINES,GL_LINE_STRIP,GL_LINE_LOOP,GL_TRIANGLES,GL_TRIANGLE_STRIP,GL_TRIANGLE_FAN,
                                        GL_QUADS,GL_QUAD_STRIP};

    return PrimTypeTab[primitiveType];
    /*
    GLenum glprimitivetype;

    switch(primitiveType)
    {
    case GAPI_POINTS:
        glprimitivetype = GL_POINTS;
        break;
    case GAPI_LINES:
        glprimitivetype = GL_LINES;
        break;
    case GAPI_LINE_STRIP:
        glprimitivetype = GL_LINE_STRIP;
        break;
    case GAPI_LINE_LOOP:
        glprimitivetype = GL_LINE_LOOP;
        break;
    case GAPI_TRIANGLES:
        glprimitivetype = GL_TRIANGLES;
        break;
    case GAPI_TRIANGLE_STRIP: 
        glprimitivetype = GL_TRIANGLE_STRIP;
        break;
    case GAPI_TRIANGLE_FAN:
        glprimitivetype = GL_TRIANGLE_FAN;
        break;
	case GAPI_QUAD:
		glprimitivetype = GL_QUADS;
		break;
	case GAPI_QUAD_STRIP:
		glprimitivetype = GL_QUAD_STRIP;
		break;
    default:
        glprimitivetype = GL_POINTS;
        break;
    }

    return glprimitivetype;
    */
}

////////////////////////////////////////////////////////////////////////////////////////////////
//Getting GAPI Primitive Draw Type
inline GAPIPRIMTYPE CGLConvFunc::GetGAPIPrimitiveType(GLenum glprim)
{
    GAPIPRIMTYPE prim;

    switch(glprim)
    {
    case GL_POINT:
        prim = GAPI_POINTS;
        break;
    case GL_LINES:
        prim = GAPI_LINES;
        break;
    case GL_LINE_LOOP:
        prim = GAPI_LINE_LOOP;
        break;
    case GL_LINE_STRIP:
        prim = GAPI_LINE_STRIP;
        break;
    case GL_TRIANGLES:
        prim = GAPI_TRIANGLES;
        break;
    case GL_TRIANGLE_STRIP:
        prim = GAPI_TRIANGLE_STRIP;
        break;
    case GL_TRIANGLE_FAN:
        prim = GAPI_TRIANGLE_FAN;
        break;
    default:
        prim = GAPI_POINTS;
        break;
    };

    return prim;
}

////////////////////////////////////////////////////////////////////////////////////////////////
// Getting OpenGL Data Type
inline GLenum CGLConvFunc::GetGLDataType(GAPIDATATYPE type)
{
    static const GLenum DataTypeTab[]={GL_NONE,GL_FLOAT,GL_DOUBLE,GL_HALF_FLOAT,GL_FIXED,GL_BYTE,
        GL_UNSIGNED_BYTE,GL_SHORT,GL_UNSIGNED_SHORT,GL_INT,GL_UNSIGNED_INT};

    return DataTypeTab[type];

    /*
	GLenum datatype;

	switch(type)
	{
	case GAPI_FLOAT:
		datatype = GL_FLOAT;
		break;
	case GAPI_DOUBLE:
		datatype = GL_DOUBLE;
		break;
	case GAPI_HALF:
		datatype = GL_HALF_FLOAT;
		break;
	case GAPI_FIXED:
		datatype = GL_FIXED;
		break;
	case GAPI_BYTE:
		datatype = GL_BYTE;
		break;
    case GAPI_UBYTE:
        datatype = GL_UNSIGNED_BYTE;
        break;
	case GAPI_INT16:
		datatype = GL_SHORT;
		break;
    case GAPI_UINT16:
        datatype = GL_UNSIGNED_SHORT;
        break;
	case GAPI_INT32:
		datatype = GL_INT;
		break;
    case GAPI_UINT32:
        datatype = GL_UNSIGNED_INT;
        break;

	default:
		datatype = 0;
		break;
	}

	return datatype;
    */
}

////////////////////////////////////////////////////////////////////////////////////////////////
// Getting GAPIDATATYPE Size In Bytes
inline GLint CGLConvFunc::GetGLDataTypeSize(GLenum type)
{
	switch(type)
	{
	case GL_FLOAT:
		return sizeof(GLfloat);
	case GL_DOUBLE:
		return sizeof(GLdouble);
	case GL_FIXED:
		return sizeof(GLfixed);
	case GL_HALF_FLOAT:
		return sizeof(GLhalfARB);
	case GL_BYTE:
    case GL_UNSIGNED_BYTE:
		return sizeof(GLubyte);
	case GL_SHORT:
    case GL_UNSIGNED_SHORT:
		return sizeof(GLshort);
	case GL_INT:    
    case GL_UNSIGNED_INT:
		return sizeof(GLint);

	default:
		return 0;
	}
}
////////////////////////////////////////////////////////////////////////////////////////////////
// Get GL Texture Internal Format
inline GLint    CGLConvFunc::GetGLTexInterFmt(TEXFMT gapifmt)
{
    static const GLint TexInterFmt[] =
    {
        0, 
        GL_LUMINANCE8,
        GL_LUMINANCE8_ALPHA8,
        GL_RGB8,
        GL_RGBA8,
        GL_RGB8,
        GL_RGBA8,
        GL_LUMINANCE16,
        GL_LUMINANCE16_ALPHA16,
        GL_RG16,
        GL_RGB16,
        GL_RGBA16,
        GL_R16F,
        GL_RG16F,
        GL_RGB16F,
        GL_RGBA16F,
        GL_R32F,
        GL_RG32F,
        GL_RGB32F,
        GL_RGBA32F,

        // No Signed Formats In OpenGL For now
        GL_RG8_SNORM,    //TEXFMT_UV8
        GL_RGBA8_SNORM,    //TEXFMT_UVWQ8
        GL_RG16_SNORM,    //TEXFMT_UV16
        GL_RGBA16_SNORM,    //TEXFMT_UVWQ16   

        GL_DEPTH_COMPONENT16,
        GL_DEPTH_COMPONENT24,
        GL_LUMINANCE4_ALPHA4,       //TEXFMT_L4A4
        GL_RGBA4,                   //TEXFMT_A4RGB4
        GL_R3_G3_B2, 
        GL_RGB5, 
        GL_RGB5_A1, 
        GL_RGB10_A2, 
        GL_R11F_G11F_B10F, 
        GL_RGB9_E5, 

        //No Support For Mixed Formats
        0,                          //TEXFMT_UV5L6
        GL_RGB10_A2UI,              //TEXFMT_UVW10A2

        GL_COMPRESSED_RGB_S3TC_DXT1_EXT,
	    GL_COMPRESSED_RGBA_S3TC_DXT3_EXT,
	    GL_COMPRESSED_RGBA_S3TC_DXT5_EXT,
        GL_COMPRESSED_RED_RGTC1_EXT, // ATI1N 
	    GL_COMPRESSED_RED_GREEN_RGTC2_EXT,//GL_COMPRESSED_LUMINANCE_ALPHA_LATC2_EXT,//GL_COMPRESSED_LUMINANCE_ALPHA_3DC_ATI, // ATI2N
    };
    
    return TexInterFmt[gapifmt];
}

////////////////////////////////////////////////////////////////////////////////////////////////
// Get GL Texture GLenum Format
inline GLenum   CGLConvFunc::GetGLTexFmt(TEXFMT fmt)
{
    GLenum texfmt = 0;

    static const GLenum TexFmt[] = 
    {
        0,                      //TEXFMT_NONE,
        GL_LUMINANCE,           //TEXFMT_L8,
        GL_LUMINANCE_ALPHA,     //TEXFMT_LA8,    
        GL_RGB,                 //TEXFMT_RGB8,
        GL_RGBA,                //TEXFMT_RGBA8,
        GL_BGR,                 //TEXFMT_BGR8,
        GL_BGRA,                //TEXFMT_BGRA8,
        GL_LUMINANCE,           //TEXFMT_L16,
        GL_LUMINANCE_ALPHA,     //TEXFMT_LA16,    
        GL_RG,                  //TEXFMT_RG16,
        GL_RGB,                 //TEXFMT_RGB16,
        GL_RGBA,                //TEXFMT_RGBA16,
        GL_RED,                 //TEXFMT_R16f,
        GL_RG,                  //TEXFMT_RG16f,
        GL_RGB,                 //TEXFMT_RGB16f,
        GL_RGBA,                //TEXFMT_RGBA16f,
        GL_RED,                 //TEXFMT_R32f,
        GL_RG,                  //TEXFMT_RG32f,
        GL_RGB,                 //TEXFMT_RGB32f,
        GL_RGBA,                //TEXFMT_RGBA32f,
        //--No For Signed Formats
        GL_RG_SNORM,                      //TEXFMT_UV8
        GL_RGBA_SNORM,                      //TEXFMT_UVWQ8
        GL_RG_SNORM,                      //TEXFMT_UV16
        GL_RGBA_SNORM,                      //TEXFMT_UVWQ16 

        GL_DEPTH_COMPONENT,     //TEXFMT_DEPTH16,
        GL_DEPTH_COMPONENT,     //TEXFMT_DEPTH24,
        GL_LUMINANCE,           //TEXFMT_L4A4,
        GL_BGRA,                //TEXFMT_A4RGB4
        GL_RGB,                 //TEXFMT_R3G3B2
        GL_RGB,                 //TEXFMT_R5G6B5,
        GL_BGRA,                //TEXFMT_A1RGB5,
        GL_BGRA,                //TEXFMT_A2RGB10,
        GL_RGB,                 //TEXFMT_RG11B10,
        GL_RGB9_E5,             //TEXFMT_RGB9E5,

        //No Support For Mixed Formats
        0,                      //TEXFMT_UV5L6
        GL_RGBA,                //TEXFMT_UVW10A2 GL_RGB10_A2UI

        GL_RGB,                 //TEXFMT_DXT1,
	    GL_RGBA,                //TEXFMT_DXT3,
	    GL_RGBA,                //TEXFMT_DXT5,
        GL_LUMINANCE,           //TEXFMT_ATI1N,
        GL_LUMINANCE_ALPHA,     //TEXFMT_ATI2N,
    };

   return TexFmt[fmt];
}
////////////////////////////////////////////////////////////////////////////////////////////////
// Get GL Texture Data Type Format
inline GLenum   CGLConvFunc::GetGLTexDataType(TEXFMT fmt)
{
    static const GLenum TexDataType[] = 
    {
        0,                  //TEXFMT_NONE, 
        GL_UNSIGNED_BYTE,   //TEXFMT_L8,
        GL_UNSIGNED_BYTE,   //TEXFMT_LA8,
        GL_UNSIGNED_BYTE,   //TEXFMT_RGB8,
        GL_UNSIGNED_BYTE,   //TEXFMT_RGBA8,
        GL_UNSIGNED_BYTE,   //TEXFMT_BGR8,
        GL_UNSIGNED_BYTE,   //TEXFMT_BGRA8,
        GL_UNSIGNED_SHORT,  //TEXFMT_L16,
        GL_UNSIGNED_SHORT,  //TEXFMT_LA16,
        GL_UNSIGNED_SHORT,  //TEXFMT_RG16,
        GL_UNSIGNED_SHORT,  //TEXFMT_RGB16,
        GL_UNSIGNED_SHORT,  //TEXFMT_RGBA16,
        GL_HALF_FLOAT_ARB,  //TEXFMT_R16f,
        GL_HALF_FLOAT_ARB,  //TEXFMT_RG16f,
        GL_HALF_FLOAT_ARB,  //TEXFMT_RGB16f,
        GL_HALF_FLOAT_ARB,  //TEXFMT_RGBA16f,
        GL_FLOAT,           //TEXFMT_R32f,
        GL_FLOAT,           //TEXFMT_RG32f,
        GL_FLOAT,           //TEXFMT_RGB32f,
        GL_FLOAT,           //TEXFMT_RGBA32f,

        //No For Signed Formats
        0,                      //TEXFMT_UV8
        0,                      //TEXFMT_UVWQ8
        0,                      //TEXFMT_UV16
        0,                      //TEXFMT_UVWQ16 

        GL_UNSIGNED_SHORT,  //TEXFMT_DEPTH16,
	    GL_UNSIGNED_INT,    //TEXFMT_DEPTH24,
    
        GL_UNSIGNED_BYTE,   //TEXFMT_L4A4,
        GL_UNSIGNED_SHORT_4_4_4_4_REV,  //TEXFMT_A4RGB4,
        GL_UNSIGNED_BYTE_3_3_2, //TEXFMT_R3G3B2,
        GL_UNSIGNED_SHORT_5_6_5, //TEXFMT_R5G6B5,
        GL_UNSIGNED_SHORT_1_5_5_5_REV, //TEXFMT_A1RGB5,
        GL_UNSIGNED_INT_2_10_10_10_REV, //TEXFMT_A2RGB10,
        GL_R11F_G11F_B10F,  //TEXFMT_RG11B10,
        GL_UNSIGNED_INT_5_9_9_9_REV, //TEXFMT_RGB9E5,

        //No Support For Mixed Formats
        0,                              //IMGFMT_UV5L6,
        GL_UNSIGNED_INT_2_10_10_10_REV, //IMGFMT_UVW10A2, GL_RGB10_A2UI

        // Compressed formats don't need a source type
        0,                  //TEXFMT_DXT1,
        0,                  //TEXFMT_DXT3,
        0,                  //TEXFMT_DXT5,
        0,                  //TEXFMT_ATI1N,
        0,                  //TEXFMT_ATI2N,

    };
    
    return TexDataType[fmt];
}
////////////////////////////////////////////////////////////////////////////////////////////////
// Get GAPI TEXFMT From Internal Format And GLenum Format
inline TEXFMT   CGLConvFunc::GetGAPITexFmt(GLint interfmt, GLenum fmt)
{
    TEXFMT gapifmt = TEXFMT_NONE;

    switch(interfmt)
    {
    case GL_LUMINANCE8:
        gapifmt = TEXFMT_L8;
        break;
    }

    return gapifmt;
}

////////////////////////////////////////////////////////////////////////////////////////////////
inline GLint    CGLConvFunc::GetGLTexFlt(TEXFLT flt)
{
    GLint glflt = 0;
    static GLint minFilters[] = {
		GL_NEAREST,
		GL_LINEAR,
		GL_LINEAR_MIPMAP_NEAREST,
		GL_LINEAR_MIPMAP_LINEAR,
		GL_LINEAR_MIPMAP_NEAREST,
		GL_LINEAR_MIPMAP_LINEAR,
	};

    return minFilters[flt];
}

////////////////////////////////////////////////////////////////////////////////////////////////
inline TEXFLT  CGLConvFunc::GetGAPITexFlt(GLint flt, GLint aniso)
{
    TEXFLT gapiflt;
    switch(flt)
    {
    case GL_NEAREST:
        gapiflt = TEXFLT_POINT;
        break;

    case GL_LINEAR:
        gapiflt = TEXFLT_LINEAR;
        break;

    case GL_LINEAR_MIPMAP_NEAREST:
        gapiflt = (aniso > 1) ? (TEXFLT_BILINEAR_ANISO) : (TEXFLT_BILINEAR);
        break;

    case GL_LINEAR_MIPMAP_LINEAR:
        gapiflt = (aniso > 1) ? (TEXFLT_TRILINEAR_ANISO) : (TEXFLT_TRILINEAR);
        break;
    }

    return gapiflt;
}

////////////////////////////////////////////////////////////////////////////////////////////////
inline GLint    CGLConvFunc::GetGLTexAddr(TEXADDR addr)
{
    GLint texaddr[] = 
    {
        GL_REPEAT, 
        GL_CLAMP_TO_EDGE,
        GL_CLAMP_TO_BORDER_ARB,
        GL_MIRRORED_REPEAT,
        GL_MIRROR_CLAMP_TO_EDGE_EXT,
	};

    return texaddr[addr];

    /*
    GLint gladdr = 0;
    switch(addr)
    {
    case TEXADDR_REPEAT:
        gladdr = GL_REPEAT;
        break;

    case TEXADDR_CLAMP:
        gladdr = GL_CLAMP_TO_EDGE;
        break;

    case TEXADDR_BORDER:
        gladdr = GL_CLAMP_TO_BORDER_ARB;
        break;

    case TEXADDR_MIRROR:
        gladdr = GL_MIRRORED_REPEAT;
        break;

    case TEXADDR_MIRRORONCE:
        gladdr = GL_MIRROR_CLAMP_TO_EDGE_EXT;
        break;
    }

    return gladdr;*/
}
////////////////////////////////////////////////////////////////////////////////////////////////
inline TEXADDR  CGLConvFunc::GetGAPITexAddr(GLint addr)
{
    TEXADDR gapiaddr = TEXADDR_REPEAT; //TEXADDR_NONE;
    switch(addr)
    {
    case GL_REPEAT:
        gapiaddr = TEXADDR_REPEAT;
        break;

    case GL_CLAMP_TO_EDGE:
        gapiaddr = TEXADDR_CLAMP;
        break;

    case GL_CLAMP_TO_BORDER_ARB:
        gapiaddr = TEXADDR_BORDER;
        break;

    case GL_MIRRORED_REPEAT:
        gapiaddr = TEXADDR_MIRROR;
        break;

    case GL_MIRROR_CLAMP_TO_EDGE_EXT:
        gapiaddr = TEXADDR_MIRRORONCE;
        break;
    }

    return gapiaddr;
}

////////////////////////////////////////////////////////////////////////////////////////////////
inline GLuint   CGLConvFunc::GetPixelSize(TEXFMT fmt)
{
    static const GLuint PixSize[] = {   0,  //TEXFMT_NONE, 
                                        1,  //TEXFMT_L8,
                                        2,  //TEXFMT_LA8,
                                        3,  //TEXFMT_RGB8,
                                        4,  //TEXFMT_RGBA8,
                                        3,  //TEXFMT_BGR8,
                                        4,  //TEXFMT_BGRA8,
                                        2,  //TEXFMT_L16,
                                        4,  //TEXFMT_LA16,
                                        4,  //TEXFMT_RG16,
                                        6,  //TEXFMT_RGB16,
                                        8,  //TEXFMT_RGBA16,
                                        2,  //TEXFMT_R16f,
                                        4,  //TEXFMT_RG16f,
                                        6,  //TEXFMT_RGB16f,
                                        8,  //TEXFMT_RGBA16f,
                                        4,  //TEXFMT_R32f,
                                        8,  //TEXFMT_RG32f,
                                        12, //TEXFMT_RGB32f,
                                        16, //TEXFMT_RGBA32f,
                                        
                                        2, //TEXFMT_UV8
                                        4, //TEXFMT_UVWQ8
                                        4, //TEXFMT_UV16
                                        8, //TEXFMT_UVWQ16 

                                        2,  //TEXFMT_DEPTH16,
                                        3,  //TEXFMT_DEPTH24,
                                        1,  //TEXFMT_L4A4,
                                        2,  //TEXFMT_A4RGB4,
                                        1,  //TEXFMT_R3G3B2,
                                        2,  //TEXFMT_R5G6B5,
                                        2,  //TEXFMT_A1RGB5,
                                        4,  //TEXFMT_A2RGB10,
                                        4,  //TEXFMT_RG11B10,
                                        4,  //TEXFMT_RGB9E5,

                                        0,  //IMGFMT_UV5L6
                                        4,  //IMGFMT_UVW10A2

                                        0,  //TEXFMT_DXT1,
                                        0,  //TEXFMT_DXT3,
                                        0,  //TEXFMT_DXT5,
                                        0,  //TEXFMT_ATI1N,
                                        0,  //TEXFMT_ATI2N,
    };

    return PixSize[fmt];
}

////////////////////////////////////////////////////////////////////////////////////////////////
inline GLint    CGLConvFunc::GetGLComponentsNum(GLenum texfmt)
{
    GLint compnum = 0;
    switch(texfmt)
    {
    case GL_LUMINANCE:
        compnum = 1;
        break;

    case GL_LUMINANCE_ALPHA:
    case GL_RG:
        compnum = 2;
        break;

    case GL_RGB:
    case GL_BGR:
        compnum = 3;
        break;

    case GL_RGBA:
    case GL_BGRA:
        compnum = 4;
        break;
    }

    return compnum;
}

////////////////////////////////////////////////////////////////////////////////////////////////
inline GLboolean CGLConvFunc::IsCompressedFormat(TEXFMT fmt)
{
    return (GLboolean)(fmt >= TEXFMT_DXT1);
}

////////////////////////////////////////////////////////////////////////////////////////////////
inline GLint CGLConvFunc::GetBlockSize(TEXFMT fmt)
{
    return (CGLConvFunc::IsCompressedFormat(fmt)) ? ((fmt == TEXFMT_DXT1 || fmt == TEXFMT_ATI1N)? 8 : 16) : (0);
}

////////////////////////////////////////////////////////////////////////////////////////////////
inline GLboolean CGLConvFunc::IsSignedFormat(TEXFMT fmt)
{
    return (fmt >= TEXFMT_UV8 && fmt <= TEXFMT_UVWQ16);
}

////////////////////////////////////////////////////////////////////////////////////////////////
inline GLboolean CGLConvFunc::IsMixedFormat(TEXFMT fmt)
{
    return (fmt >= TEXFMT_UV5L6 && fmt <= TEXFMT_UVW10A2);
}


////////////////////////////////////////////////////////////////////////////////////////////////


#endif //_GL_CONV_FUNC_H_