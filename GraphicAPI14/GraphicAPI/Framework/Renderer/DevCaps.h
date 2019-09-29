////////////////////////////////////////////////////////////////////////////////////////////////
//	DeviceCaps.h
//	Class That Creates Interface For Device Caps
//	Created: 9th August 2006
//
////////////////////////////////////////////////////////////////////////////////////////////////


class IDeviceCaps
{
public:
	IDeviceCaps(){}
	virtual ~IDeviceCaps(){}

	virtual uint MaxLights(){return 0;}
	virtual uint MaxAnisotropy(){return 0;}
	virtual uint MaxTexUnits(){return 0;}
	virtual uint MaxTexSize(){return 0;}
	virtual uint MaxVolumeTexSize(){return 0;}
	virtual uint MaxCubeMapSize(){return 0;}
	virtual uint MaxClipPlanes(){return 0;}

	virtual bool TwoSideStencil(){return false;}

};