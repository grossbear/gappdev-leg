////////////////////////////////////////////////////////////////////////////////////////////////
//	GLightMgr.h 
//	OpenGL Light Managment System Class
//	Created: 25th April 2006
//	Modified: 8th August 2006
//	
////////////////////////////////////////////////////////////////////////////////////////////////

typedef struct
{
	LIGHTTYPE type;
	GLfloat Ambient[4];
	GLfloat Diffuse[4];
	GLfloat Specular[4];
	GLfloat Position[4];
	GLfloat Direction[4];
	GLfloat Attenuation[3];
	float Range;
	float Falloff;
	float Theta;
	float Phi;
}sGLight;

class CGLightSystem:public ILightSystem
{
public:
	CGLightSystem();
	~CGLightSystem();

	uint AddLight();
	uint AddLight(const sLight &LightSrc);
	bool ConfigureLight(uint index,const sLight &LightSrc);
	bool DeleteLight(uint index);
	bool GetLight(uint index, sLight &LightSrc);

	bool SetLightAmbColor(uint index,float R,float G,float B);
	bool SetLightDifColor(uint index,float R,float G,float B);
	bool SetLightSpecColor(uint index,float R,float G,float B);
	bool SetLightPos(uint index,const Vector3D &Pos);
	bool SetLightDir(uint index,const Vector3D &Dir);
	bool SetLightAttenuation(uint index, float C1,float C2,float C3);
	bool SetLightConstantAttenuation(uint index, float C1);
	bool SetLightLinearAttenuation(uint index, float C2);
	bool SetLightQuadraticAttenuation(uint index, float C3);
	bool SetLightRange(uint index, float Range);
	bool SetLightType(uint index, LIGHTTYPE Type);
	bool SetSpotLightParams(uint index, float Falloff, float Theta, float Phi);

	bool SetLight(uint index, int LightNum);
	void TurnOffLights();
	void TurnOffLight(int LightNum);

protected:
	TArray <sGLight> LightArray;

};