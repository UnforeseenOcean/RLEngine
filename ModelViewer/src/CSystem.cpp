#include "ModelViewer.hpp"
#include "CSystem.hpp"
#include "CModel.hpp"

static ConVar g_ScreenWidth("ScreenWidth", "The Width of the rendering window", 1280, 0, INT32_MAX);
static ConVar g_ScreenHeight("ScreenHeight", "The Height of the rendering window", 800, 0, INT32_MAX);

CSystem::CSystem() {
	m_Input = nullptr;
	m_Renderer = nullptr;
	m_Timer = nullptr;
	m_Mouse = nullptr;
}

CSystem::CSystem(const CSystem&) {
}

CSystem::~CSystem() {
}

bool CSystem::Initialize() {
	//todo: assert once only.
	m_hinstance = GetModuleHandle(NULL);
	m_applicationName = L"RLModelViewer";

	core::ModuleStartup();


	bool result = Console::Initialize();
	if(!result) {
		MessageBox(nullptr, L"Could not initialize the Console.", L"Error", MB_OK);
		return false;
	}

#ifdef RL_DEBUG
	Console::Print("Debug Binaries");
#endif

	m_Window = new (nothrow) Window;
	if(m_Window == nullptr) {
		return false;
	}
	result = m_Window->Initialize(m_applicationName, 0, 0, g_ScreenWidth, g_ScreenHeight, Window::Mode::WINDOWED);
	if(!result) {
		MessageBox(nullptr, L"Could not initialize the window object.", L"Error", MB_OK);
		return false;
	}

	m_Timer = new CTimer;
	if(m_Timer == nullptr) {
		return false;
	}
	result = m_Timer->Initialize();
	if(!result) {
		MessageBox(m_Window->GethWnd(), L"Could not initialize the Timer object.", L"Error", MB_OK);
		return false;
	}

	/*m_Input = new CInput;
	if(m_Input == nullptr) {
		return false;
	}
	result = m_Input->Initialize(m_hinstance, m_Window->GethWnd(), m_Window->GetWidth(), m_Window->GetHeight());
	if(!result) {
		MessageBox(m_Window->GethWnd(), L"Could not initialize the input object.", L"Error", MB_OK);
		return false;
	}*/

	m_Mouse = new (nothrow) CMouse;
	if(m_Mouse == nullptr) {
		return false;
	}
	m_Window->SetMouse(m_Mouse);

	m_Renderer = new CRenderer;
	if(m_Renderer == nullptr) {
		return false;
	}
	result = m_Renderer->Initialize(m_Window->GethWnd(), m_Window->GetWidth(), m_Window->GetHeight(), m_Window->GetFullscreen());
	if(!result) {
		return false;
	}
			
	int model = m_Renderer->AddModel("data/models/podium.obj");
	if(model == -1) {
		MessageBox(m_Window->GethWnd(), L"Could not initialize data/models/podium.obj.", L"Error", MB_OK);
		return false;
	}
	model = m_Renderer->AddModel("data/models/piano.obj");
	if(model == -1) {
		MessageBox(m_Window->GethWnd(), L"Could not initialize data/models/podium.obj.", L"Error", MB_OK);
		return false;
	}
	m_Renderer->GetModelManager()->GetModel(model)->SetPosition(DirectX::XMFLOAT3(0.0f, 6.0f, 0.0f));
	m_Renderer->GetModelManager()->GetModel(model)->SetRotation(DirectX::XMFLOAT3(0.0f, 45.0f, 0.0f));
		
	return true;
}

void CSystem::Shutdown() {
	SAFE_SHUTDOWN(m_Renderer);
	//SAFE_SHUTDOWN(m_Input);
	SAFE_DELETE(m_Timer);	
	SAFE_SHUTDOWN(m_Window);

	Console::Shutdown();

	core::ModuleShutdown();

	m_hinstance = nullptr;

	return;
}

void CSystem::Run() {
	MSG msg = {};
	bool result;
	bool done = false;
	while(!done) {

		m_Mouse->OnFrameStart();
		if(m_Window->PumpMessages() == Window::Notification::CLOSE) {
			done = true;
		}

		result = Frame();
		if(!result) {
			MessageBox(m_Window->GethWnd(), L"Frame Processing Failed", L"Error", MB_OK);
			done = true;
		}

		//if(m_Input->IsEscapePressed()) {
		//	done = true;
		//}
	}
}

bool CSystem::Frame() {
	m_Timer->Frame();

	bool result;/* = m_Input->Frame();
	if(!result) {
		return false;
	}*/

	if(m_Mouse->WentDown(CMouse::Button::LEFT)) {
		Console::Print("LMB Went Down");
	}
	if(m_Mouse->WentDown(CMouse::Button::MIDDLE)) {
		Console::Print("MMB Went Down");
	}
	if(m_Mouse->WentDown(CMouse::Button::RIGHT)) {
		Console::Print("RMB Went Down");
	}
	if(m_Mouse->WentDown(CMouse::Button::FOUR)) {
		Console::Print("4MB Went Down");
	}
	if(m_Mouse->WentDown(CMouse::Button::FIVE)) {
		Console::Print("5MB Went Down");
	}

	if(m_Mouse->IsPressed(CMouse::Button::LEFT)) {
		Console::Print("LMB is Pressed");
	}
	if(m_Mouse->IsPressed(CMouse::Button::MIDDLE)) {
		Console::Print("MMB is Pressed");
	}
	if(m_Mouse->IsPressed(CMouse::Button::RIGHT)) {
		Console::Print("RMB is Pressed");
	}
	if(m_Mouse->IsPressed(CMouse::Button::FOUR)) {
		Console::Print("4MB is Pressed");
	}
	if(m_Mouse->IsPressed(CMouse::Button::FIVE)) {
		Console::Print("5MB is Pressed");
	}

	if(m_Mouse->WentUp(CMouse::Button::LEFT)) {
		Console::Print("LMB Went Up");
	}
	if(m_Mouse->WentUp(CMouse::Button::MIDDLE)) {
		Console::Print("MMB Went Up");
	}
	if(m_Mouse->WentUp(CMouse::Button::RIGHT)) {
		Console::Print("RMB Went Up");
	}
	if(m_Mouse->WentUp(CMouse::Button::FOUR)) {
		Console::Print("4MB Went Up");
	}
	if(m_Mouse->WentUp(CMouse::Button::FIVE)) {
		Console::Print("5MB Went Up");
	}

	//Console::Print("X: %d, Y: %d", m_Mouse->GetDelta().x, m_Mouse->GetDelta().y);


	result = m_Renderer->Frame(m_Mouse);
	if(!result) {
		return false;
	}

	return true;
}

#if 0
LRESULT CSystem::MessageHandler(HWND hwnd, UINT umsg, WPARAM wparam, LPARAM lparam) {
	switch(umsg) {
		case WM_DESTROY:
		{
			PostQuitMessage(0);
			return 0;
		}
			break;
		case WM_CLOSE:
		{
			PostQuitMessage(0);
			return 0;
		}
			break;
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
		default:
		{
			return DefWindowProc(hwnd, umsg, wparam, lparam);
		}
	}
	return 0;
}
#endif