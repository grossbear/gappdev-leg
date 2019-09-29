/////////////////////////////////////////////////////////////////////////////////////////////
// ApplWindow.cpp
//
// Application Main Window Class
/////////////////////////////////////////////////////////////////////////////////////////////

#include "../AppWindow.h"
#include "Application.h"

#define DEFAULT_WIDTH   640
#define DEFAULT_HEIGHT  480

#define WND_CLASS_NAME              "ApplWindow"
#define MSG_FAILED_REG_WND_CLASS    "Failed To Register The Window Class"
#define MSG_CLASS_REG_ERR_TITLE     "Register Error"
#define MSG_FAILED_CREATE_WND       "Failed To Create Window"
#define MSG_WND_CREATE_ERR_TITLE    "Window Creation Error"

/////////////////////////////////////////////////////////////////////////////////////////////
CWin32AppWindow::CWin32AppWindow(HINSTANCE hInstance):m_hInstance(hInstance),
m_hWnd(NULL),m_hDC(NULL),m_iWidth(0),m_iHeight(0)
{
    memset(&m_WndClass,0,sizeof(WNDCLASSEX));
    memset(m_tstrWindowsClass,0,WND_CLASS_NAME_SIZE*sizeof(TCHAR));
    memset(m_tstrApplName,0,APPL_NAME_SIZE*sizeof(TCHAR));

    m_iWidth = DEFAULT_WIDTH;
    m_iHeight = DEFAULT_HEIGHT;
}

/////////////////////////////////////////////////////////////////////////////////////////////
CWin32AppWindow::~CWin32AppWindow()
{
    DestroyWindow(m_hWnd);
	UnregisterClass(m_tstrWindowsClass,m_hInstance);
}

/////////////////////////////////////////////////////////////////////////////////////////////
bool CWin32AppWindow::CreateApplWindow(const char *name, int width, int height, bool fullscreen)
{
    int idx = 0;
    while(name[idx]!=0 && idx < APPL_NAME_SIZE)
    {
        m_tstrApplName[idx] = name[idx];
        idx++;
    }
    memcpy(m_tstrWindowsClass,_T(WND_CLASS_NAME),sizeof(TCHAR)*sizeof(_T(WND_CLASS_NAME)));

    m_iWidth = (width < 2) ? (DEFAULT_WIDTH) : (width);
    m_iHeight = (height < 2) ? (DEFAULT_HEIGHT) : (height);

    //Registering Application Window Class
    if(!RegisterApplWindowClass())
    {
        MessageBox(NULL,_T(MSG_FAILED_REG_WND_CLASS), _T(MSG_CLASS_REG_ERR_TITLE), MB_ICONERROR);
        return false;
    }

    //Creating Application Window
    m_hWnd = CreateApplWindow();
    if(!m_hWnd)
    {
        MessageBox(NULL,_T(MSG_FAILED_CREATE_WND), _T(MSG_WND_CREATE_ERR_TITLE), MB_ICONERROR);
        return false;
    }

    return true;
}

/////////////////////////////////////////////////////////////////////////////////////////////
void CWin32AppWindow::SetWindow()
{
	//Setting Window Params
    ShowWindow(m_hWnd,SW_SHOW);
    SetForegroundWindow(m_hWnd);
    SetFocus(m_hWnd);
}

/////////////////////////////////////////////////////////////////////////////////////////////
bool CWin32AppWindow::SetupGraphicApi(IGraphicApi *gapi, GAPICREATIONINFO &info)
{
    info.hWindow = m_hWnd;
    info.iWidth = m_iWidth;
    info.iHeight = m_iHeight;
    info.iColorBits = 32;
    info.iDepthBits = 24;
    info.iStencilBits = 8;
    return gapi->Create(&info);
}

/////////////////////////////////////////////////////////////////////////////////////////////
LRESULT CALLBACK CWin32AppWindow::ApplWinProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    CApplication *app = CApplication::GetSingleton();
    switch(msg)
    {
    case WM_CLOSE:
        app->CloseMsg();
        break;

    case WM_QUIT:
        app->QuitMsg();
        break;

    case WM_SIZE:
        app->SetSize((int)LOWORD(lParam), (int)HIWORD(lParam));
        break;

    case WM_DESTROY:
        break;

    case WM_KEYDOWN:
        app->KeyDown((unsigned int)wParam);
        //keys[wParam] = TRUE; 
        break;

    case WM_KEYUP:
        app->KeyUp((unsigned int)wParam);
        //Keys[wParam] = FALSE;
        break;

    case WM_LBUTTONDOWN:
        app->MouseButtonDown(BTYPE_LBUTTON,(int)LOWORD(lParam),(int)HIWORD(lParam));
        break;

    case WM_LBUTTONUP:
        app->MouseButtonUp(BTYPE_LBUTTON,(int)LOWORD(lParam),(int)HIWORD(lParam));
        break;

    case WM_LBUTTONDBLCLK:
        app->MouseButtonDblClk(BTYPE_LBUTTON,(int)LOWORD(lParam),(int)HIWORD(lParam));
        break;

    case WM_RBUTTONDOWN:
        app->MouseButtonDown(BTYPE_RBUTTON,(int)LOWORD(lParam),(int)HIWORD(lParam));
        break;

    case WM_RBUTTONUP:
        app->MouseButtonUp(BTYPE_RBUTTON,(int)LOWORD(lParam),(int)HIWORD(lParam));
        break;

    case WM_RBUTTONDBLCLK:
        app->MouseButtonDblClk(BTYPE_RBUTTON,(int)LOWORD(lParam),(int)HIWORD(lParam));
        break;

    case WM_MBUTTONDOWN:
        app->MouseButtonDown(BTYPE_MBUTTON,(int)LOWORD(lParam),(int)HIWORD(lParam));
        break;

    case WM_MBUTTONUP:
        app->MouseButtonUp(BTYPE_MBUTTON,(int)LOWORD(lParam),(int)HIWORD(lParam));
        break;

    case WM_MBUTTONDBLCLK:
        app->MouseButtonDblClk(BTYPE_MBUTTON,(int)LOWORD(lParam),(int)HIWORD(lParam));
        break;

    case WM_MOUSEMOVE:
        app->MouseMove((int)LOWORD(lParam),(int)HIWORD(lParam));
        switch(wParam)
        {
        case MK_LBUTTON:
            break;

        case MK_RBUTTON:
            break;

        case MK_MBUTTON:
            break;
        }
        break;

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

    }

    //Pass All Unhandled Messages To DefWindowProc
    return DefWindowProc(hWnd,msg,wParam,lParam);
}

/////////////////////////////////////////////////////////////////////////////////////////////
void CWin32AppWindow::ProcessMessages()
{
    MSG msg;
    bool process = true;
    CApplication *app = CApplication::GetSingleton();

    while(process)  // Loop That Runs While process=FALSE
    {
        if(PeekMessage(&msg,NULL,0,0,PM_REMOVE))    // Is There A Message Waiting?
        {
            TranslateMessage(&msg);					// Translate The Message
            DispatchMessage(&msg);					// Dispatch The Message
        }
        else
        {
            process = app->RunMainProcess();
        }
    }
    
    return ;
}


/////////////////////////////////////////////////////////////////////////////////////////////
// Ptotected Methods
/////////////////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////////////////
// Registering Application Window Class
BOOL CWin32AppWindow::RegisterApplWindowClass()
{
    WNDCLASSEX wndclass;
    memset(&wndclass,0,sizeof(WNDCLASSEX));

    wndclass.cbSize = sizeof(WNDCLASSEX);               // Size of structure
    wndclass.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;// Redraw On Size, And Own DC For Window.
    wndclass.lpfnWndProc = (WNDPROC)CApplWindow::ApplWinProc;    // WndProc Handles Messages
    wndclass.cbClsExtra = 0;                            // No Extra Window Data
    wndclass.cbWndExtra = 0;                            // No Extra Window Data
    wndclass.hInstance = m_hInstance;                   // Set The Instance
    wndclass.hIcon = LoadIcon(NULL, IDI_APPLICATION);   // Load The Default Icon
    wndclass.hCursor = LoadCursor(NULL,IDC_ARROW);      // Load The Arrow Pointer
    wndclass.hbrBackground = (HBRUSH )COLOR_BACKGROUND; // White Background
    wndclass.lpszMenuName = NULL;					    // We Don't Want A Menu
    wndclass.lpszClassName = m_tstrWindowsClass;        // Set The Class Name
    wndclass.hIconSm = LoadIcon(NULL, IDI_APPLICATION); // Load small icon

    return RegisterClassEx(&wndclass);
}

/////////////////////////////////////////////////////////////////////////////////////////////
// Creating Application Window
HWND CWin32AppWindow::CreateApplWindow()
{
    //DWORD dwStyle = WS_VISIBLE | WS_OVERLAPPEDWINDOW;

    // window styles
    DWORD dwStyle   = WS_CAPTION | WS_SYSMENU | WS_OVERLAPPEDWINDOW; //WS_MINIMIZEBOX;
    DWORD dwexStyle = WS_EX_APPWINDOW;

    //Create the window
    return CreateWindowEx(dwexStyle,            // Extended Style For The Window
        m_tstrWindowsClass,             // Class Name
        m_tstrApplName,                 // Window Title
        dwStyle,                        // Required Window Style
        CW_USEDEFAULT, CW_USEDEFAULT,   // Window Position
        m_iWidth, m_iHeight,            // Window width and height
        NULL,                           // No Parent Window
        NULL,                           // No Menu
        m_hInstance,                    // Instance
        NULL);
}

/////////////////////////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////////////////////////////