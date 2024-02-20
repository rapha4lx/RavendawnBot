#include "Client.h"
#include "../Recognition/Recognition.h"
#include "../RavendawnBot.h"
#include "../Notification/Notification.h"
#include <psapi.h>

ULONG GetProcessBaseAddress(HANDLE& hProcess, HMODULE* phmod);

int random(int low, int high) {
	return rand() % (high - low + 1) + low;
}

void Client::DisableAllFarmsFunctions() {
	this->bAutoFishing = false;
	this->bAutoTask = false;
	this->bWoodFarm = false;
	this->bAutoAttack = false;
}
//140 213
//148 230

void Client::AutoTask() {
	if (Recognition::CustomRecognition(this->hWnd, 553, 255, 727 - 553, 279 - 255, ImageType::greenBar, 0.6)) {
		PostMessage(this->hWnd, WM_KEYDOWN, 0x46, 0);
		std::this_thread::sleep_for(std::chrono::milliseconds(300));
		PostMessage(this->hWnd, WM_KEYUP, 0x46, 0);
		return;
	}
}

void Client::AutoAttack() {
	auto currentTime{ std::chrono::steady_clock::now() };
	auto lastAutoAttackMoveTime{ std::chrono::duration_cast<std::chrono::seconds>(currentTime - this->lastAutoAttackMoveTime).count() };
	auto AutoAttackIntervalTime{ std::chrono::duration_cast<std::chrono::seconds>(currentTime - this->lastAutoAttackMoveTime).count() };

	int ran{ random(0, 9) };

	if (lastAutoAttackMoveTime > 10) {
		PostMessage(this->hWnd, WM_KEYDOWN, 0x41, 0);
		std::this_thread::sleep_for(std::chrono::milliseconds(300));
		PostMessage(this->hWnd, WM_KEYUP, 0x41, 0);

		PostMessage(this->hWnd, WM_KEYDOWN, 0x44, 0);
		std::this_thread::sleep_for(std::chrono::milliseconds(300));
		PostMessage(this->hWnd, WM_KEYUP, 0x44, 0);
		this->lastAutoAttackMoveTime = std::chrono::steady_clock::now();
	}


	if (AutoAttackIntervalTime < 2) {
		return;
	}


	if (!Recognition::CustomRecognition(this->hWnd, 136, 211, 152 - 136, 230 - 211, ImageType::EnemyLocked, 0.7)) {
		if (Recognition::CustomRecognition(this->hWnd, 136, 211, 152 - 136, 230 - 211, ImageType::EnemyDetected, 0.7)) {
			PostMessage(this->hWnd, WM_KEYDOWN, VK_TAB, 0);
			std::this_thread::sleep_for(std::chrono::milliseconds(300));
			PostMessage(this->hWnd, WM_KEYUP, VK_TAB, 0);
		}
	}

	switch (ran)
	{
	case 0: {
		PostMessage(this->hWnd, WM_KEYDOWN, VK_NUMPAD0, 0);
		std::this_thread::sleep_for(std::chrono::milliseconds(300));
		PostMessage(this->hWnd, WM_KEYUP, VK_NUMPAD0, 0);
		this->AutoAttackIntervalTime = std::chrono::steady_clock::now();
		break;
	}
	case 1: {
		PostMessage(this->hWnd, WM_KEYDOWN, VK_NUMPAD1, 0);
		std::this_thread::sleep_for(std::chrono::milliseconds(300));
		PostMessage(this->hWnd, WM_KEYUP, VK_NUMPAD1, 0);
		this->AutoAttackIntervalTime = std::chrono::steady_clock::now();
		break;
	}
	case 2: {
		PostMessage(this->hWnd, WM_KEYDOWN, VK_NUMPAD2, 0);
		std::this_thread::sleep_for(std::chrono::milliseconds(300));
		PostMessage(this->hWnd, WM_KEYUP, VK_NUMPAD2, 0);
		this->AutoAttackIntervalTime = std::chrono::steady_clock::now();
		break;
	}
	case 3: {
		PostMessage(this->hWnd, WM_KEYDOWN, VK_NUMPAD3, 0);
		std::this_thread::sleep_for(std::chrono::milliseconds(300));
		PostMessage(this->hWnd, WM_KEYUP, VK_NUMPAD3, 0);
		this->AutoAttackIntervalTime = std::chrono::steady_clock::now();
		break;
	}
	case 4: {
		PostMessage(this->hWnd, WM_KEYDOWN, VK_NUMPAD4, 0);
		std::this_thread::sleep_for(std::chrono::milliseconds(300));
		PostMessage(this->hWnd, WM_KEYUP, VK_NUMPAD4, 0);
		this->AutoAttackIntervalTime = std::chrono::steady_clock::now();
		break;
	}
	case 5: {
		PostMessage(this->hWnd, WM_KEYDOWN, VK_NUMPAD5, 0);
		std::this_thread::sleep_for(std::chrono::milliseconds(300));
		PostMessage(this->hWnd, WM_KEYUP, VK_NUMPAD5, 0);
		this->AutoAttackIntervalTime = std::chrono::steady_clock::now();
		break;
	}
	case 6: {
		PostMessage(this->hWnd, WM_KEYDOWN, VK_NUMPAD6, 0);
		std::this_thread::sleep_for(std::chrono::milliseconds(300));
		PostMessage(this->hWnd, WM_KEYUP, VK_NUMPAD6, 0);
		this->AutoAttackIntervalTime = std::chrono::steady_clock::now();
		break;
	}
	case 7: {
		PostMessage(this->hWnd, WM_KEYDOWN, VK_NUMPAD7, 0);
		std::this_thread::sleep_for(std::chrono::milliseconds(300));
		PostMessage(this->hWnd, WM_KEYUP, VK_NUMPAD7, 0);
		this->AutoAttackIntervalTime = std::chrono::steady_clock::now();
		break;
	}
	case 8: {
		PostMessage(this->hWnd, WM_KEYDOWN, VK_NUMPAD8, 0);
		std::this_thread::sleep_for(std::chrono::milliseconds(300));
		PostMessage(this->hWnd, WM_KEYUP, VK_NUMPAD8, 0);
		this->AutoAttackIntervalTime = std::chrono::steady_clock::now();
		break;
	}
	case 9: {
		PostMessage(this->hWnd, WM_KEYDOWN, VK_NUMPAD9, 0);
		std::this_thread::sleep_for(std::chrono::milliseconds(300));
		PostMessage(this->hWnd, WM_KEYUP, VK_NUMPAD9, 0);
		this->AutoAttackIntervalTime = std::chrono::steady_clock::now();
		break;
	}
	default:
		break;
	}



}

void Client::AutoFishing() {
	auto currentTime{ std::chrono::steady_clock::now() };
	auto elapseTime{ std::chrono::duration_cast<std::chrono::seconds>(currentTime - this->lastFishingMovement).count() };
	auto firstHabilitTime{ std::chrono::duration_cast<std::chrono::seconds>(currentTime - this->lastFishingHabilit).count() };


	if (elapseTime < 1) {
		return;
	}

	if (Recognition::CustomRecognition(this->hWnd, ImageType::fishingRood, 0.6, false)) {
		if (firstHabilitTime > 4) {
			PostMessage(this->hWnd, WM_KEYDOWN, VK_NUMPAD1, 0);
			std::this_thread::sleep_for(std::chrono::milliseconds(300));
			PostMessage(this->hWnd, WM_KEYUP, VK_NUMPAD1, 0);
			this->lastFishingHabilit = std::chrono::steady_clock::now();
		}
		return;
	}

	if (Recognition::CustomRecognition(this->hWnd, 570, 526, 710 - 570, 542 - 526, ImageType::fishingWait, 0.4)) {
		return;
	}
	
	if (Recognition::CustomRecognition(this->hWnd, ImageType::fishiIcon, 0.6, false)) {
		Notification::Notification(this->charPerson.Nick.c_str(), "Finded possible Fishing spot");
		return;
	}

	if (Recognition::CustomRecognition(this->hWnd, 510, 256, 745 - 510, 495 - 256, ImageType::interaction, 0.5)) {
		PostMessage(this->hWnd, WM_KEYDOWN, 0x46, 0);
		std::this_thread::sleep_for(std::chrono::milliseconds(300));
		PostMessage(this->hWnd, WM_KEYUP, 0x46, 0);
		return;
	}

	switch (this->findFishingPos)
	{
		this->lastFishingHabilit = std::chrono::steady_clock::now();
	case FindPos::up: {
		SendMessage(this->hWnd, WM_MOUSEMOVE, 0, MAKELPARAM(652, 378 - 200)); // x e y são as coordenadas do clique
		std::this_thread::sleep_for(std::chrono::milliseconds(300));
		SendMessage(this->hWnd, WM_LBUTTONDOWN, MK_LBUTTON, 0);
		SendMessage(this->hWnd, WM_LBUTTONUP, MK_LBUTTON, 0);
		break;
	}
	case FindPos::right: {
		SendMessage(this->hWnd, WM_MOUSEMOVE, 0, MAKELPARAM(652 + 300, 378)); // x e y são as coordenadas do clique
		std::this_thread::sleep_for(std::chrono::milliseconds(300));
		SendMessage(this->hWnd, WM_LBUTTONDOWN, MK_LBUTTON, 0);
		SendMessage(this->hWnd, WM_LBUTTONUP, MK_LBUTTON, 0);
		break;
	}
	case FindPos::down: {
		SendMessage(this->hWnd, WM_MOUSEMOVE, 0, MAKELPARAM(652, 378 + 200)); // x e y são as coordenadas do clique
		std::this_thread::sleep_for(std::chrono::milliseconds(300));
		SendMessage(this->hWnd, WM_LBUTTONDOWN, MK_LBUTTON, 0);
		SendMessage(this->hWnd, WM_LBUTTONUP, MK_LBUTTON, 0);
		break;
	}
	case FindPos::left: {
		SendMessage(this->hWnd, WM_MOUSEMOVE, 0, MAKELPARAM(652 - 300, 378)); // x e y são as coordenadas do clique
		std::this_thread::sleep_for(std::chrono::milliseconds(300));
		SendMessage(this->hWnd, WM_LBUTTONDOWN, MK_LBUTTON, 0);
		SendMessage(this->hWnd, WM_LBUTTONUP, MK_LBUTTON, 0);
		break;
	}
	default:
		break;
	}

}

void Client::FarmWood() {
	auto currentTime{ std::chrono::steady_clock::now() };
	auto elapseTime{ std::chrono::duration_cast<std::chrono::seconds>(currentTime - this->lastAxieDetectTime).count() };
	auto changeFarmPositionTime{ std::chrono::duration_cast<std::chrono::minutes>(currentTime - this->changeFarmPositionTime).count() };

	//if (elapseTime < 1) {
	//	if (Recognition::ComparOldPos(this->hWnd, this->oldPlayerPos)) {
	//		this->woodErrorCount++;
	//		return;
	//	}
	//}

	if (changeFarmPositionTime >= 4) {
		this->changeFarmPositionTime = std::chrono::steady_clock::now();
		switch (this->farmPosition)
		{
		case FindPos::down: {
			this->farmPosition = FindPos::up;
			break;
		}
		case FindPos::up: {
			this->farmPosition = FindPos::down;
			break;
		}
		default:
			break;
		}
	}

	//568 525
	//577 541
	if (Recognition::CustomRecognition(this->hWnd, 510, 256, 745 - 510, 495 - 256, ImageType::interaction, 0.5)) {
		PostMessage(this->hWnd, WM_KEYDOWN, 0x46, 0);
		std::this_thread::sleep_for(std::chrono::milliseconds(300));
		PostMessage(this->hWnd, WM_KEYUP, 0x46, 0);
		this->woodFarmStage = 2;
		return;
	}

	if (Recognition::CustomRecognition(this->hWnd, 551, 245, 575 - 551, 285 - 245, ImageType::ColectionIcon, 0.6)
		|| Recognition::CustomRecognition(this->hWnd, 568, 525, 578 - 568, 541 - 525, ImageType::ColectionIcon, 0.2)) {
		if (Recognition::CustomRecognition(this->hWnd, 553, 255, 727 - 553, 279 - 255, ImageType::greenBar, 0.6)) {
			PostMessage(this->hWnd, WM_KEYDOWN, 0x46, 0);
			std::this_thread::sleep_for(std::chrono::milliseconds(300));
			PostMessage(this->hWnd, WM_KEYUP, 0x46, 0);
			return;
		}
		return;
	}

	//player position 
	//652 378
	// 
	//548 279
	//684 413

	if (Recognition::ComparOldPos(this->hWnd, this->oldPlayerPos)) {
		if (elapseTime < 10) {
			return;
		}
	}


	POINT pos{ 0 };
	int ran{ random(0,7) };
	switch (ran)
	{
	case 0: {
		pos.x -= 40;
		break;
	}
	case 1: {
		pos.y -= 40;
		break;
	}
	case 2: {
		pos.x += 40;
		break;
	}
	case 3: {
		pos.y += 40;
		break;
	}
	case 4: {
		pos.x -= 40;
		pos.y -= 40;
		break;
	}
	case 5: {
		pos.x += 40;
		pos.y -= 40;
		break;
	}
	case 6: {
		pos.x += 40;
		pos.y += 40;
		break;
	}
	case 7: {
		pos.x += 40;
		pos.y += 40;
		break;
	}
	default:
		pos.x + 40;
		break;
	}
	if (Recognition::CustomRecognition(this->hWnd, ImageType::axieIcon, 0.7, pos, false)) {
		//if (Recognition::ComparOldPos(this->hWnd, this->oldPlayerPos)) {
		SendMessage(this->hWnd, WM_MOUSEMOVE, 0, MAKELPARAM(pos.x, pos.y)); // x e y são as coordenadas do clique
		std::this_thread::sleep_for(std::chrono::milliseconds(300));
		SendMessage(this->hWnd, WM_LBUTTONDOWN, MK_LBUTTON, 0);
		SendMessage(this->hWnd, WM_LBUTTONUP, MK_LBUTTON, 0);
		this->lastAxieDetectTime = std::chrono::steady_clock::now();
		//}
		Recognition::UpdatePos(this->hWnd, this->oldPlayerPos);
		return;
	}
	else {
		if (Recognition::WoodFarm(this->hWnd, this->farmPosition, false)) {
			SendMessage(this->hWnd, WM_MOUSEMOVE, 0, MAKELPARAM(Recognition::bestWood.x, Recognition::bestWood.y)); // x e y são as coordenadas do clique
			std::this_thread::sleep_for(std::chrono::milliseconds(300));
			SendMessage(this->hWnd, WM_LBUTTONDOWN, MK_LBUTTON, 0);
			SendMessage(this->hWnd, WM_LBUTTONUP, MK_LBUTTON, 0);
			this->woodErrorCount = 0;
			this->woodFarmStage = 0;
			return;
		}
		else {
			switch (random(0, 3))
			{
			case 0: {
				SendMessage(this->hWnd, WM_MOUSEMOVE, 0, MAKELPARAM(100, 100)); // x e y são as coordenadas do clique
				std::this_thread::sleep_for(std::chrono::milliseconds(300));
				SendMessage(this->hWnd, WM_LBUTTONDOWN, MK_LBUTTON, 0);
				SendMessage(this->hWnd, WM_LBUTTONUP, MK_LBUTTON, 0);
				break;
			}
			case 1: {
				SendMessage(this->hWnd, WM_MOUSEMOVE, 0, MAKELPARAM(1200, 100)); // x e y são as coordenadas do clique
				std::this_thread::sleep_for(std::chrono::milliseconds(300));
				SendMessage(this->hWnd, WM_LBUTTONDOWN, MK_LBUTTON, 0);
				SendMessage(this->hWnd, WM_LBUTTONUP, MK_LBUTTON, 0);
				break;
			}
			case 2: {
				SendMessage(this->hWnd, WM_MOUSEMOVE, 0, MAKELPARAM(100, 620)); // x e y são as coordenadas do clique
				std::this_thread::sleep_for(std::chrono::milliseconds(300));
				SendMessage(this->hWnd, WM_LBUTTONDOWN, MK_LBUTTON, 0);
				SendMessage(this->hWnd, WM_LBUTTONUP, MK_LBUTTON, 0);
				break;
			}
			case 3: {
				SendMessage(this->hWnd, WM_MOUSEMOVE, 0, MAKELPARAM(1200, 620)); // x e y são as coordenadas do clique
				std::this_thread::sleep_for(std::chrono::milliseconds(300));
				SendMessage(this->hWnd, WM_LBUTTONDOWN, MK_LBUTTON, 0);
				SendMessage(this->hWnd, WM_LBUTTONUP, MK_LBUTTON, 0);
				break;
			}
			default:
				break;
			}
		}
	}
}

void ClientNamespace::RunFarm() {
	while (!guiWhile)
	{
		if (ClientsFarm.size() == 0) {
			std::this_thread::sleep_for(std::chrono::milliseconds(10));
			continue;
		}

		for (auto* client : ClientsFarm) {
			if (!client->logged) {
				auto it = std::find_if(ClientsFarm.begin(), ClientsFarm.end(), [client](const Client* obj) {
					return obj->PID == client->PID;
					});
				if (it != ClientsFarm.end()) {
					//disable all farms functions here
					client->DisableAllFarmsFunctions();
					ClientsFarm.erase(it);

				}
				continue;
			}

			if (!client->bAutoFishing && !client->bAutoTask &&
				!client->bWoodFarm && !client->bAutoAttack)
			{
				auto it = std::find_if(ClientsFarm.begin(), ClientsFarm.end(), [client](const Client* obj) {
					return obj->PID == client->PID;
					});
				if (it != ClientsFarm.end()) {
					//disable all farms functions here
					client->DisableAllFarmsFunctions();
					ClientsFarm.erase(it);
				}
				continue;
			}

			if (client->bAutoFishing) {
				client->AutoFishing();
			}

			if (client->bAutoTask) {
				client->AutoTask();
			}

			if (client->bAutoAttack) {
				client->AutoAttack();
			}

			if (client->bWoodFarm) {
				client->FarmWood();
			}

		}
	}
}

bool Client::open_handle() noexcept {
	this->handle = OpenProcess(PROCESS_ALL_ACCESS, 0, this->PID);

	GetProcessBaseAddress(this->handle, &this->hmod);

	this->LocalPlayer = read<ULONG_PTR>(this->BaseAddress + 0x027CC100);

	int x = read<int>(this->BaseAddress + 0x27CCC1C);
	int y = read<int>(this->BaseAddress + 0x27CCC20);
	int z = read<int>(this->BaseAddress + 0x27CC498);
	std::cout << "Process base: " << std::hex << this->BaseAddress << std::endl;
	std::cout
		<< "Position x: " << x
		<< " Position y: " << y
		<< " Position z: " << z << std::endl;

	int life = read<int>(this->BaseAddress + 0x027CC120 + 0x30 + 0x50 + 0x10 + 0x198 + 0xDDC);

	std::cout << "life: " << life << std::endl;

	return this->handle;
}

// Closes the handle to the process if it is open.
void Client::close_handle() noexcept {
	if (this->handle) {
		CloseHandle(this->handle);
	}
}

ULONG GetProcessBaseAddress(HANDLE& hProcess, HMODULE* phmod)
{
	ULONG cb;
	return EnumProcessModulesEx(hProcess, phmod, sizeof(HMODULE), &cb, LIST_MODULES_DEFAULT) ? 0 : GetLastError();
}

#include <fstream>

void Mapper::Start() {
	std::string fileText;

	if (!std::filesystem::exists("C:\\RavendawnBot")) {
		std::filesystem::create_directories("C:\\RavendawnBot");
		//here put a logic for downlaod recognitions files
	}

	if (!std::filesystem::exists("C:\\RavendawnBot\\accounts")) {
		std::filesystem::create_directories("C:\\RavendawnBot\\accounts");
	}

	std::ifstream file_json("C:\\RavendawnBot\\accounts\\mapper.json");
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

	int size = json.size();

	for (int i{ 0 }; i < size; i++) {
		mapper.waipoint.push_back(json[i]);
	}
}

Mapper::~Mapper() {
	
}