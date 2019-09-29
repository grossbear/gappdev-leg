////////////////////////////////////////////////////////////////////////////////////////////////
//	D3DDeviceCaps.h
//	Direct3D Device Caps Class
//	Created: 15th August 2006
//
////////////////////////////////////////////////////////////////////////////////////////////////

class CD3DDeviceCaps:public IDeviceCaps
{
public:
	CD3DDeviceCaps(D3DCAPS9 *Caps){m_pDevCaps=Caps;}
	~CD3DDeviceCaps(){m_pDevCaps=NULL;}

	uint MaxLights(){return m_pDevCaps->MaxActiveLights;}
	uint MaxAnisotropy(){return (uint)m_pDevCaps->MaxAnisotropy;}
	uint MaxTexUnits(){return m_pDevCaps->MaxSimultaneousTextures;}
	uint MaxTexSize(){return m_pDevCaps->MaxTextureWidth;}
	uint MaxVolumeTexSize(){return m_pDevCaps->MaxVolumeExtent;}
	uint MaxCubeMapSize(){return 0;}
	uint MaxClipPlanes(){return m_pDevCaps->MaxUserClipPlanes;}
	bool TwoSideStencil(){return (m_pDevCaps->StencilCaps & D3DSTENCILCAPS_TWOSIDED)?(true):(false); }

protected:
	D3DCAPS9 *m_pDevCaps;

};