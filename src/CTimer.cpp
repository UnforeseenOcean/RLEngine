#include "PCH.hpp"
#include "CTimer.hpp"

TimerClass::TimerClass() {
}

TimerClass::TimerClass(const TimerClass&) {
}

TimerClass::~TimerClass() {
}

bool TimerClass::Initialize() {
	QueryPerformanceFrequency((LARGE_INTEGER*)&m_frequency);
	if(m_frequency == 0) {
		return false;
	}

	m_ticksPerMs = (float)(m_frequency / 1000);

	QueryPerformanceCounter((LARGE_INTEGER*)&m_startTime);

	return true;
}

void TimerClass::Frame() {
	long long currentTime;
	float timeDifference;

	QueryPerformanceCounter((LARGE_INTEGER*)&currentTime);

	timeDifference = (float)(currentTime - m_startTime);

	m_frameTime = timeDifference / m_ticksPerMs;

	m_startTime = currentTime;

	return;
}

float TimerClass::GetTime() {
	return m_frameTime;
}