#include "PCH.hpp"
#include "CSystem.hpp"
#include "CModel.hpp"

CSystem* CSystem::ApplicationHandle = nullptr;

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
	Console::Print("Initializing System.");

	if(CSystem::ApplicationHandle != nullptr) {
		return false;
	}
	CSystem::ApplicationHandle = this;
	m_hinstance = GetModuleHandle(NULL);
	m_applicationName = L"RLModelViewer";

	bool result = Console::Initialize();
	if(!result) {
		MessageBox(nullptr, L"Could not initialize the Console.", L"Error", MB_OK);
		return false;
	}

#ifdef RL_DEBUG
	Console::Print("Debug Binaries");
#endif

	m_Window = new (nothrow) CWindow;
	if(m_Window == nullptr) {
		return false;
	}
	result = m_Window->Initialize(m_hinstance, m_applicationName, this);
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
	result = m_Mouse->Initialize(m_Window->GethWnd());
	if(!result) {
		MessageBox(m_Window->GethWnd(), L"Could not initialize the Mouse object.", L"Error", MB_OK);
		return false;
	}

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
	SAFE_SHUTDOWN(m_Mouse);
	//SAFE_SHUTDOWN(m_Input);
	SAFE_DELETE(m_Timer);	
	SAFE_SHUTDOWN(m_Window);

	Console::Shutdown();

	UnregisterClass(m_applicationName, m_hinstance);
	m_hinstance = nullptr;
	ApplicationHandle = nullptr;

	return;
}

void CSystem::Run() {
	MSG msg;
	bool result;

	ZeroMemory(&msg, sizeof(MSG));

	bool done = false;
	while(!done) {
		while(PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE)) {
			if(msg.message == WM_QUIT) {
				done = true;
			}
			DispatchMessage(&msg);
		}
		if(done) {
			break;
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
	
	result = m_Renderer->Frame(m_Mouse->GetX(), m_Mouse->GetY(), m_Mouse->IsLeftButtonPressed(), m_Mouse->IsWheelForward(), m_Mouse->IsWheelBackward());
	if(!result) {
		return false;
	}

	return true;
}

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
			unsigned result = GetRawInputData((HRAWINPUT)lparam, RID_INPUT, lpb, &rawInputSize, sizeof(RAWINPUTHEADER));
			if(result == 0xFFFFFFFF) {
				//bprintf("Error in GetRawInputData - GLE: %u, Size: %u\n", GetLastError(), sizeof(RAWINPUT) );
				Console::Print("Error in GetRawInputData!");
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