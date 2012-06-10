#pragma once

class Console {
public:
	static bool Initialize();
	static void Shutdown();

	static void Print(const char* message);
	
protected:
	Console();
	Console(const Console&);
	~Console();

private:
	static void* m_hWindowsConsole;
	static void* m_LogFile;
	static Console* m_Console;
};