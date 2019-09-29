//***********************************************************************************
//Application.h
//
//This header file contains declaration of CGraphicApplication class.
//***********************************************************************************

//Header file for windows.
#include <windows.h>
#include <stdio.h>
#include "Framework/Types/types.h"
#include "Framework/Timer/Timer.h"
#include "Framework/Templates/Array.h"
#include "Framework/Math/MathLib.h"
#include "Framework/Image/Image.h"
#include "Framework/3DObjects/3DObject.h"
#include "Framework/Camera/Camera.h"
#include "Framework/Renderer/Renderer.h"


#include "Framework/OpenGL/GLExtStruct.h"
#include "Framework/OpenGL/GLDevCaps.h"
#include "Framework/OpenGL/GLARBRenderElement.h"
#include "Framework/OpenGL/GLShaderObject.h"
#include "Framework/OpenGL/GLRenderer.h"

#include "Framework/Direct3D/D3DRenderer.h"

typedef struct 
{
	uint *ptr;
	uint idx;
}testStruct;

class CGraphicApplication
{
public:
	//Constructor is responsible for creating object of CGraphicApplication class.
	//Destructor is responsible for "clearning up..."
	CGraphicApplication();
	virtual ~CGraphicApplication();

	//This function handles with application messages
	//To this function comes all windows messages 
	LRESULT WINAPI GraphicApplicationWndProc(HWND hWnd,UINT msg, WPARAM wParam, LPARAM lParam);

	//This function start application
	void Start(HINSTANCE hInstance);

protected:
	//handle to an instance (the program itself)
	HINSTANCE hInst;

	//handle to a window of the program
	HWND hWnd;

	//Private GDI Device Context
	HDC hDC;

	bool	keys[256];	// Array Used For The Keyboard Routine
	bool	active;		// Window Active Flag Set To TRUE By Default
	bool	fullscreen;	// Fullscreen Flag Set To Fullscreen Mode By Default

	//structure for messages storage
	MSG msg;

	//window size
	int Width;
	int Height;

	//This class contains attributes that are registered by the RegisterClass function.
	WNDCLASSEX wndclass;

	//String pointers
	TCHAR* szWindowClass;
	TCHAR* szAppName;

	bool Done;

	bool Init();
	void Render();

private:
	CGraphicAPI* ptrGraphAPI;
	CGLObj* ptrGLObj;
	CD3DObj* ptrD3DObj;

	CCamera Cam;
	CTimer timer;
	char WinTitleAPI[30];

	C3DObject *Obj1;
	C3DObjectDesc *Obj1Desc;
	uint Obj1Idx;
};