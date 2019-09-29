////////////////////////////////////////////////////////////////////////////////////////////////
//	GLightMgr.cpp 
//	OpenGL Light Managment System Class
//	Created: 25th April 2006
//	Modified: 8th August 2006
//	
////////////////////////////////////////////////////////////////////////////////////////////////
#include <windows.h>
#include <memory.h>
#include <stdio.h>

#include "../Math/MathLib.h"
#include "../Types/types.h"
#include "../Templates/Array.h"
#include "../Camera/Camera.h"

#include "../3DObjects/3DObject.h"

#include "../Image/Image.h"
//#include "../Image/ImageIO.h"
//#include "../Image/ImgTypes.h"

//#include "Extensions/glext.h"
//#include "Extensions/wglext.h"
//#include "GLExtStruct.h"
//#include "Extensions/GLExtensions.h"

#include "../Renderer/Renderer.h"
//#include "GLARBRenderElement.h"
#include "GLExtStruct.h"
#include "GLDevCaps.h"
//#include "GLRenderer.h"


#include <gl/gl.h>
#include <gl/glu.h>

#include "GLightMgr.h"


//////////////////////////////////////////////////////////////////////////////////////////////
CGLightSystem::CGLightSystem()
{
	glGetIntegerv(GL_MAX_LIGHTS, &MaxLights);
}

//////////////////////////////////////////////////////////////////////////////////////////////
CGLightSystem::~CGLightSystem()
{
	LightArray.~LightArray();
	MaxLights=0;
}

//////////////////////////////////////////////////////////////////////////////////////////////
uint CGLightSystem::AddLight()
{
	sGLight LightStruct;
	memset(&LightStruct,0,sizeof(sGLight));

	uint index = this->LightArray.AddElement(LightStruct);
	
	return index;
}

//////////////////////////////////////////////////////////////////////////////////////////////
uint CGLightSystem::AddLight(const sLight &LightSrc)
{
	sGLight LightStruct;

	LightStruct.type	=	LightSrc.type;

	memcpy(&LightStruct.Ambient,LightSrc.Ambient,sizeof(float)*3);	
	LightStruct.Ambient[3]	=	1.0f;

	memcpy(&LightStruct.Diffuse,LightSrc.Diffuse,sizeof(float)*3);	
	LightStruct.Diffuse[3]	=	1.0f;

	memcpy(&LightStruct.Specular,LightSrc.Specular,sizeof(float)*3);
	LightStruct.Specular[3]	=	1.0f;

	memcpy(&LightStruct.Position, LightSrc.Position, sizeof(float)*3);
	memcpy(&LightStruct.Direction, LightSrc.Direction, sizeof(float)*3);
	memcpy(&LightStruct.Attenuation, LightSrc.Attenuation, sizeof(float)*3);

	if(LightStruct.type != POINTLIGHT)
		LightStruct.Position[3] = 0.0f;
	else
		LightStruct.Position[3] = 1.0f;

	LightStruct.Direction[3] = 1.0f;

	LightStruct.Falloff = LightSrc.Falloff;
	LightStruct.Range = LightSrc.Range;
	
	if(LightSrc.Phi>180)
		LightStruct.Phi = 90;
	else
		LightStruct.Phi = LightSrc.Phi/2;

	if(LightSrc.Theta > LightSrc.Phi)
		LightStruct.Theta = LightStruct.Phi;
	else
		LightStruct.Theta = LightSrc.Theta/2;

	return this->LightArray.AddElement(LightStruct);
}

//////////////////////////////////////////////////////////////////////////////////////////////
bool CGLightSystem::ConfigureLight(uint index,const sLight &LightSrc)
{
	sGLight* pLight = LightArray.GetElement(index);
	if(pLight==NULL)
		return false;

	pLight->type	=	LightSrc.type;

	memcpy(pLight->Ambient,LightSrc.Ambient,sizeof(float)*3);
	pLight->Ambient[3]	=	1.0f;

	memcpy(pLight->Diffuse,LightSrc.Diffuse,sizeof(float)*3);
	pLight->Diffuse[3]	=	1.0f;

	memcpy(pLight->Specular,LightSrc.Specular,sizeof(float)*3);
	pLight->Specular[3]	=	1.0f;

	memcpy(pLight->Position, LightSrc.Position, sizeof(float)*3);
	memcpy(pLight->Direction, LightSrc.Direction, sizeof(float)*3);
	memcpy(pLight->Attenuation, LightSrc.Attenuation, sizeof(float)*3);

	if(LightSrc.type != POINTLIGHT)
		pLight->Position[3] = 0.0f;
	else
		pLight->Position[3] = 1.0f;

	pLight->Direction[3] = 1.0f;

	pLight->Falloff = LightSrc.Falloff;
	pLight->Range = LightSrc.Range;
	
	if(LightSrc.Phi>90)
		pLight->Phi = 90;
	else
		pLight->Phi = LightSrc.Phi;

	if(LightSrc.Theta > LightSrc.Phi)
		pLight->Theta = LightSrc.Phi;
	else
		pLight->Theta = LightSrc.Theta;
	

	return true;
}

//////////////////////////////////////////////////////////////////////////////////////////////
bool CGLightSystem::DeleteLight(uint index)
{
	return LightArray.DeleteElement(index);

}

//////////////////////////////////////////////////////////////////////////////////////////////
bool CGLightSystem::GetLight(uint index,sLight &LightSrc)
{
	sGLight* pLight = LightArray.GetElement(index);

	if(pLight==NULL)
		return false;

	memcpy(LightSrc.Ambient,pLight->Ambient,sizeof(float)*3);
	memcpy(LightSrc.Diffuse,pLight->Diffuse,sizeof(float)*3);
	memcpy(LightSrc.Specular,pLight->Specular,sizeof(float)*3);

	LightSrc.type = pLight->type;

	memcpy(&LightSrc.Position, pLight->Position, sizeof(float)*3);
	memcpy(&LightSrc.Direction, pLight->Direction, sizeof(float)*3);
	memcpy(&LightSrc.Attenuation, pLight->Attenuation, sizeof(float)*3);

	LightSrc.Falloff = pLight->Falloff;
	LightSrc.Range = pLight->Range;
	LightSrc.Theta = pLight->Theta;
	LightSrc.Phi = pLight->Phi;

	return true;
}

//////////////////////////////////////////////////////////////////////////////////////////////
bool CGLightSystem::SetLightAmbColor(uint index, float R,float G,float B)
{
	sGLight* pLight = LightArray.GetElement(index);
	if(pLight==NULL)
		return false;

	pLight->Ambient[0] = R;
	pLight->Ambient[1] = G;
	pLight->Ambient[2] = B;
	pLight->Ambient[3] = 1.0f;

	return true;
}

//////////////////////////////////////////////////////////////////////////////////////////////
bool CGLightSystem::SetLightDifColor(uint index, float R,float G,float B)
{
	sGLight* pLight = LightArray.GetElement(index);
	if(pLight==NULL)
		return false;
	
	pLight->Diffuse[0] = R;
	pLight->Diffuse[1] = G;
	pLight->Diffuse[2] = B;
	pLight->Diffuse[3] = 1.0f;

	return true;
}

//////////////////////////////////////////////////////////////////////////////////////////////
bool CGLightSystem::SetLightSpecColor(uint index, float R,float G,float B)
{
	sGLight* pLight = LightArray.GetElement(index);
	if(pLight==NULL)
		return false;

	pLight->Specular[0] = R;
	pLight->Specular[1] = G;
	pLight->Specular[2] = B;
	pLight->Specular[3] = 1.0f;

	return true;
}

//////////////////////////////////////////////////////////////////////////////////////////////
bool CGLightSystem::SetLightPos(uint index, const Vector3D &Pos)
{
	sGLight* pLight = LightArray.GetElement(index);
	if(pLight==NULL)
		return false;

	memcpy(pLight->Position,Pos,sizeof(float)*3);
	
	return true;
}

//////////////////////////////////////////////////////////////////////////////////////////////
bool CGLightSystem::SetLightDir(uint index, const Vector3D &Dir)
{
	sGLight* pLight = LightArray.GetElement(index);
	if(pLight==NULL)
		return false;

	memcpy(pLight->Direction,Dir,sizeof(float)*3);
	
	return true;
}

//////////////////////////////////////////////////////////////////////////////////////////////
bool CGLightSystem::SetLightAttenuation(uint index, float C1,float C2,float C3)
{
	sGLight* pLight = LightArray.GetElement(index);
	if(pLight==NULL)
		return false;

	pLight->Attenuation[0] = C1;
	pLight->Attenuation[1] = C2;
	pLight->Attenuation[2] = C3;

	return true;
}

bool CGLightSystem::SetLightConstantAttenuation(uint index, float C1)
{
	sGLight* pLight = LightArray.GetElement(index);
	if(pLight==NULL)
		return false;

	pLight->Attenuation[0] = C1;
	
	return true;
}

bool CGLightSystem::SetLightLinearAttenuation(uint index, float C2)
{
	sGLight* pLight = LightArray.GetElement(index);
	if(pLight==NULL)
		return false;

	pLight->Attenuation[1] = C2;
	
	return true;
}

bool CGLightSystem::SetLightQuadraticAttenuation(uint index, float C3)
{
	sGLight* pLight = LightArray.GetElement(index);
	if(pLight==NULL)
		return false;

	pLight->Attenuation[2] = C3;

	return true;
}

//////////////////////////////////////////////////////////////////////////////////////////////
bool CGLightSystem::SetLightRange(uint index,float Range)
{
	sGLight* pLight = LightArray.GetElement(index);
	if(pLight==NULL)
		return false;

	pLight->Range = Range;

	return true;
}

//////////////////////////////////////////////////////////////////////////////////////////////
bool CGLightSystem::SetLightType(uint index,LIGHTTYPE Type)
{
	sGLight* pLight = LightArray.GetElement(index);
	if(pLight==NULL)
		return false;

	pLight->type = Type;

	if(Type != POINTLIGHT)
		pLight->Position[3] = 0.0f;
	else
		pLight->Position[3] = 1.0f;

	return true;
}

//////////////////////////////////////////////////////////////////////////////////////////////
bool CGLightSystem::SetSpotLightParams(uint index,float Falloff,float Theta,float Phi)
{
	sGLight* pLight = LightArray.GetElement(index);
	if(pLight==NULL)
		return false;

	if(Phi>90)
		Phi = 90;
	
	if(Theta > Phi)
		Theta = Phi;

	pLight->Falloff = Falloff;
	pLight->Theta = Theta;
	pLight->Phi = Phi;

	return true;
}

//////////////////////////////////////////////////////////////////////////////////////////////
void CGLightSystem::TurnOffLights()
{
	int Lighti=0x4000;
	for(int i=0; i<MaxLights; i++)
	{
		Lighti = Lighti+i;

		if(glIsEnabled(Lighti))
			glDisable(Lighti);
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////
void CGLightSystem::TurnOffLight(int LightNum)
{
	int Lighti=0x4000;

	if(LightNum>=MaxLights)
		return ;

	Lighti += LightNum;

	glDisable(Lighti);
}

//////////////////////////////////////////////////////////////////////////////////////////////
bool CGLightSystem::SetLight(uint index,int LightNum)
{
	if(LightNum>=MaxLights)
		return false;

	sGLight* pLight = LightArray.GetElement(index);
	if(pLight==NULL)
		return false;

	uint Lighti=0x4000;
	Lighti += LightNum;

	glEnable(Lighti);

	glLightfv(Lighti,GL_AMBIENT,pLight->Ambient);
	glLightfv(Lighti,GL_DIFFUSE,pLight->Diffuse);
	glLightfv(Lighti,GL_SPECULAR,pLight->Specular);
	glLightfv(Lighti,GL_POSITION,pLight->Position);

	if(pLight->Position[3]==0.0f)
		glLightfv(Lighti,GL_SPOT_DIRECTION,pLight->Direction);

	glLightf(Lighti,GL_CONSTANT_ATTENUATION,pLight->Attenuation[0]);
	glLightf(Lighti,GL_LINEAR_ATTENUATION,pLight->Attenuation[1]);
	glLightf(Lighti,GL_QUADRATIC_ATTENUATION,pLight->Attenuation[2]);

	return true;
}
