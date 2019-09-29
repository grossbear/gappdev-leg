//**********************************************************************
//Main.h
//
//This header file contains prototipes of the basic functions 
//called by the OS when application starts.
//***********************************************************************

//Header file for windows.
#include <windows.h>

//This is entry point of Window Application.
//Mostly this function is used for calling our window routine, deal with window messages and
//watch for user interection. For our purposes in this function we creates object of the
//CGraphicApplication class and calls class public functions. 
int WINAPI WinMain(HINSTANCE hInstance,HINSTANCE hPrevInstance,PSTR szCmdLine,int iCmdShow);

//This is declaration of the function where all window messages are dealt with.
//For our purposes we just send messages to an object of our application.
//So in the following line we define WndProc so the other function can make reference to WndProc.
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);