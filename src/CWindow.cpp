#include "PCH.hpp"
#include "CWindow.hpp"
#include "CSystem.hpp"

static ConVar g_ScreenWidth("ScreenWidth", "The Width of the rendering window", 1280, 0, INT32_MAX);
static ConVar g_ScreenHeight("ScreenHeight", "The Height of the rendering window", 800, 0, INT32_MAX);
static ConVar g_WindowType("ScreenType", "The Type of the rendering window", 1, 0, 2); // 0 = fullscreen, 1 = windowed, 2 = windowednoborder

CSystem* CWindow::Parent = nullptr;

CWindow::CWindow() {
}

CWindow::CWindow(const CWindow&) {
}

CWindow::~CWindow() {
}

bool CWindow::Initialize(HINSTANCE hInstance, const wchar_t* wszTitle, CSystem* parent) {
	Console::Print("Initializing Window.");

	Parent = parent;
	m_Type = g_WindowType;
	WNDCLASSEX wc;
	wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	wc.lpfnWndProc = WndProc;
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
		nullptr, nullptr, hInstance, nullptr);
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

LRESULT CALLBACK CWindow::WndProc(HWND hwnd, UINT umessage, WPARAM wparam, LPARAM lparam) {
	return CWindow::Parent->MessageHandler(hwnd, umessage, wparam, lparam);
}