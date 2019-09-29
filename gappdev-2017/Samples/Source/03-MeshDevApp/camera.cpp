/////////////////////////////////////////////////////////////////////////////////////////////
// camera.h
/////////////////////////////////////////////////////////////////////////////////////////////

#include "camera.h"

/////////////////////////////////////////////////////////////////////////////////////////////
CCamera::CCamera()
{
    m_bOrtho = true;
    m_CamPos = cvec3f(0.0f,0.0f,1.f);
    m_CamTgt = cvec3f(0.0f,0.0f,0.0f);
    m_CamUp = cvec3f(0.0f,1.0f,0.0f);

    m_Width = m_Height = 2.f;
    m_zNear = -2.f;
    m_zFar = 2.f;
    m_FOV = mtorad(90.f);
    m_Aspect = 1.f;

    m_bHemiSphereRot = true;

    UpdateCam();
}
/////////////////////////////////////////////////////////////////////////////////////////////
CCamera::~CCamera()
{
}

/////////////////////////////////////////////////////////////////////////////////////////////
void CCamera::SetCameraPos(float x, float y, float z)
{
    m_CamPos = cvec3f(x,y,z);
    UpdateCam();
}

/////////////////////////////////////////////////////////////////////////////////////////////
void CCamera::SetCameraTarget(float x, float y, float z)
{
    m_CamTgt = cvec3f(x,y,z);
    UpdateCam();
}

/////////////////////////////////////////////////////////////////////////////////////////////
void CCamera::SetOrthoCam(float width, float height, float znear, float zfar)
{
    m_Width = width;
    m_Height = height;
    m_zNear = znear;
    m_zFar = zfar;
    UpdateCam();
}

/////////////////////////////////////////////////////////////////////////////////////////////
void CCamera::SetPerpectiveCam(float degfov, float aspect, float znear, float zfar)
{
    m_FOV = mtorad(degfov);
    m_Aspect = aspect;
    m_zNear = znear;
    m_zFar = zfar;
    UpdateCam();
}

/////////////////////////////////////////////////////////////////////////////////////////////
void CCamera::RotateCam(float lon_delta, float lat_delta)
{
    float lon,lat,radius;
    ToPolarUpVecY(lon,lat,radius);

    lon += lon_delta;
    lat += lat_delta;

    lat = (lat>89.f)?(89.f):((lat<-89.f)?(-89.f):(lat));
    lat = (m_bHemiSphereRot && lat < 0.f)?(0.f):(lat);

    if(lon>360.f) {
        float pc = lon/360.f;
        pc = pc - (float)(int(pc));
        lon = 360.f*pc;
    }
    else if(lon < 0.f) {
        float pc = -lon/360.f;
        pc = pc - (float)(int(pc));
        pc = 1.f - pc;
        lon = 360.f*pc;
    }
    SetCamUpVecY(lon,lat,radius);
    UpdateCam();
}
/////////////////////////////////////////////////////////////////////////////////////////////
void CCamera::ToPolarUpVecY(float &lon, float &lat, float &radius)
{
    cvec3f eyevec = m_CamPos - m_CamTgt;
    
    radius = mvec3len(eyevec);   
    lat = asinf(eyevec.y/radius);
    
    cvec2f projvec(eyevec.x,eyevec.z);
    float projlen = mvec2len(projvec);
    
    lon = acosf(eyevec.x/projlen);
    if(eyevec.z<0)
        lon = M_2PI - lon;

    lon = mtodeg(lon);
    lat = mtodeg(lat);
}

/////////////////////////////////////////////////////////////////////////////////////////////
void CCamera::SetCamUpVecY(float lon, float lat, float radius)
{
    cvec3f eyevec;
    lon = mtorad(lon);
    lat = mtorad(lat);
    eyevec.x = radius*cosf(lat)*cosf(lon);
    eyevec.y = radius*sinf(lat);
    eyevec.z = radius*cosf(lat)*sinf(lon);
    m_CamPos = eyevec - m_CamTgt;
}

/////////////////////////////////////////////////////////////////////////////////////////////
void CCamera::UpdateCam()
{
    CMMatrix44<float> lookatmtx;
    CMMtx44LookAtRH(lookatmtx,m_CamPos,m_CamTgt,m_CamUp);
    //CMMtx44LookAtRH(lookatmtx,m_CamTgt,m_CamPos,m_CamUp);

    CMMatrix44<float> projmtx;
    //CMMtx44Identity(projmtx);
    if(m_bOrtho)
    {
        CMMtx44OrthoRH(projmtx,m_Width,m_Height,m_zNear,m_zFar);
    }
    else
    {
        CMMtx44PerspectiveFOVRH(projmtx,m_FOV,m_Aspect,m_zNear,m_zFar);
    }
    m_ViewProjMtx = lookatmtx*projmtx;
}

/////////////////////////////////////////////////////////////////////////////////////////////
const CMMatrix44<float>& CCamera::GetViewProjMtx() const
{
    return m_ViewProjMtx;
}

/////////////////////////////////////////////////////////////////////////////////////////////