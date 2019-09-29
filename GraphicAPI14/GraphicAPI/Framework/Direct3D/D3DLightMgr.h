////////////////////////////////////////////////////////////////////////////////////////////////
//	D3DLightMgr.h 
//	Direct3D Light Managment System Class
//	Created: 8th August 2006
//	
////////////////////////////////////////////////////////////////////////////////////////////////

class CD3DLightSystem:public ILightSystem
{
public:
	CD3DLightSystem(LPDIRECT3DDEVICE9 pDev,DWORD MaxActiveLights);
	~CD3DLightSystem();

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
	TArray <D3DLIGHT9> LightArray;
	LPDIRECT3DDEVICE9 m_pDevD3D;
};