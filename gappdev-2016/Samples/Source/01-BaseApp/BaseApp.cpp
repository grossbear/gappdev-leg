/////////////////////////////////////////////////////////////////////////////////////////////
// ApplDev.cpp
/////////////////////////////////////////////////////////////////////////////////////////////

#include "AppWindow/AppWindow.h"
#include "BaseApp.h"

#define APPLICATION_NAME "Base Graphic Application"

////////////////////////////////////////////////////////////////////////////////////////////
CApplication *CreateApplication(const char *cmdline)
{
	CBaseApp::CreateSingleton();
	return CBaseApp::GetSingleton();
}

/////////////////////////////////////////////////////////////////////////////////////////////
void CBaseApp::CreateSingleton()
{
	if(!m_pAppl)
	{
		m_pAppl = new CBaseApp();
	}
}

/////////////////////////////////////////////////////////////////////////////////////////////
CBaseApp::CBaseApp() :CApplication(APPLICATION_NAME){}

/////////////////////////////////////////////////////////////////////////////////////////////
CBaseApp::~CBaseApp(){}

/////////////////////////////////////////////////////////////////////////////////////////////
void CBaseApp::SetSize(int width, int height)
{
	if(height <= 0)
		height = 1;

	m_width = width;
	m_height = height;

	if(m_gapi)
	{
		m_gapi->Resize(this->m_width,this->m_height);  
	}
}

/////////////////////////////////////////////////////////////////////////////////////////////
bool CBaseApp::RunMainProcess()
{
	m_gapi->SetClearColor(0.4f,0.4f,0.7f,1.f);
	m_gapi->Clear();
	//m_gapi->Render();
	m_gapi->SwapGraphicBuffers();


	return !m_bKeys[0x1B] && m_bRunning;
}

/////////////////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////////////////
void CBaseApp::KeyDown(unsigned int key)
{
	int xn = 0;
}

/////////////////////////////////////////////////////////////////////////////////////////////
void CBaseApp::KeyUp(unsigned int key)
{
	int xn = 0;
}


/////////////////////////////////////////////////////////////////////////////////////////////
bool CBaseApp::StartAppl(CApplWindow *pApplWnd)
{
	if(!CApplication::StartAppl(pApplWnd))
		return false;

	return true;
}

/////////////////////////////////////////////////////////////////////////////////////////////
bool CBaseApp::InitAppl()
{
	bool res = true;
    /*
	uint32t max_vert_attrib = m_gapi->GetDevCaps(GAPI_DEVPARAM_MAX_VERTEX_ATTRIBS);
	uint32t max_vert_uni = m_gapi->GetDevCaps(GAPI_DEVPARAM_MAX_VERTEX_UNIFORM_COMPONENTS);
	uint32t max_vert_var = m_gapi->GetDevCaps(GAPI_DEVPARAM_MAX_VERTEX_VARYING_COMPONENTS);
	uint32t max_var_comp = m_gapi->GetDevCaps(GAPI_DEVPARAM_MAX_VARYING_COMPONENTS);
	uint32t max_vert_tex_units = m_gapi->GetDevCaps(GAPI_DEVPARAM_MAX_VERTEX_TEXTURE_UNITS);
	uint32t max_var_floats = m_gapi->GetDevCaps(GAPI_DEVPARAM_MAX_VARYING_FLOATS);
	uint32t max_comb_tex_units = m_gapi->GetDevCaps(GAPI_DEVPARAM_MAX_COMBINED_TEXTURE_UNITS);

	uint32t max_frag_uni = m_gapi->GetDevCaps(GAPI_DEVPARAM_MAX_FRAGMENT_UNIFORM_COMPONENTS);
	uint32t max_frag_tex_units = m_gapi->GetDevCaps(GAPI_DEVPARAM_MAX_FRAGMENT_TEXTURE_UNITS);

	uint32t geom_shad = m_gapi->GetDevCaps(GAPI_DEVPARAM_GEOMETRY_SHADER);
	uint32t max_geom_out_vert = m_gapi->GetDevCaps(GAPI_DEVPARAM_MAX_GEOMETRY_OUTPUT_VERT);
	uint32t max_geom_tex_units = m_gapi->GetDevCaps(GAPI_DEVPARAM_MAX_GEOMETRY_TEXTURE_UNITS);
	uint32t max_geom_var_comp = m_gapi->GetDevCaps(GAPI_DEVPARAM_MAX_GEOMETRY_VARYING_COMPONENTS);
	uint32t max_geom_uni_comp = m_gapi->GetDevCaps(GAPI_DEVPARAM_MAX_GEOMETRY_UNIFORM_COMPONENTS);
	uint32t max_geom_total_out_comp = m_gapi->GetDevCaps(GAPI_DEVPARAM_MAX_GEOMETRY_TOTAL_OUTPUT_COMPONENTS);

	uint32t max_vert_uni_blocks = m_gapi->GetDevCaps(GAPI_DEVPARAM_MAX_VERTEX_UNIFORM_BLOCKS);
	uint32t max_geom_uni_blocks = m_gapi->GetDevCaps(GAPI_DEVPARAM_MAX_GEOMETRY_UNIFORM_BLOCKS);
	uint32t max_frag_uni_blocks = m_gapi->GetDevCaps(GAPI_DEVPARAM_MAX_FRAGMENT_UNIFORM_BLOCKS);
	uint32t max_comb_uni_blocks = m_gapi->GetDevCaps(GAPI_DEVPARAM_MAX_COMBINED_UNIFORM_BLOCKS);

	uint32t max_uni_buf_bind = m_gapi->GetDevCaps(GAPI_DEVPARAM_MAX_UNIFORM_BUFFER_BINDINGS);
	uint32t max_uni_block_size = m_gapi->GetDevCaps(GAPI_DEVPARAM_MAX_UNIFORM_BLOCK_SIZE);

	uint32t max_comb_vert_uni_blocks = m_gapi->GetDevCaps(GAPI_DEVPARAM_MAX_COMBINED_VERTEX_UNIFORM_COMPONENTS);
	uint32t max_comb_geom_uni_blocks = m_gapi->GetDevCaps(GAPI_DEVPARAM_MAX_COMBINED_GEOMETRY_UNIFORM_COMPONENTS);
	uint32t max_comb_frag_uni_blocks = m_gapi->GetDevCaps(GAPI_DEVPARAM_MAX_COMBINED_FRAGMENT_UNIFORM_COMPONENTS);

	uint32t max_draw_buffers = m_gapi->GetDevCaps(GAPI_DEVPARAM_MAX_DRAW_BUFFERS);
	uint32t max_tex_buf_size = m_gapi->GetDevCaps(GAPI_DEVPARAM_MAX_TEXTURE_BUFFER_SIZE);

	uint32t max_cube_tex_size = m_gapi->GetDevCaps(GAPI_DEVPARAM_MAX_CUBE_MAP_TEXTURE_SIZE);

	uint32t max_render_buf_size = m_gapi->GetDevCaps(GAPI_DEVPARAM_MAX_RENDERBUFFER_SIZE);
	uint32t max_color_attach = m_gapi->GetDevCaps(GAPI_DEVPARAM_MAX_COLOR_ATTACHMENTS);
	uint32t max_samples = m_gapi->GetDevCaps(GAPI_DEVPARAM_MAX_SAMPLES);
    */
	return res;
}

/////////////////////////////////////////////////////////////////////////////////////////////