#include "Client.h"
#include "../Recognition/Recognition.h"
#include "../RavendawnBot.h"
#include <psapi.h>

#include <fstream>
#include "../Protection/xorstr.hpp"


#if defined(BOT)

ULONG GetProcessBaseAddress(HANDLE& hProcess, HMODULE* phmod);

int random(int low, int high) {
	return rand() % (high - low + 1) + low;
}

inline bool w;
inline bool a;
inline bool s;
inline bool d;


void Client::move(int x, int y, int z) {
	//	if (x > 0 && y > 0) {//diagonal direita para baixo
	//		MyPost(this->hWnd, WM_KEYDOWN, 0x44, 0);//D
	//std::this_thread::sleep_for(std::chrono::milliseconds(120));
	//		MyPost(this->hWnd, WM_KEYDOWN, 0x53, 0);//S
	//std::this_thread::sleep_for(std::chrono::milliseconds(120));
	//		MyPost(this->hWnd, WM_KEYUP, 0x44, 0);
	//		MyPost(this->hWnd, WM_KEYUP, 0x53, 0);
	//		return;
	//	}
	//	else if (x < 0 && y < 0) {//diagonal esqueda para cima
	//		MyPost(this->hWnd, WM_KEYDOWN, 0x41, 0);//A
	//std::this_thread::sleep_for(std::chrono::milliseconds(120));
	//		MyPost(this->hWnd, WM_KEYDOWN, 0x57, 0);//W
	//std::this_thread::sleep_for(std::chrono::milliseconds(120));
	//		MyPost(this->hWnd, WM_KEYUP, 0x41, 0);
	//		MyPost(this->hWnd, WM_KEYUP, 0x57, 0);
	//		return;
	//	}
	//	else if (x < 0 && y > 0) { //diagonal esqueda para baixo
	//		MyPost(this->hWnd, WM_KEYDOWN, 0x41, 0);//A
	//std::this_thread::sleep_for(std::chrono::milliseconds(120));
	//		MyPost(this->hWnd, WM_KEYDOWN, 0x53, 0);//S
	//std::this_thread::sleep_for(std::chrono::milliseconds(120));
	//		MyPost(this->hWnd, WM_KEYUP, 0x41, 0);
	//		MyPost(this->hWnd, WM_KEYUP, 0x53, 0);
	//		return;
	//	}
	//	else if (x > 0 && y < 0) { //diagonal direita para cima
	//		MyPost(this->hWnd, WM_KEYDOWN, 0x44, 0);//D
	//std::this_thread::sleep_for(std::chrono::milliseconds(120));
	//		MyPost(this->hWnd, WM_KEYDOWN, 0x57, 0);//W
	//std::this_thread::sleep_for(std::chrono::milliseconds(120));
	//		MyPost(this->hWnd, WM_KEYUP, 0x44, 0);
	//		MyPost(this->hWnd, WM_KEYUP, 0x57, 0);
	//		return;
	//	}
	//	else if (x > 0) {
	//		MyPost(this->hWnd, WM_KEYDOWN, 0x44, 0);//D
	//std::this_thread::sleep_for(std::chrono::milliseconds(120));
	//		MyPost(this->hWnd, WM_KEYUP, 0x44, 0);
	//		return;
	//	}
	//	else if (x < 0) {
	//		MyPost(this->hWnd, WM_KEYDOWN, 0x41, 0);//A
	//std::this_thread::sleep_for(std::chrono::milliseconds(120));
	//		MyPost(this->hWnd, WM_KEYUP, 0x41, 0);
	//		return;
	//	}
	//	else if (y > 0) {
	//		MyPost(this->hWnd, WM_KEYDOWN, 0x53, 0);//S
	//std::this_thread::sleep_for(std::chrono::milliseconds(120));
	//		MyPost(this->hWnd, WM_KEYUP, 0x53, 0);
	//		return;
	//	}
	//	else if (y < 0) {
	//		MyPost(this->hWnd, WM_KEYDOWN, 0x57, 0);//D
	//std::this_thread::sleep_for(std::chrono::milliseconds(120));
	//		MyPost(this->hWnd, WM_KEYUP, 0x57, 0);
	//		return;
	//	}

	if (x > 0)
	{
		MyPost(this->hWnd, WM_KEYDOWN, 0x44, 0);
		std::this_thread::sleep_for(std::chrono::milliseconds(120));
		MyPost(this->hWnd, WM_KEYUP, 0x44, 0);
	}
	else if (x < 0)
	{
		MyPost(this->hWnd, WM_KEYDOWN, 0x41, 0);
		std::this_thread::sleep_for(std::chrono::milliseconds(120));
		MyPost(this->hWnd, WM_KEYUP, 0x41, 0);
	}

	if (y > 0)
	{
		MyPost(this->hWnd, WM_KEYDOWN, 0x53, 0);
		std::this_thread::sleep_for(std::chrono::milliseconds(120));
		MyPost(this->hWnd, WM_KEYUP, 0x53, 0);
	}
	else if (y < 0)
	{
		MyPost(this->hWnd, WM_KEYDOWN, 0x57, 0);
		std::this_thread::sleep_for(std::chrono::milliseconds(120));
		MyPost(this->hWnd, WM_KEYUP, 0x57, 0);
	}
}

void Client::CheckIfHasReturnFile() {
	if (this->returnWaipont.size()) {
		this->returnWaipont.clear();
	}

	for (const auto& entry : std::filesystem::directory_iterator(xorstr_("C:\\RavendawnBot\\Farms\\Returning"))) {
		if (std::filesystem::is_regular_file(entry)) {
			std::string nomeArquivo = entry.path().stem().string();
			if (this->lastMapper == nomeArquivo) {

				std::ifstream json_file(entry.path().string());
				if (!json_file.is_open()) {
					this->bNeedReturning = false;
					return;
				}

				nlohmann::json json;
				json_file >> json;

				json_file.close();

				for (const auto& item : json) {
					Waipoint waipoint;
					from_json(item, waipoint);

					this->returnWaipont.push_back(waipoint);
				}

				this->returnIndex = 0;
				this->bNeedReturning = true;
			}
		}
	}
}

void Client::NextReturningPosition() {
	if (this->bReturnIncreseDecreseIndex) {
		this->returnIndex++;
		if (this->returnIndex > this->returnWaipont.size() - 1) {
			this->bReturnIncreseDecreseIndex = false;
			this->woodIndex--;
		}
	}
	else {
		this->returnIndex--;
		if (this->returnIndex == -1) {
			this->bReturnIncreseDecreseIndex = true;
			this->returnIndex++;
		}
	}
}

void Client::Returning() {
	if (this->getHealth() < this->getMaxHealth()) {
		if (Recognition::CustomRecognition(this->hWnd, 100, 205, 59, 36, ImageType::EnemyLocked, 0.55)) {
			this->AutoAttack(false);
			return;
		}
		else
		{
			if (Recognition::CustomRecognition(this->hWnd, 100, 205, 59, 36, ImageType::EnemyDetected, 0.5)) {
				MyPost(this->hWnd, WM_KEYDOWN, VK_TAB, 0);
				std::this_thread::sleep_for(std::chrono::milliseconds(1));
				MyPost(this->hWnd, WM_KEYUP, VK_TAB, 0);

				//this->AutoAttack(false);
				return;
			}
		}
	}

	Vector3 playerPos{ this->getPosition() };

	if (this->returnWaipont[this->returnIndex].mapperType == MapperType::returningEnd) {
		/*if (this->returnWaipont[this->returnIndex].Pos.x == playerPos.x
			&& this->returnWaipont[this->returnIndex].Pos.y == playerPos.y)
		{*/
		this->bNeedReturning = false;

		if (this->bAutoFishing) {
			findNearest(this->fishingWaipont, playerPos, MapperType::Fishi);
			return;
		}

		if (this->bWoodFarm) {
			findNearest(this->woodWaipont, playerPos, MapperType::Wood);
			return;
		}

		if (this->bOreFarm) {
			findNearest(this->oreWaipont, playerPos, MapperType::Ore);
			return;
		}


		//return;
	//}
	}

	if (this->returnWaipont[this->returnIndex].Pos.x == playerPos.x
		&& this->returnWaipont[this->returnIndex].Pos.y == playerPos.y
		/*&& this->oreWaipont[this->oreIndex].Pos.z == playerPos.z*/) {
		this->NextReturningPosition();
	}
	else
	{
		int x{ this->returnWaipont[this->returnIndex].Pos.x - playerPos.x };
		int y{ this->returnWaipont[this->returnIndex].Pos.y - playerPos.y };
		int z{ 0 };
		this->move(x, y, z);
		//this->lastOreMoveTime = std::chrono::steady_clock::now();
	}
}

void Client::DisableAllFarmsFunctions() {
	this->bAutoFishing = false;
	this->bFishingAutoMove = false;
	this->bAutoTask = false;
	this->bWoodFarm = false;
	this->bAutoAttack = false;
	this->bOreFarm = false;
	this->bMission = false;
}
//140 213
//148 230

void Client::AutoTask() {
	if (Recognition::CustomRecognition(this->hWnd, 553, 255, 727 - 553, 279 - 255, ImageType::greenBar, 0.6)) {
		MyPost(this->hWnd, WM_KEYDOWN, 0x46, 0);
		std::this_thread::sleep_for(std::chrono::milliseconds(300));
		MyPost(this->hWnd, WM_KEYUP, 0x46, 0);
		return;
	}
}

void Client::pressNumpadKey(int number) {
	switch (number)
	{
	case 0: {
		MyPost(this->hWnd, WM_KEYDOWN, 0x60, 0);
		std::this_thread::sleep_for(std::chrono::milliseconds(1));
		MyPost(this->hWnd, WM_KEYUP, 0x60, 0);
		this->AutoAttackIntervalTime = std::chrono::steady_clock::now();
		this->charPerson.skills[number].cooldownTime = std::chrono::steady_clock::now();
	}break;
	case 1: {
		MyPost(this->hWnd, WM_KEYDOWN, 0x61, 0);
		std::this_thread::sleep_for(std::chrono::milliseconds(1));
		MyPost(this->hWnd, WM_KEYUP, 0x61, 0);
		this->AutoAttackIntervalTime = std::chrono::steady_clock::now();
		this->charPerson.skills[number].cooldownTime = std::chrono::steady_clock::now();
	}break;
	case 2: {
		MyPost(this->hWnd, WM_KEYDOWN, 0x62, 0);
		std::this_thread::sleep_for(std::chrono::milliseconds(1));
		MyPost(this->hWnd, WM_KEYUP, 0x62, 0);
		this->AutoAttackIntervalTime = std::chrono::steady_clock::now();
		this->charPerson.skills[number].cooldownTime = std::chrono::steady_clock::now();
	}break;
	case 3: {
		MyPost(this->hWnd, WM_KEYDOWN, 0x63, 0);
		std::this_thread::sleep_for(std::chrono::milliseconds(1));
		MyPost(this->hWnd, WM_KEYUP, 0x63, 0);
		this->AutoAttackIntervalTime = std::chrono::steady_clock::now();
		this->charPerson.skills[number].cooldownTime = std::chrono::steady_clock::now();
	}break;
	case 4: {
		MyPost(this->hWnd, WM_KEYDOWN, 0x64, 0);
		std::this_thread::sleep_for(std::chrono::milliseconds(1));
		MyPost(this->hWnd, WM_KEYUP, 0x64, 0);
		this->AutoAttackIntervalTime = std::chrono::steady_clock::now();
		this->charPerson.skills[number].cooldownTime = std::chrono::steady_clock::now();
	}break;
	case 5: {
		MyPost(this->hWnd, WM_KEYDOWN, 0x65, 0);
		std::this_thread::sleep_for(std::chrono::milliseconds(1));
		MyPost(this->hWnd, WM_KEYUP, 0x65, 0);
		this->AutoAttackIntervalTime = std::chrono::steady_clock::now();
		this->charPerson.skills[number].cooldownTime = std::chrono::steady_clock::now();
	}break;
	case 6: {
		MyPost(this->hWnd, WM_KEYDOWN, 0x66, 0);
		std::this_thread::sleep_for(std::chrono::milliseconds(1));
		MyPost(this->hWnd, WM_KEYUP, 0x66, 0);
		this->AutoAttackIntervalTime = std::chrono::steady_clock::now();
		this->charPerson.skills[number].cooldownTime = std::chrono::steady_clock::now();
	}break;
	case 7: {
		MyPost(this->hWnd, WM_KEYDOWN, 0x67, 0);
		std::this_thread::sleep_for(std::chrono::milliseconds(1));
		MyPost(this->hWnd, WM_KEYUP, 0x67, 0);
		this->AutoAttackIntervalTime = std::chrono::steady_clock::now();
		this->charPerson.skills[number].cooldownTime = std::chrono::steady_clock::now();
	}break;
	case 8: {
		MyPost(this->hWnd, WM_KEYDOWN, 0x68, 0);
		std::this_thread::sleep_for(std::chrono::milliseconds(1));
		MyPost(this->hWnd, WM_KEYUP, 0x68, 0);
		this->AutoAttackIntervalTime = std::chrono::steady_clock::now();
		this->charPerson.skills[number].cooldownTime = std::chrono::steady_clock::now();
	}break;
	case 9: {
		MyPost(this->hWnd, WM_KEYDOWN, 0x69, 0);
		std::this_thread::sleep_for(std::chrono::milliseconds(1));
		MyPost(this->hWnd, WM_KEYUP, 0x69, 0);
		this->AutoAttackIntervalTime = std::chrono::steady_clock::now();
		this->charPerson.skills[number].cooldownTime = std::chrono::steady_clock::now();
	}break;
	default:
		break;
	}
}

void Client::AutoAttack(bool bOnlyHeal) {
	auto currentTime{ std::chrono::steady_clock::now() };
	auto lastAutoAttackMoveTime{ std::chrono::duration_cast<std::chrono::seconds>(currentTime - this->lastAutoAttackMoveTime).count() };
	auto AutoAttackIntervalTime{ std::chrono::duration_cast<std::chrono::seconds>(currentTime - this->lastAutoAttackMoveTime).count() };

	long long skill{};

	//for (int i{ 0 }; i < 12; i++) {
	//	if (!this->charPerson.skills[i].bUsable) {
	//		std::this_thread::sleep_for(std::chrono::milliseconds(1));
	//		continue;
	//	}

	//	//if (bOnlyHeal) {
	//	if (!this->charPerson.skills[i].healSkill) {
	//		std::this_thread::sleep_for(std::chrono::milliseconds(1));
	//		continue;
	//	}
	//	//}

	//	skill[i] = std::chrono::duration_cast<std::chrono::seconds>(currentTime - this->charPerson.skills[i].cooldownTime).count();
	//}

	int ran{ random(0, 9) };

	if (AutoAttackIntervalTime < 1.4) {
		return;
	}

	if (!this->charPerson.skills[ran].bUsable) {
		return;
	}

	skill = std::chrono::duration_cast<std::chrono::seconds>(currentTime - this->charPerson.skills[ran].cooldownTime).count();

	if (skill < this->charPerson.skills[ran].cooldown) {
		return;
	}

	if (bOnlyHeal) {
		if (!this->charPerson.skills[ran].healSkill) {
			return;
		}

		pressNumpadKey(ran);
		return;
	}

	pressNumpadKey(ran);
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

void Client::NextFishingReturn() {
	if (this->bFishingReturningIncreseDecreseIndex) {
		this->FishingReturningIndex++;
		if (this->FishingReturningIndex > this->FishingReturning.size() - 1) {
			this->bFishingReturningIncreseDecreseIndex = false;
			this->FishingReturningIndex--;
		}
	}
	else {
		this->FishingReturningIndex--;
		if (this->FishingReturningIndex == -1) {
			this->bFishingReturningIncreseDecreseIndex = true;
			this->FishingReturningIndex++;
		}
	}
}

void Client::AutoFishing() {
	Vector3 playerPos{ this->getPosition() };

	/*if (!this->fishingWaipont.size()) {
		this->fishingWaipont = mapper.waipoint;
		findNearest(this->fishingWaipont, playerPos, MapperType::Fishi);
	}*/

	if (this->getInvValue() == 60) {
		if (!this->bFishingReturning) {

			for (const auto& entry : std::filesystem::directory_iterator(xorstr_("C:\\RavendawnBot\\Farms\\NPC"))) {
				if (std::filesystem::is_regular_file(entry)) {
					std::string nomeArquivo = entry.path().stem().string();
					if (this->lastMapper == nomeArquivo) {

						std::ifstream json_file(entry.path().string());
						if (!json_file.is_open()) {
							return;
						}

						nlohmann::json json;
						json_file >> json;

						json_file.close();

						for (const auto& item : json) {
							Waipoint waipoint;
							from_json(item, waipoint);

							this->FishingReturning.push_back(waipoint);
						}
						break;
					}
				}
			}

			this->findNearest(this->FishingReturning, playerPos, MapperType::npc);
			this->bFishingReturning = true;
			return;
		}
	}

	if (this->bFishingReturning) {
		if (this->FishingReturning[this->FishingReturningIndex].mapperType == MapperType::npc)
		{
			if (!(this->FishingReturning[this->FishingReturningIndex].Pos.x == playerPos.x
				&& this->FishingReturning[this->FishingReturningIndex].Pos.y == playerPos.y)) {
				int x{ this->FishingReturning[this->FishingReturningIndex].Pos.x - playerPos.x };
				int y{ this->FishingReturning[this->FishingReturningIndex].Pos.y - playerPos.y };
				int z{ 0 };
				this->move(x, y, z);
				return;
			}
			else if (this->getInvValue() <= 5)
			{
				this->bFishingReturning = false;
				return;
			}

			if (Recognition::CustomRecognition(this->hWnd, /*1146, 669, 1120 - 11146, 703 - 669,*/ ImageType::sellButton, 0.7, false)) {
				//for (; Recognition::CustomRecognition(this->hWnd, /*1146, 669, 1120 - 11146, 703 - 669,*/ ImageType::sellButton, 0.7, true);) {
				MyPost(this->hWnd, WM_MOUSEMOVE, 0, MAKELPARAM(1158, 689));
				std::this_thread::sleep_for(std::chrono::milliseconds(5));
				MyPost(this->hWnd, WM_LBUTTONDOWN, MK_LBUTTON, 0);
				MyPost(this->hWnd, WM_LBUTTONUP, MK_LBUTTON, 0);

				MyPost(this->hWnd, WM_MOUSEMOVE, 0, MAKELPARAM(10, 10));
				std::this_thread::sleep_for(std::chrono::milliseconds(2));
				//}
				return;
			}

			if (Recognition::CustomRecognition(this->hWnd, /*931, 189, 1178 - 931, 225 - 189,*/ ImageType::sellText, 0.7, false)) {
				MyPost(this->hWnd, WM_MOUSEMOVE, 0, MAKELPARAM(1034, 209));
				std::this_thread::sleep_for(std::chrono::milliseconds(5));
				MyPost(this->hWnd, WM_LBUTTONDOWN, MK_LBUTTON, 0);
				MyPost(this->hWnd, WM_LBUTTONUP, MK_LBUTTON, 0);

				std::this_thread::sleep_for(std::chrono::milliseconds(5));
				MyPost(this->hWnd, WM_MOUSEMOVE, 0, MAKELPARAM(10, 10));

				if (Recognition::CustomRecognition(this->hWnd, /*66, 414, 342 - 66, 473 - 414,*/ ImageType::failSellText, 0.7, false)) {
					this->NextFishingPosition();
					return;
				}

				return;
			}
			else
			{
				MyPost(this->hWnd, WM_KEYDOWN, 0x46, MAKELPARAM(1, KF_REPEAT));
				std::this_thread::sleep_for(std::chrono::milliseconds(3));
				MyPost(this->hWnd, WM_KEYUP, 0x46, 0);
				return;
			}
		}

		if (this->FishingReturning[this->FishingReturningIndex].Pos.x == playerPos.x
			&& this->FishingReturning[this->FishingReturningIndex].Pos.y == playerPos.y
			/*&& this->fishingWaipont[this->fishingIndex].Pos.z == playerPos.z*/) {
			this->NextFishingReturn();
		}
		else
		{
			int x{ this->FishingReturning[this->FishingReturningIndex].Pos.x - playerPos.x };
			int y{ this->FishingReturning[this->FishingReturningIndex].Pos.y - playerPos.y };
			int z{ 0 };

			this->move(x, y, z);
			return;
		}
	}

	//if (this->fishingWaipont[this->fishingIndex].mapperType == MapperType::npc)
	//{
	//	/*if (!(this->fishingWaipont[this->fishingIndex].Pos.x == playerPos.x
	//		&& this->fishingWaipont[this->fishingIndex].Pos.y == playerPos.y)) {
	//		int x{ this->fishingWaipont[this->fishingIndex].Pos.x - playerPos.x };
	//		int y{ this->fishingWaipont[this->fishingIndex].Pos.y - playerPos.y };
	//		int z{ 0 };
	//		this->move(x, y, z);
	//		return;
	//	}*/

	//	
	//}


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
				MyPost(this->hWnd, WM_KEYDOWN, VK_NUMPAD1, 0);
				std::this_thread::sleep_for(std::chrono::milliseconds(3));
				MyPost(this->hWnd, WM_KEYUP, VK_NUMPAD1, 0);
				this->lastFirstFishingHabilit = std::chrono::steady_clock::now();
				this->fishingFirstHabilitCount++;
				this->FishingHabilitsCooldown = std::chrono::steady_clock::now();
				this->lastFishingMovement = std::chrono::steady_clock::now();
			}
		}

		if (thirtHabilitTime > 10.2) {
			MyPost(this->hWnd, WM_KEYDOWN, VK_NUMPAD3, 0);
			std::this_thread::sleep_for(std::chrono::milliseconds(3));
			MyPost(this->hWnd, WM_KEYUP, VK_NUMPAD3, 0);

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

	std::this_thread::sleep_for(std::chrono::milliseconds(600));
	if (Recognition::CustomRecognition(this->hWnd, 540, 516, 170, 26, ImageType::fishingWait, 0.4)) {
		return;
	}


	POINT pos{ -50, -30 };
	if (Recognition::CustomRecognition(this->hWnd, ImageType::fishiIcon, 0.5, pos, false)) {
		MyPost(this->hWnd, WM_MOUSEMOVE, 0, MAKELPARAM(pos.x, pos.y));
		//std::this_thread::sleep_for(std::chrono::milliseconds(1));
		MyPost(this->hWnd, WM_RBUTTONDOWN, MK_RBUTTON, 0);
		MyPost(this->hWnd, WM_RBUTTONUP, MK_RBUTTON, 0);
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

		if (x > 0) {
			MyPost(this->hWnd, WM_KEYDOWN, 0x44, 0);
			std::this_thread::sleep_for(std::chrono::milliseconds(10));
			MyPost(this->hWnd, WM_KEYUP, 0x44, 0);
		}
		else if (x < 0) {
			MyPost(this->hWnd, WM_KEYDOWN, 0x41, 0);
			std::this_thread::sleep_for(std::chrono::milliseconds(10));
			MyPost(this->hWnd, WM_KEYUP, 0x41, 0);
		}

		if (y > 0) {
			MyPost(this->hWnd, WM_KEYDOWN, 0x53, 0);
			std::this_thread::sleep_for(std::chrono::milliseconds(10));
			MyPost(this->hWnd, WM_KEYUP, 0x53, 0);
		}
		else if (y < 0) {
			MyPost(this->hWnd, WM_KEYDOWN, 0x57, 0);
			std::this_thread::sleep_for(std::chrono::milliseconds(10));
			MyPost(this->hWnd, WM_KEYUP, 0x57, 0);
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

void Client::FarmWood() {
	/*if (Recognition::CustomRecognition(this->hWnd, 510, 256, 745 - 510, 495 - 256, ImageType::interaction, 0.5)) {
		MyPost(this->hWnd, WM_KEYDOWN, 0x46, 0);
		std::this_thread::sleep_for(std::chrono::milliseconds(2));
		MyPost(this->hWnd, WM_KEYUP, 0x46, 0);
		this->lastAxieDetectTime = std::chrono::steady_clock::now();
		return;
	}*/

	if (this->getWoodInteraction() == 2) {
		if (Recognition::CustomRecognition(this->hWnd, 553, 255, 727 - 553, 279 - 255, ImageType::greenBar, 0.7)) {
			MyPost(this->hWnd, WM_KEYDOWN, 0x46, 0);
			std::this_thread::sleep_for(std::chrono::milliseconds(2));
			MyPost(this->hWnd, WM_KEYUP, 0x46, 0);
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

	Vector3 playerPos{ this->getPosition() };

	if (elapseTime < 1) {
		MyPost(this->hWnd, WM_KEYDOWN, 0x46, 0);
		std::this_thread::sleep_for(std::chrono::milliseconds(1));
		MyPost(this->hWnd, WM_KEYUP, 0x46, 0);
		return;
	}

	if (this->woodWaipont[this->woodIndex].mapperType == MapperType::Wood) {
		//MyPost(this->hWnd, WM_KEYDOWN, 0x46, 0);
		//std::this_thread::sleep_for(std::chrono::milliseconds(1));
		//MyPost(this->hWnd, WM_KEYUP, 0x46, 0);
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
			MyPost(this->hWnd, WM_KEYDOWN, 0x53, 0);
			std::this_thread::sleep_for(std::chrono::milliseconds(10));
			MyPost(this->hWnd, WM_KEYUP, 0x53, 0);
		}
		else if (y < 0) {
			MyPost(this->hWnd, WM_KEYDOWN, 0x57, 0);
			std::this_thread::sleep_for(std::chrono::milliseconds(10));
			MyPost(this->hWnd, WM_KEYUP, 0x57, 0);
		}*/
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

void Client::FarmOre() {
	if (this->getHealth() < this->getMaxHealth()) {
		if (Recognition::CustomRecognition(this->hWnd, 100, 205, 59, 36, ImageType::EnemyLocked, 0.55)) {
			this->AutoAttack(false);
			return;
		}
		else
		{
			if (Recognition::CustomRecognition(this->hWnd, 100, 205, 59, 36, ImageType::EnemyDetected, 0.5)) {
				MyPost(this->hWnd, WM_KEYDOWN, VK_TAB, 0);
				std::this_thread::sleep_for(std::chrono::milliseconds(1));
				MyPost(this->hWnd, WM_KEYUP, VK_TAB, 0);
			}
		}
		this->AutoAttack(true);
		return;
	}

	auto currentTime{ std::chrono::steady_clock::now() };
	auto elapseTime{ std::chrono::duration_cast<std::chrono::seconds>(currentTime - this->lastOreMoveTime).count() };
	auto lastOreDetectTime{ std::chrono::duration_cast<std::chrono::seconds>(currentTime - this->lastOreDetectTime).count() };

	Vector3 playerPos{ this->getPosition() };

	if (this->oreWaipont[this->oreIndex].mapperType == MapperType::Ore) {
		if (Recognition::CustomRecognition(this->hWnd, 553, 255, 727 - 553, 279 - 255, ImageType::greenBar, 0.7)) {
			MyPost(this->hWnd, WM_KEYDOWN, 0x46, 0);
			std::this_thread::sleep_for(std::chrono::milliseconds(120));
			MyPost(this->hWnd, WM_KEYUP, 0x46, 0);
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

			/*if (Recognition::CustomRecognition(this->hWnd, 510, 256, 745 - 510, 495 - 256, ImageType::interaction, 0.6)) {
				MyPost(this->hWnd, WM_KEYDOWN, 0x46, 0);
				std::this_thread::sleep_for(std::chrono::milliseconds(1));
				MyPost(this->hWnd, WM_KEYUP, 0x46, 0);
				return;
			}*/

			if (this->getWoodInteraction() != 2) {
				MyPost(this->hWnd, WM_KEYDOWN, 0x46, 0);
				std::this_thread::sleep_for(std::chrono::milliseconds(120));
				MyPost(this->hWnd, WM_KEYUP, 0x46, 0);
			}
			else
			{
				return;
			}

			if (elapseTime < 1.3) {
				return;
			}
		}
	}

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

void Client::NextCavePosition() {
	if (this->bCaveBotIncreseDecreseIndex) {
		this->cavebotIndex++;
		if (this->cavebotIndex > this->cavebotWaipoint.size() - 1) {
			this->bCaveBotIncreseDecreseIndex = false;
			this->cavebotIndex--;
		}
	}
	else {
		this->cavebotIndex--;
		if (this->cavebotIndex == -1) {
			this->bCaveBotIncreseDecreseIndex = true;
			this->cavebotIndex++;
		}
	}
}

void Client::CaveFarm() {
	if (this->getHealth() < this->getMaxHealth()) {
		if (Recognition::CustomRecognition(this->hWnd, 100, 205, 59, 36, ImageType::EnemyLocked, 0.55)) {
			this->AutoAttack(false);
			return;
		}
		else
		{
			if (Recognition::CustomRecognition(this->hWnd, 100, 205, 59, 36, ImageType::EnemyDetected, 0.5)) {
				MyPost(this->hWnd, WM_KEYDOWN, VK_TAB, 0);
				std::this_thread::sleep_for(std::chrono::milliseconds(1));
				MyPost(this->hWnd, WM_KEYUP, VK_TAB, 0);
			}
		}
		this->AutoAttack(true);
		return;
	}

	auto currentTime{ std::chrono::steady_clock::now() };
	auto elapseTime{ std::chrono::duration_cast<std::chrono::milliseconds>(currentTime - this->lastCaveFarm).count() };

	if (elapseTime < 300) {
		return;
	}

	Vector3 playerPos{ this->getPosition() };

	if (this->cavebotWaipoint[this->cavebotIndex].Pos.x == playerPos.x
		&& this->cavebotWaipoint[this->cavebotIndex].Pos.y == playerPos.y
		/*&& this->oreWaipont[this->oreIndex].Pos.z == playerPos.z*/) {
		this->NextCavePosition();
	}
	else
	{
		int x{ this->cavebotWaipoint[this->cavebotIndex].Pos.x - playerPos.x };
		int y{ this->cavebotWaipoint[this->cavebotIndex].Pos.y - playerPos.y };
		int z{ 0 };
		this->move(x, y, z);
		this->lastCaveFarm = std::chrono::steady_clock::now();
	}
}

void Client::Mission() {
	if (this->getHealth() < this->getMaxHealth()) {
		if (Recognition::CustomRecognition(this->hWnd, 100, 205, 59, 36, ImageType::EnemyLocked, 0.55)) {
			this->AutoAttack(false);
			return;
		}
		else
		{
			if (Recognition::CustomRecognition(this->hWnd, 100, 205, 59, 36, ImageType::EnemyDetected, 0.5)) {
				MyPost(this->hWnd, WM_KEYDOWN, VK_TAB, 0);
				std::this_thread::sleep_for(std::chrono::milliseconds(1));
				MyPost(this->hWnd, WM_KEYUP, VK_TAB, 0);
			}
		}
		this->AutoAttack(true);
		return;
	}

	auto currentTime{ std::chrono::steady_clock::now() };
	auto elapseTime{ std::chrono::duration_cast<std::chrono::milliseconds>(currentTime - this->lastMission).count() };

	if (elapseTime > 300) {
		return;
	}





}

void Client::NextMission() {
	if (this->bMissionIncreseDecreseIndex) {
		this->missionIndex++;
		if (this->missionIndex > this->missionWaipoint.size() - 1) {
			this->bMissionIncreseDecreseIndex = false;
			this->missionIndex--;
		}
	}
	else {
		this->missionIndex--;
		if (this->missionIndex == -1) {
			this->bMissionIncreseDecreseIndex = true;
			this->missionIndex++;
		}
	}
}

bool Client::open_handle() noexcept {
	this->handle = OpenProcess(PROCESS_ALL_ACCESS, 0, this->PID);

	GetProcessBaseAddress(this->handle, &this->hmod);

	//this->LocalPlayer = read<ULONG_PTR>(this->BaseAddress + 0x027CC100);

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
	if (!std::filesystem::exists(xorstr_("C:\\RavendawnBot"))) {
		std::filesystem::create_directories(xorstr_("C:\\RavendawnBot"));
		//here put a logic for downlaod recognitions files
	}

	if (!std::filesystem::exists(xorstr_("C:\\RavendawnBot\\accounts"))) {
		std::filesystem::create_directories(xorstr_("C:\\RavendawnBot\\accounts"));
	}

	if (!std::filesystem::exists(xorstr_("C:\\RavendawnBot\\Farms"))) {
		std::filesystem::create_directories(xorstr_("C:\\RavendawnBot\\Farms"));
	}

	if (!std::filesystem::exists(xorstr_("C:\\RavendawnBot\\Farms\\Fishi"))) {
		std::filesystem::create_directories(xorstr_("C:\\RavendawnBot\\Farms\\Fishi"));
	}

	if (!std::filesystem::exists(xorstr_("C:\\RavendawnBot\\Farms\\Wood"))) {
		std::filesystem::create_directories(xorstr_("C:\\RavendawnBot\\Farms\\Wood"));
	}

	if (!std::filesystem::exists(xorstr_("C:\\RavendawnBot\\Farms\\Cave"))) {
		std::filesystem::create_directories(xorstr_("C:\\RavendawnBot\\Farms\\Cave"));
	}

	std::string type;
	switch (FarmType)
	{
	case 0:
	case 1: {
		type = xorstr_("C:\\RavendawnBot\\Farms\\Wood\\") + FileName + xorstr_(".json");
		break;
	}
	case 2:
	case 3: {
		type = xorstr_("C:\\RavendawnBot\\Farms\\Fishi\\") + FileName + xorstr_(".json");
		break;
	}
	case 5:
	case 6: {
		type = xorstr_("C:\\RavendawnBot\\Farms\\Ore\\") + FileName + xorstr_(".json");
		break;
	}
	case 7:
	case 8: {
		type = xorstr_("C:\\RavendawnBot\\Farms\\Returning\\") + FileName + xorstr_(".json");
		break;
	}
	case 9: {
		type = xorstr_("C:\\RavendawnBot\\Farms\\Cave\\") + FileName + xorstr_(".json");
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
		//std::cerr << "Erro ao ler o arquivo JSON: " << e.what() << std::endl;
		return;
	}
	file_json.close();

	Vector3 playerPos{ this->getPosition() };

	if (this->woodWaipont.size()) {
		this->woodWaipont.clear();
	}

	if (this->fishingWaipont.size()) {
		this->fishingWaipont.clear();
	}

	if (this->oreWaipont.size()) {
		this->oreWaipont.clear();
	}

	if (this->returnWaipont.size()) {
		this->returnWaipont.clear();
	}

	if (this->cavebotWaipoint.size()) {
		this->cavebotWaipoint.clear();
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
		case 7:
		case 8: {
			this->returnWaipont.push_back(waipoint);
			break;
		}
		case 9: {
			this->cavebotWaipoint.push_back(waipoint);
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
	case 9: {
		findNearest(this->cavebotWaipoint, playerPos, MapperType::caveWalk);
		this->bCavebotPopup = true;
		break;
	}
	default:
		break;
	}
	//Notification::Notification(this->Login.c_str(), "Config loaded");
}

void Client::findNearest(std::vector<Waipoint>& vectors, const Vector3& location, const MapperType mapperType) {
	if (vectors.empty()) {
		throw std::invalid_argument(xorstr_("List of vectors is empty."));
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
		case 3:
		case 4: {
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
		case 7:
		case 8: {
			if (filter.mapperType == returning || filter.mapperType == returningEnd) {
				filtred.push_back(filter);
			}
			break;
		}
		case 9: {
			if (filter.mapperType == caveWalk) {
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
		//Notification::Notification(this->Login.c_str(), "Error");
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
	case 4: {
		this->FishingReturningIndex = nearest.index;
		break;
	}
	case 5:
	case 6: {
		this->oreIndex = nearest.index;
		break;
	}
	case 7:
	case 8: {
		this->returnIndex = nearest.index;
		break;
	}
	case 9: {
		this->cavebotIndex = nearest.index;
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
	if (!std::filesystem::exists(xorstr_("C:\\RavendawnBot"))) {
		std::filesystem::create_directories(xorstr_("C:\\RavendawnBot"));
		//here put a logic for downlaod recognitions files
	}

	if (!std::filesystem::exists(xorstr_("C:\\RavendawnBot\\accounts"))) {
		std::filesystem::create_directories(xorstr_("C:\\RavendawnBot\\accounts"));
	}

	if (!std::filesystem::exists(xorstr_("C:\\RavendawnBot\\Farms"))) {
		std::filesystem::create_directories(xorstr_("C:\\RavendawnBot\\Farms"));
	}

	if (!std::filesystem::exists(xorstr_("C:\\RavendawnBot\\Farms\\Fishi"))) {
		std::filesystem::create_directories(xorstr_("C:\\RavendawnBot\\Farms\\Fishi"));
	}

	if (!std::filesystem::exists(xorstr_("C:\\RavendawnBot\\Farms\\Wood"))) {
		std::filesystem::create_directories(xorstr_("C:\\RavendawnBot\\Farms\\Wood"));
	}

	if (!std::filesystem::exists(xorstr_("C:\\RavendawnBot\\Farms\\Cave"))) {
		std::filesystem::create_directories(xorstr_("C:\\RavendawnBot\\Farms\\Cave"));
	}

	std::string type;

	switch (FarmType)
	{
	case 0:
	case 1: {
		type = xorstr_("Wood");
		break;
	}
	case 2:
	case 3: {
		type = xorstr_("Fishi");
		break;
	}
	case 5:
	case 6: {
		type = xorstr_("Ore");
		break;
	}
	case 7:
	case 8: {
		type = xorstr_("Returning");
		break;
	}
	case 9: {
		type = xorstr_("Cave");
		break;
	}

	default:
		break;
	}

	std::ifstream file_json(xorstr_("C:\\RavendawnBot\\Farms\\") + type + xorstr_("\\") + FileName + xorstr_(".json"));
	if (!file_json.is_open()) {
		exit(0);
	}

	nlohmann::json json;
	try {
		file_json >> json;
	}
	catch (const std::exception& e) {
		std::cerr << xorstr_("Erro ao ler o arquivo JSON: ") << e.what() << std::endl;
		return;
	}
	file_json.close();

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

ULONG_PTR Client::getLocalPlayer() {
	return this->LocalPlayer = this->read<ULONG_PTR>(this->BaseAddress + 0x027D0200);
}

double Client::getHealth() {
	return this->read<double>(this->LocalPlayer + 0xCE8);
}

double Client::getMaxHealth() {
	return this->read<double>(this->LocalPlayer + 0xCF0);
}

int Client::getWoodInteraction() {
	return this->read<int>(this->LocalPlayer + 0x6EC);
}

int Client::getInvValue() {
	/*ULONG_PTR b0{ this->read<ULONG_PTR>(this->BaseAddress + 0x027CE7A8) };
	ULONG_PTR b1{ this->read<ULONG_PTR>(b0 + 0x80) };
	ULONG_PTR b2{ this->read<ULONG_PTR>(b1 + 0x8) };
	ULONG_PTR b3{ this->read<ULONG_PTR>(b2 + 0x10) };
	ULONG_PTR b4{ this->read<ULONG_PTR>(b3 + 0x10) };

	ULONG_PTR b5{ this->read<ULONG_PTR>(b4 + 0x0) };
	ULONG_PTR b6{ this->read<ULONG_PTR>(b5 + 0x28) };
	ULONG_PTR b7{ this->read<ULONG_PTR>(b6 + 0x80) };*/
	//return  this->read<int>(b7);
	return 0;
}

int Client::getAttacking() {
	//return this->read<int>(this->LocalPlayer + 0xF8);
	return 0;
}

Vector3 Client::getPosition() {
	return Vector3(this->read<int>(this->LocalPlayer + 0x18), this->read<int>(this->LocalPlayer + 0x1C), this->read<int>(this->LocalPlayer + 0x20));
}



#endif