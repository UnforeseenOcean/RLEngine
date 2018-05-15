#pragma once

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

class CMouse;

class Window {
public:
	// Starts the window facility, and registers the window class.
	// This is called automatically when starting the Core module.
	static bool CoreStartup();

	// Shuts down the window facility, and unregisters the window class.
	// This is called automatically when shutting down the Core module.
	static bool CoreShutdown();

public:
	enum class Mode {
		FULLSCREEN,
		WINDOWED,
		NOBORDER
	};

	enum class Notification {
		NONE,
		CLOSE,
	};

	Window();
	~Window();

	bool Initialize(const wchar_t* const name, int x, int y, unsigned int width, unsigned int height, Mode mode);
	void Shutdown();

	void SetMouse(CMouse* mouse);

	Notification PumpMessages() const;

	inline HWND GethWnd() const;
	inline int GetWidth() const;
	inline int GetHeight() const;
	inline bool GetFullscreen() const;

	// For Internal Use Only!
	LRESULT WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
private:
	Window(const Window&);

	HWND m_hWnd;
	Mode m_Mode;
	int m_Width;
	int m_Height;
	int m_PosX;
	int m_PosY;

	CMouse* m_Mouse;
};

#include "Window.inl"