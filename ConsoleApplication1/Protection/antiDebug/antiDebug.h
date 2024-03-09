#pragma once
#include "../xorstr.hpp"

#define EVENT_SELFDBG_EVENT_NAME xorstr_(L"SelfDebugging")
#define NtCurrentThread ((HANDLE)-2)

#include <Windows.h>

namespace Debug {
	bool IsDebugged();
	bool IsHooked();
	bool openProcessDebug();
	bool closeHandleDebug();
	bool loadLibraryDebug();
	bool IsProcessRunning(DWORD pid);
	bool checkCrackProcessIsRunning();

	BOOL CALLBACK EnumWindowsProc(HWND hwnd, LPARAM lParam);
	
	void checkALL();

	inline constinit bool bDebug{ false };
	inline int processCount{ 0 };
	inline int processCountLimit{ 11 };
}



