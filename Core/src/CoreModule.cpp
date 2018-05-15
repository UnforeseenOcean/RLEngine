#include "Core.hpp"
#include "Core/CoreModule.hpp"
#include "Core/Window.hpp"
#include "CMouse.hpp"

namespace core {

namespace {
	bool g_CoreInitialized = false;
}

bool ModuleStartup() {
	RL_ASSERT(!g_CoreInitialized, "Core Module has already started up!");

	bool ret = Window::CoreStartup();
	if(!ret) { return false; }

	ret = CMouse::CoreStartup();
	if(!ret) { return false; }

	g_CoreInitialized = true;
	return true;
}

bool ModuleShutdown() {
	RL_ASSERT(g_CoreInitialized, "Core Module is not started up!");
	g_CoreInitialized = false;
	
	bool ret = Window::CoreShutdown();
	if(!ret) { return false; }

	CMouse::CoreShutdown();
	if(!ret) { return false; }

	return ret;
}

}