#include "Core.hpp"
#include "CWindow.hpp"

static ConVar g_ScreenWidth("ScreenWidth", "The Width of the rendering window", 1280, 0, INT32_MAX);
static ConVar g_ScreenHeight("ScreenHeight", "The Height of the rendering window", 800, 0, INT32_MAX);
static ConVar g_WindowType("ScreenType", "The Type of the rendering window", 1, 0, 2); // 0 = fullscreen, 1 = windowed, 2 = windowednoborder

namespace {
	LRESULT CALLBACK StaticWndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		CWindow* pParent;

		if(uMsg == WM_CREATE) {
			pParent = (CWindow*)((LPCREATESTRUCT)lParam)->lpCreateParams;
			LONG ret = SetWindowLongPtr(hWnd, GWL_USERDATA, (LONG_PTR)pParent);
		}
		else {
			pParent = (CWindow*)GetWindowLongPtr(hWnd, GWL_USERDATA);
			if(!pParent) {
				return DefWindowProc(hWnd, uMsg, wParam, lParam);
			}
		}

		return pParent->WndProc(hWnd, uMsg, wParam, lParam);
	}
}


CWindow::CWindow() {
}

CWindow::CWindow(const CWindow&) {
}

CWindow::~CWindow() {
}

bool CWindow::Initialize(HINSTANCE hInstance, const wchar_t* wszTitle) {
	Console::Print("Initializing Window.");

	m_Type = g_WindowType;
	WNDCLASSEX wc;
	wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	wc.lpfnWndProc = StaticWndProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = hInstance;
	wc.hIcon = LoadIcon(NULL, IDI_WINLOGO);
	wc.hIconSm = wc.hIcon;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	wc.lpszMenuName = nullptr;
	wc.lpszClassName = wszTitle;
	wc.cbSize = sizeof(WNDCLASSEX);

	RegisterClassEx(&wc);

	if(g_ScreenWidth == 0 || g_ScreenHeight == 0) {
		m_Width = GetSystemMetrics(SM_CXSCREEN);
		m_Height = GetSystemMetrics(SM_CYSCREEN);
		g_ScreenWidth = m_Width;
		g_ScreenHeight = m_Height;
	}
	else {		
		m_Width = g_ScreenWidth;
		m_Height = g_ScreenHeight;
	}

	if(m_Type == 0) {		
		DEVMODE dmScreenSettings;
		memset(&dmScreenSettings, 0, sizeof(dmScreenSettings));
		dmScreenSettings.dmSize = sizeof(dmScreenSettings);
		dmScreenSettings.dmPelsWidth = (unsigned long)m_Width;
		dmScreenSettings.dmPelsHeight = (unsigned long)m_Height;
		dmScreenSettings.dmBitsPerPel = 32;
		dmScreenSettings.dmFields = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;

		ChangeDisplaySettings(&dmScreenSettings, CDS_FULLSCREEN);
		m_PosX = 0;
		m_PosY = 0;
	}
	else {
		m_PosX = (GetSystemMetrics(SM_CXSCREEN) - m_Width) / 2;
		m_PosY = (GetSystemMetrics(SM_CYSCREEN) - m_Height) / 2;
	}

	unsigned long style = WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_POPUP;
	if(m_Type == 1) {
		style = WS_OVERLAPPEDWINDOW | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN;
	}

	m_hWnd = CreateWindowEx(WS_EX_APPWINDOW, wc.lpszClassName, wszTitle, style, m_PosX, m_PosY, m_Width, m_Height,
		nullptr, nullptr, hInstance, this);
	SetForegroundWindow(m_hWnd);
	SetFocus(m_hWnd);

	return true;
}

void CWindow::Shutdown() {
	// If Fullscreen revert to Windowed mode before shutting down.
	if(m_Type == 0) {
		ChangeDisplaySettings(nullptr, 0);
	}
	
	DestroyWindow(m_hWnd);
	m_hWnd = nullptr;
	
	return;
}

CWindow::Notification::Enum CWindow::PumpMessages() const {
	MSG msg = {};
	while(PeekMessage(&msg, m_hWnd, 0, 0, PM_REMOVE)) {
		if(msg.message == WM_QUIT) {
			Console::Print("WM_QUIT");
			return Notification::CLOSE;
		}
		DispatchMessage(&msg);
	}

	return Notification::Enum::NONE;
}

HWND CWindow::GethWnd() {
	return m_hWnd;
}

int CWindow::GetWidth() {
	return m_Width;
}

int CWindow::GetHeight() {
	return m_Height;
}

bool CWindow::GetFullscreen() {
	return m_Type == 0;
}

LRESULT CWindow::WndProc(HWND hWnd, UINT msg, WPARAM wparam, LPARAM lparam) {
	switch(msg) {
	case WM_CLOSE:
		{
			PostQuitMessage(0);
			return 0;
		}
		break;
#if 0
	case WM_INPUT:
		{
			unsigned int rawInputSize = sizeof(RAWINPUT);
			static BYTE lpb[sizeof(RAWINPUT)];
			unsigned int result = GetRawInputData((HRAWINPUT)lparam, RID_INPUT, lpb, &rawInputSize, sizeof(RAWINPUTHEADER));
			if(result == 0xFFFFFFFF) {
				Console::Print("Error in GetRawInputData! - GLE: %u", GetLastError());
				PostQuitMessage(1);
				break;
			}

			if (((RAWINPUT&)lpb).header.dwType == RIM_TYPEMOUSE) {
				m_Mouse->Update((RAWINPUT&)lpb);
			}
		}
		break;
#endif
	default:
		{
			return DefWindowProc(hWnd, msg, wparam, lparam);
		}
	}
	return 0;
}
