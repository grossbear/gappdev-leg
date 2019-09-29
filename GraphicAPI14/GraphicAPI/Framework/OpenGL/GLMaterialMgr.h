/////////////////////////////////////////////////////////////////////////////
//	GLMaterialMgr.h
//	OpenGL Material Manager Class
//	Created: 29th May 2006
//	Modified: 9th August 2006
//	
////////////////////////////////////////////////////////////////////////////


//OpenGL Material Manager Class
class CGLMaterialSystem:public IMaterialSystem
{
public:
	CGLMaterialSystem(){}
	~CGLMaterialSystem(){MaterialArray.~MaterialArray();}

	uint AddMaterial();
	uint AddMaterial(float Ka,float Kd,float Ks,float Ke,float power);
	uint AddMaterial(const sMaterial &Material);
	bool DeleteMaterial(uint index);
	bool GetMaterial(uint index, sMaterial &Material);

	sMaterial *GetMaterial(uint index);

	bool ConfigureMaterial(uint index,float Ka,float Kd,float Ks,float Ke,float power);
	bool ConfigureMaterial(uint index,const sMaterial &Material);

	bool SetMatAmbColor(uint index,float R,float G,float B,float A);
	bool SetMatDifColor(uint index,float R,float G,float B,float A);
	bool SetMatSpecColor(uint index,float R,float G,float B,float A);
	bool SetMatEmColor(uint index,float R,float G,float B,float A);
	bool SetMatPower(uint index, float power);
	bool SetMaterial(uint index);

protected:
	TArray <sMaterial> MaterialArray;
};