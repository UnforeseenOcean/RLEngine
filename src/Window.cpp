#include "Core.hpp"
#include "Core/Window.hpp"
#include "CMouse.hpp"

namespace {
	bool g_WindowInitialized = false;

	LRESULT CALLBACK OnWindowMessage(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
		Window* window = reinterpret_cast<Window*>(GetWindowLongPtr(hWnd, GWL_USERDATA));
		if(window) {
			return window->WndProc(hWnd, uMsg, wParam, lParam);
		}
		else {
			return DefWindowProc(hWnd, uMsg, wParam, lParam);
		}
	}

	LRESULT CALLBACK StaticWindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
		if(uMsg == WM_CREATE) {
			Window* window = reinterpret_cast<Window*>(reinterpret_cast<LPCREATESTRUCT>(lParam)->lpCreateParams);
			SetWindowLongPtr(hWnd, GWL_USERDATA, reinterpret_cast<LONG_PTR>(window));
			SetWindowLongPtr(hWnd, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(OnWindowMessage));
			return window->WndProc(hWnd, uMsg, wParam, lParam);
		}

		return DefWindowProc(hWnd, uMsg, wParam, lParam);
	}
}

bool Window::CoreStartup() {
	RL_ASSERT(!g_WindowInitialized, "Window has already started up!");
	
	WNDCLASSEX wc;
	wc.cbSize = sizeof(WNDCLASSEX);
	wc.style = 0;//CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	wc.lpfnWndProc = StaticWindowProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = GetModuleHandle(nullptr);
	wc.hIcon = LoadIcon(NULL, IDI_WINLOGO);
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	wc.lpszMenuName = nullptr;
	wc.lpszClassName = L"RLEngine";
	wc.hIconSm = wc.hIcon;

	ATOM ret = RegisterClassEx(&wc);
	if(ret == 0) { return false; }

	g_WindowInitialized = true;
	return true;
}

bool Window::CoreShutdown() {
	RL_ASSERT(g_WindowInitialized, "Window is already shut down!");
	g_WindowInitialized = false;
	return UnregisterClass(L"RLEngine", GetModuleHandle(nullptr)) != FALSE;
}

Window::Window() {
}

Window::Window(const Window&) {
}

Window::~Window() {
}

bool Window::Initialize(const wchar_t* const name, int x, int y, unsigned int width, unsigned int height, Mode mode) {
	RL_ASSERT(g_WindowInitialized, "Window not started!");
	m_Mode = mode;

	if(width == 0 || height == 0) {
		m_Width = GetSystemMetrics(SM_CXSCREEN);
		m_Height = GetSystemMetrics(SM_CYSCREEN);
		//g_ScreenWidth = m_Width;
		//g_ScreenHeight = m_Height;
	}
	else {
		m_Width = width;
		m_Height = height;
	}

	if(m_Mode == Mode::FULLSCREEN) {		
		DEVMODE dmScreenSettings = {};
		dmScreenSettings.dmSize = sizeof(dmScreenSettings);
		dmScreenSettings.dmPelsWidth = static_cast<unsigned long>(m_Width);
		dmScreenSettings.dmPelsHeight = static_cast<unsigned long>(m_Height);
		dmScreenSettings.dmBitsPerPel = 32;
		dmScreenSettings.dmFields = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;

		ChangeDisplaySettings(&dmScreenSettings, CDS_FULLSCREEN);
		m_PosX = 0;
		m_PosY = 0;
	}
	else {
		m_PosX = x;
		m_PosY = y;
	}

	unsigned long style = WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_POPUP;
	if(m_Mode == Mode::WINDOWED) {
		style = WS_OVERLAPPEDWINDOW | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN;
	}

	m_hWnd = CreateWindowEx(WS_EX_APPWINDOW, L"RLEngine", name, style, m_PosX, m_PosY, m_Width, m_Height,
		nullptr, nullptr, GetModuleHandle(nullptr), this);
	//SetForegroundWindow(m_hWnd);
	//SetFocus(m_hWnd);

	return true;
}

void Window::Shutdown() {
	// If Fullscreen revert to Windowed mode before shutting down.
	if(m_Mode == Mode::FULLSCREEN) {
		ChangeDisplaySettings(nullptr, 0);
	}
	
	DestroyWindow(m_hWnd);
	m_hWnd = nullptr;
	
	return;
}

void Window::SetMouse(CMouse* mouse) {
	m_Mouse = mouse;
}

Window::Notification Window::PumpMessages() const {
	MSG msg = {};
	while(PeekMessage(&msg, m_hWnd, 0, 0, PM_REMOVE)) {
		if(msg.message == WM_CLOSE) {
			return Notification::CLOSE;
		}
		DispatchMessage(&msg);
	}

	return Notification::NONE;
}

LRESULT Window::WndProc(HWND hWnd, UINT msg, WPARAM wparam, LPARAM lparam) {
	switch(msg) {
	case WM_CLOSE:
		{
			// Forward the message to PeekMessage. Windows seems to send this
			// directly to WndProc rather than sticking it in the queue.
			PostMessage(hWnd, WM_CLOSE, wparam, lparam);
			return 0;
		}
		break;
#if 1
	case WM_INPUT:
		{
			if(!m_Mouse)
				break;
			unsigned int rawInputSize = sizeof(RAWINPUT);
			static BYTE lpb[sizeof(RAWINPUT)];
			uint32_t result = GetRawInputData((HRAWINPUT)lparam, RID_INPUT, lpb, &rawInputSize, sizeof(RAWINPUTHEADER));
			if(result == -1) {
				Console::Print("Error in GetRawInputData! - GLE: %u", GetLastError());
				break;
			}

			if (((RAWINPUT&)lpb).header.dwType == RIM_TYPEMOUSE) {
				m_Mouse->Update((RAWINPUT&)lpb);
			}
		}
		break;
#endif
	}
	return DefWindowProc(hWnd, msg, wparam, lparam);
}
