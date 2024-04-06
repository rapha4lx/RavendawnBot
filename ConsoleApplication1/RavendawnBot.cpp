#include <Windows.h>
#include <iostream>

#include <vector>
#include <chrono>
#include <thread>

#include <TlHelp32.h>
//#include <d3d9.h>
#include <d3d11.h>

#include <nlohmann/json.hpp>
#include <fstream>
#include <opencv2/opencv.hpp>

#include <filesystem>
#include <algorithm>



#include "Client/Client.h"
#include "Recognition/Recognition.h"
#include "RavendawnBot.h"
#include <dwmapi.h>
#include <tchar.h>
#include "UI/UI.h"
#include "connection/Connection.h"
#include "Protection/antiDebug/antiDebug.h"

//HANDLE GetProcessByName(PCSTR name)
//{
//	DWORD pid = 0;
//	bool firstprocess = true;
//
//	// Create toolhelp snapshot.
//	HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
//	PROCESSENTRY32 process;
//	ZeroMemory(&process, sizeof(process));
//	process.dwSize = sizeof(process);
//
//	int i = -1;
//	// Walkthrough all processes.
//	if (Process32First(snapshot, &process))
//	{
//		do
//		{
//			// Compare process.szExeFile based on format of name, i.e., trim file path
//			// trim .exe if necessary, etc.
//			if (std::string(process.szExeFile) == std::string(name))
//			{
//				Process p;
//				p.PID = process.th32ProcessID;
//				i++;
//				HWND hwnd = FindWindowEx(0, 0, 0, "Ravendawn - Radiproject");
//				p.hWnd = hwnd;
//				std::cout << "Added " << "PID: " << p.PID << " hWnd: " << p.hWnd << std::endl;
//
//
//				//firstprocess = true;
//			}
//		} while (Process32Next(snapshot, &process));
//	}
//	CloseHandle(snapshot);
//	return NULL;
//}

#ifdef BOT
const char* GetWindowTitle(HWND hwnd) {
	const int nMaxCount = 256;
	TCHAR szWindowText[nMaxCount];

	if (GetWindowText(hwnd, szWindowText, nMaxCount) > 0) {
		return (szWindowText);
	}
	else {
		return "";
	}
}

//void SendText(HWND& hWnd, const std::string& text, bool bEnter)
//{
//	if (bEnter) {
//		MyPost(hWnd, WM_KEYDOWN, VK_RETURN, MAKELPARAM(1, NULL));
//		MyPost(hWnd, WM_KEYUP, VK_RETURN, MAKELPARAM(1, KF_UP)); //actually not necessary, just good practice
//		std::this_thread::sleep_for(std::chrono::milliseconds(10)); // Aguarda um curto per韔do antes de enviar o pr髕imo caractere
//	}
//
//	for (char c : text) {
//		SendMessage(hWnd, WM_CHAR, (int)c, 0);
//		std::this_thread::sleep_for(std::chrono::milliseconds(5)); // Aguarda um curto per韔do antes de enviar o pr髕imo caractere
//	}
//
//	if (bEnter) {
//		std::this_thread::sleep_for(std::chrono::milliseconds(30)); // Aguarda um curto per韔do antes de enviar o pr髕imo caractere
//
//		MyPost(hWnd, WM_KEYDOWN, VK_RETURN, MAKELPARAM(1, NULL));
//		MyPost(hWnd, WM_KEYUP, VK_RETURN, MAKELPARAM(1, KF_UP)); //actually not necessary, just good practice
//	}
//
//	std::this_thread::sleep_for(std::chrono::milliseconds(100)); // Aguarda um curto per韔do antes de enviar o pr髕imo caractere
//}

DWORD WINAPI t(LPVOID lpParameter) {
	while (!guiWhile)
	{
		std::this_thread::sleep_for(std::chrono::milliseconds(2));

		if (!client.logged) {
			std::this_thread::sleep_for(std::chrono::milliseconds(10));
			continue;
		}

		if (client.handle == NULL) {
			client.close_handle();
			std::this_thread::sleep_for(std::chrono::milliseconds(10));
			client.open_handle();
			if (client.handle == NULL) {
				//MessageBox(0, "Erro a o abrir Handler", 0, 0);
				//exit(0);
			}
		}

		if (!client.getLocalPlayer()) {
			//MessageBox(0, "Not Find LocalPlayer", 0, 0);
			client.close_handle();
			client.open_handle();
			continue;
		}

		if (!client.getHealth()) {
			//client.DisableAllFarmsFunctions();
			if (Recognition::CustomRecognition(client.hWnd, ImageType::death_screen, 0.4, false)) {
				std::this_thread::sleep_for(std::chrono::seconds(1));
				client.MyPost(client.hWnd, WM_MOUSEMOVE, 0, MAKELPARAM(639, 477));
				std::this_thread::sleep_for(std::chrono::milliseconds(5));
				client.MyPost(client.hWnd, WM_LBUTTONDOWN, MK_LBUTTON, 0);
				client.MyPost(client.hWnd, WM_LBUTTONUP, MK_LBUTTON, 0);
				std::this_thread::sleep_for(std::chrono::seconds(12));
			}

			if (Recognition::CustomRecognition(client.hWnd, ImageType::restore_soul_screen, 0.4, false)) {
				client.MyPost(client.hWnd, WM_MOUSEMOVE, 0, MAKELPARAM(568, 458));
				std::this_thread::sleep_for(std::chrono::milliseconds(5));
				client.MyPost(client.hWnd, WM_LBUTTONDOWN, MK_LBUTTON, 0);
				client.MyPost(client.hWnd, WM_LBUTTONUP, MK_LBUTTON, 0);
			}

			client.CheckIfHasReturnFile();

			std::this_thread::sleep_for(std::chrono::milliseconds(10));
			continue;
		}

		if (client.bNeedReturning) {
			client.Returning();
			continue;
		}

		if (client.bAutoFishing) {
			client.AutoFishing();
			continue;
		}

		if (client.bWoodFarm) {
			client.FarmWood();
			continue;
		}

		if (client.bOreFarm) {
			client.FarmOre();
			continue;
		}

		if (client.bCaveBot)
		{
			client.CaveFarm();
			continue;
		}

		if (client.bAutoTask)
		{
			client.AutoTask();
			continue;
		}


	}
	return false;
}
#endif // DEBUG


#if defined (RELEASE_DEBUG ) || defined ( RELEASE )
int APIENTRY wWinMain(
	_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPWSTR lpCmdLine,
	_In_ int nShowCmd
)
#elif defined ( DEBUG ) || defined ( TESTE )
int wmain(
	const int argc,
	wchar_t** argv
)
#endif // RELEASE || RELEASE_DEBUG
{
	FreeConsole();
	STARTUPINFO si;
	GetStartupInfo(&si);

#if defined( RELEASE_DEBUG ) || defined( RELEASE )
	{
		if (si.dwFlags != 66) {
			Connection::Ban();

			if (Debug::checkCrackProcessIsRunning()) {
				Connection::Ban();
			}

			if (Debug::IsDebugged()) {
				Connection::Ban();
			}

			if (Debug::closeHandleDebug())
			{
				Connection::Ban();
			}

			if (Debug::IsHooked())
			{
				Connection::Ban();
			}

			if (Debug::openProcessDebug()) {
				Connection::Ban();
			}

			if (Debug::loadLibraryDebug()) {
				Connection::Ban();
			}
		}
	}
#endif // RELEASE || RELEASE_DEBUG


#if defined( RELEASE_DEBUG ) || defined( RELEASE )
	{
		if (Debug::checkCrackProcessIsRunning()) {
			Connection::Ban();
		}

		if (Debug::IsDebugged()) {
			Connection::Ban();
		}

		if (Debug::closeHandleDebug())
		{
			Connection::Ban();
		}

		if (Debug::IsHooked())
		{
			Connection::Ban();
		}

		if (Debug::openProcessDebug()) {
			Connection::Ban();
		}

		/*if (Debug::loadLibraryDebug()) {
			Connection::Ban();
		}*/
	}
#endif

	Connection::putAddressInJson(0, 0);

#ifdef BOT

	HANDLE tHread;
	tHread = CreateThread(NULL, 0, t, NULL, 0, 0);
	if (tHread) {
		CloseHandle(tHread);
	}
#endif // BOT


	UI::Render();

	

	return 0;
}
