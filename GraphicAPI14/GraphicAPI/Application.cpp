/**************************************************************************
Application.cpp

This file consists definition CGraphicApplication class and all global
functions called by Operation System to start application
***************************************************************************/
#include "Application.h"
#include "Main.h"

//default constructor of the CGraphicApplication class
CGraphicApplication::CGraphicApplication()
{
	//Set window size
	//Width=808;
	//Height=627;
	Width = 264;
	Height = 302;

	//fill up wndclass structure with zeros
	//memset(&wndclass,0,sizeof(WNDCLASSEX));
	ZeroMemory(&wndclass,sizeof(WNDCLASSEX));

	//fill up keys array with zeros
	//memset(&keys,0,sizeof(keys));
	ZeroMemory(&keys,sizeof(keys));

	//give classes names
	szWindowClass = TEXT("mainwindow");
	szAppName     = TEXT("Graphic API");

	ptrGraphAPI=NULL;
	ptrGLObj=NULL;
	ptrD3DObj=NULL;

	Done=false;
}

//class destructor
CGraphicApplication::~CGraphicApplication()
{
	DestroyWindow(hWnd);
	UnregisterClass(szWindowClass,hInst);

	ptrGraphAPI->Delete3DObjFromBuffer(Obj1Idx);
	delete(ptrGraphAPI);
	ptrGraphAPI=NULL;
	ptrGLObj=NULL;
	ptrD3DObj=NULL;

	memset(&WinTitleAPI,0,sizeof(char)*strlen(WinTitleAPI));
}

////////////////////////////////////////////////////////////
//Function that starts application
////////////////////////////////////////////////////////////
void CGraphicApplication::Start(HINSTANCE hInstance)
{
	hInst = hInstance;

	//Creating NULL Renderer Object
	ptrGraphAPI = new CGraphicAPI();

	//Register window class
	wndclass.cbSize = sizeof(WNDCLASSEX);			//Size of structure
	wndclass.style = CS_HREDRAW|CS_VREDRAW|CS_OWNDC; // Redraw On Size, And Own DC For Window.
	wndclass.lpfnWndProc = WndProc;					// WndProc Handles Messages
	wndclass.cbClsExtra = 0;						// No Extra Window Data
	wndclass.cbWndExtra = 0;						// No Extra Window Data
	wndclass.hInstance = hInstance;					// Set The Instance
	wndclass.hIcon = LoadIcon(NULL, IDI_APPLICATION); // Load The Default Icon
	wndclass.hCursor = LoadCursor(NULL,IDC_ARROW);	// Load The Arrow Pointer
	wndclass.hbrBackground = NULL;					// No Background Required
	wndclass.lpszMenuName = NULL;					// We Don't Want A Menu
	wndclass.lpszClassName = szWindowClass;			// Set The Class Name
	wndclass.hIconSm = LoadIcon(NULL, IDI_APPLICATION); // Load small icon

	if (!RegisterClassEx (&wndclass))
    {
		MessageBox (NULL, "Failed To Register The Window Class.", "Error", MB_ICONERROR);
        return  ;
    }

	
	//Create the window
	if(!(hWnd=CreateWindowEx(0,								// Extended Style For The Window
							szWindowClass,					// Class Name
							szAppName,		// Window Title
                            WS_OVERLAPPEDWINDOW | WS_CLIPCHILDREN, // Required Window Style
                            CW_USEDEFAULT, CW_USEDEFAULT,	// Window Position
   							Width, Height,					//Window width and height
                            NULL,							// No Parent Window
							NULL,							// No Menu
							hInstance,						// Instance
							NULL)))							// Dont Pass Anything To WM_CREATE	
	{						
		MessageBox(NULL,"Window Creation Error.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return ;								// Return FALSE
	}

	
	ShowWindow(hWnd,SW_SHOW);							// Show The Window
	SetForegroundWindow(hWnd);							// Slightly Higher Priority
	SetFocus(hWnd);										// Sets Keyboard Focus To The Window

//#define _D3DRENDERING_
	delete (ptrGraphAPI);
#ifdef _D3DRENDERING_
	// Creating Direct3D Render Object
	ptrD3DObj = new CD3DObj();
	ptrGraphAPI = ptrD3DObj;
	strcpy(WinTitleAPI,"GraphicAPI Direct3D Rendering");
#else
	// Creating OpenGL Renderer Object
	ptrGLObj = new CGLObj();
	ptrGraphAPI = ptrGLObj;
	strcpy(WinTitleAPI,"GraphicAPI OpenGL Rendering");
#endif
	
	SetWindowText(hWnd,WinTitleAPI);

	if(!ptrGraphAPI->Create(hWnd))
		return ;

	if(!ptrGraphAPI->Init())
		return ;

	ptrGraphAPI->SetView(Cam);
	ptrGraphAPI->Resize(Width,Height);

	if(!Init())
		return ;

	int fps=0;
	float sumtime=0;
	while(!Done)										// Loop That Runs While done=FALSE
	{
		if(PeekMessage(&msg,NULL,0,0,PM_REMOVE))		// Is There A Message Waiting?
		{
			if(msg.message == WM_QUIT)					// Have We Received A Quit Message?
				Done=TRUE;								// If So done=TRUE
			else										// If Not, Deal With Window Messages
			{
				TranslateMessage(&msg);					// Translate The Message
				DispatchMessage(&msg);					// Dispatch The Message
			}
		}
		else
		{
			timer.Update();
			if(active)
			{
				float time=timer.GetInterval();
				sumtime+=time;
				if(sumtime>1.0f)
				{
					char WinTitle[60]={0};
					sprintf(WinTitle, "%s  Frames Per Second: %i",WinTitleAPI, fps);
					SetWindowText(hWnd,WinTitle);
					sumtime=0.0f;
					fps=0;
				}
				
				Render();

				fps+=1;
			}
		}
	}
}

bool CGraphicApplication::Init()
{
	Obj1 = new C3DObject();
	Obj1->CreateVertexArray(4);
	Obj1->CreateIndexArray(6);
	Obj1->AddTexCoordArray(2);
	Obj1->AddTexCoordArray(1);
	Obj1->CreatePrimaryColorArray();
	
	//Matrix4x4 M; M.RotationZ(DegToRad(70.0f));
	//Obj1->SetMatrix(M);

	Vector3D *Vert = Obj1->GetVertexArray();
	Vert[0].Set(-1,0,-1);	Vert[1].Set(1,0,-1);	Vert[2].Set(1,0,1);	Vert[3].Set(-1,0,1);
	Vector3D *Norm = Obj1->GetNormalArray();
	Norm[0] = Norm[1] = Norm[2] = Norm[3] = Vector3D(0,-1,0);
	uint *ind = Obj1->GetIndexArray();
	ind[0]=0; ind[1]=1; ind[2]=2; ind[3]=0; ind[4]=2; ind[5]=3;

	ubyte *clr = Obj1->GetPrimaryColorArray();
	*clr++ = 255; *clr++ = 255; *clr++ = 0; *clr++ = 255;
	*clr++ = 255; *clr++ = 255; *clr++ = 0; *clr++ = 255;
	*clr++ = 255; *clr++ = 255; *clr++ = 0; *clr++ = 255;
	*clr++ = 55; *clr++ = 155; *clr++ = 255; *clr++ = 255;

	//Setting Texture Coords
	float *coord1 = Obj1->GetTexCoordArray(0);
	float *coord2 = Obj1->GetTexCoordArray(1);

	*coord1++ = 0.0f; *coord1++ = 0.0f;
	*coord1++ = 1.0f; *coord1++ = 0.0f;
	*coord1++ = 1.0f; *coord1++ = 1.0f;
	*coord1++ = 0.0f; *coord1++ = 1.0f;
	
	*coord2++ = 0.0f; *coord2++ = 2.0f;
	*coord2++ = 2.0f; *coord2++ = 0.0f;

	
	Obj1Desc = new C3DObjectDesc();
	CGPUProgramDesc *shdesc = Obj1Desc->GetShaderDesc();
	uint data = 0;
	Vector4D vec4(0.8f,0.8f,0.5f,1);
	shdesc->AddShaderUniformDesc(WORLD_VIEW_PROJECTION_MATRIX,"WorldViewProjMatrix",NULL);
	shdesc->AddShaderUniformDesc(TEXTURE_UNIT,"BoatTex",&data);data++;
	shdesc->AddShaderUniformDesc(TEXTURE_UNIT,"stripes",&data);
	shdesc->AddShaderUniformDesc(USER_VECTOR4,"darken",&vec4);data=0;
	
	//Setting Texture Units
	Obj1Desc->SetTexUnits(2);
	Obj1Desc->SetTexID(0,ptrGraphAPI->pTexMgr->CreateTextureFromFile("../test/boat.bmp"));
	Obj1Desc->SetTexID(1,ptrGraphAPI->pTexMgr->CreateTextureFromFile("../test/1D.bmp"));

	ptrGraphAPI->pTexMgr->SetTexFilter(1,TEXF_LINEAR,TEXF_LINEAR,TEXF_LINEAR);
	ptrGraphAPI->pTexMgr->SetAnisotropicFilter(1,16);

#ifdef _D3DRENDERING_
	//Creating Shader Object In DirectX
	uint ShaderObj = ptrGraphAPI->CreateShaderFromFile("../test/test1.fx");
#else
	//Creating Shader Object In OpenGL
	uint ShaderObj = ptrGraphAPI->CreateShaderFromFiles("../test/test1.vert","../test/test1.frag");
#endif
	if(!ShaderObj)
		return false;
	Obj1Desc->SetGPUProgramID(ShaderObj);
	Obj1Idx = ptrGraphAPI->Add3DObjToBuffer(Obj1,Obj1Desc);
	if(!Obj1Idx)
		return false;
	
	return true;
}

void CGraphicApplication::Render()
{
	uint objidx=5;
	if(ptrGraphAPI)
	{
		ptrGraphAPI->SetOrthoProj(-1,1,-1,1,0,3);
		//ptrGraphAPI->SetView();
		ptrGraphAPI->Clear();
		ptrGraphAPI->Render(1,&Obj1Idx);
		ptrGraphAPI->SwapImageBuffers();
	}
}

//This is our functions that deals with messages
LRESULT WINAPI CGraphicApplication::GraphicApplicationWndProc(HWND hWnd, // Handle For This Window
															  UINT msg,  // Message For This Window
															  WPARAM wParam, // Additional Message Information
															  LPARAM lParam) // Additional Message Information
{
	switch(msg)
	{
	case WM_ACTIVATE:							// Watch For Window Activate Message
	{
		if (!HIWORD(wParam))					// Check Minimization State
			active=TRUE;						// Program Is Active
		else
			active=FALSE;						// Program Is No Longer Active
			
		return 0;								// Return To The Message Loop
	}

	case WM_SYSCOMMAND:							// Intercept System Commands
	{
		switch (wParam)							// Check System Calls
		{
			case SC_SCREENSAVE:					// Screensaver Trying To Start?
			case SC_MONITORPOWER:				// Monitor Trying To Enter Powersave?
			return 0;							// Prevent From Happening
		}
		break;									// Exit
	}
	
	case WM_CLOSE:								// Did We Receive A Close Message?
		PostQuitMessage(0);						// Send A Quit Message
		break;

	case WM_SIZE:
		Width = LOWORD(lParam);
		Height = HIWORD(lParam);
		ptrGraphAPI->Resize(Width, Height);  // LoWord=Width, HiWord=Height
		return 0;

	case WM_DESTROY:
		break;

	case WM_KEYDOWN:							// Is A Key Being Held Down?
		switch(wParam)
		{
		case VK_ESCAPE:
			PostQuitMessage(0);
			break;
		}
		keys[wParam]=TRUE;						// If So, Mark It As TRUE
		break;

	case WM_KEYUP:								// Has A Key Been Released?
		keys[wParam]=FALSE;						// If So, Mark It As FALSE
		break;
	}
	// Pass All Unhandled Messages To DefWindowProc
	return DefWindowProc(hWnd,msg,wParam,lParam);
}