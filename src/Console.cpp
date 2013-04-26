#include "Core.hpp"
#include "Console.hpp"

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <cstdio>
#include <chrono>
#include <ctime>

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

	Console::Print("Console Initialized.");

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

void Console::Print(const char* const format, ...) {
	if(m_Console == nullptr) {
		RL_DEBUGBREAK();
		return;
	}

	char buffer[1024];
	DWORD nBytesToWrite = 0;

	std::time_t t = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
	strftime(buffer, sizeof(buffer), "%X", gmtime(&t));
	WriteConsoleA(m_hWindowsConsole, buffer, strnlen_s(buffer, 1024), nullptr, nullptr);
	WriteConsoleA(m_hWindowsConsole, " ", 1, nullptr, nullptr);

	WriteFile(m_LogFile, buffer, strnlen_s(buffer, 1024), &nBytesToWrite, NULL);
	WriteFile(m_LogFile, " ", 1, &nBytesToWrite, NULL);


	va_list val;
	va_start(val, format);
	//int ret =
	vsprintf_s(buffer, format, val); //todo: Overflow protection, look at doom3's code.
	va_end(val);

	WriteConsoleA(m_hWindowsConsole, buffer, strnlen_s(buffer, 1024), nullptr, nullptr);
	WriteConsoleA(m_hWindowsConsole, "\n", 1, nullptr, nullptr);

	WriteFile(m_LogFile, buffer, strnlen_s(buffer, 1024), &nBytesToWrite, NULL);
	WriteFile(m_LogFile, "\n", 1, &nBytesToWrite, NULL);
	
	return;
}