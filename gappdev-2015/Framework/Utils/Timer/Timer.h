/////////////////////////////////////////////////////////////////////////////////////////////
// Timer.h
//
// Time Measure Class
/////////////////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////////////////
#ifndef __TIMER_H__
#define __TIMER_H__

#ifdef WIN32
#include "sys/WinTimer.h"
typedef CWinTimer CTimer;
typedef CWinHighResTimer CHighResTimer;
#else
#error "For non win system timer class not defined"
#endif 

#endif //__TIMER_H__
/////////////////////////////////////////////////////////////////////////////////////////////