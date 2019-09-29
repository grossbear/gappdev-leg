/////////////////////////////////////////////////////////////////////////////////////////////
// scene.h
/////////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "PlatformTypes.h"
//#include "Renderapi.h"
class IGraphicApi;
class CCamera;
//class CMeshData;
class md2data;
class md2renddata;




/////////////////////////////////////////////////////////////////////////////////////////////
class CSimpleScene
{
public:
    CSimpleScene(IGraphicApi *gapi);
    ~CSimpleScene();

    bool LoadScene();

    void ResizeSceneView(int width, int height);
    void RotateView(float lon, float lat);
    void RenderScene();

protected:
    char *LoadShaderSource(const char *filename);
    bool LoadShaders();

    bool LoadMD2Model(const char *filename);
    void RenderMD2Model();
    void ReleaseMD2Model();

    bool CreateQuadMesh();
    void RenderQuadMesh();
    void ReleaseQuadMesh();

private:
    IGraphicApi *m_gapi;

    CCamera *m_pCam;

    //Mesh Data And Buffers // not yet ready
    //CMeshData *m_pMesh;
    uint m_MeshVertBufIdx;
    uint m_MeshClrBufIdx;

    //MD2 Model Data
    md2data *m_pMD2Data;
    md2renddata *m_pMD2RendData;

    //Quad Mesh Data
    uint m_QuadVertBufIdx;
    uint m_QuadClrBufIdx;
    uint m_QuadVertNum;

    //Shaders Indices
    uint m_VertShaderIdx;
    uint m_VertClrShaderIdx;
    uint m_VertTexShaderIdx;
    
};
/////////////////////////////////////////////////////////////////////////////////////////////