#pragma once

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

class TimerClass {
public:
	TimerClass();
	TimerClass(const TimerClass&);
	~TimerClass();

	bool Initialize();
	void Frame();

	float GetTime();

private:
	long long m_startTime;
	long long m_frequency;
	float m_ticksPerMs;
	float m_frameTime;
};