#pragma once

#pragma message("Creating precompiled header.\n")

#include <new>
#include <cstdint>

using std::nothrow;

#include "Assertion.hpp"
#include "ConVar.hpp"
#include "Console.hpp"


#ifdef _DEBUG
	#define RL_DEBUG 1
#endif

#ifdef RL_DEBUG
	#define RL_DEBUGBREAK() __debugbreak()
#else
	#define RL_DEBUGBREAK() ((void)0)
#endif

#define SAFE_DELETE(__ptr__) RL_ASSERT(__ptr__ != nullptr, "Double-Deleted a pointer!"); delete __ptr__; __ptr__ = nullptr
#define SAFE_DELETE_ARRAY(__ptr__) RL_ASSERT(__ptr__ != nullptr, "Double-Deleted a pointer!"); delete[] __ptr__; __ptr__ = nullptr
#define SAFE_SHUTDOWN(__ptr__) RL_ASSERT(__ptr__ != nullptr, "Double-Deleted a pointer!"); __ptr__->Shutdown(); delete __ptr__; __ptr__ = nullptr
#define SAFE_RELEASE(__ptr__) RL_ASSERT(__ptr__ != nullptr, "Double-Deleted a pointer!"); __ptr__->Release(); delete __ptr__; __ptr__ = nullptr