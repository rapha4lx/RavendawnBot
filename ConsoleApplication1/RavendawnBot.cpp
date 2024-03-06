
#include <Windows.h>
#include <iostream>

#include <vector>
#include <chrono>
#include <thread>

#include <TlHelp32.h>
#include <d3d9.h>
#include <d3dx9tex.h>
#pragma comment(lib, "d3d9.lib")

#include "Notification/Notification.h"
#include "ImGui/imgui.h"
#include "ImGui/imgui_impl_win32.h"
#include "ImGui/imgui_impl_dx9.h"

#include <nlohmann/json.hpp>
#include <fstream>
#include <opencv2/opencv.hpp>
#include <opencv2/dnn.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>

#include <filesystem>
#include <algorithm>



#include "Client/Client.h"
#include "Recognition/Recognition.h"
#include "RavendawnBot.h"

class Movements {
public:
	DWORD PID{ NULL };

	bool sysKey{ false };
	bool keyUp{ false };
	DWORD key{ NULL };

	int event{ NULL };
	POINT position{ NULL };
};

std::vector<Movements> movements;


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

void UpdateAccounts() {
	if (!std::filesystem::exists("C:\\RavendawnBot")) {
		std::filesystem::create_directories("C:\\RavendawnBot");
		//here put a logic for downlaod recognitions files
	}

	if (!std::filesystem::exists("C:\\RavendawnBot\\accounts")) {
		std::filesystem::create_directories("C:\\RavendawnBot\\accounts");
	}

	if (Clients.size()) {
		Clients.clear();
		system("cls");
	}
	std::string fileText;

	std::ifstream file_json("C:\\Ravendawn accounts.json");
	if (!file_json.is_open()) {
		std::ofstream("C:\\Ravendawn accounts.json").close();
		exit(0);
	}

	nlohmann::json json;
	try {
		file_json >> json;
	}
	catch (const std::exception& e) {
		std::cerr << "Erro ao ler o arquivo JSON: " << e.what() << std::endl;
		return;
	}
	file_json.close();

	int size = json["Accounts"][0].size();

	char* buffer{ nullptr };
	size_t sz{ 0 };
	if (_dupenv_s(&buffer, &sz, "USERPROFILE") == 0 && buffer != nullptr)
	{

	}

	std::string sourceDir{ (std::string)(buffer)+"\\AppData\\Roaming\\Ravendawn\\ravendawn" };
	for (int i = 0; i < size; i++) {
		Client proces;
		proces = json["Accounts"][0][i];

		std::string name{ proces.Login };
		std::string nick{ proces.charPerson.Nick };
		std::cout << "Login: " << name << " Nick: " << nick << std::endl;

		std::string processName{ "Ravendawn - " + nick };

		proces.hWnd = FindWindowEx(0, 0, 0, processName.c_str());
		if (proces.hWnd != NULL) {
			proces.logged = true;
			GetWindowThreadProcessId(proces.hWnd, &proces.PID);
			//proces.open_handle();
			MoveWindow(proces.hWnd, 0, 0, 0, 0, TRUE);
		}



		Clients.push_back(proces);

		if (!std::filesystem::exists("C:\\RavendawnBot\\accounts\\" + name)) {
			std::filesystem::create_directory("C:\\RavendawnBot\\accounts\\" + name);
			std::this_thread::sleep_for(std::chrono::milliseconds(8));
			for (const auto& entry : std::filesystem::directory_iterator(sourceDir)) {
				const auto& path = entry.path();
				if (std::filesystem::is_regular_file(path)) {
					if (path.filename().extension().string() == ".dll" || path.filename().extension().string() == ".exe") {
						std::filesystem::copy_file(path, "C:\\RavendawnBot\\accounts\\" + name + "\\" + path.filename().string(), std::filesystem::copy_options::overwrite_existing);
						std::cout << "Copied: " << path.filename() << std::endl;
					}
				}
			}
		}


	}
}

void SendText(HWND& hWnd, const std::string& text, bool bEnter)
{
	if (bEnter) {
		PostMessage(hWnd, WM_KEYDOWN, VK_RETURN, MAKELPARAM(1, NULL));
		PostMessage(hWnd, WM_KEYUP, VK_RETURN, MAKELPARAM(1, KF_UP)); //actually not necessary, just good practice
		std::this_thread::sleep_for(std::chrono::milliseconds(10)); // Aguarda um curto per韔do antes de enviar o pr髕imo caractere
	}

	for (char c : text) {
		SendMessage(hWnd, WM_CHAR, (int)c, 0);
		std::this_thread::sleep_for(std::chrono::milliseconds(5)); // Aguarda um curto per韔do antes de enviar o pr髕imo caractere
	}

	if (bEnter) {
		std::this_thread::sleep_for(std::chrono::milliseconds(30)); // Aguarda um curto per韔do antes de enviar o pr髕imo caractere

		PostMessage(hWnd, WM_KEYDOWN, VK_RETURN, MAKELPARAM(1, NULL));
		PostMessage(hWnd, WM_KEYUP, VK_RETURN, MAKELPARAM(1, KF_UP)); //actually not necessary, just good practice
	}

	std::this_thread::sleep_for(std::chrono::milliseconds(100)); // Aguarda um curto per韔do antes de enviar o pr髕imo caractere
}

void onMouse(int event, int x, int y, int flags, void* userdata) {
	Client* params = static_cast<Client*>(userdata);

	switch (event)
	{
	case cv::EVENT_LBUTTONDBLCLK: {
		//std::cout << "Clique esquerdo detectado em: (" << x << ", " << y << ")" << std::endl;
		movements.push_back({ params->PID, false, false, NULL, event, {x, y} });
		break;
	}

	case cv::EVENT_LBUTTONDOWN: {
		//std::cout << "Clique esquerdo detectado em: (" << x << ", " << y << ")" << std::endl;
		movements.push_back({ params->PID, false, false, NULL, event, {x, y} });
		//SendMessage(params->hWnd, WM_MOUSEMOVE, 0, MAKELPARAM(x, y)); // x e y são as coordenadas do clique
		/*std::this_thread::sleep_for(std::chrono::milliseconds(300));
		SendMessage(params->hWnd, WM_LBUTTONDOWN, MK_LBUTTON, 0);
		SendMessage(params->hWnd, WM_LBUTTONUP, MK_LBUTTON, 0);*/
		break;
	}

	case cv::EVENT_LBUTTONUP: {
		//std::cout << "Clique esquerdo detectado em: (" << x << ", " << y << ")" << std::endl;
		movements.push_back({ params->PID, false, false, NULL, event, {x, y} });
		break;
	}

	case cv::EVENT_RBUTTONDBLCLK: {
		//std::cout << "Clique direito detectado em: (" << x << ", " << y << ")" << std::endl;
		movements.push_back({ params->PID, false, false, NULL, event, {x, y} });
		break;
	}

	case cv::EVENT_MOUSEWHEEL: {
		if (flags > 0) {
			//std::cout << "MouseWheel para cima detectado" << std::endl;
			movements.push_back({ params->PID, false, false, NULL, event, {0, -1} });
		}
		else if (flags < 0) {
			//std::cout << "MouseWheel para baixo detectado" << std::endl;
			movements.push_back({ params->PID, false, false, NULL, event, {0, 1} });
		}
		break;
	}

	case cv::EVENT_MOUSEMOVE: {
		if ((x - lastMousePosition.x) > 10 || (x - lastMousePosition.x) < -10) {
			//std::cout << "Mouse move detectado em: (" << x << ", " << y << ")" << std::endl;
			movements.push_back({ params->PID, false, false, NULL, event, {x, y} });
			lastMousePosition = { x, y };
		}
		else if ((x - lastMousePosition.x) > 10 || (x - lastMousePosition.x) < -10) {
			//std::cout << "Mouse move detectado em: (" << x << ", " << y << ")" << std::endl;
			movements.push_back({ params->PID, false, false, NULL, event, {x, y} });
			lastMousePosition = { x, y };
		}
		break;
	}

	default:
		break;
	}
}

void cvWindows() {
	cv::destroyAllWindows();
	while (!guiWhile)
	{
		for (auto& client : Clients) {
			if (!client.bDebug) {
				continue;
			}

			if (!client.logged) {
				continue;
			}

			RECT rect;
			GetClientRect(client.hWnd, &rect);
			int width = rect.right;
			int height = rect.bottom;

			HDC hdcScreen = GetDC(client.hWnd);
			if (!hdcScreen) {
				throw std::runtime_error("Failed to get screen device context (hdcScreen is null)");
			}

			HDC hdcMem = CreateCompatibleDC(hdcScreen);
			if (!hdcMem) {
				ReleaseDC(client.hWnd, hdcScreen); // Liberar hdcScreen antes de lançar a exceção
				throw std::runtime_error("Failed to create compatible device context (hdcMem is null)");
			}

			HBITMAP hBitmap = CreateCompatibleBitmap(hdcScreen, width, height);
			if (!hBitmap) {
				ReleaseDC(client.hWnd, hdcScreen); // Liberar hdcScreen e hdcMem antes de lançar a exceção
				DeleteDC(hdcMem);
				throw std::runtime_error("Failed to create compatible bitmap (hBitmap is null)");
			}

			SelectObject(hdcMem, hBitmap);

			BitBlt(hdcMem, 0, 0, width, height, hdcScreen, 0, 0, SRCCOPY);

			BITMAPINFOHEADER bi;
			bi.biSize = sizeof(BITMAPINFOHEADER);
			bi.biWidth = width;
			bi.biHeight = -height;
			bi.biPlanes = 1;
			bi.biBitCount = 24;
			bi.biCompression = BI_RGB;
			bi.biSizeImage = 0;
			bi.biXPelsPerMeter = 0;
			bi.biYPelsPerMeter = 0;
			bi.biClrUsed = 0;
			bi.biClrImportant = 0;

			cv::Mat screenShot;
			try
			{
				screenShot = cv::Mat(height, width, CV_8UC3);
			}
			catch (const cv::Exception& e)
			{
				screenShot.release();
				std::cout << e.what() << std::endl;
				DeleteDC(hdcMem);
				DeleteObject(hBitmap);
				ReleaseDC(client.hWnd, hdcScreen);
			}

			GetDIBits(hdcScreen, hBitmap, 0, height, screenShot.data, (BITMAPINFO*)&bi, DIB_RGB_COLORS);

			try
			{
				cv::namedWindow(client.charPerson.Nick, cv::WINDOW_NORMAL);
				//cv::setWindowProperty(client.account.charPerson.Nick, cv::WND_PROP_TOPMOST, 1);
				cv::imshow(client.charPerson.Nick, screenShot);
				cv::setMouseCallback(client.charPerson.Nick, onMouse, &client);

				// Pressione 'q' para sair do loop
				if (cv::waitKey(1) == 'q') {
					screenShot.release();
					DeleteDC(hdcMem);
					DeleteObject(hBitmap);
					ReleaseDC(client.hWnd, hdcScreen);
					break;
				}

			}
			catch (const cv::Exception& e)
			{
				screenShot.release();

				std::cerr << "Exceção capturada: " << e.what() << std::endl;
				DeleteDC(hdcMem);
				DeleteObject(hBitmap);
				ReleaseDC(client.hWnd, hdcScreen);
			}

			screenShot.release();

			DeleteDC(hdcMem);
			DeleteObject(hBitmap);
			ReleaseDC(client.hWnd, hdcScreen);
		}
	}
}

void ListarArquivos(const std::string& pasta, Client& client, MapperType FarmType) {
	for (const auto& entry : std::filesystem::directory_iterator(pasta)) {
		if (std::filesystem::is_regular_file(entry)) {
			std::string nomeArquivo = entry.path().stem().string();
			if (ImGui::MenuItem(nomeArquivo.c_str())) {
				client.LoadWaipointConfig(nomeArquivo.c_str(), FarmType);
			}
		}
	}
}

static int imgPoint{ 0 };

void gui() {
	// Create application window
	//ImGui_ImplWin32_EnableDpiAwareness();
	WNDCLASSEXW wc = { sizeof(wc), CS_CLASSDC, WndProc, 0L, 0L, GetModuleHandle(nullptr), nullptr, nullptr, nullptr, nullptr, L"ImGui Example", nullptr };
	::RegisterClassExW(&wc);
	HWND hwnd = ::CreateWindowW(wc.lpszClassName, L"Dear ImGui DirectX9 Example", WS_OVERLAPPEDWINDOW, 800, 0, 600, 1080, nullptr, nullptr, wc.hInstance, nullptr);

	// Initialize Direct3D
	if (!CreateDeviceD3D(hwnd))
	{
		CleanupDeviceD3D();
		::UnregisterClassW(wc.lpszClassName, wc.hInstance);
		return;
	}

	// Show the window
	::ShowWindow(hwnd, SW_SHOWDEFAULT);
	::UpdateWindow(hwnd);

	//Notification Initialize
	Notification::Initialize();

	// Setup Dear ImGui context
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

	// Setup Dear ImGui style
	ImGui::StyleColorsDark();
	//ImGui::StyleColorsLight();

	// Setup Platform/Renderer backends
	ImGui_ImplWin32_Init(hwnd);
	ImGui_ImplDX9_Init(g_pd3dDevice);


	// Load Fonts
	// - If no fonts are loaded, dear imgui will use the default font. You can also load multiple fonts and use ImGui::PushFont()/PopFont() to select them.
	// - AddFontFromFileTTF() will return the ImFont* so you can store it if you need to select the font among multiple.
	// - If the file cannot be loaded, the function will return a nullptr. Please handle those errors in your application (e.g. use an assertion, or display an error and quit).
	// - The fonts will be rasterized at a given size (w/ oversampling) and stored into a texture when calling ImFontAtlas::Build()/GetTexDataAsXXXX(), which ImGui_ImplXXXX_NewFrame below will call.
	// - Use '#define IMGUI_ENABLE_FREETYPE' in your imconfig file to use Freetype for higher quality font rendering.
	// - Read 'docs/FONTS.md' for more instructions and details.
	// - Remember that in C/C++ if you want to include a backslash \ in a string literal you need to write a double backslash \\ !
	//io.Fonts->AddFontDefault();
	//io.Fonts->AddFontFromFileTTF("c:\\Windows\\Fonts\\segoeui.ttf", 18.0f);
	//io.Fonts->AddFontFromFileTTF("../../misc/fonts/DroidSans.ttf", 16.0f);
	//io.Fonts->AddFontFromFileTTF("../../misc/fonts/Roboto-Medium.ttf", 16.0f);
	//io.Fonts->AddFontFromFileTTF("../../misc/fonts/Cousine-Regular.ttf", 15.0f);
	//ImFont* font = io.Fonts->AddFontFromFileTTF("c:\\Windows\\Fonts\\ArialUni.ttf", 18.0f, nullptr, io.Fonts->GetGlyphRangesJapanese());
	//IM_ASSERT(font != nullptr);
	bool showPopup{ false };
	ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

	while (!guiWhile)
	{
		// Poll and handle messages (inputs, window resize, etc.)
		// See the WndProc() function below for our to dispatch events to the Win32 backend.
		MSG msg;
		while (::PeekMessage(&msg, nullptr, 0U, 0U, PM_REMOVE))
		{
			::TranslateMessage(&msg);
			::DispatchMessage(&msg);
			if (msg.message == WM_QUIT)
				guiWhile = true;
		}
		if (guiWhile)
			break;

		// Handle window resize (we don't resize directly in the WM_SIZE handler)
		if (g_ResizeWidth != 0 && g_ResizeHeight != 0)
		{
			g_d3dpp.BackBufferWidth = g_ResizeWidth;
			g_d3dpp.BackBufferHeight = g_ResizeHeight;
			g_ResizeWidth = g_ResizeHeight = 0;
			ResetDevice();
		}

		// Start the Dear ImGui frame
		ImGui_ImplDX9_NewFrame();
		ImGui_ImplWin32_NewFrame();
		ImGui::NewFrame();

		// 2. Show a simple window that we create ourselves. We use a Begin/End pair to create a named window.
		{
			ImGui::Begin("test ravendawn");
			if (ImGui::Button("GetWindows")) {
				UpdateAccounts();
			}

			/*if (ImGui::Button("Open Game")) {
				system("cd \"C:\\Program Files\\AVAST Software\\Avast\\ashCmd.exe \" --run=app \"C:\\Program Files (x86)\\Tavernlight Games\\Ravendawn\\ravendawn_dx.exe\"");
			}*/

			if (ImGui::Button("Abrir Popup")) {
				showPopup = true;
			}

			if (showPopup) {
				ImGui::OpenPopup("Menu Popup");
				showPopup = false;
			}

			if (ImGui::BeginPopup("Menu Popup")) {
				static char	Nick[32];
				static char	Email[125];
				static char	Pass[30];
				static char	Login[32];

				ImGui::InputText("Nick", Nick, IM_ARRAYSIZE(Nick));
				ImGui::InputText("Email", Email, IM_ARRAYSIZE(Email));
				ImGui::InputText("Pass", Pass, IM_ARRAYSIZE(Pass));
				ImGui::InputText("Login", Login, IM_ARRAYSIZE(Login));

				if (ImGui::MenuItem("Add account")) {
					std::ofstream json_account("C:\\Ravendawn accounts.json");

					Client account;
					account.charPerson.Nick = Nick;
					account.Email = Email;
					account.Pass = Pass;
					account.Login = Login;

					Clients.push_back(account);
					nlohmann::json json;
					json["Accounts"].push_back(Clients);

					json_account << std::setw(4) << json << std::endl;
					json_account.close();
				}

				ImGui::EndPopup();
			}

			if (Clients.size()) {
				for (auto& client : Clients) {
					ImGui::BeginChild(client.Login.c_str(), ImVec2(600, 200)); {
						ImGui::Text(client.Login.c_str());
						ImVec2 textSize = ImGui::CalcTextSize(client.Login.c_str());
						ImGui::SameLine(textSize.x + 20.20);

						if (client.logged) {
							ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0, 1, 0, 1));
						}
						else
						{
							ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1, 0, 0, 1));
						}

						ImGui::Text(client.logged ? "on" : "off");
						//ImGui::SameLine(textSize.x + 40.20);
						if (ImGui::Button("Edit")) {
							client.bEditPerson = true;
						}

						if (ImGui::Button("Open Handle")) {
							client.open_handle();
						}

						if (ImGui::Button("Close Handle")) {
							client.close_handle();
						}

						if (client.bEditPerson) {
							ImGui::OpenPopup("Edit Popup");
							client.bEditPerson = false;
						}
						
						if (ImGui::BeginPopup("Edit Popup")) {

							for (int i = 0; i < 12; i++) {
								std::string skillNumber = "Skill " + std::to_string( i);

								ImGui::SliderFloat(skillNumber.c_str(), & client.charPerson.skills[i].cooldown, 0, 60, "%.1f");
								ImGui::Checkbox((skillNumber + "Usable?").c_str(), &client.charPerson.skills[i].bUsable);
								ImGui::Checkbox((skillNumber + "heal Skill?").c_str(), &client.charPerson.skills[i].healSkill);
							}

							if (ImGui::MenuItem("Save")) {
								std::ofstream json_account("C:\\Ravendawn accounts.json");

								nlohmann::json json;
								json["Accounts"].push_back(Clients);

								json_account << std::setw(4) << json << std::endl;
								json_account.close();
							}

							ImGui::EndPopup();
						}

						if (client.logged) {
							ImGui::Checkbox("bDebug", &client.bDebug);
							ImGui::Checkbox("SyncMovement", &client.bSyncMovement);
							//ImGui::Checkbox("AutoAttack", &client.bAutoAttack);


							if (!client.bAutoFishing) {
								if (ImGui::Button("Auto Fishing")) {
									client.bAutoFishingPopup = true;
								}

								if (client.bAutoFishingPopup) {
									ImGui::OpenPopup("AutoFishing Popup");
									client.bAutoFishingPopup = false;
								}

								if (ImGui::BeginPopup("AutoFishing Popup")) {
									ImGui::Checkbox("Fishi Auto Move", &client.bFishingAutoMove);

									ListarArquivos("C:\\RavendawnBot\\Farms\\Fishi",client, MapperType::Fishi);
									
									if (ImGui::Button("Start AutoFishing")) {
										client.bAutoFishing = true;
										ClientsFarm.push_back(&client);
									}

									ImGui::EndPopup();
								}
							}
							else
							{
								/*if (ImGui::Button("Move Up")) {
									client.findFishingPos = FindPos::up;
								}
								ImGui::SameLine();
								if (ImGui::Button("Move Down")) {
									client.findFishingPos = FindPos::down;
								}
								ImGui::SameLine();
								if (ImGui::Button("Move left")) {
									client.findFishingPos = FindPos::left;
								}
								ImGui::SameLine();
								if (ImGui::Button("Move right")) {
									client.findFishingPos = FindPos::right;
								}*/

								if (ImGui::Button("Stop AutoFishing")) {
									auto it = std::find_if(ClientsFarm.begin(), ClientsFarm.end(), [client](const Client* obj) {
										return obj->PID == client.PID;
										});
									ClientsFarm.erase(it);
									client.bAutoFishing = false;
									client.bFishingAutoMove = false;
									client.bAutoFishingPopup = false;
								}
							}

							if (!client.bAutoTask) {
								if (ImGui::Checkbox("Auto Task", &client.bAutoTask)) {
									ClientsFarm.push_back(&client);
									client.bAutoTask = true;
								}
							}
							else
							{
								if (ImGui::Button("Stop bAutoTask")) {
									auto it = std::find_if(ClientsFarm.begin(), ClientsFarm.end(), [client](const Client* obj) {
										return obj->PID == client.PID;
										});
									ClientsFarm.erase(it);
									client.bAutoTask = false;
								}
							}

							if (!client.bAutoAttack) {
								if (ImGui::Checkbox("Auto Attack", &client.bAutoAttack)) {
									ClientsFarm.push_back(&client);
									client.bAutoAttack = true;
								}
							}
							else
							{
								if (ImGui::Button("Stop Auto Attack")) {
									auto it = std::find_if(ClientsFarm.begin(), ClientsFarm.end(), [client](const Client* obj) {
										return obj->PID == client.PID;
										});
									ClientsFarm.erase(it);
									client.bAutoAttack = false;
								}
							}

							if (!client.bWoodFarm) {
								if (ImGui::Button("Farm Wood")) {
									client.bAutoWoodFarmPopup = true;
								}

								if (client.bAutoWoodFarmPopup) {
									ImGui::OpenPopup("Auto Farm Wood");
									client.bAutoWoodFarmPopup = false;
								}

								if (ImGui::BeginPopup("Auto Farm Wood")) {
									ListarArquivos("C:\\RavendawnBot\\Farms\\Wood", client, MapperType::Wood);
									if (ImGui::Button("Start Farm Wood")) {
										client.bWoodFarm = true;
										ClientsFarm.push_back(&client);
									}
									ImGui::EndPopup();
								}
							}
							else
							{
								if (ImGui::Button("Stop Wood Farm")) {
									auto it = std::find_if(ClientsFarm.begin(), ClientsFarm.end(), [client](const Client* obj) {
										return obj->PID == client.PID;
										});
									ClientsFarm.erase(it);
									client.bWoodFarm = false;
									client.bAutoWoodFarmPopup = false;
								}
							}

							if (!client.bOreFarm) {
								if (ImGui::Button("Farm Ore")) {
									client.bAutoOreFarmPopup = true;
								}

								if (client.bAutoOreFarmPopup) {
									ImGui::OpenPopup("Auto Farm Ore");
									client.bAutoOreFarmPopup = false;
								}

								if (ImGui::BeginPopup("Auto Farm Ore")) {
									ListarArquivos("C:\\RavendawnBot\\Farms\\Ore", client, MapperType::Ore);
									if (ImGui::Button("Start Farm Ore")) {
										client.bOreFarm = true;
										ClientsFarm.push_back(&client);
									}
									ImGui::EndPopup();
								}
							}
							else
							{
								if (ImGui::Button("Stop Ore Farm")) {
									auto it = std::find_if(ClientsFarm.begin(), ClientsFarm.end(), [client](const Client* obj) {
										return obj->PID == client.PID;
										});
									ClientsFarm.erase(it);
									client.bOreFarm = false;
									client.bAutoOreFarmPopup = false;
								}
							}

							POINT cursorPos{};
							GetCursorPos(&cursorPos);

							ScreenToClient(client.hWnd, &cursorPos);

							RECT windowRect{  };
							GetWindowRect(client.hWnd, &windowRect);

							ImGui::Text("X: %d \\ Y: %d", cursorPos.x /*- windowRect.right / 2*/, cursorPos.y /*- windowRect.bottom / 2*/);
							ImGui::Text("WindowPosX: %d \\ WindowPosY: %d", windowRect.left, windowRect.top);
							ImGui::Text("life: %lf ", client.getHealth());
							//std::cout << "Life: %lf " << client.read<int>(client.LocalPlayer + /*0xDDC*/ 0xCE0) << std::endl;
							/*ImGui::Text("Player Position: X:%d Y:%d Z:%d",
								client.read<int>(client.BaseAddress + 0x27CCC1C),
								client.read<int>(client.BaseAddress + 0x27CCC20),
								client.read<int>(client.BaseAddress + 0x27CC498));*/

							ImGui::Text("Interaction: %d // invValue: %d", client.getWoodInteraction(), client.getInvValue());

							if (ImGui::Button("Take Print")) {
								RECT rect;
								GetClientRect(client.hWnd, &rect);
								int width = rect.right;
								int height = rect.bottom;

								HDC hdcScreen = GetDC(client.hWnd);
								if (!hdcScreen) {
									throw std::runtime_error("Failed to get screen device context (hdcScreen is null)");
								}

								HDC hdcMem = CreateCompatibleDC(hdcScreen);
								if (!hdcMem) {
									ReleaseDC(client.hWnd, hdcScreen); // Liberar hdcScreen antes de lançar a exceção
									throw std::runtime_error("Failed to create compatible device context (hdcMem is null)");
								}

								HBITMAP hBitmap = CreateCompatibleBitmap(hdcScreen, width, height);
								if (!hBitmap) {
									ReleaseDC(client.hWnd, hdcScreen); // Liberar hdcScreen e hdcMem antes de lançar a exceção
									DeleteDC(hdcMem);
									throw std::runtime_error("Failed to create compatible bitmap (hBitmap is null)");
								}

								SelectObject(hdcMem, hBitmap);

								BitBlt(hdcMem, 0, 0, width, height, hdcScreen, 0, 0, SRCCOPY);

								BITMAPINFOHEADER bi;
								bi.biSize = sizeof(BITMAPINFOHEADER);
								bi.biWidth = width;
								bi.biHeight = -height;
								bi.biPlanes = 1;
								bi.biBitCount = 24;
								bi.biCompression = BI_RGB;
								bi.biSizeImage = 0;
								bi.biXPelsPerMeter = 0;
								bi.biYPelsPerMeter = 0;
								bi.biClrUsed = 0;
								bi.biClrImportant = 0;

								cv::Mat screenShot;
								try
								{
									screenShot = cv::Mat(height, width, CV_8UC3);
								}
								catch (const cv::Exception& e)
								{
									std::cout << e.what() << std::endl;
									DeleteDC(hdcMem);
									DeleteObject(hBitmap);
									ReleaseDC(client.hWnd, hdcScreen);
								}

								GetDIBits(hdcScreen, hBitmap, 0, height, screenShot.data, (BITMAPINFO*)&bi, DIB_RGB_COLORS);

								std::string filename = "C:\\RavendawnBot\\training\\screenshot" + std::to_string(imgPoint) + ".png";
								imgPoint++;
								// Salve a imagem usando o OpenCV
								cv::imwrite(filename, screenShot);

								DeleteDC(hdcMem);
								DeleteObject(hBitmap);
								ReleaseDC(client.hWnd, hdcScreen);
							}

							if (ImGui::Button("try dnn")) {
								//cv::dnn::Net net = cv::dnn::readNet("C:\\RavendawnBot\\training\\python\\exp\\weights\\yolov5s.onnx");
								////cv::dnn::Net net = cv::dnn::readNetFromONNX("C:\\RavendawnBot\\best.onnx");
								//RECT rect;
								//GetClientRect(client.hWnd, &rect);
								//int width = rect.right;
								//int height = rect.bottom;
								//HDC hdcScreen = GetDC(client.hWnd);
								//if (!hdcScreen) {
								//	throw std::runtime_error("Failed to get screen device context (hdcScreen is null)");
								//}
								//HDC hdcMem = CreateCompatibleDC(hdcScreen);
								//if (!hdcMem) {
								//	ReleaseDC(client.hWnd, hdcScreen); // Liberar hdcScreen antes de lançar a exceção
								//	throw std::runtime_error("Failed to create compatible device context (hdcMem is null)");
								//}
								//HBITMAP hBitmap = CreateCompatibleBitmap(hdcScreen, width, height);
								//if (!hBitmap) {
								//	ReleaseDC(client.hWnd, hdcScreen); // Liberar hdcScreen e hdcMem antes de lançar a exceção
								//	DeleteDC(hdcMem);
								//	throw std::runtime_error("Failed to create compatible bitmap (hBitmap is null)");
								//}
								//SelectObject(hdcMem, hBitmap);
								//BitBlt(hdcMem, 0, 0, width, height, hdcScreen, 0, 0, SRCCOPY);
								//BITMAPINFOHEADER bi;
								//bi.biSize = sizeof(BITMAPINFOHEADER);
								//bi.biWidth = width;
								//bi.biHeight = -height;
								//bi.biPlanes = 1;
								//bi.biBitCount = 24;
								//bi.biCompression = BI_RGB;
								//bi.biSizeImage = 0;
								//bi.biXPelsPerMeter = 0;
								//bi.biYPelsPerMeter = 0;
								//bi.biClrUsed = 0;
								//bi.biClrImportant = 0;
								//cv::Mat screenShot;
								//try
								//{
								//	screenShot = cv::Mat(height, width, CV_8UC3);
								//}
								//catch (const cv::Exception& e)
								//{
								//	std::cout << e.what() << std::endl;
								//	DeleteDC(hdcMem);
								//	DeleteObject(hBitmap);
								//	ReleaseDC(client.hWnd, hdcScreen);
								//}
								//GetDIBits(hdcScreen, hBitmap, 0, height, screenShot.data, (BITMAPINFO*)&bi, DIB_RGB_COLORS);
								//cv::cvtColor(screenShot, screenShot, cv::COLOR_BGRA2BGR);
								//// Definir entrada para a rede neural
								//cv::Mat blob = cv::dnn::blobFromImage(screenShot, 1.0 / 255.0, cv::Size(640, 640), cv::Scalar(0, 0, 0), true, false);
								//net.setInput(screenShot);
								//// Realizar inferência
								//cv::Mat detection = net.forward();
								//// Processar as detecções e desenhar caixas delimitadoras
								//for (int i = 0; i < detection.rows; i++) {
								//	float confidence = detection.at<float>(i, 4);
								//	if (confidence > 0.3) {  // Defina o limite de confiança conforme necessário
								//		int x1 = static_cast<int>(detection.at<float>(i, 0) * width);
								//		int y1 = static_cast<int>(detection.at<float>(i, 1) * height);
								//		int x2 = static_cast<int>(detection.at<float>(i, 2) * width);
								//		int y2 = static_cast<int>(detection.at<float>(i, 3) * height);
								//		// Desenhar caixa delimitadora
								//		cv::rectangle(screenShot, cv::Point(x1, y1), cv::Point(x2, y2), cv::Scalar(0, 255, 0), 2);
								//	}
								//}
								//// Exibir a imagem com as detecções
								//cv::imshow("Detected Objects", screenShot);
								//cv::waitKey(0);
								//DeleteDC(hdcMem);
								//DeleteObject(hBitmap);
								//ReleaseDC(client.hWnd, hdcScreen);

								POINT pos{};
								RECT rect;
								GetClientRect(client.hWnd, &rect);
								int width = rect.right;
								int height = rect.bottom;

								HDC hdcScreen = GetDC(client.hWnd);
								if (!hdcScreen) {
									throw std::runtime_error("Failed to get screen device context (hdcScreen is null)");
								}

								HDC hdcMem = CreateCompatibleDC(hdcScreen);
								if (!hdcMem) {
									ReleaseDC(client.hWnd, hdcScreen); // Liberar hdcScreen antes de lançar a exceção
									throw std::runtime_error("Failed to create compatible device context (hdcMem is null)");
								}

								HBITMAP hBitmap = CreateCompatibleBitmap(hdcScreen, width, height);
								if (!hBitmap) {
									ReleaseDC(client.hWnd, hdcScreen); // Liberar hdcScreen e hdcMem antes de lançar a exceção
									DeleteDC(hdcMem);
									throw std::runtime_error("Failed to create compatible bitmap (hBitmap is null)");
								}

								SelectObject(hdcMem, hBitmap);

								BitBlt(hdcMem, 0, 0, width, height, hdcScreen, 0, 0, SRCCOPY);

								BITMAPINFOHEADER bi;
								bi.biSize = sizeof(BITMAPINFOHEADER);
								bi.biWidth = width;
								bi.biHeight = -height;
								bi.biPlanes = 1;
								bi.biBitCount = 24;
								bi.biCompression = BI_RGB;
								bi.biSizeImage = 0;
								bi.biXPelsPerMeter = 0;
								bi.biYPelsPerMeter = 0;
								bi.biClrUsed = 0;
								bi.biClrImportant = 0;

								cv::Mat screenShot;
								try
								{
									screenShot = cv::Mat(height, width, CV_8UC3);
								}
								catch (const cv::Exception& e)
								{
									std::cout << e.what() << std::endl;
									DeleteDC(hdcMem);
									DeleteObject(hBitmap);
									ReleaseDC(client.hWnd, hdcScreen);
								}

								GetDIBits(hdcScreen, hBitmap, 0, height, screenShot.data, (BITMAPINFO*)&bi, DIB_RGB_COLORS);

								for (auto& image : Images) {
									if (image.second != ImageType::street) {
										continue;
									}

									cv::Mat subImage = cv::imread(image.first);

									if (subImage.empty()) {
										continue;
									}

									int resultCols{};
									int resultRows{};
									cv::Mat resultImage;
									try
									{
										resultCols = screenShot.cols - subImage.cols + 1;
										resultRows = screenShot.rows - subImage.rows + 1;
										resultImage = cv::Mat(resultRows, resultCols, CV_32FC1);
									}
									catch (const cv::Exception& e)
									{
										std::cout << e.what() << std::endl;
										continue;
									}
									catch (const std::exception& e)
									{
										std::cout << e.what() << std::endl;
										continue;
									}

									cv::Mat thresholdedImage;
									std::vector<cv::Point> locations;
									try
									{
										cv::matchTemplate(screenShot, subImage, resultImage, cv::TM_CCOEFF_NORMED);
										cv::threshold(resultImage, thresholdedImage, 0.5, 1.0, cv::THRESH_TOZERO);
										cv::findNonZero(thresholdedImage, locations);
									}
									catch (const cv::Exception& e)
									{
										std::cout << e.what() << std::endl;
										continue;
									}

									try
									{
										if (locations.size() > 0) {
											for (const auto& loc : locations) {
												cv::rectangle(screenShot, loc, cv::Point(loc.x + subImage.cols, loc.y + subImage.rows), cv::Scalar(0, 255, 0), 2);
												int centerX = loc.x + subImage.cols / 2;
												int centerY = loc.y + subImage.rows / 2;
												cv::Point center(centerX, centerY);
												cv::circle(screenShot, center, 5, cv::Scalar(0, 0, 255), -1);
											}
										}
									}
									catch (const cv::Exception& e)
									{
										std::cout << e.what() << std::endl;
										continue;
									}
									catch (const std::exception& e)
									{
										std::cout << e.what() << std::endl;
										continue;
									}
									DeleteDC(hdcMem);
									DeleteObject(hBitmap);
									ReleaseDC(client.hWnd, hdcScreen);
								}
								try
								{
									cv::namedWindow("Debug", cv::WINDOW_NORMAL);
									cv::imshow("Debug", screenShot);

									// Pressione 'q' para sair do loop
									if (cv::waitKey(1) == 'q') {

									}

								}
								catch (const cv::Exception& e)
								{
									std::cerr << "Exceção capturada: " << e.what() << std::endl;
								}
								catch (const std::exception& e)
								{
									std::cout << e.what() << std::endl;
								}

							}


							if (client.fishingWaipont.size())
								ImGui::Text("FishingIndex: %d \\ FishingPos: X:%d  Y:%d", client.fishingIndex, client.fishingWaipont[client.fishingIndex].Pos.x, client.fishingWaipont[client.fishingIndex].Pos.y);
							ImGui::Combo("MapperType", &mapperIndex, mapperTypeIndex, IM_ARRAYSIZE(mapperTypeIndex));

							if (ImGui::Button("Clear")) {
								mapper.waipoint.clear();
								mapper.maxIndex = -1;
							}

							ImGui::SameLine();

							if (ImGui::Button("Add")) {
								mapper.maxIndex++;
								Waipoint map;
								map.Pos = Vector3(
									client.read<int>(client.BaseAddress + 0x27CCC1C),
									client.read<int>(client.BaseAddress + 0x27CCC20),
									client.read<int>(client.BaseAddress + 0x27CC498));

								map.index = mapper.maxIndex;

								switch (mapperIndex)
								{
								case 0: {
									map.mapperType = MapperType::Wood;
									break;
								}
								case 1: {
									map.mapperType = MapperType::WalkWood;
									break;
								}
								case 2: {
									map.mapperType = MapperType::Fishi;
									break;
								}
								case 3: {
									map.mapperType = MapperType::WalkFishi;
									break;
								}
								case 4: {
									map.mapperType = MapperType::npc;
									break;
								}
								case 5: {
									map.mapperType = MapperType::Ore;
									break;
								}
								case 6: {
									map.mapperType = MapperType::OreWalk;
									break;
								}
								default:
									break;
								}

								mapper.waipoint.push_back(map);
							}

							ImGui::SameLine();

							if (ImGui::Button("Save")) {
								client.bShowSavePopup = true;
							}

							if (client.bShowSavePopup) {
								ImGui::OpenPopup("Save Popup");
								client.bShowSavePopup = false;
							}

							if (ImGui::BeginPopup("Save Popup")) {
								static char	Filename[120];
								static int FarmType{ 0 };

								ImGui::InputText("File Name", Filename, IM_ARRAYSIZE(Filename));
								ImGui::Combo("Farm Type", &FarmType, mapperTypeIndex, IM_ARRAYSIZE(mapperTypeIndex));

								if (ImGui::Button("Save")) {

									std::string file{ Filename };
									std::string FarmTypeFile{};
									switch (FarmType)
									{
									case 0:
									case 1: {
										FarmTypeFile = "Wood";
										break;
									}
									case 2:
									case 3: {
										FarmTypeFile = "Fishi";
										break;
									}
									case 5:
									case 6: {
										FarmTypeFile = "Ore";
										break;
									}

									default:
										break;
									}
									std::ofstream json_account("C:\\RavendawnBot\\Farms\\" + FarmTypeFile + "\\" + file + ".json");

									nlohmann::json json;
									json = mapper.waipoint;
									json_account << std::setw(4) << json << std::endl;

									json_account.close();
								}

								ImGui::EndPopup();
							}

							for (auto& mapper_ : mapper.waipoint) {
								switch (mapperIndex)
								{
								case 0:
								case 1: {
									if (!(mapper_.mapperType == MapperType::WalkWood || mapper_.mapperType == MapperType::Wood)) {
										continue;
									}
									break;
								}

								case 2:
								case 3: {
									if (!(mapper_.mapperType == MapperType::Fishi || mapper_.mapperType == MapperType::WalkFishi)) {
										continue;
									}
									break;
								}

								case 5:
								case 6: {
									if (!(mapper_.mapperType == MapperType::OreWalk || mapper_.mapperType == MapperType::Ore)) {
										continue;
									}
									break;
								}

								default:
									break;
								}
								
								std::string typ;
								switch (mapper_.mapperType)
								{
								case 0: {
									typ = "Wood";
									break;
								}
								case 1: {
									typ = "WalkWood";
									break;
								}
								case 2: {
									typ = "Fishi";
									break;
								}
								case 3: {
									typ = "WalkFishi";
									break;
								}
								case 4: {
									typ = "npc";
									break;
								}
								case 5: {
									typ = "ore";
									break;
								}
								case 6: {
									typ = "OreWal";
									break;
								}
								default:
									break;
								}

								ImGui::Text("MapperType: %s MapperXYZ: %d %d %d", typ.c_str(), mapper_.Pos.x, mapper_.Pos.y, mapper_.Pos.z);
								ImGui::SameLine();
								ImGui::Button("Edit");
								//ImGui::SameLine();
							}
						}
						else {
							if (ImGui::Button("Open Game")) {
								std::string dir{ "C:\\RavendawnBot\\accounts\\" + client.Login };

								for (const auto& entry : std::filesystem::directory_iterator(dir)) {
									const auto& path = entry.path();
									if (std::filesystem::is_regular_file(path)) {
										if (path.filename().extension().string() == ".exe") {
											system(path.string().c_str());
										}
									}
								}

								//std::this_thread::sleep_for(std::chrono::seconds(50));
								////570 244 login input
								//PostMessage(client.hWnd, WM_MOUSEMOVE, 0, MAKELPARAM(570, 244));
								//std::this_thread::sleep_for(std::chrono::milliseconds(5));
								//PostMessage(client.hWnd, WM_LBUTTONDOWN, MK_LBUTTON, 0);
								//PostMessage(client.hWnd, WM_LBUTTONUP, MK_LBUTTON, 0);

								//SendText(client.hWnd, client.account.Login, true);

								////599 293 pass input
								//PostMessage(client.hWnd, WM_MOUSEMOVE, 0, MAKELPARAM(599, 293));
								//std::this_thread::sleep_for(std::chrono::milliseconds(5));
								//PostMessage(client.hWnd, WM_LBUTTONDOWN, MK_LBUTTON, 0);
								//PostMessage(client.hWnd, WM_LBUTTONUP, MK_LBUTTON, 0);

								//SendText(client.hWnd, client.account.Pass, true);

								////574 488 first server selected
								//PostMessage(client.hWnd, WM_MOUSEMOVE, 0, MAKELPARAM(574, 488));
								//std::this_thread::sleep_for(std::chrono::milliseconds(5));
								//PostMessage(client.hWnd, WM_LBUTTONDOWN, MK_LBUTTON, 0);
								//PostMessage(client.hWnd, WM_LBUTTONUP, MK_LBUTTON, 0);

								////751 601 Enter world
								//PostMessage(client.hWnd, WM_MOUSEMOVE, 0, MAKELPARAM(751, 601));
								//std::this_thread::sleep_for(std::chrono::milliseconds(5));
								//PostMessage(client.hWnd, WM_LBUTTONDOWN, MK_LBUTTON, 0);
								//PostMessage(client.hWnd, WM_LBUTTONUP, MK_LBUTTON, 0);
							}
						}








						ImGui::PopStyleColor();
					}ImGui::EndChild();
				}
			}




			ImGui::End();
		}

		// Rendering
		ImGui::EndFrame();
		g_pd3dDevice->SetRenderState(D3DRS_ZENABLE, FALSE);
		g_pd3dDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
		g_pd3dDevice->SetRenderState(D3DRS_SCISSORTESTENABLE, FALSE);
		D3DCOLOR clear_col_dx = D3DCOLOR_RGBA((int)(clear_color.x * clear_color.w * 255.0f), (int)(clear_color.y * clear_color.w * 255.0f), (int)(clear_color.z * clear_color.w * 255.0f), (int)(clear_color.w * 255.0f));
		g_pd3dDevice->Clear(0, nullptr, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, clear_col_dx, 1.0f, 0);
		if (g_pd3dDevice->BeginScene() >= 0)
		{
			ImGui::Render();
			ImGui_ImplDX9_RenderDrawData(ImGui::GetDrawData());
			g_pd3dDevice->EndScene();
		}
		HRESULT result = g_pd3dDevice->Present(nullptr, nullptr, nullptr, nullptr);

		// Handle loss of D3D9 device
		if (result == D3DERR_DEVICELOST && g_pd3dDevice->TestCooperativeLevel() == D3DERR_DEVICENOTRESET)
			ResetDevice();
	}

	ImGui_ImplDX9_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();

	CleanupDeviceD3D();
	::DestroyWindow(hwnd);
	::UnregisterClassW(wc.lpszClassName, wc.hInstance);
}

LRESULT CALLBACK LowLevelKeyboardProc(int nCode, WPARAM wParam, LPARAM lParam)
{
	KBDLLHOOKSTRUCT* pKeyBoard = (KBDLLHOOKSTRUCT*)lParam;

	HWND hWnd = GetForegroundWindow();
	DWORD pid;
	GetWindowThreadProcessId(hWnd, &pid); // Obter o PID associado à janela

	POINT cursorPos{};
	switch (wParam)
	{
	case WM_KEYDOWN:
	{
		movements.push_back({ pid, false, false, pKeyBoard->vkCode, 0, NULL });
		break;

	case WM_KEYUP:
		movements.push_back({ pid, false, true, pKeyBoard->vkCode, 0, NULL });
		break;

	case WM_SYSKEYDOWN:
		movements.push_back({ pid, true, false, pKeyBoard->vkCode, 0, NULL });
		break;

	case WM_SYSKEYUP:
		movements.push_back({ pid, true, true, pKeyBoard->vkCode, 0, NULL });
		break;

	default:
		return CallNextHookEx(NULL, nCode, wParam, lParam);
	}
	}
	return 0;
}

LRESULT CALLBACK LowLevelMouseProc(int nCode, WPARAM wParam, LPARAM lParam) {
	HWND hWnd = GetForegroundWindow();
	DWORD pid;
	GetWindowThreadProcessId(hWnd, &pid); // Obter o PID associado à janela

	POINT cursorPos{};
	switch (wParam)
	{
	case WM_LBUTTONDOWN:
		GetCursorPos(&cursorPos);
		ScreenToClient(hWnd, &cursorPos);
		movements.push_back({ pid, false, false, NULL, cv::EVENT_LBUTTONDOWN, cursorPos });

		break;

	case WM_RBUTTONDOWN:
		GetCursorPos(&cursorPos);
		ScreenToClient(hWnd, &cursorPos);
		movements.push_back({ pid, false, false, NULL, cv::EVENT_RBUTTONDOWN, cursorPos });
		break;

	default:
		return CallNextHookEx(NULL, nCode, wParam, lParam);
	}

	return 0;
}

HHOOK hKeyHook;
//HHOOK hMouseHook;

DWORD WINAPI KeyLogger(LPVOID lpParameter)
{
	HINSTANCE hExe = GetModuleHandle(NULL);
	if (hExe == NULL)
	{
		return 1;
	}
	else
	{
		hKeyHook = SetWindowsHookEx(WH_KEYBOARD_LL, (HOOKPROC)LowLevelKeyboardProc, hExe, 0);
		//hMouseHook = SetWindowsHookEx(WH_MOUSE_LL, (HOOKPROC)LowLevelMouseProc, hExe, 0);
		MSG msg;
		while (GetMessage(&msg, NULL, 0, 0) != 0)
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}
	return 0;
}

//DWORD WINAPI MouseLogger(LPVOID lpParameter)
//{
//	HHOOK hMouseHook;
//	HINSTANCE hExe = GetModuleHandle(NULL);
//	if (hExe == NULL)
//	{
//		return 1;
//	}
//	else
//	{
//		hMouseHook = SetWindowsHookEx(WH_MOUSE_LL, (HOOKPROC)LowLevelMouseProc, hExe, 0);
//		MSG msg;
//		while (GetMessage(&msg, NULL, 0, 0) != 0)
//		{
//			TranslateMessage(&msg);
//			DispatchMessage(&msg);
//		}
//		UnhookWindowsHookEx(hMouseHook);
//	}
//	return 0;
//}

int StartKeyLogging()
{
	HANDLE hThread;
	DWORD dwThread;
	hThread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)KeyLogger, NULL, 0, NULL);
	if (hThread)
	{
		return WaitForSingleObject(hThread, IGNORE);
	}
	else
	{
		return 1;
	}
}

bool bDisableAllSync{ false };

int main(int, char**)
{
	UpdateAccounts();
	std::thread(gui).detach();
	std::thread(cvWindows).detach();

	//StartKeyLogging();
	//StartCvWindows();

	while (!guiWhile)
	{

		std::this_thread::sleep_for(std::chrono::milliseconds(100));
	}

	//std::this_thread::sleep_for(std::chrono::seconds(10));
	//cv::destroyAllWindows();

	//UnhookWindowsHookEx(hKeyHook);
	return 0;
}

bool CreateDeviceD3D(HWND hWnd)
{
	if ((g_pD3D = Direct3DCreate9(D3D_SDK_VERSION)) == nullptr)
		return false;

	// Create the D3DDevice
	ZeroMemory(&g_d3dpp, sizeof(g_d3dpp));
	g_d3dpp.Windowed = TRUE;
	g_d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
	g_d3dpp.BackBufferFormat = D3DFMT_UNKNOWN; // Need to use an explicit format with alpha if needing per-pixel alpha composition.
	g_d3dpp.EnableAutoDepthStencil = TRUE;
	g_d3dpp.AutoDepthStencilFormat = D3DFMT_D16;
	g_d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_ONE;           // Present with vsync
	//g_d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;   // Present without vsync, maximum unthrottled framerate
	if (g_pD3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd, D3DCREATE_HARDWARE_VERTEXPROCESSING, &g_d3dpp, &g_pd3dDevice) < 0)
		return false;

	return true;
}

void CleanupDeviceD3D()
{
	if (g_pd3dDevice) { g_pd3dDevice->Release(); g_pd3dDevice = nullptr; }
	if (g_pD3D) { g_pD3D->Release(); g_pD3D = nullptr; }
}

void ResetDevice()
{
	ImGui_ImplDX9_InvalidateDeviceObjects();
	HRESULT hr = g_pd3dDevice->Reset(&g_d3dpp);
	if (hr == D3DERR_INVALIDCALL)
		IM_ASSERT(0);
	ImGui_ImplDX9_CreateDeviceObjects();
}

// Forward declare message handler from imgui_impl_win32.cpp
extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

// Win32 message handler
// You can read the io.WantCaptureMouse, io.WantCaptureKeyboard flags to tell if dear imgui wants to use your inputs.
// - When io.WantCaptureMouse is true, do not dispatch mouse input data to your main application, or clear/overwrite your copy of the mouse data.
// - When io.WantCaptureKeyboard is true, do not dispatch keyboard input data to your main application, or clear/overwrite your copy of the keyboard data.
// Generally you may always pass all inputs to dear imgui, and hide them from your application based on those two flags.
LRESULT WINAPI WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	if (ImGui_ImplWin32_WndProcHandler(hWnd, msg, wParam, lParam))
		return true;

	switch (msg)
	{
	case WM_SIZE:
		if (wParam == SIZE_MINIMIZED)
			return 0;
		g_ResizeWidth = (UINT)LOWORD(lParam); // Queue resize
		g_ResizeHeight = (UINT)HIWORD(lParam);
		return 0;
	case WM_SYSCOMMAND:
		if ((wParam & 0xfff0) == SC_KEYMENU) // Disable ALT application menu
			return 0;
		break;
	case WM_DESTROY:
		::PostQuitMessage(0);
		return 0;

	case WM_CLOSE:
		//exit(0);
		::PostQuitMessage(0);
		return 0;
	}
	return ::DefWindowProcW(hWnd, msg, wParam, lParam);
}
