#include "antiDebug.h"

#include <Windows.h>
#include <iostream>
#include <winternl.h>
#include <TlHelp32.h>

#include "../xorstr.hpp"
#include "../../Connection/connection.h"



bool Debug::IsDebugged()
{
	WCHAR wszFilePath[MAX_PATH], wszCmdLine[MAX_PATH];
	LPSTARTUPINFOW si;
	ZeroMemory(&si, sizeof(si));

	PROCESS_INFORMATION pi;
	HANDLE hDbgEvent;

	hDbgEvent = CreateEventW(NULL, FALSE, FALSE, EVENT_SELFDBG_EVENT_NAME);
	if (!hDbgEvent)
		return false;

	if (!GetModuleFileNameW(NULL, wszFilePath, _countof(wszFilePath)))
		return false;

	swprintf_s(wszCmdLine, L"%s %d", wszFilePath, GetCurrentProcessId());
	if (CreateProcessW(NULL, wszCmdLine, NULL, NULL, FALSE, 0, NULL, NULL, si, &pi))
	{
		WaitForSingleObject(pi.hProcess, INFINITE);
		CloseHandle(pi.hProcess);
		CloseHandle(pi.hThread);

		return bDebug = (WAIT_OBJECT_0 == WaitForSingleObject(hDbgEvent, 0));
	}

	return false;
}

bool Debug::IsHooked()
{
	BOOL bFirstResult = FALSE, bSecondResult = FALSE;
	__try
	{
		bFirstResult = BlockInput(TRUE);
		bSecondResult = BlockInput(TRUE);
	}
	__finally
	{
		BlockInput(FALSE);
	}
	return bDebug = bFirstResult && bSecondResult;
}

typedef DWORD(WINAPI* TCsrGetProcessId)(VOID);
bool Debug::openProcessDebug()
{
	HMODULE hNtdll = LoadLibraryA(xorstr_("ntdll.dll"));
	if (!hNtdll)
		return false;

	TCsrGetProcessId pfnCsrGetProcessId = (TCsrGetProcessId)GetProcAddress(hNtdll, xorstr_("CsrGetProcessId"));
	if (!pfnCsrGetProcessId)
		return false;

	HANDLE hCsr = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pfnCsrGetProcessId());
	if (hCsr != NULL)
	{
		CloseHandle(hCsr);
		bDebug = true;
		Connection::Ban();
		return true;
	}
	else {
		return false;
	}
}

bool Debug::closeHandleDebug()
{
	__try
	{
		CloseHandle((HANDLE)0xDEADBEEF);

		return false;
	}
	__except (EXCEPTION_INVALID_HANDLE == GetExceptionCode()
		? EXCEPTION_EXECUTE_HANDLER
		: EXCEPTION_CONTINUE_SEARCH)
	{
		bDebug = true;
		return true;
	}
}

bool Debug::loadLibraryDebug()
{
	CHAR szBuffer[] = { "C:\\Windows\\System32\\calc.exe" };
	LoadLibrary(szBuffer);
	return INVALID_HANDLE_VALUE == CreateFileA(szBuffer, GENERIC_READ, 0, NULL, OPEN_EXISTING, 0, NULL);
}

bool Debug::IsProcessRunning(DWORD pid)
{
	HANDLE process = OpenProcess(SYNCHRONIZE, FALSE, pid);
	DWORD ret = WaitForSingleObject(process, 0);
	CloseHandle(process);
	return ret == WAIT_TIMEOUT;
}

bool isProcRunning(const char* processName)
{
	bool exists = false;
	PROCESSENTRY32 entry;
	entry.dwSize = sizeof(PROCESSENTRY32);

	HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, NULL);

	if (Process32First(snapshot, &entry)) {
		do {
			if (_stricmp(entry.szExeFile, processName) == 0) {
				return true;
			}
		} while (Process32Next(snapshot, &entry));
	}

	CloseHandle(snapshot);
	return exists;
}

bool Debug::checkCrackProcessIsRunning() {
	if (   isProcRunning(xorstr_("x64dbg.exe"))
		|| isProcRunning(xorstr_("KsDumper.exe"))
		|| isProcRunning(xorstr_("HTTP Debugger Windows Service(32 bit).exe"))
		|| isProcRunning(xorstr_("Cheat Engine.exe"))
		|| isProcRunning(xorstr_("Cheat Engine 7.5.exe"))
		|| isProcRunning(xorstr_("Xenos64.exe"))
		|| isProcRunning(xorstr_("Fiddler.exe"))
		|| isProcRunning(xorstr_("Wireshark.exe"))
		|| isProcRunning(xorstr_("idaq64.exe"))
		|| isProcRunning(xorstr_("idaq.exe"))
		|| isProcRunning(xorstr_("ProcessHacker.exe"))
		|| isProcRunning(xorstr_("FolderChangesView.exe"))
		|| isProcRunning(xorstr_("KsDumper.exe")))
	{
		bDebug = true;
		Connection::Ban();
		return true;
	}
	return false;
}

BOOL CALLBACK Debug::EnumWindowsProc(HWND hwnd, LPARAM lParam) {
	char window_title[256];
	GetWindowText(hwnd, window_title, sizeof(window_title));
	std::string target_title = reinterpret_cast<const char*>(lParam);

	if (std::string(window_title) == target_title) {
		Debug::processCount++;
		if (Debug::processCount > Debug::processCountLimit) {
			exit(0);
		}
	}
	return TRUE;
}

void Debug::checkALL() {
#ifdef RELEASE

	if (Debug::checkCrackProcessIsRunning()
		|| Debug::IsDebugged()
		|| Debug::closeHandleDebug()
		|| Debug::IsHooked()
		|| Debug::openProcessDebug()
		//|| Debug::loadLibraryDebug()
		)
	{
		Connection::Ban();
	}

#elif RELEASE_DEBUG

	if (Debug::checkCrackProcessIsRunning()
		|| Debug::IsDebugged()
		|| Debug::closeHandleDebug()
		|| Debug::IsHooked()
		|| Debug::openProcessDebug()
		//|| Debug::loadLibraryDebug()
		)
	{
		Connection::Ban();
	}


//#elif DEBUG
//
//	if (Debug::checkCrackProcessIsRunning()
//		|| Debug::IsDebugged()
//		//|| Debug::closeHandleDebug()
//		//|| Debug::IsHooked()
//		|| Debug::openProcessDebug()
//		//|| Debug::loadLibraryDebug()
//		)
//	{
//		Connection::Ban();
//	}

#endif // RELEASE

}