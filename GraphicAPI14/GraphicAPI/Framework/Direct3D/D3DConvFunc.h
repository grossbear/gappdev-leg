////////////////////////////////////////////////////////////////////////////////////////////////
//	D3DConvFunc.h
//	Function for converting enum types of abstract graphic API to DirectGraphics types
//	Created: 23 April 2006
//	
////////////////////////////////////////////////////////////////////////////////////////////////


//Convert From FUNCTION Enum Type to D3DCMPFUNC Type
inline D3DCMPFUNC GetD3DFunc(FUNC func)
{
	D3DCMPFUNC d3dfunc;

	switch(func)
	{
	case 0:
		d3dfunc = D3DCMP_NEVER;
		break;

	case 1:
		d3dfunc = D3DCMP_LESS;
		break;

	case 2:
		d3dfunc = D3DCMP_EQUAL;
		break;

	case 3:
		d3dfunc = D3DCMP_LESSEQUAL;
		break;

	case 4:
		d3dfunc = D3DCMP_GREATER;
		break;

	case 5:
		d3dfunc = D3DCMP_NOTEQUAL;
		break;

	case 6:
		d3dfunc = D3DCMP_GREATEREQUAL;
		break;

	case 7:
		d3dfunc = D3DCMP_ALWAYS;
		break;
	}

	return d3dfunc;
}

//Convert From OPERATION Enum Type to D3DSTENCILOP Type
inline D3DSTENCILOP GetD3DOp(OP op)
{
	D3DSTENCILOP d3dop;

	switch(op)
	{
	case 0:
		d3dop = D3DSTENCILOP_KEEP;
		break;

	case 1:
		d3dop = D3DSTENCILOP_ZERO;
		break;

	case 2:
		d3dop = D3DSTENCILOP_REPLACE;
		break;

	case 3:
		d3dop = D3DSTENCILOP_INCRSAT;
		break;

	case 4:
		d3dop = D3DSTENCILOP_DECRSAT;
		break;

	case 5:
		d3dop = D3DSTENCILOP_INVERT;
		break;

	case 6:
		d3dop = D3DSTENCILOP_INCR;
		break;

	case 7:
		d3dop = D3DSTENCILOP_DECR;
		break;
	}

	return d3dop;
}

//Converts From BLENDOPERATION Enum Type to GLenum Type
inline D3DBLEND GetD3DBlendOp(BLENDOP blendop)
{
	D3DBLEND d3dblendop;

	switch(blendop)
	{
	case 0:
		d3dblendop = D3DBLEND_ZERO;
		break;

	case 1:
		d3dblendop = D3DBLEND_ONE;
		break;

	case 2:
		d3dblendop = D3DBLEND_DESTCOLOR;
		break;

	case 3:
		d3dblendop = D3DBLEND_INVDESTCOLOR;
		break;

	case 4:
		d3dblendop = D3DBLEND_SRCALPHA;
		break;

	case 5:
		d3dblendop = D3DBLEND_INVSRCALPHA;
		break;

	case 6:
		d3dblendop = D3DBLEND_DESTALPHA;
		break;

	case 7:
		d3dblendop = D3DBLEND_INVDESTALPHA;
		break;

	case 8:
		d3dblendop = D3DBLEND_SRCALPHASAT;
		break;

	case 9:
		d3dblendop = D3DBLEND_SRCCOLOR;
		break;

	case 10:
		d3dblendop = D3DBLEND_INVSRCCOLOR;
		break;
	}

	return d3dblendop;
}

//Convert From enum LIGHTTYPE To D3DLIGHTTYPE
inline D3DLIGHTTYPE GetD3DLightType(LIGHTTYPE LightType)
{
	_D3DLIGHTTYPE d3dlightType;
	switch(LightType)
	{
	case 0:
		d3dlightType = D3DLIGHT_POINT;
		break;

	case 1:
		d3dlightType = D3DLIGHT_DIRECTIONAL;
		break;

	case 2:
		d3dlightType = D3DLIGHT_SPOT;
		break;

	default:
		d3dlightType = D3DLIGHT_POINT;
		break;
	}

	return d3dlightType;
}

//Convert From D3DLIGHTTYPE To LIGHTTYPE
inline LIGHTTYPE GetLightType(D3DLIGHTTYPE d3dlightType)
{
	LIGHTTYPE lightType;
	switch(d3dlightType)
	{
	case D3DLIGHT_POINT:
		lightType = POINTLIGHT;
		break;

	case D3DLIGHT_SPOT:
		lightType = SPOT;
		break;

	case D3DLIGHT_DIRECTIONAL:
		lightType = DIRECTIONAL;
		break;
	}

	return lightType;
}

// Get D3D Texture Address From enum TEXTUREADDRESS 
inline D3DTEXTUREADDRESS GetD3DTexAddrFromTEXADDR(TEXADDR addr)
{
	D3DTEXTUREADDRESS param;

	switch(addr)
	{
	case TEXADDR_REPEAT:
		param = D3DTADDRESS_WRAP ;
		break;

	case TEXADDR_CLAMP:
		//param = GL_CLAMP;
		param = D3DTADDRESS_CLAMP;
		break;

	case TEXADDR_BORDER:
		param = D3DTADDRESS_BORDER;
		break;

	case TEXADDR_MIRROR:
		param = D3DTADDRESS_MIRROR;
		break;

	case TEXADDR_MIRRORONCE:
		param = D3DTADDRESS_MIRRORONCE;
		break;

	default:
		param = D3DTADDRESS_WRAP;
		break;
	}

	return param;
}

// Get D3DFORMAT From Image Class Format 
inline D3DFORMAT GetD3DFmtFromImageFmt(FORMAT fmt)
{
	D3DFORMAT format;

	switch(fmt)
	{
	case FORMAT_GREY8:
		format = D3DFMT_L8;
		break;

	case FORMAT_GREY16:
		format = D3DFMT_L16;
		break;

	case FORMAT_RGB8:
		format = D3DFMT_X8R8G8B8;
		break;

	case FORMAT_RGBA8:
		format = D3DFMT_A8R8G8B8;
		break;

	case FORMAT_RGBA16:
		format = D3DFMT_A16B16G16R16;
		break;

	case FORMAT_RGBA32f:
		format = D3DFMT_A16B16G16R16F;
		break;

	default:
		format = D3DFMT_UNKNOWN;
		break;
	}

	return format;
}

// Helper function for setting a D3DVERTEXELEMENT9
inline void SetElement(D3DVERTEXELEMENT9 &element, WORD stream, WORD offset, BYTE type,
					   BYTE method, BYTE usage, BYTE usageIndex)
{
	element.Stream = stream;
	element.Offset = offset;
	element.Type = type;
	element.Method = method;
	element.Usage = usage;
	element.UsageIndex = usageIndex;
}



//Helper Function For Clearing D3DRenderElement Structure
inline void ClearD3DRenderElement(D3DRenderElement &RenderElement)
{
	memset(&RenderElement,0,sizeof(D3DRenderElement));

	RenderElement.pVertexVB = NULL;
	RenderElement.pNormalVB = NULL;
	RenderElement.pDiffuseVB = NULL;
	RenderElement.pSpecularVB = NULL;
	RenderElement.pBlendIndicesVB = NULL;
	RenderElement.pVertexWeightsVB = NULL;
	RenderElement.pVertexDeclaration = NULL;
	for(int i=0; i<32; i++)
		RenderElement.pTextureVB[i] = NULL;

	RenderElement.pIndexBuffer = NULL;
}

// Getting Size In Bytes Of Specified D3DFORMAT
inline int GetD3DFORMATSize(D3DFORMAT fmt)
{
	int size = 0;

	switch(fmt)
	{
	case D3DFMT_A8P8:
	case D3DFMT_P8:
	case D3DFMT_L8:
	case D3DFMT_A8:
	case D3DFMT_R3G3B2:
		size = 1;
		break;

	case D3DFMT_R16F:
	case D3DFMT_V8U8:
	case D3DFMT_L6V5U5:
	case D3DFMT_X1R5G5B5:
	case D3DFMT_R5G6B5:
	case D3DFMT_A1R5G5B5:
	case D3DFMT_A4R4G4B4:
	case D3DFMT_A8R3G3B2:
	case D3DFMT_X4R4G4B4:
	case D3DFMT_L16:
	case D3DFMT_A8L8:
		size = 2;
		break;

	case D3DFMT_R8G8B8:
		size = 3;
		break;

	case D3DFMT_G16R16F:
	case D3DFMT_R32F:
	case D3DFMT_X8L8V8U8:
	case D3DFMT_Q8W8V8U8:
	case D3DFMT_V16U16:
	case D3DFMT_A8R8G8B8:
	case D3DFMT_X8R8G8B8:
	case D3DFMT_A2B10G10R10:
	case D3DFMT_A8B8G8R8:
	case D3DFMT_X8B8G8R8:
	case D3DFMT_G16R16:
	case D3DFMT_A2R10G10B10:
		size = 4;
		break;

	case D3DFMT_A16B16G16R16F:
	case D3DFMT_G32R32F:
	case D3DFMT_Q16W16V16U16:
	case D3DFMT_A16B16G16R16:
		size = 8;
		break;

	case D3DFMT_A32B32G32R32F:
		size = 16;
		break;

	default:
		size = 0;
		break;
	}

	return size;
}

// Get D3DFORMAT From RENDERSURFACEFORMAT
inline D3DFORMAT GetD3DFMTFromRSURFMT(RSURFMT fmt)
{
	D3DFORMAT d3dformat;

	switch(fmt)
	{
	case RSURFMT_RGBA8:
		d3dformat = D3DFMT_A8R8G8B8;
		break;

	case RSURFMT_RGBA16:
		d3dformat = D3DFMT_A16B16G16R16;
		break;

	case RSURFMT_DEPTH16:
		d3dformat = D3DFMT_D16;
		break;

	case RSURFMT_DEPTH24:
		d3dformat = D3DFMT_D24S8;
		break;

	case RSURFMT_DEPTH32:
		d3dformat = D3DFMT_D32;
		break;
	}

	return d3dformat;
}
/*
//Helper Function For Setting D3DVERTEXELEMENT9 From VertexDesc Structure
inline void SetD3DElementFromVertexDesc(D3DVERTEXELEMENT9 &element, VertexDesc &VertDesc)
{
	int i = 0;

}
*/