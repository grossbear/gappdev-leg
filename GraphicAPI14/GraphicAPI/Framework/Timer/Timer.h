
class CTimer
{
protected:
	unsigned int startTime;
	unsigned int prevTime;
	unsigned int curTime;
public:
	CTimer(): startTime(0),prevTime(0), curTime(0) {}
	virtual ~CTimer();
	void StartTimer();
	float GetTime();
	void Update();
	float GetInterval();
};