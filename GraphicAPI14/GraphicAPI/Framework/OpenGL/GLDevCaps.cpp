////////////////////////////////////////////////////////////////////////////////////////////////
//	GLDeviceCaps.h
//	OpenGL Device Caps Class
//	Created: 15th August 2006
//
////////////////////////////////////////////////////////////////////////////////////////////////
#include <windows.h>
//#include <stdio.h>
#include "../Types/Types.h"
#include "../Math/MathLib.h"

#include "../Templates/Array.h"
#include "../Camera/Camera.h"


#include "../3DObjects/3DObject.h"

//#include "../Image/Image.h"
#include "../Renderer/Renderer.h"

//typedef struct GLExtensions;

#include <gl/gl.h>
#include <gl/glu.h>
#include "Extensions/glext.h"
#include "Extensions/wglext.h"
#include "GLExtStruct.h"
//#include "GLConvFunc.h"
#include "GLDevCaps.h"


extern bool IsExtensionSupported(const char *extension);


///////////////////////////////////////////////////////////////////////////////////////////
void CGLDeviceCaps::GetGLDevCaps(sGLDevCaps &Caps)
{
	Caps.MaxAnisotropy = this->MaxAnisotropy();
	Caps.MaxLights = this->MaxLights();
	Caps.MaxTextureUnits = this->MaxTexUnits();
	Caps.MaxTextureSize = this->MaxTexUnits();
	Caps.Max3DTextureSize = this->MaxVolumeTexSize();
	Caps.MaxCubeMapSize = this->MaxCubeMapSize();
	Caps.MaxClipPlanes = this->MaxClipPlanes();
	Caps.TwoSideStencil = this->TwoSideStencil();

}


///////////////////////////////////////////////////////////////////////////////////////////
uint CGLDeviceCaps::MaxLights()
{
	int MaxLights=0;
	glGetIntegerv(GL_MAX_LIGHTS,&MaxLights);

	return uint(MaxLights);
}

///////////////////////////////////////////////////////////////////////////////////////////
uint CGLDeviceCaps::MaxAnisotropy()
{
	GLint MaxAnisotropy=0;
	glGetIntegerv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &MaxAnisotropy);

	return (uint)MaxAnisotropy;
}

///////////////////////////////////////////////////////////////////////////////////////////
uint CGLDeviceCaps::MaxTexUnits()
{
	int TexUnits=0;
	glGetIntegerv(GL_MAX_TEXTURE_UNITS,&TexUnits);

	return uint(TexUnits);
}

///////////////////////////////////////////////////////////////////////////////////////////
uint CGLDeviceCaps::MaxTexSize()
{
	int TexSize=0;
	glGetIntegerv(GL_MAX_TEXTURE_SIZE,&TexSize);

	return uint(TexSize);
}

///////////////////////////////////////////////////////////////////////////////////////////
uint CGLDeviceCaps::MaxVolumeTexSize()
{
	int VolTexSize=0;
	glGetIntegerv(GL_MAX_3D_TEXTURE_SIZE_EXT,&VolTexSize);

	return uint(VolTexSize);
}

uint CGLDeviceCaps::MaxCubeMapSize()
{
	int CubeMapSize=0;
	glGetIntegerv(GL_MAX_CUBE_MAP_TEXTURE_SIZE,&CubeMapSize);

	return uint(CubeMapSize);
}

///////////////////////////////////////////////////////////////////////////////////////////
uint CGLDeviceCaps::MaxClipPlanes()
{
	int MaxClipPlanes=0;
	glGetIntegerv(GL_MAX_CLIP_PLANES,&MaxClipPlanes);

	return uint(MaxClipPlanes);

}

///////////////////////////////////////////////////////////////////////////////////////////
bool CGLDeviceCaps::TwoSideStencil()
{
	if(m_pExt->GL_EXT_stencil_two_side_flag)
		return true;

	if(m_pExt->GL_ATI_separate_stencil_flag)
		return true;

	return false;
}