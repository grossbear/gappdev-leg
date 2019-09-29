/////////////////////////////////////////////////////////////////////////////////////////////
// camera.h
/////////////////////////////////////////////////////////////////////////////////////////////

#include "Math/MathLib.h"

#ifndef _CAMERA_H_
#define _CAMERA_H_
/////////////////////////////////////////////////////////////////////////////////////////////
class CCamera
{
public:
    CCamera();
    ~CCamera();

    void SetCameraPos(float x, float y, float z);
    void SetCameraTarget(float x, float y, float z);
    void SetOrthoCam(float width, float height, float znear, float zfar);
    void SetPerpectiveCam(float degfov, float aspect, float znear, float zfar);

    void RotateCam(float lon, float lat);

    const CMMatrix44<float>& GetViewProjMtx() const;

protected:
    void UpdateCam();
    void ToPolarUpVecY(float &lon, float &lat, float &radius);
    void SetCamUpVecY(float lon, float lat, float radius);

    bool m_bOrtho;
    cvec3f m_CamPos;
    cvec3f m_CamTgt;
    cvec3f m_CamUp;

    float m_Width,m_Height;
    float m_zNear,m_zFar;
    float m_FOV;
    float m_Aspect;

    bool m_bHemiSphereRot;

    CMMatrix44<float> m_ViewProjMtx;
};
/////////////////////////////////////////////////////////////////////////////////////////////

#endif //_CAMERA_H_