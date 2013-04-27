#pragma once

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

class CSystem;

class CWindow {
public:
	struct Notification
	{
		enum Enum
		{
			CLOSE,
			NONE
		};
	};

	CWindow();
	CWindow(const CWindow&);
	~CWindow();

	bool Initialize(HINSTANCE hInstance, const wchar_t* wszTitle);
	void Shutdown();

	Notification::Enum PumpMessages() const;

	HWND GethWnd();
	int GetWidth();
	int GetHeight();
	bool GetFullscreen();

	// For Internal Use Only!
	LRESULT WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
private:
	HWND m_hWnd;
	int m_Type;
	int m_Width;
	int m_Height;
	int m_PosX;
	int m_PosY;
};