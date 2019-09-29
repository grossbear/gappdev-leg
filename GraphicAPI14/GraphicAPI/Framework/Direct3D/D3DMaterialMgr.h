/////////////////////////////////////////////////////////////////////////////
//	D3DMaterialMgr.h
//	Direct3D Material Manager Class
//	Created: 9th August 2006
//	
////////////////////////////////////////////////////////////////////////////

//Direct3D Material Manager Class
class CD3DMaterialSystem:public IMaterialSystem
{
public:
	CD3DMaterialSystem(LPDIRECT3DDEVICE9 pDev){m_pDevD3D = pDev;}
	~CD3DMaterialSystem(){MaterialArray.~MaterialArray();}

	uint AddMaterial();
	uint AddMaterial(float Ka,float Kd,float Ks,float Ke,float power);
	uint AddMaterial(const sMaterial &Material);
	bool DeleteMaterial(uint index);
	bool GetMaterial(uint index, sMaterial &Material);

	bool ConfigureMaterial(uint index,float Ka,float Kd,float Ks,float Ke,float power);
	bool ConfigureMaterial(uint index,const sMaterial &Material);

	bool SetMatAmbColor(uint index,float R,float G,float B,float A);
	bool SetMatDifColor(uint index,float R,float G,float B,float A);
	bool SetMatSpecColor(uint index,float R,float G,float B,float A);
	bool SetMatEmColor(uint index,float R,float G,float B,float A);
	bool SetMatPower(uint index, float power);
	bool SetMaterial(uint index);

protected:
	TArray <D3DMATERIAL9> MaterialArray;
	LPDIRECT3DDEVICE9 m_pDevD3D;
};