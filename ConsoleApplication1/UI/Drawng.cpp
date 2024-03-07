#include "Drawing.h"
#include <Windows.h>
#include <iostream>

#include <vector>
#include <chrono>
#include <thread>

#include <TlHelp32.h>
#include <d3d9.h>
#include <d3dx9tex.h>
#pragma comment(lib, "d3d9.lib")



#include <nlohmann/json.hpp>
#include <fstream>
#include <opencv2/opencv.hpp>
#include <opencv2/dnn.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>

#include <filesystem>
#include <algorithm>



#include "../Client/Client.h"
#include "../Recognition/Recognition.h"
#include "../RavendawnBot.h"
#include <dwmapi.h>
#include <tchar.h>





LPCSTR Drawing::lpWindowName = "RadiationProject";
ImVec2 Drawing::vWindowSize = { 350, 400 };
ImGuiWindowFlags Drawing::WindowFlags = ImGuiWindowFlags_UnsavedDocument | ImGuiWindowFlags_NoResize;
bool Drawing::bDraw = true;

void Drawing::Active()
{
	bDraw = true;
}

bool Drawing::isActive()
{
	return bDraw == true;
}

void ListarArquivos(const std::string& pasta, Client& client, MapperType FarmType) {
	for (const auto& entry : std::filesystem::directory_iterator(pasta)) {
		if (std::filesystem::is_regular_file(entry)) {
			std::string nomeArquivo = entry.path().stem().string();
			if (ImGui::MenuItem(nomeArquivo.c_str())) {
				client.lastMapper = nomeArquivo;
				client.LoadWaipointConfig(nomeArquivo.c_str(), FarmType);
			}
		}
	}
}

void Drawing::Draw()
{
	if (isActive())
	{
		ImGui::SetNextWindowSize(vWindowSize, ImGuiCond_Once);
		ImGui::SetNextWindowBgAlpha(1.0f);
		ImGui::Begin(lpWindowName, &bDraw, WindowFlags);

		//static char inputPID[6];
		static char nick[30];
		//ImGui::InputText("Set PID", inputPID, IM_ARRAYSIZE(inputPID));
		ImGui::InputText("Set Nick", nick, IM_ARRAYSIZE(nick));

		if (ImGui::Button("Select")) {
			if (nick[0] == NULL || nick[0] == '\0') {
				return;
			}

			/*if (inputPID[0] == NULL || inputPID[0] == '\0') {
				continue;
			}*/

			if (!std::filesystem::exists("C:\\RavendawnBot")) {
				std::filesystem::create_directories("C:\\RavendawnBot");
			}

			if (!std::filesystem::exists("C:\\RavendawnBot\\accounts")) {
				std::filesystem::create_directories("C:\\RavendawnBot\\accounts");
			}


			char* buffer{ nullptr };
			size_t sz{ 0 };
			if (_dupenv_s(&buffer, &sz, "USERPROFILE") == 0 && buffer != nullptr)
			{

			}

			std::string sourceDir{ (std::string)(buffer)+"\\AppData\\Roaming\\Ravendawn\\ravendawn" };
			if (!std::filesystem::exists("C:\\RavendawnBot\\accounts\\" + static_cast<std::string>(nick))) {
				std::filesystem::create_directory("C:\\RavendawnBot\\accounts\\" + static_cast<std::string>(nick));
				std::this_thread::sleep_for(std::chrono::milliseconds(8));
				for (const auto& entry : std::filesystem::directory_iterator(sourceDir)) {
					const auto& path = entry.path();
					if (std::filesystem::is_regular_file(path)) {
						if (path.filename().extension().string() == ".dll" || path.filename().extension().string() == ".exe") {
							std::filesystem::copy_file(path, "C:\\RavendawnBot\\accounts\\" + static_cast<std::string>(nick) + "\\" + path.filename().string(), std::filesystem::copy_options::overwrite_existing);
							std::cout << "Copied: " << path.filename() << std::endl;
						}
					}
				}
			}

			std::string name = static_cast<std::string>(nick) + "\\" + nick + ".json";
			std::ifstream file("C:\\RavendawnBot\\accounts\\" + name);
			if (!file.is_open()) {
				nlohmann::json json;
				//json.push_back(client);

				json = client;

				std::ofstream ofile("C:\\RavendawnBot\\accounts\\" + name);
				ofile << std::setw(4) << json << std::endl;
				ofile.close();
			}
			else
			{
				nlohmann::json json;
				file >> json;

				client = json;
			}

			std::string processName{ "Ravendawn - " + static_cast<std::string>(nick) };

			client.hWnd = FindWindowEx(0, 0, 0, processName.c_str());
			if (client.hWnd) {
				client.logged = true;
				//client.open_handle();
				GetWindowThreadProcessId(client.hWnd, &client.PID);
				MoveWindow(client.hWnd, 0, 0, 0, 0, TRUE);
			}
			else
			{
				//MessageBox(0, "Janela nao foi encontrada", 0, 0);
				exit(0);
			}

		}

		ImGui::SameLine();

		if (ImGui::Button("Open Game")) {
			char* buffer{ nullptr };
			size_t sz{ 0 };
			if (_dupenv_s(&buffer, &sz, "USERPROFILE") == 0 && buffer != nullptr)
			{

			}

			std::string sourceDir{ (std::string)(buffer)+"\\AppData\\Roaming\\Ravendawn\\ravendawn" };
			if (!std::filesystem::exists("C:\\RavendawnBot\\accounts\\" + static_cast<std::string>(nick))) {
				std::filesystem::create_directory("C:\\RavendawnBot\\accounts\\" + static_cast<std::string>(nick));
				std::this_thread::sleep_for(std::chrono::milliseconds(8));
				for (const auto& entry : std::filesystem::directory_iterator(sourceDir)) {
					const auto& path = entry.path();
					if (std::filesystem::is_regular_file(path)) {
						if (path.filename().extension().string() == ".dll" || path.filename().extension().string() == ".exe") {
							std::filesystem::copy_file(path, "C:\\RavendawnBot\\accounts\\" + static_cast<std::string>(nick) + "\\" + path.filename().string(), std::filesystem::copy_options::overwrite_existing);
							std::cout << "Copied: " << path.filename() << std::endl;
						}
					}
				}
			}

			std::string dir{ "C:\\RavendawnBot\\accounts\\" + static_cast<std::string>(nick) };

			for (const auto& entry : std::filesystem::directory_iterator(dir)) {
				const auto& path = entry.path();
				if (std::filesystem::is_regular_file(path)) {
					if (path.filename().extension().string() == ".exe") {
						system(path.string().c_str());
					}
				}
			}
		}

		for (; client.logged;) {
			if (!client.handle) {
				break;
			}

			if (!client.getLocalPlayer()) {
				break;
			}

			if (ImGui::Button("Edit")) {
				client.bEditPerson = true;
			}

			if (client.bEditPerson) {
				ImGui::OpenPopup("Edit Popup");
				client.bEditPerson = false;
			}

			if (ImGui::BeginPopup("Edit Popup")) {

				for (int i = 0; i < 12; i++) {
					std::string skillNumber = "Skill " + std::to_string(i);

					ImGui::SliderFloat(skillNumber.c_str(), &client.charPerson.skills[i].cooldown, 0, 60, "%.1f");
					ImGui::Checkbox((skillNumber + " Usable?").c_str(), &client.charPerson.skills[i].bUsable);
					ImGui::Checkbox((skillNumber + " heal Skill?").c_str(), &client.charPerson.skills[i].healSkill);
				}

				if (ImGui::MenuItem("Save")) {
					std::ofstream json_account("C:\\RavendawnBot\\accounts\\"
						+ static_cast<std::string>(nick) + "\\"
						+ static_cast<std::string>(nick) + ".json");

					nlohmann::json json;
					//json.push_back(client);
					json = client;

					json_account << std::setw(4) << json << std::endl;
					json_account.close();
				}

				ImGui::EndPopup();
			}

#if defined (DEBUG) 
			POINT cursorPos{};
			GetCursorPos(&cursorPos);

			ScreenToClient(client.hWnd, &cursorPos);

			RECT windowRect{  };
			GetWindowRect(client.hWnd, &windowRect);

			ImGui::Text("X: %d \\ Y: %d", cursorPos.x /*- windowRect.right / 2*/, cursorPos.y /*- windowRect.bottom / 2*/);
			ImGui::Text("WindowPosX: %d \\ WindowPosY: %d", windowRect.left, windowRect.top);
			ImGui::Text("Health %lf", client.getHealth());
#endif // (DEBUG)

			if (ImGui::Checkbox("Auto Task", &client.bAutoTask)) {
				client.bAutoTask = true;
			}

			if (ImGui::Checkbox("Auto Attack", &client.bAutoAttack)) {
				client.bAutoAttack = true;
			}

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

					ListarArquivos("C:\\RavendawnBot\\Farms\\Fishi", client, MapperType::Fishi);

					if (ImGui::Button("Start AutoFishing")) {
						client.bAutoFishing = true;
					}

					ImGui::EndPopup();
				}
			}
			else
			{
				if (ImGui::Button("Stop AutoFishing")) {
					client.bAutoFishing = false;
					client.bFishingAutoMove = false;
					client.bAutoFishingPopup = false;
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
					}
					ImGui::EndPopup();
				}
			}
			else
			{
				if (ImGui::Button("Stop Wood Farm")) {
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
					}
					ImGui::EndPopup();
				}
			}
			else
			{
				if (ImGui::Button("Stop Ore Farm")) {
					client.bOreFarm = false;
					client.bAutoOreFarmPopup = false;
				}
			}

			if (!client.bCaveBot) {
				if (ImGui::Button("CaveBot")) {
					client.bCavebotPopup = true;
				}

				if (client.bCavebotPopup) {
					ImGui::OpenPopup("CaveBot");
					client.bCavebotPopup = false;
				}

				if (ImGui::BeginPopup("CaveBot")) {
					ListarArquivos("C:\\RavendawnBot\\Farms\\Cavebot", client, MapperType::caveWalk);
					if (ImGui::Button("Start CaveBot")) {
						client.bCaveBot = true;
					}
					ImGui::EndPopup();
				}
			}

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
					case 7:
					case 8: {
						FarmTypeFile = "Returning";
						break;
					}
					case 9: {
						FarmTypeFile = "Cave";
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

				case 7:
				case 8: {
					if (!(mapper_.mapperType == MapperType::returning || mapper_.mapperType == MapperType::returningEnd)) {
						continue;
					}
					break;
				}
				case 9: {
					if (!(mapper_.mapperType == MapperType::caveWalk)) {
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
				case 7: {
					typ = "returning";
					break;
				}
				case 8: {
					typ = "returningEnd";
					break;
				}
				case 9: {
					typ = "CaveWalk";
					break;
				}
				default:
					break;
				}

				ImGui::Text("MapperType: %s MapperXYZ: %d %d %d", typ.c_str(), mapper_.Pos.x, mapper_.Pos.y, mapper_.Pos.z);
				ImGui::SameLine();
				//ImGui::Button("Edit");
			}

			std::this_thread::sleep_for(std::chrono::milliseconds(1));
			break;
		}

		if (!client.returnIndex) {
			if (ImGui::Button("Force return")) {
				client.CheckIfHasReturnFile();
				client.returnIndex = 0;
				client.bNeedReturning = true;
			}
		}
		else {
			if (ImGui::Button("Stop return")) {
				client.bNeedReturning = false;
			}
		}


		ImGui::End();
	}

#ifdef _WINDLL
	if (GetAsyncKeyState(VK_INSERT) & 1)
		bDraw = !bDraw;
#endif
	}
