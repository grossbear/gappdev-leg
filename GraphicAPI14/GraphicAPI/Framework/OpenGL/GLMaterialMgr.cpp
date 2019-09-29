/////////////////////////////////////////////////////////////////////////////
//	GLMaterialMgr.cpp
//	OpenGL Material Manager Class
//	Created: 29th May 2006
//	Modified: 9th August 2006
//	
////////////////////////////////////////////////////////////////////////////

#include <windows.h>
#include <memory.h>
#include <stdio.h>

#include "../Types/types.h"
#include "../Math/MathLib.h"
#include "../Templates/Array.h"
#include "../Camera/Camera.h"
//#include "../3DObjects/3DObjectDesc.h"
#include "../3DObjects/3DObject.h"
#include "../Image/Image.h"
//#include "../Image/ImageIO.h"
//#include "../Image/ImgTypes.h"

#include "../Renderer/Renderer.h"
//#include "GLARBRenderElement.h"
//#include "GLExtStruct.h"
//#include "GLDevCaps.h"
//#include "GLRenderer.h"

#include <gl/gl.h>
#include <gl/glu.h>

#include "GLMaterialMgr.h"

////////////////////////////////////////////////////////////////////////////
uint CGLMaterialSystem::AddMaterial()
{
	sMaterial Mat;
	memset(&Mat,0,sizeof(sMaterial));

	return MaterialArray.AddElement(Mat);
}

////////////////////////////////////////////////////////////////////////////
uint CGLMaterialSystem::AddMaterial(float Ka,float Kd,float Ks,float Ke,float power)
{
	sMaterial Mat;
	memset(&Mat,0,sizeof(sMaterial));

	//Ka = Clamp(0.0f, 1.0f, Ka);
	Mat.Ambient.Set(Ka,Ka,Ka,Ka);

	//Kd = Clamp(0.0f, 1.0f, Kd);
	Mat.Diffuse.Set(Kd,Kd,Kd,Kd);

	//Ks = Clamp(0.0f, 1.0f, Ks);
	Mat.Specular.Set(Ks,Ks,Ks,Ks);

	//Ke = Clamp(0.0f, 1.0f, Ke);
	Mat.Emission.Set(Ke,Ke,Ke,Ke);

	Mat.Power = Clamp(1.0f, 128.0f, power);

	return MaterialArray.AddElement(Mat);
}

////////////////////////////////////////////////////////////////////////////
uint CGLMaterialSystem::AddMaterial(const sMaterial &Material)
{
	sMaterial Mat;
	memcpy(&Mat,&Material,sizeof(sMaterial));
	
	//ClampValues(0.0f, 1.0f, Mat.Ambient);
	//ClampValues(0.0f, 1.0f, Mat.Diffuse);
	//ClampValues(0.0f, 1.0f, Mat.Specular);
	//ClampValues(0.0f, 1.0f, Mat.Emission);
	Mat.Power = Clamp(1.0f, 128.0f, Material.Power);

	return MaterialArray.AddElement(Mat);
}

////////////////////////////////////////////////////////////////////////////
bool CGLMaterialSystem::DeleteMaterial(uint index)
{
	return MaterialArray.DeleteElement(index);
}

////////////////////////////////////////////////////////////////////////////
bool CGLMaterialSystem::GetMaterial(uint index, sMaterial &Material)
{
	sMaterial *Mat = MaterialArray.GetElement(index);

	if(Mat==NULL)
		return false;

	memcpy(&Material, Mat, sizeof(sMaterial));

	return true;
}

sMaterial *CGLMaterialSystem::GetMaterial(uint index)
{
	return MaterialArray.GetElement(index);
}

////////////////////////////////////////////////////////////////////////////
bool CGLMaterialSystem::ConfigureMaterial(uint index,float Ka,float Kd,float Ks,float Ke,float power)
{
	sMaterial *Mat = MaterialArray.GetElement(index);

	if(Mat==NULL)
		return false;

	//Ka = Clamp(0.0f, 1.0f, Ka);
	Mat->Ambient.Set(Ka,Ka,Ka,Ka);

	//Kd = Clamp(0.0f, 1.0f, Kd);
	Mat->Diffuse.Set(Kd,Kd,Kd,Kd);

	//Ks = Clamp(0.0f, 1.0f, Ks);
	Mat->Specular.Set(Ks,Ks,Ks,Ks);

	//Ke = Clamp(0.0f, 1.0f, Ke);
	Mat->Emission.Set(Ke,Ke,Ke,Ke);

	Mat->Power = Clamp(1.0f, 128.0f, power);

	return true;
}

////////////////////////////////////////////////////////////////////////////
bool CGLMaterialSystem::ConfigureMaterial(uint index,const sMaterial &Material)
{
	sMaterial *Mat = MaterialArray.GetElement(index);

	if(Mat==NULL)
		return false;

	memcpy(Mat,&Material,sizeof(sMaterial));
	
	//ClampValues(0.0f, 1.0f, Mat->Ambient);
	//ClampValues(0.0f, 1.0f, Mat->Diffuse);
	//ClampValues(0.0f, 1.0f, Mat->Specular);
	//ClampValues(0.0f, 1.0f, Mat->Emission);
	Mat->Power = Clamp(0.0f, 1.0f, Mat->Power);

	return true;
}

////////////////////////////////////////////////////////////////////////////
bool CGLMaterialSystem::SetMatAmbColor(uint index,float R,float G,float B,float A)
{
	sMaterial *Mat = MaterialArray.GetElement(index);

	if(Mat==NULL)
		return false;

	/*
	Mat->Ambient.Set(Clamp(0.0f,1.0f,R), Clamp(0.0f,1.0f,G),
					Clamp(0.0f,1.0f,B), Clamp(0.0f,1.0f,A));
	*/
	Mat->Ambient.Set(R,G,B,A);
	return true;
}

////////////////////////////////////////////////////////////////////////////
bool CGLMaterialSystem::SetMatDifColor(uint index,float R,float G,float B,float A)
{
	sMaterial *Mat = MaterialArray.GetElement(index);

	if(Mat==NULL)
		return false;
	/*
	Mat->Diffuse.Set(Clamp(0.0f,1.0f,R), Clamp(0.0f,1.0f,G),
					Clamp(0.0f,1.0f,B), Clamp(0.0f,1.0f,A));
	*/
	Mat->Diffuse.Set(R,G,B,A);
	return true;
}

////////////////////////////////////////////////////////////////////////////
bool CGLMaterialSystem::SetMatSpecColor(uint index,float R,float G,float B,float A)
{
	sMaterial *Mat = MaterialArray.GetElement(index);

	if(Mat==NULL)
		return false;
	/*
	Mat->Specular.Set(Clamp(0.0f,1.0f,R), Clamp(0.0f,1.0f,G),
					Clamp(0.0f,1.0f,B), Clamp(0.0f,1.0f,A));
	*/
	Mat->Specular.Set(R,G,B,A);

	return true;
}

////////////////////////////////////////////////////////////////////////////
bool CGLMaterialSystem::SetMatEmColor(uint index,float R,float G,float B,float A)
{
	sMaterial *Mat = MaterialArray.GetElement(index);

	if(Mat==NULL)
		return false;

	/*
	Mat->Emission.Set(Clamp(0.0f,1.0f,R), Clamp(0.0f,1.0f,G),
					Clamp(0.0f,1.0f,B), Clamp(0.0f,1.0f,A));
	*/
	Mat->Emission.Set(R,G,B,A);

	return true;
}

////////////////////////////////////////////////////////////////////////////
bool CGLMaterialSystem::SetMatPower(uint index, float power)
{
	sMaterial *Mat = MaterialArray.GetElement(index);

	if(Mat==NULL)
		return false;
	
	Mat->Power = Clamp(1.0f, 128.0f, power);

	return true;
}

////////////////////////////////////////////////////////////////////////////
bool CGLMaterialSystem::SetMaterial(uint index)
{
	sMaterial *Material = MaterialArray.GetElement(index);

	if(Material == NULL)
		return false;

	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, Material->Ambient);
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, Material->Diffuse);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, Material->Specular);
	glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, Material->Emission);
	glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, Material->Power);

	return true;
}