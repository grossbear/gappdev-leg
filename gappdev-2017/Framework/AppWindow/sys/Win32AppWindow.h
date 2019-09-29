/////////////////////////////////////////////////////////////////////////////////////////////
// Win32AppWindow.h
//
// Win32 Application Main Window Class
/////////////////////////////////////////////////////////////////////////////////////////////

#ifndef __WIN32APPWINDOW_H__
#define __WIN32APPWINDOW_H__

#define _CRT_SECURE_NO_WARNINGS

#include <windows.h>
#ifdef UNICODE
#include <TCHAR.H>
#endif //UNICODE

#include "Renderapi.h"

#define WND_CLASS_NAME_SIZE 128
#define APPL_NAME_SIZE 128

class IGraphicApi;

class CWin32AppWindow
{
public:
	CWin32AppWindow(HINSTANCE hInstance);
	~CWin32AppWindow();

	bool CreateApplWindow(const char *name, int width, int height, bool fullscreen=false);
	void SetWindow();
	bool SetupGraphicApi(IGraphicApi *gapi, GAPICREATIONINFO &info);

	static LRESULT CALLBACK ApplWinProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
	void ProcessMessages();

protected:
	BOOL		RegisterApplWindowClass();
	HWND        CreateApplWindow();

    // Window Handlers And Structers
	HINSTANCE   m_hInstance;
	HWND        m_hWnd;
	HDC         m_hDC;
	WNDCLASSEX  m_WndClass;

	TCHAR       m_tstrWindowsClass[WND_CLASS_NAME_SIZE];// Window Class Name String
	TCHAR       m_tstrApplName[APPL_NAME_SIZE];// Application Name String

    // Size Of The Window
	int         m_iWidth;
	int         m_iHeight;

    // Full Screen Flag
	bool        m_bFullScreen;

};


#endif //__WIN32APPWINDOW_H__