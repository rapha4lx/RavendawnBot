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

void Client::move(int x, int y, int z) {
	//if (x > 0 && y > 0) {//diagonal direita para baixo
	//	PostMessage(this->hWnd, WM_KEYDOWN, 0x44, 0);//D
	//	std::this_thread::sleep_for(std::chrono::milliseconds(5));
	//	PostMessage(this->hWnd, WM_KEYDOWN, 0x53, 0);//S
	//	std::this_thread::sleep_for(std::chrono::milliseconds(5));
	//	PostMessage(this->hWnd, WM_KEYUP, 0x44, 0);
	//	PostMessage(this->hWnd, WM_KEYUP, 0x53, 0);
	//	return;
	//}
	//else if (x < 0 && y < 0) {//diagonal esqueda para cima
	//	PostMessage(this->hWnd, WM_KEYDOWN, 0x41, 0);//A
	//	std::this_thread::sleep_for(std::chrono::milliseconds(5));
	//	PostMessage(this->hWnd, WM_KEYDOWN, 0x57, 0);//W
	//	std::this_thread::sleep_for(std::chrono::milliseconds(5));
	//	PostMessage(this->hWnd, WM_KEYUP, 0x41, 0);
	//	PostMessage(this->hWnd, WM_KEYUP, 0x57, 0);
	//	return;
	//}
	//else if (x < 0 && y > 0) { //diagonal esqueda para baixo
	//	PostMessage(this->hWnd, WM_KEYDOWN, 0x41, 0);//A
	//	std::this_thread::sleep_for(std::chrono::milliseconds(5));
	//	PostMessage(this->hWnd, WM_KEYDOWN, 0x53, 0);//S
	//	std::this_thread::sleep_for(std::chrono::milliseconds(5));
	//	PostMessage(this->hWnd, WM_KEYUP, 0x41, 0);
	//	PostMessage(this->hWnd, WM_KEYUP, 0x53, 0);
	//	return;
	//}
	//else if (x > 0 && y < 0) { //diagonal direita para cima
	//	PostMessage(this->hWnd, WM_KEYDOWN, 0x44, 0);//D
	//	std::this_thread::sleep_for(std::chrono::milliseconds(5));
	//	PostMessage(this->hWnd, WM_KEYDOWN, 0x57, 0);//W
	//	std::this_thread::sleep_for(std::chrono::milliseconds(5));
	//	PostMessage(this->hWnd, WM_KEYUP, 0x44, 0);
	//	PostMessage(this->hWnd, WM_KEYUP, 0x57, 0);
	//	return;
	//}
	//else if (x > 0) {
	//	PostMessage(this->hWnd, WM_KEYDOWN, 0x44, 0);//D
	//	std::this_thread::sleep_for(std::chrono::milliseconds(5));
	//	PostMessage(this->hWnd, WM_KEYUP, 0x44, 0);
	//	return;
	//}
	//else if (x < 0) {
	//	PostMessage(this->hWnd, WM_KEYDOWN, 0x41, 0);//A
	//	std::this_thread::sleep_for(std::chrono::milliseconds(5));
	//	PostMessage(this->hWnd, WM_KEYUP, 0x41, 0);
	//	return;
	//}
	//else if (y > 0) {
	//	PostMessage(this->hWnd, WM_KEYDOWN, 0x53, 0);//S
	//	std::this_thread::sleep_for(std::chrono::milliseconds(5));
	//	PostMessage(this->hWnd, WM_KEYUP, 0x53, 0);
	//	return;
	//}
	//else if (y < 0) {
	//	PostMessage(this->hWnd, WM_KEYDOWN, 0x57, 0);//D
	//	std::this_thread::sleep_for(std::chrono::milliseconds(5));
	//	PostMessage(this->hWnd, WM_KEYUP, 0x57, 0);
	//	return;
	//}

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

void Client::DisableAllFarmsFunctions() {
	this->bAutoFishing = false;
	this->bFishingAutoMove = false;
	this->bAutoTask = false;
	this->bWoodFarm = false;
	this->bAutoAttack = false;
	this->bOreFarm = false;
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

void Client::AutoAttack(bool bOnlyHeal) {
	auto currentTime{ std::chrono::steady_clock::now() };
	auto lastAutoAttackMoveTime{ std::chrono::duration_cast<std::chrono::seconds>(currentTime - this->lastAutoAttackMoveTime).count() };
	auto AutoAttackIntervalTime{ std::chrono::duration_cast<std::chrono::seconds>(currentTime - this->lastAutoAttackMoveTime).count() };

	long long skill[12]{};

	for (int i{ 0 }; i < 12; i++) {
		if (!this->charPerson.skills[i].bUsable) {
			continue;
		}

		if (bOnlyHeal && !this->charPerson.skills[i].healSkill) {
			continue;
		}

		skill[i] = std::chrono::duration_cast<std::chrono::seconds>(currentTime - this->charPerson.skills[i].cooldownTime).count();
	}

	int ran{ random(0, 11) };

	//if (lastAutoAttackMoveTime > 10) {
	//	PostMessage(this->hWnd, WM_KEYDOWN, 0x41, 0);
	//	std::this_thread::sleep_for(std::chrono::milliseconds(300));
	//	PostMessage(this->hWnd, WM_KEYUP, 0x41, 0);

	//	PostMessage(this->hWnd, WM_KEYDOWN, 0x44, 0);
	//	std::this_thread::sleep_for(std::chrono::milliseconds(300));
	//	PostMessage(this->hWnd, WM_KEYUP, 0x44, 0);
	//	this->lastAutoAttackMoveTime = std::chrono::steady_clock::now();
	//}


	if (AutoAttackIntervalTime < 1.2) {
		return;
	}


	/*if (!Recognition::CustomRecognition(this->hWnd, 136, 211, 152 - 136, 230 - 211, ImageType::EnemyLocked, 0.7)) {
	//	if (Recognition::CustomRecognition(this->hWnd, 136, 211, 152 - 136, 230 - 211, ImageType::EnemyDetected, 0.7)) {
	//	}
	//}*/

	//PostMessage(this->hWnd, WM_KEYDOWN, VK_TAB, 0);
	//std::this_thread::sleep_for(std::chrono::milliseconds(300));
	//PostMessage(this->hWnd, WM_KEYUP, VK_TAB, 0);

	switch (ran)
	{
	case 0: {
		if (!this->charPerson.skills[ran].bUsable) {
			//this->AutoAttack();
			return;
		}

		if (skill[ran] < this->charPerson.skills[ran].cooldown) {
			//this->AutoAttack();
			return;
		}

		if (bOnlyHeal && !this->charPerson.skills[ran].healSkill) {
			return;
		}

		this->charPerson.skills[ran].cooldownTime = std::chrono::steady_clock::now();

		PostMessage(this->hWnd, WM_KEYDOWN, VK_NUMPAD0, 0);
		std::this_thread::sleep_for(std::chrono::milliseconds(1));
		PostMessage(this->hWnd, WM_KEYUP, VK_NUMPAD0, 0);
		this->AutoAttackIntervalTime = std::chrono::steady_clock::now();
		break;
	}
	case 1: {
		if (!this->charPerson.skills[ran].bUsable) {
			//this->AutoAttack();
			return;
		}

		if (skill[ran] < this->charPerson.skills[ran].cooldown) {
			//this->AutoAttack();
			return;
		}

		if (bOnlyHeal && !this->charPerson.skills[ran].healSkill) {
			return;
		}

		this->charPerson.skills[ran].cooldownTime = std::chrono::steady_clock::now();

		PostMessage(this->hWnd, WM_KEYDOWN, VK_NUMPAD1, 0);
		std::this_thread::sleep_for(std::chrono::milliseconds(1));
		PostMessage(this->hWnd, WM_KEYUP, VK_NUMPAD1, 0);
		this->AutoAttackIntervalTime = std::chrono::steady_clock::now();
		break;
	}
	case 2: {
		if (!this->charPerson.skills[ran].bUsable) {
			//this->AutoAttack();
			return;
		}

		if (skill[ran] < this->charPerson.skills[ran].cooldown) {
			//this->AutoAttack();
			return;
		}
		if (bOnlyHeal && !this->charPerson.skills[ran].healSkill) {
			return;
		}

		this->charPerson.skills[ran].cooldownTime = std::chrono::steady_clock::now();

		PostMessage(this->hWnd, WM_KEYDOWN, VK_NUMPAD2, 0);
		std::this_thread::sleep_for(std::chrono::milliseconds(1));
		PostMessage(this->hWnd, WM_KEYUP, VK_NUMPAD2, 0);
		this->AutoAttackIntervalTime = std::chrono::steady_clock::now();
		break;
	}
	case 3: {
		if (!this->charPerson.skills[ran].bUsable) {
			//this->AutoAttack();
			return;
		}

		if (skill[ran] < this->charPerson.skills[ran].cooldown) {
			//this->AutoAttack();
			return;
		}
		if (bOnlyHeal && !this->charPerson.skills[ran].healSkill) {
			return;
		}

		this->charPerson.skills[ran].cooldownTime = std::chrono::steady_clock::now();

		PostMessage(this->hWnd, WM_KEYDOWN, VK_NUMPAD3, 0);
		std::this_thread::sleep_for(std::chrono::milliseconds(1));
		PostMessage(this->hWnd, WM_KEYUP, VK_NUMPAD3, 0);
		this->AutoAttackIntervalTime = std::chrono::steady_clock::now();
		break;
	}
	case 4: {
		if (!this->charPerson.skills[ran].bUsable) {
			//this->AutoAttack();
			return;
		}

		if (skill[ran] < this->charPerson.skills[ran].cooldown) {
			//this->AutoAttack();
			return;
		}
		if (bOnlyHeal && !this->charPerson.skills[ran].healSkill) {
			return;
		}

		this->charPerson.skills[ran].cooldownTime = std::chrono::steady_clock::now();

		PostMessage(this->hWnd, WM_KEYDOWN, VK_NUMPAD4, 0);
		std::this_thread::sleep_for(std::chrono::milliseconds(1));
		PostMessage(this->hWnd, WM_KEYUP, VK_NUMPAD4, 0);
		this->AutoAttackIntervalTime = std::chrono::steady_clock::now();
		break;
	}
	case 5: {
		if (!this->charPerson.skills[ran].bUsable) {
			//this->AutoAttack();
			return;
		}

		if (skill[ran] < this->charPerson.skills[ran].cooldown) {
			//this->AutoAttack();
			return;
		}
		if (bOnlyHeal && !this->charPerson.skills[ran].healSkill) {
			return;
		}

		this->charPerson.skills[ran].cooldownTime = std::chrono::steady_clock::now();

		PostMessage(this->hWnd, WM_KEYDOWN, VK_NUMPAD5, 0);
		std::this_thread::sleep_for(std::chrono::milliseconds(1));
		PostMessage(this->hWnd, WM_KEYUP, VK_NUMPAD5, 0);
		this->AutoAttackIntervalTime = std::chrono::steady_clock::now();
		break;
	}
	case 6: {
		if (!this->charPerson.skills[ran].bUsable) {
			//this->AutoAttack();
			return;
		}

		if (skill[ran] < this->charPerson.skills[ran].cooldown) {
			//this->AutoAttack();
			return;
		}
		if (bOnlyHeal && !this->charPerson.skills[ran].healSkill) {
			return;
		}

		this->charPerson.skills[ran].cooldownTime = std::chrono::steady_clock::now();

		PostMessage(this->hWnd, WM_KEYDOWN, VK_NUMPAD6, 0);
		std::this_thread::sleep_for(std::chrono::milliseconds(3));
		PostMessage(this->hWnd, WM_KEYUP, VK_NUMPAD6, 0);
		this->AutoAttackIntervalTime = std::chrono::steady_clock::now();
		break;
	}
	case 7: {
		if (!this->charPerson.skills[ran].bUsable) {
			//this->AutoAttack();
			return;
		}

		if (skill[ran] < this->charPerson.skills[ran].cooldown) {
			//this->AutoAttack();
			return;
		}
		if (bOnlyHeal && !this->charPerson.skills[ran].healSkill) {
			return;
		}

		this->charPerson.skills[ran].cooldownTime = std::chrono::steady_clock::now();

		PostMessage(this->hWnd, WM_KEYDOWN, VK_NUMPAD7, 0);
		std::this_thread::sleep_for(std::chrono::milliseconds(1));
		PostMessage(this->hWnd, WM_KEYUP, VK_NUMPAD7, 0);
		this->AutoAttackIntervalTime = std::chrono::steady_clock::now();
		break;
	}
	case 8: {
		if (!this->charPerson.skills[ran].bUsable) {
			//this->AutoAttack();
			return;
		}

		if (skill[ran] < this->charPerson.skills[ran].cooldown) {
			//this->AutoAttack();
			return;
		}
		if (bOnlyHeal && !this->charPerson.skills[ran].healSkill) {
			return;
		}

		this->charPerson.skills[ran].cooldownTime = std::chrono::steady_clock::now();

		PostMessage(this->hWnd, WM_KEYDOWN, VK_NUMPAD8, 0);
		std::this_thread::sleep_for(std::chrono::milliseconds(1));
		PostMessage(this->hWnd, WM_KEYUP, VK_NUMPAD8, 0);
		this->AutoAttackIntervalTime = std::chrono::steady_clock::now();
		break;
	}
	case 9: {
		if (!this->charPerson.skills[ran].bUsable) {
			//this->AutoAttack();
			return;
		}

		if (skill[ran] < this->charPerson.skills[ran].cooldown) {
			//this->AutoAttack();
			return;
		}
		if (bOnlyHeal && !this->charPerson.skills[ran].healSkill) {
			return;
		}

		this->charPerson.skills[ran].cooldownTime = std::chrono::steady_clock::now();

		PostMessage(this->hWnd, WM_KEYDOWN, VK_NUMPAD9, 0);
		std::this_thread::sleep_for(std::chrono::milliseconds(1));
		PostMessage(this->hWnd, WM_KEYUP, VK_NUMPAD9, 0);
		this->AutoAttackIntervalTime = std::chrono::steady_clock::now();
		break;
	}
	case 10: {
		if (!this->charPerson.skills[ran].bUsable) {
			//this->AutoAttack();
			return;
		}

		if (skill[ran] < this->charPerson.skills[ran].cooldown) {
			//this->AutoAttack();
			return;
		}
		if (bOnlyHeal && !this->charPerson.skills[ran].healSkill) {
			return;
		}

		this->charPerson.skills[ran].cooldownTime = std::chrono::steady_clock::now();

		PostMessage(this->hWnd, WM_KEYDOWN, VK_SUBTRACT, 0);
		std::this_thread::sleep_for(std::chrono::milliseconds(1));
		PostMessage(this->hWnd, WM_KEYUP, VK_SUBTRACT, 0);
		this->AutoAttackIntervalTime = std::chrono::steady_clock::now();
		break;
	}
	default:
		//this->AutoAttack();
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

void Client::NextWoodPosition() {
	if (this->bWoodIncreseDecreseIndex) {
		this->woodIndex++;
		if (this->woodIndex > this->woodWaipont.size() - 1) {
			this->bWoodIncreseDecreseIndex = false;
			this->woodIndex--;
		}
	}
	else {
		this->woodIndex--;
		if (this->woodIndex == -1) {
			this->bWoodIncreseDecreseIndex = true;
			this->woodIndex++;
		}
	}
}

void Client::NextOrePosition() {
	if (this->bOreIncreseDecreseIndex) {
		this->oreIndex++;
		if (this->oreIndex > this->oreWaipont.size() - 1) {
			this->bOreIncreseDecreseIndex = false;
			this->oreIndex--;
		}
	}
	else {
		this->oreIndex--;
		if (this->oreIndex == -1) {
			this->bOreIncreseDecreseIndex = true;
			this->oreIndex++;
		}
	}
}

void Client::AutoFishing() {
	Vector3 playerPos{
		this->read<int>(this->BaseAddress + 0x27CCC1C),
		this->read<int>(this->BaseAddress + 0x27CCC20),
		this->read<int>(this->BaseAddress + 0x27CC498)
	};

	/*if (!this->fishingWaipont.size()) {
		this->fishingWaipont = mapper.waipoint;
		findNearest(this->fishingWaipont, playerPos, MapperType::Fishi);
	}*/

	if (this->fishingWaipont[this->fishingIndex].mapperType == MapperType::npc)
	{
		if (!(this->fishingWaipont[this->fishingIndex].Pos.x == playerPos.x
			&& this->fishingWaipont[this->fishingIndex].Pos.y == playerPos.y)) {
			int x{ this->fishingWaipont[this->fishingIndex].Pos.x - playerPos.x };
			int y{ this->fishingWaipont[this->fishingIndex].Pos.y - playerPos.y };
			int z{ 0 };
			this->move(x, y, z);
			return;
		}

		if (Recognition::CustomRecognition(this->hWnd, /*1146, 669, 1120 - 11146, 703 - 669,*/ ImageType::sellButton, 0.7, false)) {
			//for (; Recognition::CustomRecognition(this->hWnd, /*1146, 669, 1120 - 11146, 703 - 669,*/ ImageType::sellButton, 0.7, true);) {
			PostMessage(this->hWnd, WM_MOUSEMOVE, 0, MAKELPARAM(1158, 689));
			std::this_thread::sleep_for(std::chrono::milliseconds(5));
			PostMessage(this->hWnd, WM_LBUTTONDOWN, MK_LBUTTON, 0);
			PostMessage(this->hWnd, WM_LBUTTONUP, MK_LBUTTON, 0);

			PostMessage(this->hWnd, WM_MOUSEMOVE, 0, MAKELPARAM(10, 10));
			std::this_thread::sleep_for(std::chrono::milliseconds(2));
			//}
			return;
		}

		if (Recognition::CustomRecognition(this->hWnd, /*931, 189, 1178 - 931, 225 - 189,*/ ImageType::sellText, 0.7, false)) {
			PostMessage(this->hWnd, WM_MOUSEMOVE, 0, MAKELPARAM(1034, 209));
			std::this_thread::sleep_for(std::chrono::milliseconds(5));
			PostMessage(this->hWnd, WM_LBUTTONDOWN, MK_LBUTTON, 0);
			PostMessage(this->hWnd, WM_LBUTTONUP, MK_LBUTTON, 0);

			std::this_thread::sleep_for(std::chrono::milliseconds(5));
			PostMessage(this->hWnd, WM_MOUSEMOVE, 0, MAKELPARAM(10, 10));

			if (Recognition::CustomRecognition(this->hWnd, /*66, 414, 342 - 66, 473 - 414,*/ ImageType::failSellText, 0.7, false)) {
				this->NextFishingPosition();
				return;
			}

			return;
		}
		else
		{
			PostMessage(this->hWnd, WM_KEYDOWN, 0x46, MAKELPARAM(1, KF_REPEAT));
			std::this_thread::sleep_for(std::chrono::milliseconds(3));
			PostMessage(this->hWnd, WM_KEYUP, 0x46, 0);
			return;
		}
		return;
	}

	auto currentTime{ std::chrono::steady_clock::now() };
	auto elapseTime{ std::chrono::duration_cast<std::chrono::seconds>(currentTime - this->lastFishingMovement).count() };
	auto firstHabilitTime{ std::chrono::duration_cast<std::chrono::seconds>(currentTime - this->lastFirstFishingHabilit).count() };
	auto thirtHabilitTime{ std::chrono::duration_cast<std::chrono::seconds>(currentTime - this->lastThirtFishingHabilit).count() };
	auto FishingHabilitsCooldown{ std::chrono::duration_cast<std::chrono::seconds>(currentTime - this->FishingHabilitsCooldown).count() };

	if (Recognition::CustomRecognition(this->hWnd, ImageType::fishingRood, 0.7, false)) {
		if (FishingHabilitsCooldown < 4.2) {
			return;
		}

		if (firstHabilitTime > 4.2) {
			if (this->fishingFirstHabilitCount < 2) {
				PostMessage(this->hWnd, WM_KEYDOWN, VK_NUMPAD1, 0);
				std::this_thread::sleep_for(std::chrono::milliseconds(3));
				PostMessage(this->hWnd, WM_KEYUP, VK_NUMPAD1, 0);
				this->lastFirstFishingHabilit = std::chrono::steady_clock::now();
				this->fishingFirstHabilitCount++;
				this->FishingHabilitsCooldown = std::chrono::steady_clock::now();
				this->lastFishingMovement = std::chrono::steady_clock::now();
			}
		}

		if (thirtHabilitTime > 10.2) {
			PostMessage(this->hWnd, WM_KEYDOWN, VK_NUMPAD3, 0);
			std::this_thread::sleep_for(std::chrono::milliseconds(3));
			PostMessage(this->hWnd, WM_KEYUP, VK_NUMPAD3, 0);

			this->fishingFirstHabilitCount = 0;
			this->FishingHabilitsCooldown = std::chrono::steady_clock::now();
			this->lastThirtFishingHabilit = std::chrono::steady_clock::now();
			this->lastFishingMovement = std::chrono::steady_clock::now();
		}
		return;
	}

	/*if (elapseTime < 8) {
		return;
	}*/

	if (Recognition::CustomRecognition(this->hWnd, /*560, 526, 710 - 560, 542 - 526,*/ ImageType::fishingWait, 0.4, true)) {
		return;
	}

	//if (Recognition::CustomRecognition(this->hWnd, 510, 256, 745 - 510, 495 - 256, ImageType::interaction, 0.6)) {
	//	PostMessage(this->hWnd, WM_KEYDOWN, 0x46, 0);
	//	std::this_thread::sleep_for(std::chrono::milliseconds(3));
	//	PostMessage(this->hWnd, WM_KEYUP, 0x46, 0);
	//	this->lastFishingMovement = std::chrono::steady_clock::now();
	//	return;
	//}

	POINT pos{};
	if (Recognition::CustomRecognition(this->hWnd, ImageType::fishiIcon, 0.5, pos, true)) {
		PostMessage(this->hWnd, WM_MOUSEMOVE, 0, MAKELPARAM(pos.x, pos.x));
		std::this_thread::sleep_for(std::chrono::milliseconds(1));
		PostMessage(this->hWnd, WM_RBUTTONDOWN, MK_RBUTTON, 0);
		PostMessage(this->hWnd, WM_RBUTTONUP, MK_RBUTTON, 0);
		if (elapseTime > 4) {

			this->lastFishingMovement = std::chrono::steady_clock::now();

			if (pos.x < (1280 / 2)) {
				move(-10, 0, 0);
			}
			else if (pos.x > (1280 / 2))
			{
				move(10, 0, 0);
			}

			if (pos.y < (720 / 2)) {
				move(0, -10, 0);
			}
			else if (pos.y > (720 / 2))
			{
				move(0, 10, 0);
			}
		}
		return;
	}

	if (!this->bFishingAutoMove) {
		return;
	}

	if (this->fishingWaipont[this->fishingIndex].Pos.x == playerPos.x
		&& this->fishingWaipont[this->fishingIndex].Pos.y == playerPos.y
		/*&& this->fishingWaipont[this->fishingIndex].Pos.z == playerPos.z*/) {
		this->NextFishingPosition();
	}
	else
	{
		int x{ this->fishingWaipont[this->fishingIndex].Pos.x - playerPos.x };
		int y{ this->fishingWaipont[this->fishingIndex].Pos.y - playerPos.y };
		int z{ 0 };

		this->move(x, y, z);
	}
}

void Client::FarmWood() {
	/*if (Recognition::CustomRecognition(this->hWnd, 510, 256, 745 - 510, 495 - 256, ImageType::interaction, 0.5)) {
		PostMessage(this->hWnd, WM_KEYDOWN, 0x46, 0);
		std::this_thread::sleep_for(std::chrono::milliseconds(2));
		PostMessage(this->hWnd, WM_KEYUP, 0x46, 0);
		this->lastAxieDetectTime = std::chrono::steady_clock::now();
		return;
	}*/

	if (this->getWoodInteraction() == 2) {
		if (Recognition::CustomRecognition(this->hWnd, 553, 255, 727 - 553, 279 - 255, ImageType::greenBar, 0.7)) {
			PostMessage(this->hWnd, WM_KEYDOWN, 0x46, 0);
			std::this_thread::sleep_for(std::chrono::milliseconds(2));
			PostMessage(this->hWnd, WM_KEYUP, 0x46, 0);
		}
		//this->lastAxieDetectTime = std::chrono::steady_clock::now();
		//if (Recognition::CustomRecognition(this->hWnd, 570, 526, 710 - 570, 542 - 526, ImageType::axieWaiting, 0.4)) {
		//	return;
		//}
		return;
	}

	auto currentTime{ std::chrono::steady_clock::now() };
	auto elapseTime{ std::chrono::duration_cast<std::chrono::seconds>(currentTime - this->lastAxieDetectTime).count() };
	//auto changeFarmPositionTime{ std::chrono::duration_cast<std::chrono::minutes>(currentTime - this->changeFarmPositionTime).count() };

	Vector3 playerPos{
		this->read<int>(this->BaseAddress + 0x27CCC1C),
		this->read<int>(this->BaseAddress + 0x27CCC20),
		this->read<int>(this->BaseAddress + 0x27CC498)
	};

	if (elapseTime < 1) {
		PostMessage(this->hWnd, WM_KEYDOWN, 0x46, 0);
		std::this_thread::sleep_for(std::chrono::milliseconds(1));
		PostMessage(this->hWnd, WM_KEYUP, 0x46, 0);
		return;
	}

	if (this->woodWaipont[this->woodIndex].mapperType == MapperType::Wood) {
		//PostMessage(this->hWnd, WM_KEYDOWN, 0x46, 0);
		//std::this_thread::sleep_for(std::chrono::milliseconds(1));
		//PostMessage(this->hWnd, WM_KEYUP, 0x46, 0);
		if (this->woodWaipont[this->woodIndex].Pos.x == playerPos.x
			&& this->woodWaipont[this->woodIndex].Pos.y == playerPos.y
			/*&& this->woodWaipont[this->woodIndex].Pos.z == playerPos.z*/)
		{
			this->lastAxieDetectTime = std::chrono::steady_clock::now();
		}
	}

	//if (!this->woodWaipont.size()) {
	//	this->woodWaipont = mapper.waipoint;
	//	this->findNearest(this->woodWaipont, playerPos, MapperType::Wood);
	//}


	if (this->woodWaipont[this->woodIndex].Pos.x == playerPos.x
		&& this->woodWaipont[this->woodIndex].Pos.y == playerPos.y
		/*&& this->fishingWaipont[this->fishingIndex].Pos.z == playerPos.z*/) {
		this->NextWoodPosition();
	}
	else
	{
		int x{ this->woodWaipont[this->woodIndex].Pos.x - playerPos.x };
		int y{ this->woodWaipont[this->woodIndex].Pos.y - playerPos.y };
		int z{ 0 };
		this->move(x, y, z);

		/*if (y > 0) {
			PostMessage(this->hWnd, WM_KEYDOWN, 0x53, 0);
			std::this_thread::sleep_for(std::chrono::milliseconds(10));
			PostMessage(this->hWnd, WM_KEYUP, 0x53, 0);
		}
		else if (y < 0) {
			PostMessage(this->hWnd, WM_KEYDOWN, 0x57, 0);
			std::this_thread::sleep_for(std::chrono::milliseconds(10));
			PostMessage(this->hWnd, WM_KEYUP, 0x57, 0);
		}*/
	}
}

void Client::FarmOre() {
	if (this->getHealth() < this->getMaxHealth()) {
		if (Recognition::CustomRecognition(this->hWnd, 100, 205, 59, 36, ImageType::EnemyLocked, 0.6)) {
			this->AutoAttack();
			return;
		}
		else
		{
			this->AutoAttack(true);
			PostMessage(this->hWnd, WM_KEYDOWN, VK_TAB, 0);
			std::this_thread::sleep_for(std::chrono::milliseconds(1));
			PostMessage(this->hWnd, WM_KEYUP, VK_TAB, 0);
			return;
		}
	}

	/*if (Recognition::CustomRecognition(this->hWnd, 100, 205, 59, 36, ImageType::EnemyLocked, 0.6)) {
		this->AutoAttack();
		return;
	}
	else if (Recognition::CustomRecognition(this->hWnd, 100, 205, 59, 36, ImageType::EnemyDetected, 0.6)) {
		PostMessage(this->hWnd, WM_KEYDOWN, VK_TAB, 0);
		std::this_thread::sleep_for(std::chrono::milliseconds(5));
		PostMessage(this->hWnd, WM_KEYUP, VK_TAB, 0);

	}*/

	//if (Recognition::CustomRecognition(this->hWnd, 510, 256, 745 - 510, 495 - 256, ImageType::interaction, 0.5)) {
	////if (this->oreWaipont[this->oreIndex].mapperType == MapperType::Ore) {
	//	PostMessage(this->hWnd, WM_KEYDOWN, 0x46, 0);
	//	std::this_thread::sleep_for(std::chrono::milliseconds(3));
	//	PostMessage(this->hWnd, WM_KEYUP, 0x46, 0);
	//	this->lastOreDetectTime = std::chrono::steady_clock::now();

	//	//this->NextOrePosition();
	//	/*if (Recognition::CustomRecognition(this->hWnd, 500, 250, 760 - 500, 510 - 256, ImageType::error_mining, 0.4)) {
	//		Vector3 playerPos{
	//			this->read<int>(this->BaseAddress + 0x27CCC1C),
	//			this->read<int>(this->BaseAddress + 0x27CCC20),
	//			this->read<int>(this->BaseAddress + 0x27CC498)
	//		};
	//		int x{ this->oreWaipont[this->oreIndex].Pos.x - playerPos.x };
	//		int y{ this->oreWaipont[this->oreIndex].Pos.y - playerPos.y };
	//		int z{ 0 };
	//		this->move(x, y, z);
	//	}*/
	//	return;
	//}

	/*if (Recognition::CustomRecognition(this->hWnd, 570, 526, 710 - 570, 542 - 526, ImageType::axieWaiting, 0.4)) {
		return;
	}*/

	auto currentTime{ std::chrono::steady_clock::now() };
	auto elapseTime{ std::chrono::duration_cast<std::chrono::seconds>(currentTime - this->lastOreMoveTime).count() };
	auto lastOreDetectTime{ std::chrono::duration_cast<std::chrono::seconds>(currentTime - this->lastOreDetectTime).count() };

	Vector3 playerPos{
		this->read<int>(this->BaseAddress + 0x27CCC1C),
		this->read<int>(this->BaseAddress + 0x27CCC20),
		this->read<int>(this->BaseAddress + 0x27CC498)
	};

	if (this->oreWaipont[this->oreIndex].mapperType == MapperType::Ore) {
		if (Recognition::CustomRecognition(this->hWnd, 553, 255, 727 - 553, 279 - 255, ImageType::greenBar, 0.7)) {
			PostMessage(this->hWnd, WM_KEYDOWN, 0x46, 0);
			std::this_thread::sleep_for(std::chrono::milliseconds(1));
			PostMessage(this->hWnd, WM_KEYUP, 0x46, 0);
			this->lastOreDetectTime = std::chrono::steady_clock::now();
			this->lastOreMoveTime = std::chrono::steady_clock::now();
			return;
		}

		if (this->oreWaipont[this->oreIndex].Pos.x == playerPos.x
			&& this->oreWaipont[this->oreIndex].Pos.y == playerPos.y
			/*&& this->oreWaipont[this->oreIndex].Pos.z == playerPos.z*/)
		{
			if (Recognition::CustomRecognition(this->hWnd, 570, 526, 710 - 570, 542 - 526, ImageType::axieWaiting, 0.4)) {
				this->lastOreDetectTime = std::chrono::steady_clock::now();
				this->lastOreMoveTime = std::chrono::steady_clock::now();
				return;
			}

			PostMessage(this->hWnd, WM_KEYDOWN, 0x46, 0);
			std::this_thread::sleep_for(std::chrono::milliseconds(3));
			PostMessage(this->hWnd, WM_KEYUP, 0x46, 0);

			if (elapseTime < 3) {
				return;
			}
		}
	}

	/*if (!this->oreWaipont.size()) {
		this->oreWaipont = mapper.waipoint;
		findNearest(this->oreWaipont, playerPos, MapperType::Wood);
	}*/

	if (lastOreDetectTime < 4) {
		return;
	}

	if (this->oreWaipont[this->oreIndex].Pos.x == playerPos.x
		&& this->oreWaipont[this->oreIndex].Pos.y == playerPos.y
		/*&& this->oreWaipont[this->oreIndex].Pos.z == playerPos.z*/) {
		this->NextOrePosition();
	}
	else
	{
		int x{ this->oreWaipont[this->oreIndex].Pos.x - playerPos.x };
		int y{ this->oreWaipont[this->oreIndex].Pos.y - playerPos.y };
		int z{ 0 };
		this->move(x, y, z);
		this->lastOreMoveTime = std::chrono::steady_clock::now();
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
				!client->bWoodFarm && !client->bAutoAttack && !client->bOreFarm)
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

			if (client->bOreFarm) {
				client->FarmOre();
			}

		}
	}
}

bool Client::open_handle() noexcept {
	this->handle = OpenProcess(PROCESS_ALL_ACCESS, 0, this->PID);

	GetProcessBaseAddress(this->handle, &this->hmod);

	this->LocalPlayer = read<ULONG_PTR>(this->BaseAddress + 0x027CC100);

	/*int x = read<int>(this->BaseAddress + 0x27CCC1C);
	int y = read<int>(this->BaseAddress + 0x27CCC20);
	int z = read<int>(this->BaseAddress + 0x27CC498);

	std::cout << "Process base: " << std::hex << this->BaseAddress << std::endl;
	std::cout
		<< "Position x: " << x
		<< " Position y: " << y
		<< " Position z: " << z << std::endl;

	int life = read<int>(this->BaseAddress + 0x027CC120 + 0x30 + 0x50 + 0x10 + 0x198 + 0xDDC);

	std::cout << "life: " << life << std::endl;*/

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
		type = "C:\\RavendawnBot\\Farms\\Wood\\" + FileName + ".json";
		break;
	}
	case 2:
	case 3: {
		type = "C:\\RavendawnBot\\Farms\\Fishi\\" + FileName + ".json";
		break;
	}
	case 5:
	case 6: {
		type = "C:\\RavendawnBot\\Farms\\Ore\\" + FileName + ".json";
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

	if (this->oreWaipont.size()) {
		this->oreWaipont.clear();
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
		case 5:
		case 6: {
			this->oreWaipont.push_back(waipoint);
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
	case 5:
	case 6: {
		findNearest(this->oreWaipont, playerPos, MapperType::Ore);
		this->bAutoOreFarmPopup = true;
		break;
	}
	default:
		break;
	}
	Notification::Notification(this->Login.c_str(), "Config loaded");
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
		case 5:
		case 6: {
			if (filter.mapperType == OreWalk || filter.mapperType == Ore) {
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
		this->woodIndex = nearest.index;
		break;
	}
	case 2://fishing
	case 3: {
		this->fishingIndex = nearest.index;
		break;
	}

	case 5:
	case 6: {
		this->oreIndex = nearest.index;
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







//Memory Client 

double Client::getHealth() {
	return this->read<double>(this->LocalPlayer + 0xCE0);
}

double Client::getMaxHealth() {
	return this->read<double>(this->LocalPlayer + 0xCE8);
}

int Client::getWoodInteraction() {
	return this->read<int>(this->read<ULONG_PTR>(this->BaseAddress + 0x0271A470) + 0xF80);
}

int Client::getInvValue() {
	return this->read<int>(this->read<ULONG_PTR>(this->read<ULONG_PTR>(this->read<ULONG_PTR>(this->BaseAddress + 0x027CC128) + 0x0) + 0x28) + 0x54);
}

int Client::getAttacking() {
	return this->read<int>(this->LocalPlayer + 0xF8);
}