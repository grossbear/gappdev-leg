/////////////////////////////////////////////////////////////////////////////////////////////
// WinTimer.h
//
// Windows Time Measure Class
/////////////////////////////////////////////////////////////////////////////////////////////

#ifndef __WINTIMER_H__
#define __WINTIMER_H__

/////////////////////////////////////////////////////////////////////////////////////////////
class CWinTimer
{
public:
    CWinTimer();
    ~CWinTimer();

    void StartTimer();
    void Update();
    float GetTime();
    unsigned int GetTicksNum();
    float GetTimeDiff();
    unsigned int GetTicksDiff();

protected:
    unsigned int m_StartTime;
    unsigned int m_PrevTime;
    unsigned int m_CurrTime;
};
/////////////////////////////////////////////////////////////////////////////////////////////
class CWinHighResTimer
{
public:
    CWinHighResTimer();
    ~CWinHighResTimer();

    void StartTimer();
    void Update();
    double GetTime();
    double GetTimeDiff();
protected:
    __int64 m_StartTime;
    __int64 m_PrevTime;
    __int64 m_CurrTime;
    __int64 m_HighResFreq;
};

/////////////////////////////////////////////////////////////////////////////////////////////

#endif //__WINTIMER_H__