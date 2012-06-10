#pragma once

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

class CSystem;

class CWindow {
public:
	CWindow();
	CWindow(const CWindow&);
	~CWindow();

	bool Initialize(HINSTANCE hInstance, const wchar_t* wszTitle, CSystem*);
	void Shutdown();

	HWND GethWnd();
	int GetWidth();
	int GetHeight();

private:
	HWND m_hWnd;
	int m_Type;
	int m_Width;
	int m_Height;
	int m_PosX;
	int m_PosY;

	static CSystem* Parent;
	static LRESULT CALLBACK WndProc(HWND hwnd, UINT umessage, WPARAM wparam, LPARAM lparam);
};