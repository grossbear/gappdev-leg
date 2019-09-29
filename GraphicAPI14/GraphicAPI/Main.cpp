//******************************************************************
//Main.cpp
//
//This file contains basic functions called by our Operation System
//at the moment when application is created.
//******************************************************************

//This header file contains CGraphicApplication class implementation.
#include "Application.h"
//This file contains prototipes of basic functions called by our OS.
#include "Main.h"

//This is our global pointer of CGraphicApplication class.
static CGraphicApplication* g_ptrCGraphAppl=NULL;

//WinMain function is entry point of our application.
//Mostly this function is used for calling our window routine, deal with window messages and
//watch for user interection. For our purposes in this function we creates object of the
//CGraphicApplication class and calls class public functions. 
int WINAPI WinMain(HINSTANCE hInstance,HINSTANCE hPrevInstance,LPSTR lpCmdLine,int nShowCmd)
{
	//through pointer we are creating new object of CGraphicApplication class. 
	g_ptrCGraphAppl = new CGraphicApplication();

	//Application works until function Start ends.
	g_ptrCGraphAppl->Start(hInstance);

	//Clearning up after class.
	delete(g_ptrCGraphAppl);

	//End of program.
	return 0;
}

//This is function where all window messages are dealt with.
//We just send messages to our object.
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	return g_ptrCGraphAppl->GraphicApplicationWndProc(hWnd,message,wParam,lParam);
}