/////////////////////////////////////////////////////////////////////////////////////////////
// scene.cpp
/////////////////////////////////////////////////////////////////////////////////////////////

#include "scene.h"
#include "camera.h"
#include "mdldata/md2data.h"

#include "Utils/IOStream/FileStream.h"

#include "Models/Mesh/MeshData.h"
#include "Models/ModelsLoader/MD2Loader.h"


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
    LoadShaders();
    CreateQuadMesh();
    LoadMD2Model("..\\..\\..\\..\\Media\\Models\\Q2\\PLAYERS\\MALE\\TRIS.MD2");

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
    RenderQuadMesh();
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
    char *srcshader1 = LoadShaderSource("..\\..\\..\\..\\Media\\Shaders\\MeshDevShaders\\VertexTransfShd.glsl");
    if(srcshader1) {
        m_VertShaderIdx = m_gapi->CreateShader(srcshader1);
        delete [] srcshader1;
    }

    char *srcshader2 = LoadShaderSource("..\\..\\..\\..\\Media\\Shaders\\MeshDevShaders\\VtxClrTransfShd.glsl");
    if(srcshader2) {
        m_VertClrShaderIdx = m_gapi->CreateShader(srcshader2);
        delete [] srcshader2;
    }
    return true;
}

/////////////////////////////////////////////////////////////////////////////////////////////
void CSimpleScene::CreateTestMesh()
{
    /*
    m_pMesh1 = new CMeshData();
    m_pMesh1->SetVertNum(9);

    m_pMesh1->SetVertPos(0, -0.5f,  0.5f, 0.0f);
    m_pMesh1->SetVertPos(1,  0.0f,  0.5f, 0.0f);
    m_pMesh1->SetVertPos(2,  0.5f,  0.5f, 0.0f);

    m_pMesh1->SetVertPos(3, -0.5f,  0.0f, 0.0f);
    m_pMesh1->SetVertPos(4,  0.0f,  0.0f, 0.0f);
    m_pMesh1->SetVertPos(5,  0.5f,  0.0f, 0.0f);

    m_pMesh1->SetVertPos(6, -0.5f, -0.5f, 0.0f);
    m_pMesh1->SetVertPos(7,  0.0f, -0.5f, 0.0f);
    m_pMesh1->SetVertPos(8,  0.5f, -0.5f, 0.0f);

    m_pMesh1->AddFace(0,3,1);
    m_pMesh1->AddFace(1,3,4);
    m_pMesh1->AddFace(1,4,2);
    m_pMesh1->AddFace(2,4,5);
    m_pMesh1->AddFace(3,6,4);
    m_pMesh1->AddFace(4,6,7);
    m_pMesh1->AddFace(4,7,5);
    m_pMesh1->AddFace(5,7,8);

    m_pMesh1->BuildRenderMesh();
    */
}
/////////////////////////////////////////////////////////////////////////////////////////////
void CSimpleScene::RenderTestMesh(){}
/////////////////////////////////////////////////////////////////////////////////////////////
void CSimpleScene::ReleaseTestMesh(){}

/////////////////////////////////////////////////////////////////////////////////////////////
bool CSimpleScene::LoadMD2Model(const char *filename)
{
    ReleaseMD2Model();

    CMD2Loader md2loader;
    const char *filepath = "..\\..\\..\\..\\Media\\Models\\Q2\\PLAYERS\\MALE\\TRIS.MD2";
    const MD2ModelData *mdldata = md2loader.LoadModel(filepath);

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

    uint vertid = m_gapi->GetVertexAttribLocation(m_VertClrShaderIdx,"vVertex");
    uint clrid = m_gapi->GetVertexAttribLocation(m_VertClrShaderIdx,"vColor");

    m_gapi->SetVertexAttribBuffer(vertid,3,GAPI_FLOAT,false,0,0,m_QuadVertBufIdx);
    m_gapi->SetVertexAttribBuffer(clrid,3,GAPI_FLOAT,false,0,0,m_QuadClrBufIdx);

    m_gapi->SetShader(m_VertClrShaderIdx);
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