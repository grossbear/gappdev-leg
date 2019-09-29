////////////////////////////////////////////////////////////////////////////////////////////////
//	GLDeviceCaps.h
//	OpenGL Device Caps Class
//	Created: 15th August 2006
//
////////////////////////////////////////////////////////////////////////////////////////////////


typedef struct {
	uint MaxLights;
	uint MaxAnisotropy;
	uint MaxTextureUnits;
	uint MaxTextureSize;
	uint Max3DTextureSize;
	uint MaxCubeMapSize;
	uint MaxClipPlanes;

	bool TwoSideStencil;

}sGLDevCaps;

class CGLDeviceCaps:public IDeviceCaps
{
public:
	CGLDeviceCaps(GLExtensions *pExt){m_pExt = pExt;}
	//CGLDeviceCaps(){}
	virtual ~CGLDeviceCaps(){m_pExt = NULL;}

	void GetGLDevCaps(sGLDevCaps &Caps);
	uint MaxLights();
	uint MaxAnisotropy();
	uint MaxTexUnits();
	uint MaxTexSize();
	uint MaxVolumeTexSize();
	uint MaxCubeMapSize();
	uint MaxClipPlanes();

	bool TwoSideStencil();

protected:
	GLExtensions *m_pExt;
};