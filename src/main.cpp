#include "PCH.hpp"
#include "CSystem.hpp"

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {	
	CSystem* system = new (nothrow) CSystem;
	if(system == nullptr) {
		return 1;
	}

	bool result = system->Initialize();
	if(result) {
		system->Run();
	}

	system->Shutdown();
	delete system;
	return !result;
}