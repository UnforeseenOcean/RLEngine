#include "Core.hpp"
#include "CMouse.hpp"

#ifndef HID_USAGE_PAGE_GENERIC
	#define HID_USAGE_PAGE_GENERIC ((const USHORT)0x01)
#endif
#ifndef HID_USAGE_GENERIC_MOUSE
	#define HID_USAGE_GENERIC_MOUSE ((const USHORT)0x02)
#endif

CMouse::CMouse() {
	m_LeftButtonDown = false;
	m_WheelForward = false;
	m_WheelBackward = false;
	m_x = 0;
	m_y = 0;
}

CMouse::CMouse(const CMouse&) {
}

CMouse::~CMouse() {
}

bool CMouse::Initialize(HWND hwnd) {
	Console::Print("Initializing Raw Mouse Input.");

	m_RawDevice[0].usUsagePage = 0x01;
	m_RawDevice[0].usUsage = HID_USAGE_GENERIC_MOUSE;
	m_RawDevice[0].dwFlags = 0;
	m_RawDevice[0].hwndTarget = hwnd;

	if (RegisterRawInputDevices(m_RawDevice, 1, sizeof(m_RawDevice[0])) == FALSE) {
		Console::Print("Error in CMouse::Initialize RegisterRawInputDevices! - GLE: %u", GetLastError());
		return false;
	}

	return true;
}

void CMouse::Shutdown() {
	return;
}

void CMouse::Update(RAWINPUT& lpb) {
	if(lpb.data.mouse.usButtonFlags & RI_MOUSE_LEFT_BUTTON_DOWN) {
		m_LeftButtonDown = true;
	}

	if(lpb.data.mouse.usButtonFlags & RI_MOUSE_LEFT_BUTTON_UP) {
		m_LeftButtonDown = false;
	}

	if(lpb.data.mouse.usButtonFlags & RI_MOUSE_WHEEL) {
		if(lpb.data.mouse.usButtonData == 120) { //down: 65416 up: 120			
			m_WheelForward = true;
			m_WheelBackward = false;
		}
		else if(lpb.data.mouse.usButtonData == 65416) {
			m_WheelForward = false;
			m_WheelBackward = true;
		}
		else {
			Console::Print("MouseWheel Data was not 120 or 65416!");
		}
	}

	if(lpb.data.mouse.usFlags & MOUSE_ATTRIBUTES_CHANGED) {
		Console::Print("Mouse Attributes Changed!");
	}

	m_x += lpb.data.mouse.lLastX;
	m_y += lpb.data.mouse.lLastY;

	return;
}

bool CMouse::IsLeftButtonPressed() {
	return m_LeftButtonDown;
}

bool CMouse::IsWheelForward() {
	bool ret = m_WheelForward;	
	m_WheelForward = false;
	return ret;
}

bool CMouse::IsWheelBackward() {
	bool ret = m_WheelBackward;
	m_WheelBackward = false;
	return ret;
}

int CMouse::GetX() {
	return m_x;
}

int CMouse::GetY() {
	return m_y;
}