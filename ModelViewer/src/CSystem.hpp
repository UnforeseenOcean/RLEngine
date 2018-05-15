#pragma once

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

#include "Core/Window.hpp"
#include "CInput.hpp"
#include "CRenderer.hpp"
#include "CTimer.hpp"
#include "CMouse.hpp"
#include "CModelManager.hpp"

class CSystem {
public:
	CSystem();
	CSystem(const CSystem&);
	~CSystem();

	bool Initialize();
	void Shutdown();
	void Run();

private:
	bool Frame();

	LPCWSTR m_applicationName;
	HINSTANCE m_hinstance;
	Window* m_Window;
	CTimer* m_Timer;
	CRenderer* m_Renderer;
	CInput* m_Input;
	CMouse* m_Mouse;
};
