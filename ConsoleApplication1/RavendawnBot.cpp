
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

#include <filesystem>


class Process {
public:
	DWORD PID{ 0 };
	HWND hWnd{ NULL };

	class Account {
	public:
		std::string Login{ NULL };
		std::string Email{ NULL };
		std::string Pass{ NULL };

		bool logged{ false };

		bool bSyncMovement{ false };

		bool bDebug{ false };
		std::chrono::steady_clock::time_point lastDebug;

		class CharPerson {
		public:
			std::string Nick{ NULL };
		};

		CharPerson charPerson;

		// Operador de inserção para converter objeto da classe para JSON
		friend void to_json(nlohmann::json& j, const Account& account) {
			j = nlohmann::json{ {"Login", account.Login }, {"Pass", account.Pass},
				{"Email" , account.Email}, {"Nick" , account.charPerson.Nick} };
		}

		// Operador de extração para converter JSON para objeto da classe
		friend void from_json(const nlohmann::json& j, Account& account) {
			j.at("Login").get_to(account.Login);
			j.at("Pass").get_to(account.Pass);
			j.at("Email").get_to(account.Email);
			j.at("Nick").get_to(account.charPerson.Nick);
		}

		bool bAutoAttack{ false };

	};

	Account account;
};

inline std::vector<Process> Clients;

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

	int size = json["Accounts"].size();

	char* buffer{ nullptr };
	size_t sz{ 0 };
	if (_dupenv_s(&buffer, &sz, "USERPROFILE") == 0 && buffer != nullptr)
	{

	}

	std::string sourceDir{ (std::string)(buffer)+"\\AppData\\Roaming\\Ravendawn\\ravendawn" };
	for (int i = 0; i < size; i++) {
		std::string name{ json["Accounts"][i]["Login"] };
		std::string nick{ json["Accounts"][i]["Nick"] };
		std::cout << "Login: " << name << " Nick: " << nick << std::endl;

		std::string processName{ "Ravendawn - " + nick };

		Process proces;

		proces.hWnd = FindWindowEx(0, 0, 0, processName.c_str());
		GetWindowThreadProcessId(proces.hWnd, &proces.PID);
		proces.account = json["Accounts"][i];

		if (proces.hWnd != NULL) {
			proces.account.logged = true;
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


// Data
static LPDIRECT3D9              g_pD3D = nullptr;
static LPDIRECT3DDEVICE9        g_pd3dDevice = nullptr;
static UINT                     g_ResizeWidth = 0, g_ResizeHeight = 0;
static D3DPRESENT_PARAMETERS    g_d3dpp = {};

// Forward declarations of helper functions
bool CreateDeviceD3D(HWND hWnd);
void CleanupDeviceD3D();
void ResetDevice();
LRESULT WINAPI WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);


bool guiWhile{ false };
POINT lastMousePosition{ NULL };
void onMouse(int event, int x, int y, int flags, void* userdata) {
	Process* params = static_cast<Process*>(userdata);

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

std::vector<std::string> Images{/*"C:\\RavendawnBot\\axie icon0.png",
								"C:\\RavendawnBot\\axie icon1.png",
								"C:\\RavendawnBot\\axie icon2.png",*/
								//"C:\\RavendawnBot\\green bar 0.png",
								//"C:\\RavendawnBot\\green bar 1.png"
								//"C:\\RavendawnBot\\green bar 2.png",
								//"C:\\RavendawnBot\\green bar 3.png",
								"C:\\RavendawnBot\\street.png",
								"C:\\RavendawnBot\\street2.png",
								"C:\\RavendawnBot\\street3.png",
								"C:\\RavendawnBot\\street4.png",
								"C:\\RavendawnBot\\street5.png",
								"C:\\RavendawnBot\\street6.png",
								"C:\\RavendawnBot\\street7.png",
								"C:\\RavendawnBot\\street8.png"
};

void cvWindows() {
	cv::destroyAllWindows();
	while (!guiWhile)
	{
		for (auto& client : Clients) {

			try
			{

				if (client.account.bDebug) {
					//auto currentTime = std::chrono::steady_clock::now();
					//auto elapsedTime = std::chrono::duration_cast<std::chrono::milliseconds>(currentTime - client.account.lastDebug).count();

					//if (elapsedTime > 1) {

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

					//553 255 
					//727 279

					//int posX = 553;
					//int posY = 55;
					//int sizeX = 727 - posX;
					//int sizeY = 279 - posY;



					//posX = std::max(0, std::min(posX, width - 1));
					//posY = std::max(0, std::min(posY, height - 1));
					//sizeX = std::min(sizeX, width - posX);
					//sizeY = std::min(sizeY, height - posY);

					//// Cortar a área específica
					//cv::Rect roi(posX, posY, sizeX, sizeY);
					//cv::Mat croppedImage = screenShot(roi).clone();  // Clone para garantir uma cópia independente

					//// Selecionar uma área específica e tamanho
					//int startX = width / 2 - 200/* coordenada X inicial */;
					//int startY = height / 2 - 200/* coordenada Y inicial */;
					//int cropWidth = 600/* largura da área de corte */;
					//int cropHeight = 400/* altura da área de corte */;
					// Garantir que a área de corte esteja dentro dos limites da imagem
					/*startX = std::max(0, std::min(startX, width - 1));
					startY = std::max(0, std::min(startY, height - 1));
					cropWidth = std::min(cropWidth, width - startX);
					cropHeight = std::min(cropHeight, height - startY);*/
					// Cortar a área específica
					//cv::Rect roi(startX, startY, cropWidth, cropHeight);
					//cv::Mat croppedImage = screenShot(roi).clone();  // Clone para garantir uma cópia independente

					for (auto& image : Images) {
						cv::Mat subImage = cv::imread(image);

						// Verifica se a subimagem foi carregada corretamente
						if (subImage.empty()) {
							continue;
						}

						int resultCols = screenShot.cols - subImage.cols + 1;
						int resultRows = screenShot.rows - subImage.rows + 1;
						cv::Mat resultImage;
						try
						{
							resultImage = cv::Mat(resultRows, resultCols, CV_32FC1);
						}
						catch (const cv::Exception&)
						{
							continue;
						}

						// Realiza a correspondência da subimagem na imagem base
						cv::matchTemplate(screenShot, subImage, resultImage, cv::TM_CCOEFF_NORMED);

						// Define um limiar para a correspondência
						//double threshold = 0.6;
						cv::Mat thresholdedImage;
						cv::threshold(resultImage, thresholdedImage, 0.3, 1.0, cv::THRESH_TOZERO);

						// Encontra as correspondências com alta confiança
						std::vector<cv::Point> locations;
						cv::findNonZero(thresholdedImage, locations);

						//if (locations.size() > 0) {
						//	//PostMessage(client.hWnd, WM_KEYDOWN, 0x46, 0);
						//	//std::this_thread::sleep_for(std::chrono::milliseconds(300));
						//	//PostMessage(client.hWnd, WM_KEYUP, 0x46, 0);
						//	break;
						//}

						for (const auto& loc : locations) {
							cv::rectangle(screenShot, loc, cv::Point(loc.x + subImage.cols, loc.y + subImage.rows), cv::Scalar(0, 255, 0), 2);
						}

						{
							//cv::Mat subImage = cv::imread(image);

							//// Verifica se a subimagem foi carregada corretamente
							//if (subImage.empty()) {
							//	continue;
							//}

							//int resultCols = croppedImage.cols - subImage.cols + 1;
							//int resultRows = croppedImage.rows - subImage.rows + 1;
							//cv::Mat resultImage;
							//try
							//{
							//	resultImage = cv::Mat(resultRows, resultCols, CV_32FC1);
							//}
							//catch (const cv::Exception&)
							//{
							//	continue;
							//}

							//// Realiza a correspondência da subimagem na imagem base
							//cv::matchTemplate(croppedImage, subImage, resultImage, cv::TM_CCOEFF_NORMED);

							//// Define um limiar para a correspondência
							////double threshold = 0.6;
							//cv::Mat thresholdedImage;
							//cv::threshold(resultImage, thresholdedImage, 0.8, 1.0, cv::THRESH_TOZERO);

							//// Encontra as correspondências com alta confiança
							//std::vector<cv::Point> locations;
							//cv::findNonZero(thresholdedImage, locations);

							//if (locations.size() > 0) {
							//	//PostMessage(client.hWnd, WM_KEYDOWN, 0x46, 0);
							//	//std::this_thread::sleep_for(std::chrono::milliseconds(300));
							//	//PostMessage(client.hWnd, WM_KEYUP, 0x46, 0);
							//	break;
							//}

							///*for (const auto& loc : locations) {
							//	cv::rectangle(croppedImage, loc, cv::Point(loc.x + subImage.cols, loc.y + subImage.rows), cv::Scalar(0, 255, 0), 2);
							//}*/

						}
					}


					try
					{
						cv::namedWindow(client.account.charPerson.Nick, cv::WINDOW_NORMAL);
						//cv::setWindowProperty(client.account.charPerson.Nick, cv::WND_PROP_TOPMOST, 1);
						cv::imshow(client.account.charPerson.Nick, screenShot);
						cv::setMouseCallback(client.account.charPerson.Nick, onMouse, &client);

						// Pressione 'q' para sair do loop
						if (cv::waitKey(1) == 'q') {
							DeleteDC(hdcMem);
							DeleteObject(hBitmap);
							ReleaseDC(client.hWnd, hdcScreen);
							break;
						}

					}
					catch (const cv::Exception& e)
					{
						std::cerr << "Exceção capturada: " << e.what() << std::endl;
						DeleteDC(hdcMem);
						DeleteObject(hBitmap);
						ReleaseDC(client.hWnd, hdcScreen);
					}

					DeleteDC(hdcMem);
					DeleteObject(hBitmap);
					ReleaseDC(client.hWnd, hdcScreen);

					client.account.lastDebug = std::chrono::steady_clock::now();
					//}
				}
			}
			catch (const std::exception& e)
			{
				std::cerr << "Exceção capturada: " << e.what() << std::endl;

			}
			catch (const cv::Exception& e) {
				std::cerr << "Exceção capturada: " << e.what() << std::endl;

			}
		}
	}
}

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

				if (ImGui::MenuItem("Add account")) {
					std::ofstream json_account("C:\\Ravendawn accounts.json");

					Process::Account account;

					account.charPerson.Nick = "Rapha4lx";
					account.Email = "rapha4lx@gmail.com";
					account.Pass = "33165201r";
					account.Login = "rapha4lx";

					nlohmann::json json;
					json["Accounts"].push_back(account);

					json_account << std::setw(4) << json << std::endl;
					json_account.close();
				}

				ImGui::EndPopup();
			}

			if (Clients.size()) {
				for (auto& client : Clients) {
					ImGui::BeginChild(client.account.Login.c_str(), ImVec2(600, 200)); {
						ImGui::Text(client.account.Login.c_str());
						ImVec2 textSize = ImGui::CalcTextSize(client.account.Login.c_str());
						ImGui::SameLine(textSize.x + 20.20);

						if (client.account.logged) {
							ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0, 1, 0, 1));
						}
						else
						{
							ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1, 0, 0, 1));
						}

						ImGui::Text(client.account.logged ? "on" : "off");

						if (client.account.logged) {
							ImGui::Checkbox("bDebug", &client.account.bDebug);
							ImGui::Checkbox("SyncMovement", &client.account.bSyncMovement);
							ImGui::Checkbox("AutoAttack", &client.account.bAutoAttack);

							if (client.account.bAutoAttack) {
								PostMessage(client.hWnd, WM_KEYDOWN, VK_TAB, MAKELPARAM(1, KF_REPEAT));
								std::this_thread::sleep_for(std::chrono::milliseconds(300));
								PostMessage(client.hWnd, WM_KEYUP, VK_TAB, 0);

								PostMessage(client.hWnd, WM_KEYDOWN, VK_NUMPAD1, MAKELPARAM(1, KF_REPEAT));
								std::this_thread::sleep_for(std::chrono::milliseconds(300));
								PostMessage(client.hWnd, WM_KEYUP, VK_NUMPAD1, 0);

								PostMessage(client.hWnd, WM_KEYDOWN, 0x41, MAKELPARAM(1, KF_REPEAT));
								std::this_thread::sleep_for(std::chrono::milliseconds(300));
								PostMessage(client.hWnd, WM_KEYUP, 0x41, 0);

								PostMessage(client.hWnd, WM_KEYDOWN, 0x44, MAKELPARAM(1, KF_REPEAT));
								std::this_thread::sleep_for(std::chrono::milliseconds(300));
								PostMessage(client.hWnd, WM_KEYUP, 0x44, 0);

							}

							POINT cursorPos{};
							GetCursorPos(&cursorPos);

							ScreenToClient(client.hWnd, &cursorPos);

							RECT windowRect{  };
							GetWindowRect(client.hWnd, &windowRect);

							ImGui::Text("X: %d \\ Y: %d", cursorPos.x, cursorPos.y);
							ImGui::Text("WindowPosX: %d \\ WindowPosY: %d", windowRect.left, windowRect.top);
						}
						else {
							if (ImGui::Button("Loggin")) {
								std::string dir{ "C:\\RavendawnBot\\accounts\\" + client.account.Login };

								for (const auto& entry : std::filesystem::directory_iterator(dir)) {
									const auto& path = entry.path();
									if (std::filesystem::is_regular_file(path)) {
										if (path.filename().extension().string() == ".exe") {
											system(path.string().c_str());
										}
									}
								}

								std::this_thread::sleep_for(std::chrono::seconds(50));
								//570 244 login input
								PostMessage(client.hWnd, WM_MOUSEMOVE, 0, MAKELPARAM(570, 244));
								std::this_thread::sleep_for(std::chrono::milliseconds(5));
								PostMessage(client.hWnd, WM_LBUTTONDOWN, MK_LBUTTON, 0);
								PostMessage(client.hWnd, WM_LBUTTONUP, MK_LBUTTON, 0);

								SendText(client.hWnd, client.account.Login, true);

								//599 293 pass input
								PostMessage(client.hWnd, WM_MOUSEMOVE, 0, MAKELPARAM(599, 293));
								std::this_thread::sleep_for(std::chrono::milliseconds(5));
								PostMessage(client.hWnd, WM_LBUTTONDOWN, MK_LBUTTON, 0);
								PostMessage(client.hWnd, WM_LBUTTONUP, MK_LBUTTON, 0);

								SendText(client.hWnd, client.account.Pass, true);

								//574 488 first server selected
								PostMessage(client.hWnd, WM_MOUSEMOVE, 0, MAKELPARAM(574, 488));
								std::this_thread::sleep_for(std::chrono::milliseconds(5));
								PostMessage(client.hWnd, WM_LBUTTONDOWN, MK_LBUTTON, 0);
								PostMessage(client.hWnd, WM_LBUTTONUP, MK_LBUTTON, 0);

								//751 601 Enter world
								PostMessage(client.hWnd, WM_MOUSEMOVE, 0, MAKELPARAM(751, 601));
								std::this_thread::sleep_for(std::chrono::milliseconds(5));
								PostMessage(client.hWnd, WM_LBUTTONDOWN, MK_LBUTTON, 0);
								PostMessage(client.hWnd, WM_LBUTTONUP, MK_LBUTTON, 0);
							}
						}
						ImGui::PopStyleColor();

						//cvWindows(client);



					}ImGui::EndChild();


					//const char* ClientName = GetWindowTitle(client.hWnd);
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

	StartKeyLogging();
	//StartCvWindows();

	while (!guiWhile)
	{
		if (GetAsyncKeyState(VK_DELETE) < 0) {
			::PostQuitMessage(0);
		}

		if (GetAsyncKeyState(VK_PAUSE) < 0) {
			if (bDisableAllSync) {
				Beep(1000, 30);
				bDisableAllSync = false;
				Sleep(100);
			}
			else if (!bDisableAllSync) {
				Beep(1000, 30);
				Beep(1000, 30);
				bDisableAllSync = true;
				Sleep(100);
			}
		}

		for (auto& movement : movements) {

			if (bDisableAllSync) {
				movements.erase(movements.begin());
				break;
			}

			for (auto& client : Clients) {
				if (!client.account.logged) {
					//std::this_thread::sleep_for(std::chrono::milliseconds(1));
					continue;
				}

				if (!client.account.bSyncMovement) {
					continue;
				}

				if (GetAsyncKeyState(VK_CAPITAL) < 0) {
					if (movement.PID != client.PID) {
						continue;
					}

					if (movement.key != NULL) {
						if (!movement.keyUp) {
							PostMessage(client.hWnd, WM_KEYDOWN, movement.key, 0);
						}
						else
						{
							PostMessage(client.hWnd, WM_KEYUP, movement.key, 0);
						}
						continue;
					}

					switch (movement.event)
					{
					case cv::EVENT_LBUTTONDBLCLK: {
						SendMessage(client.hWnd, WM_MOUSEMOVE, 0, MAKELPARAM(movement.position.x, movement.position.y)); // x e y são as coordenadas do clique
						std::this_thread::sleep_for(std::chrono::milliseconds(3));
						SendMessage(client.hWnd, WM_LBUTTONDOWN, MK_LBUTTON, 0);
						SendMessage(client.hWnd, WM_LBUTTONUP, MK_LBUTTON, 0);
						break;
					}

					case cv::EVENT_LBUTTONDOWN: {
						SendMessage(client.hWnd, WM_LBUTTONDOWN, MK_LBUTTON, 0);
						break;
					}

					case cv::EVENT_LBUTTONUP: {
						SendMessage(client.hWnd, WM_LBUTTONUP, MK_LBUTTON, 0);
						break;
					}

					case cv::EVENT_RBUTTONDOWN: {
						SendMessage(client.hWnd, WM_MOUSEMOVE, 0, MAKELPARAM(movement.position.x, movement.position.y)); // x e y são as coordenadas do clique
						std::this_thread::sleep_for(std::chrono::milliseconds(3));
						SendMessage(client.hWnd, WM_RBUTTONDOWN, MK_RBUTTON, 0);
						SendMessage(client.hWnd, WM_RBUTTONUP, MK_RBUTTON, 0);
						break;
					}

					case cv::EVENT_MOUSEMOVE: {
						SendMessage(client.hWnd, WM_MOUSEMOVE, 0, MAKELPARAM(movement.position.x, movement.position.y)); // x e y são as coordenadas do clique
						std::this_thread::sleep_for(std::chrono::milliseconds(2));
						break;
					}

					case cv::EVENT_MOUSEWHEEL: {
						if (movement.position.y > 0) {
							SendMessage(client.hWnd, WM_MOUSEWHEEL, MAKEWPARAM(0, -WHEEL_DELTA), NULL);
						}
						else if (movement.position.y < 0) {
							SendMessage(client.hWnd, WM_MOUSEWHEEL, MAKEWPARAM(0, WHEEL_DELTA), NULL);
						}
						break;
					}

					default:
						break;
					}
				}

				if (movement.key != NULL) {
					if (!movement.keyUp) {
						PostMessage(client.hWnd, WM_KEYDOWN, movement.key, 0);
					}
					else
					{
						PostMessage(client.hWnd, WM_KEYUP, movement.key, 0);
					}
					continue;
				}

				switch (movement.event)
				{
				case cv::EVENT_LBUTTONDBLCLK: {
					SendMessage(client.hWnd, WM_MOUSEMOVE, 0, MAKELPARAM(movement.position.x, movement.position.y)); // x e y são as coordenadas do clique
					std::this_thread::sleep_for(std::chrono::milliseconds(3));
					SendMessage(client.hWnd, WM_LBUTTONDOWN, MK_LBUTTON, 0);
					SendMessage(client.hWnd, WM_LBUTTONUP, MK_LBUTTON, 0);
					break;
				}

				case cv::EVENT_LBUTTONDOWN: {
					SendMessage(client.hWnd, WM_LBUTTONDOWN, MK_LBUTTON, 0);
					break;
				}

				case cv::EVENT_LBUTTONUP: {
					SendMessage(client.hWnd, WM_LBUTTONUP, MK_LBUTTON, 0);
					break;
				}

				case cv::EVENT_RBUTTONDOWN: {
					SendMessage(client.hWnd, WM_MOUSEMOVE, 0, MAKELPARAM(movement.position.x, movement.position.y)); // x e y são as coordenadas do clique
					std::this_thread::sleep_for(std::chrono::milliseconds(3));
					SendMessage(client.hWnd, WM_RBUTTONDOWN, MK_RBUTTON, 0);
					SendMessage(client.hWnd, WM_RBUTTONUP, MK_RBUTTON, 0);
					break;
				}

				case cv::EVENT_MOUSEMOVE: {
					SendMessage(client.hWnd, WM_MOUSEMOVE, 0, MAKELPARAM(movement.position.x, movement.position.y)); // x e y são as coordenadas do clique
					std::this_thread::sleep_for(std::chrono::milliseconds(2));
					break;
				}

				case cv::EVENT_MOUSEWHEEL: {
					if (movement.position.y > 0) {
						SendMessage(client.hWnd, WM_MOUSEWHEEL, MAKEWPARAM(0, -WHEEL_DELTA), NULL);
					}
					else if (movement.position.y < 0) {
						SendMessage(client.hWnd, WM_MOUSEWHEEL, MAKEWPARAM(0, WHEEL_DELTA), NULL);
					}
					break;
				}

				default:
					break;
				}
			}
			movements.erase(movements.begin());
		}

		//std::this_thread::sleep_for(std::chrono::milliseconds(1));
	}

	UnhookWindowsHookEx(hKeyHook);
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
