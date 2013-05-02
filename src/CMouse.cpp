#include "Core.hpp"
#include "CMouse.hpp"

namespace {
	bool g_MouseInitialized = false;

	const USHORT HID_USAGE_PAGE_GENERIC = 0x01;
	const USHORT HID_USAGE_GENERIC_MOUSE = 0x02;
}

bool CMouse::CoreStartup() {
	RL_ASSERT(!g_MouseInitialized, "Mouse has already started up!");

	RAWINPUTDEVICE rid[1];
	rid[0].usUsagePage = HID_USAGE_PAGE_GENERIC;
	rid[0].usUsage = HID_USAGE_GENERIC_MOUSE;
	rid[0].dwFlags = 0;
	rid[0].hwndTarget = nullptr;

	if(RegisterRawInputDevices(rid, 1, sizeof(rid[0])) == FALSE) {
		return false;
	}

	g_MouseInitialized = true;
	return true;
}

bool CMouse::CoreShutdown() {
	RL_ASSERT(g_MouseInitialized, "Mouse is already shut down!");
	g_MouseInitialized = false;

	RAWINPUTDEVICE rid[1];
	rid[0].usUsagePage = HID_USAGE_PAGE_GENERIC;
	rid[0].usUsage = HID_USAGE_GENERIC_MOUSE;
	rid[0].dwFlags = RIDEV_REMOVE;
	rid[0].hwndTarget = nullptr;

	if(RegisterRawInputDevices(rid, 1, sizeof(rid[0])) == FALSE) {
		return false;
	}
	
	return true;
}

CMouse::CMouse() : m_Delta(0,0), m_wheelDelta(0) {
	RL_ASSERT(g_MouseInitialized, "You must Startup the mouse subsystem first!");
	ZeroMemory(m_states, sizeof(m_states));
}

CMouse::~CMouse() {
}

void CMouse::OnFrameStart() {
	for(int i = 0; i < Button::TOTAL; ++i) {
		m_states[i].m_wentDown = false;
		m_states[i].m_wentUp = false;
		//m_states[i].m_isPressed = false;
		m_states[i].m_isDoubleClicked = false;
	}
	m_wheelDelta = 0;
	m_Delta.Set(0, 0);
}

void CMouse::Update(RAWINPUT& lpb) {
	if(lpb.data.mouse.usFlags) {
		// Never been set for me, and specs on what most of them
		// actually mean is sorely lacking.  Also, specs erroneously
		// indicate MOUSE_MOVE_RELATIVE is a flag, when it's really
		// 0...
		return;
	}

	Console::Print("X: %d + %d, Y: %d + %d", m_Delta.x, lpb.data.mouse.lLastX, m_Delta.y, lpb.data.mouse.lLastY);
	m_Delta.Set(m_Delta.x + lpb.data.mouse.lLastX, m_Delta.y + lpb.data.mouse.lLastY);

	/*if(lpb.data.mouse.usButtonFlags & RI_MOUSE_LEFT_BUTTON_DOWN) {
		m_states[Button::LEFT].m_wentDown = true;
		m_states[Button::LEFT].m_isPressed = true;
	}

	if(lpb.data.mouse.usButtonFlags & RI_MOUSE_LEFT_BUTTON_UP) {
		m_states[Button::LEFT].m_wentUp = true; //todo: Set m_wentDown to false here?
		m_states[Button::LEFT].m_isPressed = false;
	}

	if(lpb.data.mouse.usButtonFlags & RI_MOUSE_RIGHT_BUTTON_DOWN) {
		m_states[Button::RIGHT].m_wentDown = true;
		m_states[Button::RIGHT].m_isPressed = true;
	}

	if(lpb.data.mouse.usButtonFlags & RI_MOUSE_RIGHT_BUTTON_UP) {
		m_states[Button::RIGHT].m_wentUp = true; //todo: Set m_wentDown to false here?
		m_states[Button::RIGHT].m_isPressed = false;
	}

	if(lpb.data.mouse.usButtonFlags & RI_MOUSE_MIDDLE_BUTTON_DOWN) {
		m_states[Button::MIDDLE].m_wentDown = true;
		m_states[Button::MIDDLE].m_isPressed = true;
	}

	if(lpb.data.mouse.usButtonFlags & RI_MOUSE_MIDDLE_BUTTON_UP) {
		m_states[Button::MIDDLE].m_wentUp = true; //todo: Set m_wentDown to false here?
		m_states[Button::MIDDLE].m_isPressed = false;
	}

	if(lpb.data.mouse.usButtonFlags & RI_MOUSE_BUTTON_4_DOWN) {
		m_states[Button::FOUR].m_wentDown = true;
		m_states[Button::FOUR].m_isPressed = true;
	}

	if(lpb.data.mouse.usButtonFlags & RI_MOUSE_BUTTON_4_UP) {
		m_states[Button::FOUR].m_wentUp = true; //todo: Set m_wentDown to false here?
		m_states[Button::FOUR].m_isPressed = false;
	}
	
	if(lpb.data.mouse.usButtonFlags & RI_MOUSE_BUTTON_5_DOWN) {
		m_states[Button::FIVE].m_wentDown = true;
		m_states[Button::FIVE].m_isPressed = true;
	}

	if(lpb.data.mouse.usButtonFlags & RI_MOUSE_BUTTON_5_UP) {
		m_states[Button::FIVE].m_wentUp = true; //todo: Set m_wentDown to false here?
		m_states[Button::FIVE].m_isPressed = false;
	}*/

	// http://msdn.microsoft.com/en-us/library/windows/desktop/ms645578.aspx
	for(int i = Button::LEFT, buttonflag = RI_MOUSE_LEFT_BUTTON_DOWN; i <= Button::FIVE; ++i) {
		if(lpb.data.mouse.usButtonFlags & buttonflag) { // Down
			m_states[i].m_wentDown = true;
			m_states[i].m_isPressed = true;
		}
		buttonflag <<= 1;
		if(lpb.data.mouse.usButtonFlags & buttonflag) { // Up
			m_states[i].m_wentUp = true; //todo: Set m_wentDown to false here?
			m_states[i].m_isPressed = false;
		}
		buttonflag <<= 1;
		/*if(m_states[i].m_wentDown && m_states[i].m_wentUp) {
			Console::Print("WARNING: MOUSE BUTTON WENT DOWN AND UP IN THE SAME FRAME");
		}*/
	}
	
	if(lpb.data.mouse.usButtonFlags & RI_MOUSE_WHEEL) {
		m_wheelDelta += static_cast<short>(lpb.data.mouse.usButtonData) / WHEEL_DELTA;
	}

	return;
}