#include "Client.h"
#include "../Recognition/Recognition.h"
#include "../RavendawnBot.h"
#include "../Notification/Notification.h"
#include <psapi.h>

#include <fstream>

ULONG GetProcessBaseAddress(HANDLE& hProcess, HMODULE* phmod);

int random(int low, int high) {
	return rand() % (high - low + 1) + low;
}

void Client::DisableAllFarmsFunctions() {
	this->bAutoFishing = false;
	this->bFishingAutoMove = false;
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

void Client::NextFishingPosition() {
	if (this->bFishingIncreseDecreseIndex) {
		this->fishingIndex++;
		if (this->fishingIndex > this->fishingWaipont.size() - 1) {
			this->bFishingIncreseDecreseIndex = false;
			this->fishingIndex--;
		}
	}
	else {
		this->fishingIndex--;
		if (this->fishingIndex == -1) {
			this->bFishingIncreseDecreseIndex = true;
			this->fishingIndex++;
		}
	}
}

void Client::AutoFishing() {
	Vector3 playerPos{
		this->read<int>(this->BaseAddress + 0x27CCC1C),
		this->read<int>(this->BaseAddress + 0x27CCC20),
		this->read<int>(this->BaseAddress + 0x27CC498)
	};

	if (!this->fishingWaipont.size()) {
		this->fishingWaipont = mapper.waipoint;
		findNearest(this->fishingWaipont, playerPos, MapperType::Fishi);
	}

	for (; this->fishingWaipont[this->fishingIndex].mapperType == MapperType::npc;)
	{
		if (!(this->fishingWaipont[this->fishingIndex].Pos.x == playerPos.x
			&& this->fishingWaipont[this->fishingIndex].Pos.y == playerPos.y)) {
			break;
		}

		if (Recognition::CustomRecognition(this->hWnd, /*1146, 669, 1120 - 11146, 703 - 669,*/ ImageType::sellButton, 0.7, true)) {
			for (; Recognition::CustomRecognition(this->hWnd, /*1146, 669, 1120 - 11146, 703 - 669,*/ ImageType::sellButton, 0.7, true);) {
				PostMessage(this->hWnd, WM_MOUSEMOVE, 0, MAKELPARAM(1158, 689));
				std::this_thread::sleep_for(std::chrono::milliseconds(5));
				PostMessage(this->hWnd, WM_LBUTTONDOWN, MK_LBUTTON, 0);
				PostMessage(this->hWnd, WM_LBUTTONUP, MK_LBUTTON, 0);

				PostMessage(this->hWnd, WM_MOUSEMOVE, 0, MAKELPARAM(10, 10));
				std::this_thread::sleep_for(std::chrono::milliseconds(20));

			}

			this->NextFishingPosition();
			return;
		}

		if (Recognition::CustomRecognition(this->hWnd, /*931, 189, 1178 - 931, 225 - 189,*/ ImageType::sellText, 0.7, true)) {
			PostMessage(this->hWnd, WM_MOUSEMOVE, 0, MAKELPARAM(1034, 209));
			std::this_thread::sleep_for(std::chrono::milliseconds(5));
			PostMessage(this->hWnd, WM_LBUTTONDOWN, MK_LBUTTON, 0);
			PostMessage(this->hWnd, WM_LBUTTONUP, MK_LBUTTON, 0);

			std::this_thread::sleep_for(std::chrono::milliseconds(5));
			PostMessage(this->hWnd, WM_MOUSEMOVE, 0, MAKELPARAM(10, 10));

			if (Recognition::CustomRecognition(this->hWnd, /*66, 414, 342 - 66, 473 - 414,*/ ImageType::failSellText, 0.7, true)) {
				this->NextFishingPosition();
				return;
			}

			return;
		}
		else
		{
			//SetForegroundWindow(this->hWnd);
			PostMessage(this->hWnd, WM_KEYDOWN, 0x46, MAKELPARAM(1, KF_REPEAT));
			std::this_thread::sleep_for(std::chrono::milliseconds(300));
			PostMessage(this->hWnd, WM_KEYUP, 0x46, 0);
			return;
		}

		return;
	}

	//if (Recognition::CustomRecognition(this->hWnd, 1202, 555, 1258 - 1202, 599 - 555, ImageType::cleanSlotInv, 0.1)) {
	//	this->bFishingIncreseDecreseIndex = false;
	//}

	auto currentTime{ std::chrono::steady_clock::now() };
	auto elapseTime{ std::chrono::duration_cast<std::chrono::seconds>(currentTime - this->lastFishingMovement).count() };
	auto firstHabilitTime{ std::chrono::duration_cast<std::chrono::seconds>(currentTime - this->lastFirstFishingHabilit).count() };
	auto thirtHabilitTime{ std::chrono::duration_cast<std::chrono::seconds>(currentTime - this->lastThirtFishingHabilit).count() };
	auto FishingHabilitsCooldown{ std::chrono::duration_cast<std::chrono::seconds>(currentTime - this->FishingHabilitsCooldown).count() };

	if (Recognition::CustomRecognition(this->hWnd, ImageType::fishingRood, 0.7, true)) {
		if (FishingHabilitsCooldown < 4.2) {
			return;
		}

		if (firstHabilitTime > 4.2) {
			if (this->fishingFirstHabilitCount < 2) {
				PostMessage(this->hWnd, WM_KEYDOWN, VK_NUMPAD1, 0);
				std::this_thread::sleep_for(std::chrono::milliseconds(300));
				PostMessage(this->hWnd, WM_KEYUP, VK_NUMPAD1, 0);
				this->lastFirstFishingHabilit = std::chrono::steady_clock::now();
				this->fishingFirstHabilitCount++;
				this->FishingHabilitsCooldown = std::chrono::steady_clock::now();
				this->lastFishingMovement = std::chrono::steady_clock::now();
			}
		}

		if (thirtHabilitTime > 10.2) {
			PostMessage(this->hWnd, WM_KEYDOWN, VK_NUMPAD3, 0);
			std::this_thread::sleep_for(std::chrono::milliseconds(300));
			PostMessage(this->hWnd, WM_KEYUP, VK_NUMPAD3, 0);

			this->fishingFirstHabilitCount = 0;
			this->FishingHabilitsCooldown = std::chrono::steady_clock::now();
			this->lastThirtFishingHabilit = std::chrono::steady_clock::now();
			this->lastFishingMovement = std::chrono::steady_clock::now();
		}
		return;
	}

	if (elapseTime < 8) {
		return;
	}

	if (Recognition::CustomRecognition(this->hWnd, 570, 526, 710 - 570, 542 - 526, ImageType::fishingWait, 0.5)) {
		return;
	}

	//if (Recognition::CustomRecognition(this->hWnd, ImageType::fishiIcon, 0.6, false)) {
	//	Notification::Notification(this->charPerson.Nick.c_str(), "Finded possible Fishing spot");
	//	return;
	//}

	if (Recognition::CustomRecognition(this->hWnd, 510, 256, 745 - 510, 495 - 256, ImageType::interaction, 0.6)) {
		PostMessage(this->hWnd, WM_KEYDOWN, 0x46, 0);
		std::this_thread::sleep_for(std::chrono::milliseconds(300));
		PostMessage(this->hWnd, WM_KEYUP, 0x46, 0);
		this->lastFishingMovement = std::chrono::steady_clock::now();
		return;
	}

	if (!this->bFishingAutoMove) {
		return;
	}

	if (this->fishingWaipont[this->fishingIndex].Pos.x == playerPos.x
		&& this->fishingWaipont[this->fishingIndex].Pos.y == playerPos.y
		/*&& this->fishingWaipont[this->fishingIndex].Pos.z == playerPos.z*/) {
		NextFishingPosition();
	}
	else
	{
		int x = this->fishingWaipont[this->fishingIndex].Pos.x - playerPos.x;
		int y = this->fishingWaipont[this->fishingIndex].Pos.y - playerPos.y;

		/*if (this->fishingWaipont[this->fishingIndex].Pos.x < playerPos.x) {
			pos.x = this->fishingWaipont[this->fishingIndex].Pos.x - playerPos.x;
		}
		else
		{
			pos.x = this->fishingWaipont[this->fishingIndex].Pos.x + playerPos.x;
		}

		if (this->fishingWaipont[this->fishingIndex].Pos.y < playerPos.y) {
			pos.y = this->fishingWaipont[this->fishingIndex].Pos.y - playerPos.y;
		}
		else
		{
			pos.y = this->fishingWaipont[this->fishingIndex].Pos.x + playerPos.x;
		}*/
		/*POINT pos{};
		pos.x = (1296 / 2 - 24) + x * 2;
		pos.y = (759 / 2 - 25) + y * 2;*/


		//if (pos.x < 350) {
		//	pos.x = 350;
		//}
		//else if (pos.x > 1012) {
		//	pos.x = 1012;
		//}
		//else {
		//	pos.x += 1280 / 2 - 24;
		//}

		//if (pos.y > 552) {
		//	pos.y = 552;
		//}
		//else if (pos.y < 100) {
		//	pos.y = 100;
		//}
		//else {
		//	pos.y += 720 / 2 - 25;
		//}

		//SendMessage(this->hWnd, WM_MOUSEMOVE, 0, MAKELPARAM(pos.x, pos.y)); // x e y são as coordenadas do clique
		//std::this_thread::sleep_for(std::chrono::milliseconds(300));
		//SendMessage(this->hWnd, WM_LBUTTONDOWN, MK_LBUTTON, 0);
		//SendMessage(this->hWnd, WM_LBUTTONUP, MK_LBUTTON, 0);

		if (x > 0) {
			PostMessage(this->hWnd, WM_KEYDOWN, 0x44, 0);
			std::this_thread::sleep_for(std::chrono::milliseconds(10));
			PostMessage(this->hWnd, WM_KEYUP, 0x44, 0);
		}
		else if (x < 0) {
			PostMessage(this->hWnd, WM_KEYDOWN, 0x41, 0);
			std::this_thread::sleep_for(std::chrono::milliseconds(10));
			PostMessage(this->hWnd, WM_KEYUP, 0x41, 0);
		}

		if (y > 0) {
			PostMessage(this->hWnd, WM_KEYDOWN, 0x53, 0);
			std::this_thread::sleep_for(std::chrono::milliseconds(10));
			PostMessage(this->hWnd, WM_KEYUP, 0x53, 0);
		}
		else if (y < 0) {
			PostMessage(this->hWnd, WM_KEYDOWN, 0x57, 0);
			std::this_thread::sleep_for(std::chrono::milliseconds(10));
			PostMessage(this->hWnd, WM_KEYUP, 0x57, 0);
		}

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
			if (client == NULL) {
				continue;
			}
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

void Client::LoadWaipointConfig(const std::string& FileName, MapperType FarmType) {
	if (!std::filesystem::exists("C:\\RavendawnBot")) {
		std::filesystem::create_directories("C:\\RavendawnBot");
		//here put a logic for downlaod recognitions files
	}

	if (!std::filesystem::exists("C:\\RavendawnBot\\accounts")) {
		std::filesystem::create_directories("C:\\RavendawnBot\\accounts");
	}

	if (!std::filesystem::exists("C:\\RavendawnBot\\Farms")) {
		std::filesystem::create_directories("C:\\RavendawnBot\\Farms");
	}

	if (!std::filesystem::exists("C:\\RavendawnBot\\Farms\\Fishi")) {
		std::filesystem::create_directories("C:\\RavendawnBot\\Farms\\Fishi");
	}

	if (!std::filesystem::exists("C:\\RavendawnBot\\Farms\\Wood")) {
		std::filesystem::create_directories("C:\\RavendawnBot\\Farms\\Wood");
	}

	std::string type;
	switch (FarmType)
	{
	case 0:
	case 1: {
		type = "C:\\RavendawnBot\\Farms\\Wood\\"+ FileName + ".json";
		break;
	}
	case 2:
	case 3: {
		type = "C:\\RavendawnBot\\Farms\\Fishi\\" + FileName + ".json";
		break;
	}
	default:
		break;
	}

	std::ifstream file_json(type);
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

	Vector3 playerPos{
	this->read<int>(this->BaseAddress + 0x27CCC1C),
	this->read<int>(this->BaseAddress + 0x27CCC20),
	this->read<int>(this->BaseAddress + 0x27CC498)
	};

	if (this->woodWaipont.size()) {
		this->woodWaipont.clear();
	}

	if (this->fishingWaipont.size()) {
		this->fishingWaipont.clear();
	}

	for (const auto& item : json) {
		Waipoint waipoint;
		from_json(item, waipoint);
		switch (FarmType)
		{
		case 0:
		case 1: {
			this->woodWaipont.push_back(waipoint);
			break;
		}
		case 2:
		case 3: {
			this->fishingWaipont.push_back(waipoint);
			break;
		}
		default:
			break;
		}
	}

	switch (FarmType)
	{
	case 0:
	case 1: {
		findNearest(this->woodWaipont, playerPos, MapperType::Wood);
		this->bAutoWoodFarmPopup = true;
		break;
	}

	case 2:
	case 3: {
		findNearest(this->fishingWaipont, playerPos, MapperType::Fishi);
		this->bAutoFishingPopup = true;
		break;
	}

	default:
		break;
	}
}

void Client::findNearest(std::vector<Waipoint>& vectors, const Vector3& location, const MapperType mapperType) {
	if (vectors.empty()) {
		throw std::invalid_argument("List of vectors is empty.");
	}

	std::vector<Waipoint> filtred;
	for (const auto& filter : vectors) {
		switch (mapperType)
		{
		case 0:
		case 1: {
			if (filter.mapperType == Wood || filter.mapperType == WalkWood) {
				filtred.push_back(filter);
			}
			break;
		}
		case 2:
		case 3: {
			if (filter.mapperType == npc || filter.mapperType == Fishi || filter.mapperType == WalkFishi) {
				filtred.push_back(filter);
			}
			break;
		}
		default:
			break;
		}
	}

	if (!filtred.size())
	{
		Notification::Notification(this->Login.c_str(), "Error");
		return;
	}

	Waipoint nearest = filtred.front();
	double minDistance = distance(filtred.front().Pos, location);

	for (const auto& item : filtred) {
		double dist = distance(item.Pos, location);
		if (dist < minDistance) {
			minDistance = dist;
			nearest = item;
		}
	}

	vectors = filtred;

	switch (mapperType)
	{
	case 0: //wood
	case 1: {

		break;
	}
	case 2://fishing
	case 3: {
		this->fishingIndex = nearest.index;
		break;
	}
	default:
		break;
	}
}

ULONG GetProcessBaseAddress(HANDLE& hProcess, HMODULE* phmod)
{
	ULONG cb;
	return EnumProcessModulesEx(hProcess, phmod, sizeof(HMODULE), &cb, LIST_MODULES_DEFAULT) ? 0 : GetLastError();
}

#include <fstream>

void Mapper::Load(const std::string& FileName, MapperType FarmType) {
	if (!std::filesystem::exists("C:\\RavendawnBot")) {
		std::filesystem::create_directories("C:\\RavendawnBot");
		//here put a logic for downlaod recognitions files
	}

	if (!std::filesystem::exists("C:\\RavendawnBot\\accounts")) {
		std::filesystem::create_directories("C:\\RavendawnBot\\accounts");
	}

	if (!std::filesystem::exists("C:\\RavendawnBot\\Farms")) {
		std::filesystem::create_directories("C:\\RavendawnBot\\Farms");
	}

	if (!std::filesystem::exists("C:\\RavendawnBot\\Farms\\Fishi")) {
		std::filesystem::create_directories("C:\\RavendawnBot\\Farms\\Fishi");
	}

	if (!std::filesystem::exists("C:\\RavendawnBot\\Farms\\Wood")) {
		std::filesystem::create_directories("C:\\RavendawnBot\\Farms\\Wood");
	}


	std::string type;

	switch (FarmType)
	{
	case 0:
	case 1: {
		type = "Wood";
		break;
	}
	case 2:
	case 3: {
		type = "Fishi";
		break;
	}
	default:
		break;
	}

	std::ifstream file_json("C:\\RavendawnBot\\Farms\\" + type + "\\" + FileName + ".json");
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

	/*int size = json.size();

	for (int i{ 0 }; i < size; i++) {
		mapper.waipoint.push_back(json[i]);
	}*/

	for (const auto& item : json) {
		Waipoint waipoint;
		from_json(item, waipoint);
		mapper.waipoint.push_back(waipoint);
	}

	mapper.maxIndex = mapper.waipoint.size() - 1;
}

Mapper::~Mapper() {

}