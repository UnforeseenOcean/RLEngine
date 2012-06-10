#pragma once
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

class CMouse {
public:
	CMouse();
	CMouse(const CMouse&);
	~CMouse();

	bool Initialize(HWND hwnd);
	void Shutdown();

	void Update(RAWINPUT& lpb);

	bool IsLeftButtonPressed();
	bool IsWheelForward();
	bool IsWheelBackward();
	int GetX();
	int GetY();

private:
	RAWINPUTDEVICE m_RawDevice[1];
	bool m_LeftButtonDown;
	bool m_WheelForward;
	bool m_WheelBackward;
	long m_x;
	long m_y;
};