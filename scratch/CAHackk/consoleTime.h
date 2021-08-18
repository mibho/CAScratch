#ifndef _TIMERS_
#define _TIMERS_

#pragma comment (lib,"winmm.lib")

#include <Windows.h>
#include <MMSystem.h>

class CTimer
{
public:
	CTimer(void) { dwStop = 0; }
	~CTimer(void) { }

	void Start(float fSec){	dwStop = (DWORD)(fSec * 1) + timeGetTime();}

	void Stop(void)	{ dwStop = 0;}

	float TimeLeft(void)
	{
		if (Running())
			return (((float)(dwStop - timeGetTime()) / 1));
		return 0;
	}

	bool Running(void){	return (dwStop > timeGetTime());}

protected:
	DWORD dwStop;
};

#endif