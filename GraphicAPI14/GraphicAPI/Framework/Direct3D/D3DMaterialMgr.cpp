/////////////////////////////////////////////////////////////////////////////
//	D3DMaterialMgr.cpp
//	Direct3D Material Manager Class
//	Created: 9th August 2006
//	
////////////////////////////////////////////////////////////////////////////
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
#include "D3DMaterialMgr.h"

#include <memory.h>

////////////////////////////////////////////////////////////////////////////
uint CD3DMaterialSystem::AddMaterial()
{
	D3DMATERIAL9 Mat;
	memset(&Mat,0,sizeof(D3DMATERIAL9));

	return MaterialArray.AddElement(Mat);
}
	
////////////////////////////////////////////////////////////////////////////
uint CD3DMaterialSystem::AddMaterial(float Ka,float Kd,float Ks,float Ke,float power)
{
	D3DMATERIAL9 Mat;
	memset(&Mat,0,sizeof(D3DMATERIAL9));

	//Ka = Clamp(0.0f, 1.0f, Ka);
	Mat.Ambient.r = Mat.Ambient.g = Mat.Ambient.b = Mat.Ambient.a = Ka;

	//Kd = Clamp(0.0f, 1.0f, Kd);
	Mat.Diffuse.r = Mat.Diffuse.g = Mat.Diffuse.b = Mat.Diffuse.a = Kd;

	//Ks = Clamp(0.0f, 1.0f, Ks);
	Mat.Specular.r = Mat.Specular.g = Mat.Specular.b = Mat.Specular.a = Kd;

	//Ke = Clamp(0.0f, 1.0f, Ke);
	Mat.Emissive.r = Mat.Emissive.g = Mat.Emissive.b = Mat.Emissive.a = Ke;
	
	Mat.Power = Clamp(1.0f, 128.0f, power);

	return MaterialArray.AddElement(Mat);
}
	
////////////////////////////////////////////////////////////////////////////
uint CD3DMaterialSystem::AddMaterial(const sMaterial &Material)
{
	D3DMATERIAL9 Mat;
	memcpy(&Mat, &Material, sizeof(D3DMATERIAL9));

	Mat.Power = Clamp(1.0f, 128.0f, Material.Power);

	return MaterialArray.AddElement(Mat);
}

////////////////////////////////////////////////////////////////////////////
bool CD3DMaterialSystem::DeleteMaterial(uint index)
{
	return MaterialArray.DeleteElement(index);
}
	
////////////////////////////////////////////////////////////////////////////
bool CD3DMaterialSystem::GetMaterial(uint index, sMaterial &Material)
{
	D3DMATERIAL9 *Mat = MaterialArray.GetElement(index);
	if(Mat == NULL)
		return false;

	memcpy(&Material, Mat, sizeof(D3DMATERIAL9));

	return true;
}

	
////////////////////////////////////////////////////////////////////////////
bool CD3DMaterialSystem::ConfigureMaterial(uint index,float Ka,float Kd,float Ks,float Ke,float power)
{
	D3DMATERIAL9 *Mat = MaterialArray.GetElement(index);
	if(Mat == NULL)
		return false;

	Mat->Ambient.r = Mat->Ambient.g = Mat->Ambient.b = Mat->Ambient.a = Ka;
	Mat->Diffuse.r = Mat->Diffuse.g = Mat->Diffuse.b = Mat->Diffuse.a = Kd;
	Mat->Specular.r = Mat->Specular.g = Mat->Specular.b = Mat->Specular.a = Ks;
	Mat->Emissive.r = Mat->Emissive.g = Mat->Emissive.b = Mat->Emissive.a = Ke;
	Mat->Power = Clamp(1.0f, 128.0f, power);

	return true;
}
	
////////////////////////////////////////////////////////////////////////////
bool CD3DMaterialSystem::ConfigureMaterial(uint index,const sMaterial &Material)
{
	D3DMATERIAL9 *Mat = MaterialArray.GetElement(index);
	if(Mat == NULL)
		return false;

	memcpy(Mat, &Material, sizeof(D3DMATERIAL9));

	Mat->Power = Clamp(1.0f, 128.0f, Material.Power);

	return true;
}

	
////////////////////////////////////////////////////////////////////////////
bool CD3DMaterialSystem::SetMatAmbColor(uint index,float R,float G,float B,float A)
{
	D3DMATERIAL9 *Mat = MaterialArray.GetElement(index);
	if(Mat == NULL)
		return false;

	Mat->Ambient.r = R;
	Mat->Ambient.g = G;
	Mat->Ambient.b = B;
	Mat->Ambient.a = A;

	return true;
}
	
////////////////////////////////////////////////////////////////////////////
bool CD3DMaterialSystem::SetMatDifColor(uint index,float R,float G,float B,float A)
{
	D3DMATERIAL9 *Mat = MaterialArray.GetElement(index);
	if(Mat == NULL)
		return false;

	Mat->Diffuse.r = R;
	Mat->Diffuse.g = G;
	Mat->Diffuse.b = B;
	Mat->Diffuse.a = A;
		
	return true;
}
	
////////////////////////////////////////////////////////////////////////////
bool CD3DMaterialSystem::SetMatSpecColor(uint index,float R,float G,float B,float A)
{
	D3DMATERIAL9 *Mat = MaterialArray.GetElement(index);
	if(Mat == NULL)
		return false;

	Mat->Specular.r = R;
	Mat->Specular.g = G;
	Mat->Specular.b = B;
	Mat->Specular.a = A;

	return true;
}
	
////////////////////////////////////////////////////////////////////////////
bool CD3DMaterialSystem::SetMatEmColor(uint index,float R,float G,float B,float A)
{
	D3DMATERIAL9 *Mat = MaterialArray.GetElement(index);
	if(Mat == NULL)
		return false;

	Mat->Emissive.r = R;
	Mat->Emissive.g = G;
	Mat->Emissive.b = B;
	Mat->Emissive.a = A;

	return true;
}
	
////////////////////////////////////////////////////////////////////////////
bool CD3DMaterialSystem::SetMatPower(uint index, float power)
{
	D3DMATERIAL9 *Mat = MaterialArray.GetElement(index);
	if(Mat == NULL)
		return false;

	Mat->Power = Clamp(1.0f, 128.0f, power);
		
	return true;
}
	
////////////////////////////////////////////////////////////////////////////
bool CD3DMaterialSystem::SetMaterial(uint index)
{
	D3DMATERIAL9 *Mat = MaterialArray.GetElement(index);
	if(Mat == NULL)
		return false;

	m_pDevD3D->SetMaterial(Mat);

	return true;
}