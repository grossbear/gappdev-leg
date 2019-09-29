////////////////////////////////////////////////////////////////////////////////////////////////
//	D3DLightMgr.cpp 
//	Direct3D Light Managment System Class
//	Created: 8th August 2006
//	
////////////////////////////////////////////////////////////////////////////////////////////////
#include "../Math/MathLib.h"
#include "../Types/types.h"
#include "../Camera/Camera.h"
#include "../Templates/Array.h"

#include "../3DObjects/3DObject.h"
#include "../Image/Image.h"
#include "../Renderer/Renderer.h"


#include "D3DRenderer.h"
//#include "D3DRenderElement.h"
#include "D3DConvFunc.h"
#include "D3DLightMgr.h"

#include <memory.h>

//////////////////////////////////////////////////////////////////////////////////////////////
CD3DLightSystem::CD3DLightSystem(LPDIRECT3DDEVICE9 pDev,DWORD MaxActiveLights)
{
	m_pDevD3D = pDev;
	MaxLights = MaxActiveLights;
}

//////////////////////////////////////////////////////////////////////////////////////////////
CD3DLightSystem::~CD3DLightSystem()
{
	LightArray.~LightArray();
	m_pDevD3D = NULL;
	MaxLights = 0;
}

///////////////////////////////////////////////////////////////////////////////////////////////
uint CD3DLightSystem::AddLight()
{
	D3DLIGHT9 LightStruct;
	memset(&LightStruct,0,sizeof(D3DLIGHT9));

	return LightArray.AddElement(LightStruct);
}

///////////////////////////////////////////////////////////////////////////////////////////////
uint CD3DLightSystem::AddLight(const sLight &LightSrc)
{
	D3DLIGHT9 LightStruct;

	//Getting D3DLIGHTTYPE
	LightStruct.Type = GetD3DLightType(LightSrc.type);
	
	//Copy Light Ambient Color
	memcpy(&LightStruct.Ambient, LightSrc.Ambient, sizeof(float)*3);
	LightStruct.Ambient.a = 1.0f;

	//Copy Light Diffuse Color
	memcpy(&LightStruct.Diffuse, LightSrc.Diffuse, sizeof(float)*3);
	LightStruct.Diffuse.a = 1.0f;

	//Copy Light Specular Color
	memcpy(&LightStruct.Specular, LightSrc.Specular, sizeof(float)*3);
	LightStruct.Specular.a = 1.0f;

	//Copy Light Source Position
	memcpy(&LightStruct.Position, LightSrc.Position, sizeof(float)*3);
	//Copy Light Source Direction
	memcpy(&LightStruct.Direction, &LightSrc.Direction, sizeof(float)*3);

	LightStruct.Attenuation0 = LightSrc.Attenuation[0];
	LightStruct.Attenuation1 = LightSrc.Attenuation[1];
	LightStruct.Attenuation2 = LightSrc.Attenuation[2];

	LightStruct.Falloff = LightSrc.Falloff;
	LightStruct.Range = LightSrc.Range;

	if(LightSrc.Phi > 180)
		LightStruct.Phi = float(M_PI);
	else
		LightStruct.Phi = DegToRad(LightSrc.Phi);

	if((LightSrc.Theta > LightSrc.Phi)  || (LightSrc.Theta > 180))
		LightStruct.Theta = LightStruct.Phi;
	else
		LightStruct.Theta = DegToRad(LightSrc.Theta);

	return LightArray.AddElement(LightStruct);
}

///////////////////////////////////////////////////////////////////////////////////////////////
bool CD3DLightSystem::ConfigureLight(uint index,const sLight &LightSrc)
{
	D3DLIGHT9 *pLight = LightArray.GetElement(index);

	if(pLight == NULL)
		return false;

	pLight->Type = GetD3DLightType(LightSrc.type);

	//Copy Light Ambient Color
	memcpy(&pLight->Ambient, LightSrc.Ambient, sizeof(float)*3);
	pLight->Ambient.a = 1.0f;

	//Copy Light Diffuse Color
	memcpy(&pLight->Diffuse, LightSrc.Diffuse, sizeof(float)*3);
	pLight->Diffuse.a = 1.0f;

	//Copy Light Specular Color
	memcpy(&pLight->Specular, LightSrc.Specular, sizeof(float)*3);
	pLight->Specular.a = 1.0f;

	//Copy Light Source Position
	memcpy(&pLight->Position, LightSrc.Position, sizeof(float)*3);
	//Copy Light Source Direction
	memcpy(&pLight->Direction, LightSrc.Direction, sizeof(float)*3);

	pLight->Attenuation0 = LightSrc.Attenuation[0];
	pLight->Attenuation1 = LightSrc.Attenuation[1];
	pLight->Attenuation2 = LightSrc.Attenuation[2];

	pLight->Falloff = LightSrc.Falloff;
	pLight->Range = LightSrc.Range;

	if(LightSrc.Phi > 180)
		pLight->Phi = float(M_PI);
	else
		pLight->Phi = DegToRad(LightSrc.Phi);

	if((LightSrc.Theta > LightSrc.Phi)  || (LightSrc.Theta > 180))
		pLight->Theta = pLight->Phi;
	else
		pLight->Theta = DegToRad(LightSrc.Theta);

	return true;
}

///////////////////////////////////////////////////////////////////////////////////////////////
bool CD3DLightSystem::DeleteLight(uint index)
{
	return LightArray.DeleteElement(index);
}

///////////////////////////////////////////////////////////////////////////////////////////////
bool  CD3DLightSystem::GetLight(uint index, sLight &LightSrc)
{
	D3DLIGHT9 *pLight = LightArray.GetElement(index);

	if(pLight == NULL)
		return false;

	LightSrc.type = GetLightType(pLight->Type);

	//Copy Light Ambient Color
	memcpy(LightSrc.Ambient, &pLight->Ambient, sizeof(float)*3);

	//Copy Light Diffuse Color
	memcpy(LightSrc.Diffuse, &pLight->Diffuse, sizeof(float)*3);
	
	//Copy Light Specular Color
	memcpy(LightSrc.Specular, &pLight->Specular, sizeof(float)*3);
	
	//Copy Light Source Position
	memcpy(LightSrc.Position, &pLight->Position, sizeof(float)*3);
	//Copy Light Source Direction
	memcpy(LightSrc.Direction, &pLight->Direction, sizeof(float)*3);

	LightSrc.Attenuation[0] = pLight->Attenuation0;
	LightSrc.Attenuation[1] = pLight->Attenuation1;
	LightSrc.Attenuation[2] = pLight->Attenuation2;

	LightSrc.Falloff = pLight->Falloff;
	LightSrc.Range = pLight->Range;

	LightSrc.Phi = RadToDeg(pLight->Phi);
	LightSrc.Theta = RadToDeg(pLight->Theta);

	
	return true;
}

///////////////////////////////////////////////////////////////////////////////////////////////
bool CD3DLightSystem::SetLightAmbColor(uint index,float R,float G,float B)
{
	D3DLIGHT9 *pLight = LightArray.GetElement(index);

	if(pLight == NULL)
		return false;

	pLight->Ambient.r = R;	pLight->Ambient.g = G; pLight->Ambient.b = B;

	return true;
}

///////////////////////////////////////////////////////////////////////////////////////////////
bool CD3DLightSystem::SetLightDifColor(uint index,float R,float G,float B)
{
	D3DLIGHT9 *pLight = LightArray.GetElement(index);

	if(pLight == NULL)
		return false;

	pLight->Diffuse.r = R;	pLight->Diffuse.g = G;	pLight->Diffuse.b = B;

	return true;
}

///////////////////////////////////////////////////////////////////////////////////////////////
bool CD3DLightSystem::SetLightSpecColor(uint index,float R,float G,float B)
{
	D3DLIGHT9 *pLight = LightArray.GetElement(index);

	if(pLight == NULL)
		return false;

	pLight->Specular.r = R;	pLight->Specular.g = G;	pLight->Specular.b = B;

	return true;
}

///////////////////////////////////////////////////////////////////////////////////////////////
bool CD3DLightSystem::SetLightPos(uint index,const Vector3D &Pos)
{
	D3DLIGHT9 *pLight = LightArray.GetElement(index);

	if(pLight == NULL)
		return false;

	memcpy(&pLight->Position, Pos, sizeof(float)*3);

	return true;
}

///////////////////////////////////////////////////////////////////////////////////////////////
bool CD3DLightSystem::SetLightDir(uint index,const Vector3D &Dir)
{
	D3DLIGHT9 *pLight = LightArray.GetElement(index);

	if(pLight == NULL)
		return false;

	memcpy(&pLight->Direction, Dir, sizeof(float)*3);

	return true;
}

///////////////////////////////////////////////////////////////////////////////////////////////
bool CD3DLightSystem::SetLightAttenuation(uint index, float C1,float C2,float C3)
{
	D3DLIGHT9 *pLight = LightArray.GetElement(index);

	if(pLight == NULL)
		return false;

	pLight->Attenuation0 = C1;
	pLight->Attenuation1 = C2;
	pLight->Attenuation2 = C3;

	return true;
}

///////////////////////////////////////////////////////////////////////////////////////////////
bool CD3DLightSystem::SetLightConstantAttenuation(uint index, float C1)
{
	D3DLIGHT9 *pLight = LightArray.GetElement(index);

	if(pLight == NULL)
		return false;

	pLight->Attenuation0 = C1;

	return true;
}

///////////////////////////////////////////////////////////////////////////////////////////////
bool CD3DLightSystem::SetLightLinearAttenuation(uint index, float C2)
{
	D3DLIGHT9 *pLight = LightArray.GetElement(index);

	if(pLight == NULL)
		return false;

	pLight->Attenuation1 = C2;

	return true;
}

///////////////////////////////////////////////////////////////////////////////////////////////
bool CD3DLightSystem::SetLightQuadraticAttenuation(uint index, float C3)
{
	D3DLIGHT9 *pLight = LightArray.GetElement(index);

	if(pLight == NULL)
		return false;

	pLight->Attenuation2 = C3;

	return true;
}

///////////////////////////////////////////////////////////////////////////////////////////////
bool CD3DLightSystem::SetLightRange(uint index, float Range)
{
	D3DLIGHT9 *pLight = LightArray.GetElement(index);

	if(pLight == NULL)
		return false;

	pLight->Range = Range;

	return true;
}

///////////////////////////////////////////////////////////////////////////////////////////////
bool CD3DLightSystem::SetLightType(uint index, LIGHTTYPE Type)
{
	D3DLIGHT9 *pLight = LightArray.GetElement(index);

	if(pLight == NULL)
		return false;

	pLight->Type = GetD3DLightType(Type);

	return true;
}

///////////////////////////////////////////////////////////////////////////////////////////////
bool CD3DLightSystem::SetSpotLightParams(uint index, float Falloff, float Theta, float Phi)
{
	D3DLIGHT9 *pLight = LightArray.GetElement(index);

	if(pLight == NULL)
		return false;

	if(Phi>180)
		pLight->Phi = float(M_PI);
	else
		pLight->Phi = DegToRad(Phi);

	if((Theta > Phi) || (Theta > 180))
		pLight->Theta = pLight->Phi;
	else
		pLight->Theta = DegToRad(Theta);

	pLight->Falloff = Falloff;

	return true;
}


///////////////////////////////////////////////////////////////////////////////////////////////
bool CD3DLightSystem::SetLight(uint index, int LightNum)
{
	if(LightNum >= MaxLights)
		return false;

	D3DLIGHT9 *pLight = LightArray.GetElement(index);

	if(pLight == NULL)
		return false;

	m_pDevD3D->SetLight(LightNum,pLight);
	m_pDevD3D->LightEnable(LightNum, TRUE);

	return true;
}

///////////////////////////////////////////////////////////////////////////////////////////////
void CD3DLightSystem::TurnOffLights()
{
	for(int i=0; i<MaxLights; i++)
		m_pDevD3D->LightEnable(i,FALSE);
}

///////////////////////////////////////////////////////////////////////////////////////////////
void CD3DLightSystem::TurnOffLight(int LightNum)
{
	if(LightNum >= MaxLights)
		return ;

	m_pDevD3D->LightEnable(LightNum, FALSE);
}