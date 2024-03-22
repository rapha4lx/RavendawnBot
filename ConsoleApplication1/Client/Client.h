#pragma once

#include <iostream>
#include <Windows.h>
#include <nlohmann/json.hpp>
#include <thread>

#include <opencv2/opencv.hpp>
#include <list>
#include <cmath>
#include "../Protection/xorstr.hpp"

#pragma comment (lib, "ntdll.lib")

#if defined(BOT)
enum MapperType {
	Wood = 0,
	WalkWood = 1,
	Fishi = 2,
	WalkFishi = 3,
	npc = 4,
	Ore = 5,
	OreWalk = 6,
	returning = 7,
	returningEnd = 8,
	caveWalk = 9
};

struct Vector3 {
	constexpr Vector3(
		const int x = 0,
		const int y = 0,
		const int z = 0) noexcept :
		x(x), y(y), z(z) { }

	// operator overloads
	constexpr const Vector3& operator-(const Vector3& other) const noexcept
	{
		return Vector3{ x - other.x, y - other.y, z - other.z };
	}

	constexpr const Vector3& operator+(const Vector3& other) const noexcept
	{
		return Vector3{ x + other.x, y + other.y, z + other.z };
	}

	constexpr const Vector3& operator/(const int factor) const noexcept
	{
		return Vector3{ x / factor, y / factor, z / factor };
	}

	constexpr const Vector3& operator*(const int factor) const noexcept
	{
		return Vector3{ x * factor, y * factor, z * factor };
	}

	constexpr const bool operator>(const Vector3& other) const noexcept {
		return x > other.x && y > other.y && z > other.z;
	}

	constexpr const bool operator>=(const Vector3& other) const noexcept {
		return x >= other.x && y >= other.y && z >= other.z;
	}

	constexpr const bool operator<(const Vector3& other) const noexcept {
		return x < other.x && y < other.y && z < other.z;
	}

	constexpr const bool operator<=(const Vector3& other) const noexcept {
		return x <= other.x && y <= other.y && z <= other.z;
	}

	//// utils
	//constexpr const Vector3& ToAngle() const noexcept
	//{
	//	return Vector3{
	//		std::atan2(-z, std::hypot(x, y)) * (180.0f / std::numbers::pi_v<int>),
	//		std::atan2(y, x) * (180.0f / std::numbers::pi_v<int>),
	//		0.0f };
	//}

	int length() const {
		return std::sqrt(x * x + y * y + z * z);
	}

	int length2d() const {
		return std::sqrt(x * x + y * y);
	}

	constexpr const bool IsZero() const noexcept
	{
		return x == 0.f && y == 0.f && z == 0.f;
	}

	// struct data
	int x, y, z;
};

EXTERN_C NTSTATUS NTAPI NtReadVirtualMemory(HANDLE, PVOID, PVOID, ULONG, PULONG);

class Waipoint {
public:
	Vector3 Pos{};
	MapperType mapperType{ MapperType::Wood };
	int index{ 0 };

	// Operador de inserção para converter objeto da classe para JSON
	friend void to_json(nlohmann::json& j, const Waipoint& waipoint) {
		j = nlohmann::json{
			{xorstr_("PosX"),  waipoint.Pos.x},
			{xorstr_("PosY"),  waipoint.Pos.y},
			{xorstr_("PosZ"),  waipoint.Pos.z},
			{xorstr_("MapperType"),  waipoint.mapperType},
			{xorstr_("Index"), waipoint.index}
		};
	}

	// Operador de extração para converter JSON para objeto da classe
	friend void from_json(const nlohmann::json& j, Waipoint& waipoint) {
		j.at(xorstr_("PosX")).get_to(waipoint.Pos.x);
		j.at(xorstr_("PosY")).get_to(waipoint.Pos.y);
		j.at(xorstr_("PosZ")).get_to(waipoint.Pos.z);
		j.at(xorstr_("MapperType")).get_to(waipoint.mapperType);

		if (j.contains(xorstr_("Index"))) {
			j.at(xorstr_("Index")).get_to(waipoint.index);
		}
		else {
			// Se o campo "Index" não estiver presente, define como 0
			waipoint.index = 0;
		}

	}
};

class Client {
public:
	DWORD PID{ 0 };
	HWND hWnd{ NULL };
	HANDLE handle;

	union {
		HMODULE hmod;
		ULONG_PTR BaseAddress;
	};
	ULONG_PTR LocalPlayer;


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

		class Skills {
		public:
			float cooldown{ 0 };
			std::chrono::steady_clock::time_point cooldownTime;
			bool bUsable{ false };
			bool healSkill{ false };

		private:
		};
		Skills skills[12];
	};

	CharPerson charPerson;

	// Operador de inserção para converter objeto da classe para JSON
	friend void to_json(nlohmann::json& j, const Client& account) {
		j = nlohmann::json{ {xorstr_("Login"), account.Login }, {xorstr_("Pass"), account.Pass},
			{xorstr_("Email") , account.Email}, {xorstr_("Nick") , account.charPerson.Nick}, //CharPerson
			{xorstr_("skills") , nlohmann::json::array()}
		};

		for (int i = 0; i < 12; i++) {
			j[xorstr_("skills")][i] = nlohmann::json{
				{xorstr_("cooldown"), account.charPerson.skills[i].cooldown},
				{xorstr_("usable"), account.charPerson.skills[i].bUsable},
				{xorstr_("healSkill"), account.charPerson.skills[i].healSkill}
				//{"cooldownTime", account.charPerson.skills[i].cooldownTime.time_since_epoch().count()} // Converte para um tipo serializável
			};
		}
	}

	// Operador de extração para converter JSON para objeto da classe
	friend void from_json(const nlohmann::json& j, Client& account) {
		j.at(xorstr_("Login")).get_to(account.Login);
		j.at(xorstr_("Pass")).get_to(account.Pass);
		j.at(xorstr_("Email")).get_to(account.Email);
		j.at(xorstr_("Nick")).get_to(account.charPerson.Nick);
		//j.at("CharPerson").at().get_to(account.charPerson);
		if (j.contains(xorstr_("skills"))) {
			auto& skillsArray = j.at(xorstr_("skills"));
			for (int i = 0; i < 11 /*&& i < skillsArray.size()*/; ++i) {
				skillsArray[i].at(xorstr_("cooldown")).get_to(account.charPerson.skills[i].cooldown);
				skillsArray[i].at(xorstr_("usable")).get_to(account.charPerson.skills[i].bUsable);
				skillsArray[i].at(xorstr_("healSkill")).get_to(account.charPerson.skills[i].healSkill);
			}
		}
	}

	void move(int x, int y, int z);
	void pressNumpadKey(int number);

	//Auto solve task
	void AutoTask();
	bool bAutoTask{ false };

	//char attack
	void AutoAttack(bool bOnlyHeal = false);
	bool bAutoAttack{ false };
	bool bAutoAttackMove{ false };
	std::chrono::steady_clock::time_point lastAutoAttackMoveTime;
	std::chrono::steady_clock::time_point AutoAttackIntervalTime;

	//Auto fishing
	void AutoFishing();
	std::vector<Waipoint> fishingWaipont;
	std::vector<Waipoint> oldFishingWaipont;
	int fishingIndex{ -1 };
	bool bAutoFishing{ false };
	bool bFishingAutoMove{ false };
	std::chrono::steady_clock::time_point lastFishingMovement;
	std::chrono::steady_clock::time_point lastFirstFishingHabilit;
	std::chrono::steady_clock::time_point lastThirtFishingHabilit;
	std::chrono::steady_clock::time_point FishingHabilitsCooldown;
	int fishingFirstHabilitCount{ 0 };
	void NextFishingPosition();
	bool bFishingIncreseDecreseIndex{ true };

	//fihing return
	bool bFishingReturning{ false };
	bool bFishingReturningIncreseDecreseIndex{ true };
	std::vector<Waipoint>FishingReturning;
	int FishingReturningIndex{ -1 };
	void NextFishingReturn();

	//Wood Farm
	void FarmWood();
	std::vector<Waipoint> woodWaipont;
	int woodIndex{ -1 };
	int woodFarmStage{ 0 };
	int woodErrorCount{ 0 };
	bool bWoodFarm{ false };
	std::chrono::steady_clock::time_point lastAxieDetectTime;
	std::chrono::steady_clock::time_point changeFarmPositionTime;
	void NextWoodPosition();
	bool bWoodIncreseDecreseIndex{ true };

	//Ore Farm
	void FarmOre();
	std::vector<Waipoint> oreWaipont;
	int oreIndex{ -1 };
	int bOreFarm{ false };
	std::chrono::steady_clock::time_point lastOreMoveTime;
	std::chrono::steady_clock::time_point lastOreDetectTime;
	void NextOrePosition();
	bool bOreIncreseDecreseIndex{ true };

	//Return Farm
	bool bNeedReturning{ false };
	bool bReturnIncreseDecreseIndex{ true };
	int returnIndex{ -1 };
	std::vector<Waipoint> returnWaipont;
	void CheckIfHasReturnFile();
	void NextReturningPosition();
	void Returning();
	std::string lastMapper;

	//cave Farm
	void CaveFarm();
	bool bCaveBot{ false };
	bool bCaveBotIncreseDecreseIndex{ true };
	std::vector<Waipoint> cavebotWaipoint;
	int cavebotIndex{ -1 };
	std::chrono::steady_clock::time_point lastCaveFarm;
	void NextCavePosition();


	void DisableAllFarmsFunctions();

	bool open_handle() noexcept;

	void close_handle() noexcept;

	template <typename T>
	constexpr T read(const std::uintptr_t& addr) noexcept {
		T value;
		// Call NtReadVirtualMemory to read the value from the target process
		NTSTATUS status = NtReadVirtualMemory(this->handle, reinterpret_cast<PVOID>(addr), &value, sizeof(T), 0);
		// Basic Error Handling
		if (status != 0) {
			//std::cout << "ERROR - Failed to read value" << std::endl;
			//MessageBox(0, "Failed to read value", 0, 0);
		}
		return value;
	}

	void findNearest(std::vector<Waipoint>& vectors, const Vector3& location, const MapperType mapperType);


	HMODULE hUser32;

	~Client() {
		FreeLibrary(hUser32);
		close_handle();
	};

	typedef BOOL(__stdcall* myPost)(_In_opt_ HWND hWnd,
		_In_ UINT Msg,
		_In_ WPARAM wParam,
		_In_ LPARAM lParam);

	myPost MyPost;

	Client() {
		/*if (!std::filesystem::exists("C:\\Windows\\System32\\user323.dll")) {

			std::filesystem::copy_file("C:\\Windows\\System32\\user32.dll", "C:\\Windows\\System32\\user323.dll");
		}

		if (!std::filesystem::exists("C:\\Windows\\SysWOW64\\user323.dll")) {

			std::filesystem::copy_file("C:\\Windows\\SysWOW64\\user32.dll", "C:\\Windows\\SysWOW64\\user323.dll");
		}*/

		hUser32 = LoadLibraryA("user32.dll");

		if (hUser32 != NULL) {
			//std::cout << "Biblioteca user32.dll carregada com sucesso!" << std::endl;

			MyPost = (myPost)GetProcAddress(hUser32, "PostMessageA");
			
			FreeLibrary(hUser32);
		}
		else {
			std::cerr << "Erro ao carregar a biblioteca user32.dll." << std::endl;
			return; 
		}
	}


	void LoadWaipointConfig(const std::string& FileName, MapperType FarmType);

	//ui
	bool bEditPerson{ false };
	bool bAutoFishingPopup{ false };
	bool bAutoWoodFarmPopup{ false };
	bool bAutoOreFarmPopup{ false };
	bool bCavebotPopup{ false };
	bool bShowSavePopup{ false };

	//Client Memory
	ULONG_PTR getLocalPlayer();
	double getHealth();
	double getMaxHealth();
	int getWoodInteraction();
	int getInvValue();
	int getAttacking();
	Vector3 getPosition();

private:
	double distance(const Vector3& v1, const Vector3& v2) {
		return std::sqrt(std::pow(v1.x - v2.x, 2) + std::pow(v1.y - v2.y, 2) + std::pow(v1.z - v2.z, 2));
	}

};

class Mapper {
public:
	int maxIndex{ -1 };
	std::vector<Waipoint> waipoint;
	void Load(const std::string& FileName, MapperType FarmType);
	~Mapper();
};

inline Client client;

inline const char* mapperTypeIndex[]{
	"Wood", "WalkWood",
	"Fishi", "WalkFishi",
	"npc", "Ore", "OreWalk",
	"Returning", "returningEnd",
	"CaveWalk"
};
inline int mapperIndex{ 0 };

inline Mapper mapper;
#endif 