#include "PCH.hpp"
#include "Console.hpp"

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <cstdio>

HANDLE Console::m_hWindowsConsole = INVALID_HANDLE_VALUE;
HANDLE Console::m_LogFile = INVALID_HANDLE_VALUE;
Console* Console::m_Console = nullptr;

Console::Console() {
}

Console::Console(const Console&) {
}

Console::~Console() {
}

bool Console::Initialize() {
	if(m_Console != nullptr) {
		return false;
	}
	
	m_Console = new (nothrow) Console;
	if(m_Console == nullptr) {
		return false;
	}

	bool result = AllocConsole() != 0;
	if(!result) {
		delete m_Console;
		m_Console = nullptr;
		m_hWindowsConsole = nullptr;
		return false;
	}

	m_hWindowsConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	if(m_hWindowsConsole == INVALID_HANDLE_VALUE) {
		FreeConsole();
		delete m_Console;
		m_Console = nullptr;
		m_hWindowsConsole = nullptr;
		return false;
	}

	m_LogFile = CreateFileA("lastrun.log", GENERIC_WRITE, 0, nullptr, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL | FILE_FLAG_SEQUENTIAL_SCAN, nullptr);
	if (m_LogFile == INVALID_HANDLE_VALUE) {
		FreeConsole();
		delete m_Console;
		m_Console = nullptr;
		m_hWindowsConsole = nullptr;
		return false;
	}

	return true;
}

void Console::Shutdown() {
	if(m_Console != nullptr) {
		FreeConsole();
		delete m_Console;
		m_Console = nullptr;
		m_hWindowsConsole = nullptr;

		CloseHandle(m_LogFile);
		m_LogFile = INVALID_HANDLE_VALUE;
	}

	return;
}

void Console::Print(const char* message) {
	if(m_Console == nullptr) {
		RL_DEBUGBREAK();
		return;
	}

	WriteConsoleA(m_hWindowsConsole, message, strnlen_s(message, 1024), nullptr, nullptr);
	WriteConsoleA(m_hWindowsConsole, "\n", 1, nullptr, nullptr);
	//printf("%s\n", message);

	DWORD nBytesToWrite = 0;
	//TODO: Add Time to this.
	WriteFile(m_LogFile, message, strnlen_s(message, 1024), &nBytesToWrite, NULL);
	WriteFile(m_LogFile, "\r\n", 2, &nBytesToWrite, NULL);
	
	return;
}