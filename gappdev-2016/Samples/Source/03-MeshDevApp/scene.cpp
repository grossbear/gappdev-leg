/////////////////////////////////////////////////////////////////////////////////////////////
// scene.cpp
/////////////////////////////////////////////////////////////////////////////////////////////

#include "Renderapi.h"
#include "scene.h"
#include "camera.h"
#include "mdldata/md2data.h"

#include "Utils/IOStream/FileStream.h"

#include "Models/Mesh/MeshData.h"
#include "Models/ModelsLoader/MD2Loader.h"

#define MODEL_FILE_PATH "../Media/Models/Q2/knight/knight.md2"
#define SHADER_FILE_PATH1 "../Media/Shaders/MeshDevShaders/VertexTransfShd.glsl"
#define SHADER_FILE_PATH2 "../Media/Shaders/MeshDevShaders/VtxClrTransfShd.glsl"


/////////////////////////////////////////////////////////////////////////////////////////////
CSimpleScene::CSimpleScene(IGraphicApi *gapi):m_gapi(gapi),
m_pMesh(NULL),m_MeshVertBufIdx(NULL),m_MeshClrBufIdx(NULL),
m_pMD2Data(NULL),m_pMD2RendData(NULL),
m_QuadVertBufIdx(0),m_QuadClrBufIdx(0),m_QuadVertNum(0),
m_VertShaderIdx(0),m_VertClrShaderIdx(0),m_VertTexShaderIdx(0)
{
    m_pCam = new CCamera();
}
/////////////////////////////////////////////////////////////////////////////////////////////
CSimpleScene::~CSimpleScene()
{
    ReleaseMD2Model();
    ReleaseQuadMesh();
    delete (m_pCam);
}

/////////////////////////////////////////////////////////////////////////////////////////////
bool CSimpleScene::LoadScene()
{
    bool bload = false;
    bload = LoadShaders();
    if(bload==false) return false;
    
    bload = CreateQuadMesh();
    if(bload==false) return false;

    bload = LoadMD2Model(MODEL_FILE_PATH);
    if(bload==false) return false;

    return true;
}
/////////////////////////////////////////////////////////////////////////////////////////////
void CSimpleScene::ResizeSceneView(int width, int height)
{
    float aspect = float(width)/float(height);
    m_pCam->SetOrthoCam(80.f,80.f/aspect,-40.f,40.f);
}
/////////////////////////////////////////////////////////////////////////////////////////////
void CSimpleScene::RotateView(float lon, float lat)
{
    m_pCam->RotateCam(lon,lat);
}
/////////////////////////////////////////////////////////////////////////////////////////////
void CSimpleScene::RenderScene()
{
    RenderMD2Model();
}
/////////////////////////////////////////////////////////////////////////////////////////////
char *CSimpleScene::LoadShaderSource(const char *filename)
{
    CFileStream file;
    file.Open(filename,open_read);
    if(!file.IsOpened()) return NULL;

    isize_t isize = file.GetSize();
    if(isize==0) {
        file.Close();
        return NULL;
    }

    char *strdata = new char[isize+1];
    strdata[isize] = 0;
    uint_t readed = file.Read(strdata,isize);
    if(readed != isize) 
    {
        file.Close();
        delete [] strdata;
    }

    file.Close();

    return strdata;
}
/////////////////////////////////////////////////////////////////////////////////////////////
bool CSimpleScene::LoadShaders()
{
    char *srcshader1 = LoadShaderSource(SHADER_FILE_PATH1);
    if(srcshader1) {
        m_VertShaderIdx = m_gapi->CreateShader(srcshader1);
        delete [] srcshader1;
    }

    char *srcshader2 = LoadShaderSource(SHADER_FILE_PATH2);
    if(srcshader2) {
        m_VertClrShaderIdx = m_gapi->CreateShader(srcshader2);
        delete [] srcshader2;
    }
    return true;
}

/////////////////////////////////////////////////////////////////////////////////////////////
bool CSimpleScene::LoadMD2Model(const char *filename)
{
    ReleaseMD2Model();

    CMD2Loader md2loader;
    const MD2ModelData *mdldata = md2loader.LoadModel(filename);

    if(!mdldata) return false;

    m_pMD2Data = new md2data(mdldata);
    m_pMD2RendData = new md2renddata();
    Setmd2RendData(m_pMD2RendData,m_pMD2Data,m_gapi);

    return true;
}
/////////////////////////////////////////////////////////////////////////////////////////////
void CSimpleScene::RenderMD2Model()
{
    if(!m_pMD2RendData) return ;

    //rendering md2 model
    CMMatrix44<float> rotmtx;
    CMMtx44RotateX(rotmtx,mtorad(-90.f));
    CMMatrix44<float> viewprojmtx = m_pCam->GetViewProjMtx();
    CMMatrix44<float> modelviewproj = rotmtx*viewprojmtx;

    memcpy(m_pMD2RendData->m_ModelViewProjMtx,(const float*)&modelviewproj,sizeof(float)*16);

    Rendermd2RendData(m_pMD2RendData,m_gapi,m_VertShaderIdx,0xffffffff);
}
/////////////////////////////////////////////////////////////////////////////////////////////
void CSimpleScene::ReleaseMD2Model()
{
    //releasing md2 render data
    if(m_pMD2Data) {
        delete (m_pMD2Data);
        m_pMD2Data = NULL;
    }

    if(m_pMD2RendData) {
        Releasemd2RendData(m_pMD2RendData,m_gapi);
        delete (m_pMD2RendData);
        m_pMD2RendData = NULL;
    }
}
/////////////////////////////////////////////////////////////////////////////////////////////
bool CSimpleScene::CreateQuadMesh()
{
    float quadverts[] = {   -0.5f,0.5f,0.0f, -0.5f,-0.5f,0.0f, 0.0f,0.5f,0.0f,
                            0.0f,0.5f,0.0f, -0.5f,-0.5f,0.0f, 0.0f,-0.5f,0.0f,
                            0.5f,0.5f,0.0f, 0.0f,0.5f,0.0f, 0.0f,-0.5f,0.0f,
                            0.5f,0.5f,0.0f, 0.0f,-0.5f,0.0f, 0.5f,-0.5f,0.0f,    
                        };

    float quadclrs[] = {    1.0f,0.5f,0.5f, 1.0f,0.5f,0.25f, 0.5f,0.0f,0.0f,
                            0.5f,0.0f,0.0f, 1.0f,0.5f,0.25f, 0.5f,0.25f,0.f,
                            0.0f,0.5f,0.0f, 0.5f,1.0f,0.0f, 0.5f,1.0f,0.5f,
                            0.0f,0.5f,0.0f, 0.5f,1.0f,0.5f, 0.5f,0.5f,1.0f,
                        };

    m_QuadVertNum = sizeof(quadverts)/(sizeof(float)*3);
    m_QuadVertBufIdx = m_gapi->CreateVertexBuffer(GAPI_BUFUSAGE_DYNAMIC,m_QuadVertNum*sizeof(float)*3,quadverts);
    m_QuadClrBufIdx = m_gapi->CreateVertexBuffer(GAPI_BUFUSAGE_DYNAMIC,m_QuadVertNum*sizeof(float)*3,quadclrs);

    return true;
}
/////////////////////////////////////////////////////////////////////////////////////////////
void CSimpleScene::RenderQuadMesh()
{
    CMMatrix44<float> viewprojmtx = m_pCam->GetViewProjMtx();

    m_gapi->SetShader(m_VertClrShaderIdx);

    uint vertid = m_gapi->GetVertexAttribLocation(m_VertClrShaderIdx,"vVertex");
    uint clrid = m_gapi->GetVertexAttribLocation(m_VertClrShaderIdx,"vColor");

    m_gapi->SetVertexAttribBuffer(vertid,3,GAPI_FLOAT,false,0,0,m_QuadVertBufIdx);
    m_gapi->SetVertexAttribBuffer(clrid,3,GAPI_FLOAT,false,0,0,m_QuadClrBufIdx);

    
    m_gapi->SetShaderUniform(GAPI_UNIFORM_MTX44F,"TransformMtx",viewprojmtx);

    //m_gapi->SetFillMode(GAPIFILL_WIREFRAME);
    //m_gapi->SetCullMode(false,true);

    //m_gapi->Render();
    m_gapi->DrawArrays(GAPI_TRIANGLES,0,m_QuadVertNum);
    m_gapi->SetShader(0);
}
/////////////////////////////////////////////////////////////////////////////////////////////
void CSimpleScene::ReleaseQuadMesh()
{
}
/////////////////////////////////////////////////////////////////////////////////////////////

