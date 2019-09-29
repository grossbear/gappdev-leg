////////////////////////////////////////////////////////////////////////////////////////////////
//	MaterialMgr.h
//	Class That Creates Interface For Material Managment System
//	Created: 9th August 2006
//
////////////////////////////////////////////////////////////////////////////////////////////////

typedef struct 
{
	Vector4D Diffuse;
	Vector4D Ambient;
	Vector4D Specular;
	Vector4D Emission;
	float Power;	
}sMaterial;


//Class That Handles With Material System
class IMaterialSystem
{
public:
	IMaterialSystem(){};
	virtual ~IMaterialSystem(){};

	virtual uint AddMaterial(){return 0;}
	virtual uint AddMaterial(uint Ka,uint Kd,uint Ks,uint Ke,float power){return 0;}
	virtual uint AddMaterial(const sMaterial &Material){return 0;}
	virtual bool DeleteMaterial(uint index){return false;}
	virtual bool GetMaterial(uint index, sMaterial &Material){return false;}

	virtual bool ConfigureMaterial(uint index,float Ka,float Kd,float Ks,float Ke,float power){return false;}
	virtual bool ConfigureMaterial(uint index, const sMaterial &Material){return false;}

	virtual bool SetMatAmbColor(uint index,float R,float G,float B,float A){return false;}
	virtual bool SetMatDifColor(uint index,float R,float G,float B,float A){return false;}
	virtual bool SetMatSpecColor(uint index,float R,float G,float B,float A){return false;}
	virtual bool SetMatEmColor(uint index,float R,float G,float B,float A){return false;}
	virtual bool SetMatPower(uint index, float power){return false;}
	virtual bool SetMaterial(uint index){return false;}
};