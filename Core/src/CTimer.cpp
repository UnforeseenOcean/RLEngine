#include "Core.hpp"
#include "CTimer.hpp"

CTimer::CTimer() {
}

CTimer::CTimer(const CTimer&) {
}

CTimer::~CTimer() {
}

bool CTimer::Initialize() {
	Console::Print("Initializing Timer.");

	QueryPerformanceFrequency((LARGE_INTEGER*)&m_frequency);
	if(m_frequency == 0) {
		return false;
	}

	m_ticksPerMs = (float)(m_frequency / 1000);

	QueryPerformanceCounter((LARGE_INTEGER*)&m_startTime);

	return true;
}

void CTimer::Frame() {
	long long currentTime;
	float timeDifference;

	QueryPerformanceCounter((LARGE_INTEGER*)&currentTime);

	timeDifference = (float)(currentTime - m_startTime);

	m_frameTime = timeDifference / m_ticksPerMs;

	m_startTime = currentTime;

	return;
}

float CTimer::GetTime() {
	return m_frameTime;
}