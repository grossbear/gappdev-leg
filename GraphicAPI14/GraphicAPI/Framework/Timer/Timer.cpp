#include <windows.h>
#include <mmsystem.h>
#include "Timer.h"

#pragma comment (lib, "winmm.lib")

CTimer::~CTimer()
{
	timeEndPeriod(1);
}

void CTimer::StartTimer()
{
	timeBeginPeriod(1);
	startTime=timeGetTime();	
}

float CTimer::GetTime()
{
	return float(timeGetTime() - startTime)/1000.0f;
}

void CTimer::Update()
{
	prevTime = curTime;
	curTime  = timeGetTime();
}

float CTimer::GetInterval()
{
	return float(curTime - prevTime)/1000.0f;
}