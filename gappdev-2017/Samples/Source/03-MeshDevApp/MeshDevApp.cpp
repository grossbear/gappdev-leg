/////////////////////////////////////////////////////////////////////////////////////////////
// MeshDevApp.cpp
/////////////////////////////////////////////////////////////////////////////////////////////

#include "AppWindow/AppWindow.h"
#include "MeshDevApp.h"

#include "Utils/IOStream/FileStream.h"
#include "Renderapi.h"
#include "scene.h"


#define APP_NAME "MeshDev Application"


/////////////////////////////////////////////////////////////////////////////////////////////
CApplication *CreateApplication(const char *cmdline)
{
	CMeshDevApp::CreateSingleton();
	return CMeshDevApp::GetSingleton();
}

/////////////////////////////////////////////////////////////////////////////////////////////
void CMeshDevApp::CreateSingleton()
{
	if(!m_pAppl) 
	{
		m_pAppl = new CMeshDevApp();
	}
}
/////////////////////////////////////////////////////////////////////////////////////////////
CMeshDevApp::CMeshDevApp() :CApplication(APP_NAME), 
m_bMDown(false), m_MPosX(0), m_MPosY(0), m_pScene(NULL)
{
    m_strApplName = APP_NAME;

}
/////////////////////////////////////////////////////////////////////////////////////////////
CMeshDevApp::~CMeshDevApp()
{
    delete (m_pScene);
}
/////////////////////////////////////////////////////////////////////////////////////////////
void CMeshDevApp::SetSize(int width, int height)
{
    if(height <= 0)
        height = 1;
    m_width = width;
    m_height = height;

    if(m_gapi)
    {
        m_gapi->Resize(this->m_width,this->m_height);
    }

    if(m_pScene)
        m_pScene->ResizeSceneView(m_width,m_height);
}
/////////////////////////////////////////////////////////////////////////////////////////////
bool CMeshDevApp::RunMainProcess()
{
    m_gapi->SetClearColor(0.4f,0.4f,0.7f,1.f);
    m_gapi->Clear();

    m_pScene->RenderScene();

    /*
    m_pFontMgr->SetFontTexture("Arial",font_arial_24_fontsize);
    m_pFontMgr->SetTextPos(0,0);
    uint color = 0xff00ff00;
    m_pFontMgr->PrintText("Simple text with outline",color,0xff000000,true);
    */

    m_gapi->SwapGraphicBuffers();

    return !m_bKeys[0x1B] && m_bRunning;
}

/////////////////////////////////////////////////////////////////////////////////////////////
void CMeshDevApp::KeyDown(unsigned int key)
{
	int xn = 0;
}

/////////////////////////////////////////////////////////////////////////////////////////////
void CMeshDevApp::KeyUp(unsigned int key)
{
	int xn = 0;
}
/////////////////////////////////////////////////////////////////////////////////////////////
void CMeshDevApp::MouseMove(int x, int y)
{
    if(m_bMDown==false) return ;

    int offx = x - m_MPosX;
    int offy = y - m_MPosY;
    m_MPosX = x;
    m_MPosY = y;
    float foffx = float(offx);
    float foffy = float(offy);
    m_pScene->RotateView(foffx,foffy);
}
/////////////////////////////////////////////////////////////////////////////////////////////
void CMeshDevApp::MouseButtonDown(ButtonType btype, int x, int y)
{
    if(btype == BTYPE_LBUTTON) {
        m_MPosX = x;
        m_MPosY = y;
        m_bMDown = true;
    }
}
/////////////////////////////////////////////////////////////////////////////////////////////
void CMeshDevApp::MouseButtonUp(ButtonType btype, int x, int y)
{
    if(btype == BTYPE_LBUTTON)
        m_bMDown = false;
}


/////////////////////////////////////////////////////////////////////////////////////////////
bool CMeshDevApp::StartAppl(CApplWindow *pApplWnd)
{
    if(!CApplication::StartAppl(pApplWnd))
        return false;
	
    return true;
}

/////////////////////////////////////////////////////////////////////////////////////////////
bool CMeshDevApp::InitAppl()
{
    bool res = true;

    m_pScene = new CSimpleScene(m_gapi);
    m_pScene->LoadScene();
    m_pScene->ResizeSceneView(this->m_width,this->m_height);
	
    return res;
}

/////////////////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////////////////
