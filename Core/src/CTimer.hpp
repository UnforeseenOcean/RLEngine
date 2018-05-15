#pragma once

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

class CTimer {
public:
	CTimer();
	CTimer(const CTimer&);
	~CTimer();

	bool Initialize();
	void Frame();

	float GetTime();

private:
	long long m_startTime;
	long long m_frequency;
	float m_ticksPerMs;
	float m_frameTime;
};