////////////////////////////////////////////////////////////////////////////////////////////////
//	LightMgr.h
//	Class That Creates Interface For Light Managment System
//	Created: 8th August 2006
//
////////////////////////////////////////////////////////////////////////////////////////////////

enum LIGHTTYPE{
	POINTLIGHT	=	0,
	DIRECTIONAL	=	1,
	SPOT		=	2,
};

typedef struct
{
	LIGHTTYPE type;
	float Ambient[3];// Values From 0 to 1
	float Diffuse[3];// Values From 0 to 1
	float Specular[3];// Values From 0 to 1
	Vector3D Position;
	Vector3D Direction;
	float Attenuation[3];//3 attenuation parameters: constant, linear and quadratic.
	float Range;
	float Falloff;
	float Theta;
	float Phi;//values from 0 to PI
}sLight;


//Class That Handles With Light System
class ILightSystem
{
public:
	ILightSystem(){}
	virtual ~ILightSystem(){}

	virtual uint AddLight(){return 0;}
	virtual uint AddLight(const sLight &LightSrc){return 0;}
	virtual bool ConfigureLight(uint index,const sLight &LightSrc){return false;}
	virtual bool DeleteLight(uint index){return false;}
	virtual bool GetLight(uint index, sLight &LightSrc){return false;}

	virtual bool SetLightAmbColor(uint index,float R,float G,float B){return false;}
	virtual bool SetLightDifColor(uint index,float R,float G,float B){return false;}
	virtual bool SetLightSpecColor(uint index,float R,float G,float B){return false;}
	virtual bool SetLightPos(uint index,const Vector3D &Pos){return false;}
	virtual bool SetLightDir(uint index,const Vector3D &Dir){return false;}
	virtual bool SetLightAttenuation(uint index, float C1,float C2,float C3){return false;}
	virtual bool SetLightConstantAttenuation(uint index, float C1){return false;}
	virtual bool SetLightLinearAttenuation(uint index, float C2){return false;}
	virtual bool SetLightQuadraticAttenuation(uint index, float C3){return false;}
	virtual bool SetLightRange(uint index, float Range){return false;}
	virtual bool SetLightType(uint index, LIGHTTYPE Type){return false;}
	virtual bool SetSpotLightParams(uint index, float Falloff, float Theta, float Phi){return false;}

	virtual bool SetLight(uint index, int LightNum){return false;}
	virtual void TurnOffLights(){}
	virtual void TurnOffLight(int LightNum){}
protected:
	int MaxLights;
	
};