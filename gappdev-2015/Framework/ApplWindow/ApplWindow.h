/////////////////////////////////////////////////////////////////////////////////////////////
// ApplWindow.h
//
/////////////////////////////////////////////////////////////////////////////////////////////

#ifdef FREEGLUT
#include "GlutAppWindow.h"

#elif _WIN32
#include "Win32AppWindow.h"
typedef CWin32AppWindow CApplWindow;
#else
#error "Application not defined"

#endif