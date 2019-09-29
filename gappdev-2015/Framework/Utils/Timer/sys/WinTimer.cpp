/////////////////////////////////////////////////////////////////////////////////////////////
// WinTimer.cpp
//
// Windows Time Measure Class
/////////////////////////////////////////////////////////////////////////////////////////////
#include <windows.h>
#include <MMSystem.h>
#include "WinTimer.h"

#pragma comment (lib, "winmm.lib")
/////////////////////////////////////////////////////////////////////////////////////////////
CWinTimer::CWinTimer()
{
    m_StartTime = 0;
    m_PrevTime = 0;
    m_CurrTime = 0;
}
/////////////////////////////////////////////////////////////////////////////////////////////
CWinTimer::~CWinTimer()
{
    timeEndPeriod(1);
}
/////////////////////////////////////////////////////////////////////////////////////////////
void CWinTimer::StartTimer()
{
    timeBeginPeriod(1);
    m_StartTime = timeGetTime();
    m_PrevTime = m_StartTime;
    m_CurrTime = m_StartTime;
}
/////////////////////////////////////////////////////////////////////////////////////////////
void CWinTimer::Update()
{
    m_PrevTime = m_CurrTime;
    m_CurrTime = timeGetTime();
}
/////////////////////////////////////////////////////////////////////////////////////////////
float CWinTimer::GetTime()
{
    unsigned int currtickscnt = timeGetTime();
    return float(currtickscnt - m_StartTime)/1000.f;
}
/////////////////////////////////////////////////////////////////////////////////////////////
unsigned int CWinTimer::GetTicksNum()
{
    unsigned int currtickscnt = timeGetTime();
    return currtickscnt - m_StartTime;
}
/////////////////////////////////////////////////////////////////////////////////////////////
float CWinTimer::GetTimeDiff()
{
    float timediff = (m_CurrTime - m_PrevTime)/1000.f;
    return timediff;
}
/////////////////////////////////////////////////////////////////////////////////////////////
unsigned int CWinTimer::GetTicksDiff()
{
    unsigned int tickdiff = m_CurrTime - m_PrevTime;
    return tickdiff;
}
/////////////////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////////////////
CWinHighResTimer::CWinHighResTimer():m_StartTime(0),m_PrevTime(0),m_CurrTime(0),m_HighResFreq(0)
{
}
/////////////////////////////////////////////////////////////////////////////////////////////
CWinHighResTimer::~CWinHighResTimer()
{
}
/////////////////////////////////////////////////////////////////////////////////////////////
void CWinHighResTimer::StartTimer()
{
    QueryPerformanceFrequency((LARGE_INTEGER*)&m_HighResFreq);
    QueryPerformanceCounter((LARGE_INTEGER*)&m_StartTime);
    m_CurrTime = m_StartTime;
    m_PrevTime = m_StartTime;
}
/////////////////////////////////////////////////////////////////////////////////////////////
void CWinHighResTimer::Update()
{
    m_PrevTime = m_CurrTime;
    QueryPerformanceCounter((LARGE_INTEGER*)&m_CurrTime);
}
/////////////////////////////////////////////////////////////////////////////////////////////
double CWinHighResTimer::GetTime()
{
    __int64 currtime;
    QueryPerformanceCounter((LARGE_INTEGER*)&currtime);
    double highfreqtime = (double)(currtime - m_StartTime)/(double)m_HighResFreq*1000000.0;
    return highfreqtime;
}
/////////////////////////////////////////////////////////////////////////////////////////////
double CWinHighResTimer::GetTimeDiff()
{
    double highfreqtime = (double)(m_CurrTime - m_PrevTime)/(double)m_HighResFreq*1000000.0;
    return highfreqtime;
}
/////////////////////////////////////////////////////////////////////////////////////////////
