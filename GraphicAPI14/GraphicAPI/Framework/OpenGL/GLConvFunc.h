////////////////////////////////////////////////////////////////////////////////////////////////
//	GLConvFunc.h
//	Function for converting enum types of abstract graphic API to OpenGL types
//	Created: 23 April 2006
//	31 July 00:49 Added GetGLBlendOp(BLENDOP blendop)
////////////////////////////////////////////////////////////////////////////////////////////////

//Convert From FUNCTION Enum Type to GLenum Type
inline GLenum GetGLFunc(FUNC func)
{
	GLenum glfunc;

	switch(func)
	{
	case 0:
		glfunc = GL_NEVER;
		break;

	case 1:
		glfunc = GL_LESS;
		break;

	case 2:
		glfunc = GL_EQUAL;
		break;

	case 3:
		glfunc = GL_LEQUAL;
		break;

	case 4:
		glfunc = GL_GREATER;
		break;

	case 5:
		glfunc = GL_NOTEQUAL;
		break;

	case 6:
		glfunc = GL_GEQUAL;
		break;

	case 7:
		glfunc = GL_ALWAYS;
		break;
	}

	return glfunc;
}

//Convert From OPERATION Enum Type to GLenum Type
inline GLenum GetGLOp(OP op)
{
	GLenum glop;

	switch(op)
	{
	case 0:
		glop = GL_KEEP;
		break;

	case 1:
		glop = GL_ZERO;
		break;

	case 2:
		glop = GL_REPLACE;
		break;

	case 3:
		glop = GL_INCR;
		break;

	case 4:
		glop = GL_DECR;
		break;

	case 5:
		glop = GL_INVERT;
		break;

	case 6:
		glop = GL_INCR_WRAP_EXT;
		break;

	case 7:
		glop = GL_DECR_WRAP_EXT;
		break;
	}

	return glop;
}

//Converts From BLENDOPERATION Enum Type to GLenum Type
inline GLenum GetGLBlendOp(BLENDOP blendop)
{
	GLenum glblendop;

	switch(blendop)
	{
	case 0:
		glblendop = GL_ZERO;
		break;

	case 1:
		glblendop = GL_ONE;
		break;

	case 2:
		glblendop = GL_DST_COLOR;
		break;

	case 3:
		glblendop = GL_ONE_MINUS_DST_COLOR;
		break;

	case 4:
		glblendop = GL_SRC_ALPHA;
		break;

	case 5:
		glblendop = GL_ONE_MINUS_SRC_ALPHA;
		break;

	case 6:
		glblendop = GL_DST_ALPHA;
		break;

	case 7:
		glblendop = GL_ONE_MINUS_DST_ALPHA;
		break;

	case 8:
		glblendop = GL_SRC_ALPHA_SATURATE;
		break;

	case 9:
		glblendop = GL_SRC_COLOR;
		break;

	case 10:
		glblendop = GL_ONE_MINUS_SRC_COLOR;
		break;
	}

	return glblendop;
}

//Getting OpenGL Texture Format From CImage FORMAT 
inline GLenum GetGLTexFormatFromCImageFmt(FORMAT fmt)
{
	GLenum format;

	switch(fmt)
	{
	case FORMAT_GREY8:
	case FORMAT_GREY16:
		format = GL_LUMINANCE;
		break;

	case FORMAT_RGB8:
	case FORMAT_RGB16:
		format = GL_RGB;
		break;

	case FORMAT_RGBA8:
	case FORMAT_RGBA16:
		format = GL_RGBA;
		break;

	default:
		format = 0;
		break;
	}

	return format;
}

//Getting OpenGL Texture Data Format From CImage FORMAT 
inline GLenum GetGLDataTypeFromCImageFmt(FORMAT fmt)
{
	GLenum type;

	switch(fmt)
	{
	case FORMAT_GREY8:	
	case FORMAT_RGB8:		
	case FORMAT_RGBA8:
		type = GL_UNSIGNED_BYTE;
		break;

	case FORMAT_GREY16:	
	case FORMAT_RGB16:		
	case FORMAT_RGBA16:
		type = GL_UNSIGNED_SHORT;
		break;

	case FORMAT_GREY32f:	
	case FORMAT_RGB32f:		
	case FORMAT_RGBA32f:
		type = GL_FLOAT;
		break;

	default:
		type = 0;
		break;
	}

	return type;
}

// Get GL Texture Wrap From enum TEXTUREADDRESS 
inline GLenum GetGLTexWrapFromTEXADDR(TEXADDR addr)
{
	GLenum param;

	switch(addr)
	{
	case TEXADDR_REPEAT:
		param = GL_REPEAT;
		break;

	case TEXADDR_CLAMP:
		//param = GL_CLAMP;
		param = GL_CLAMP_TO_EDGE;
		break;

	case TEXADDR_BORDER:
		param = GL_CLAMP_TO_BORDER;
		break;

	case TEXADDR_MIRROR:
		param = GL_MIRRORED_REPEAT;
		break;

	case TEXADDR_MIRRORONCE:
		param = GL_MIRROR_CLAMP_TO_EDGE_EXT;
		break;

	default:
		param = 0;
		break;
	}

	return param;
}

// Getting GL Format From RSURF Format
inline GLenum GetGLFormatFromRSurfFmt(RSURFMT RSurf)
{
	GLenum format;

	switch(RSurf)
	{
	case RSURFMT_RGBA8:
	case RSURFMT_RGBA16:
		format = GL_RGBA;
		break;

	case RSURFMT_DEPTH16:		
	//case RSURFMT_DEPTH24:
	case RSURFMT_DEPTH32:
		format = GL_DEPTH_COMPONENT;
		break;

	default:
		format = 0;
		break;
	}
	return format;
}

// Getting GL Data Type From RSURF Format
inline GLenum GetGLDataTypeFromRSurfFmt(RSURFMT RSurf)
{
	GLenum type;

	switch(RSurf)
	{
	case RSURFMT_RGBA8:
		type = GL_UNSIGNED_BYTE;
		break;

	case RSURFMT_RGBA16:
		type = GL_UNSIGNED_SHORT;
		break;

	case RSURFMT_DEPTH16:		
		type = GL_UNSIGNED_SHORT;
		break;

	case RSURFMT_DEPTH32:
		type = GL_UNSIGNED_INT;
		break;

	default:
		type = 0;
		break;
	}

	return type;
}

/*
// Get GL Texture Dimension From enum TEXTURETYPE
inline GLenum GetGLTexDimFromTEXTYPE(TEXTURETYPE textype)
{
	GLenum target;
	switch(textype)
	{
	case _1D:
		target = GL_TEXTURE_1D;
		break;

	case _2D:
		target = GL_TEXTURE_2D;
		break;

	case _3D:
		target = GL_TEXTURE_3D;
		break;

	case _CUBE:
		target = GL_TEXTURE_CUBE_MAP_ARB;
		break;
	}

	return target;
}
*/