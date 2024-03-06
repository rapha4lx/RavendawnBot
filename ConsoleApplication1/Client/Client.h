#pragma once

#include <iostream>
#include <Windows.h>
#include <nlohmann/json.hpp>
#include <thread>

#include <opencv2/opencv.hpp>
#include <list>
#include <cmath>


//class Process {
//public:

#pragma comment (lib, "ntdll.lib")

enum FindPos {
	up = 0,
	down = 1,
	left = 2,
	right = 3
};

enum MapperType {
	Wood = 0,
	WalkWood = 1,
	Fishi = 2,
	WalkFishi = 3,
	npc = 4,
	Ore = 5,
	OreWalk = 6
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
			{"PosX",  waipoint.Pos.x},
			{"PosY",  waipoint.Pos.y},
			{"PosZ",  waipoint.Pos.z},
			{"MapperType",  waipoint.mapperType},
			{"Index", waipoint.index}
		};
	}

	// Operador de extração para converter JSON para objeto da classe
	friend void from_json(const nlohmann::json& j, Waipoint& waipoint) {
		j.at("PosX").get_to(waipoint.Pos.x);
		j.at("PosY").get_to(waipoint.Pos.y);
		j.at("PosZ").get_to(waipoint.Pos.z);
		j.at("MapperType").get_to(waipoint.mapperType);

		if (j.contains("Index")) {
			j.at("Index").get_to(waipoint.index);
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
		j = nlohmann::json{ {"Login", account.Login }, {"Pass", account.Pass},
			{"Email" , account.Email}, {"Nick" , account.charPerson.Nick}, //CharPerson
			{"skills" , nlohmann::json::array()}
		};

		for (int i = 0; i < 12; i++) {
			j["skills"][i] = nlohmann::json{
				{"cooldown", account.charPerson.skills[i].cooldown},
				{"usable", account.charPerson.skills[i].bUsable},
				{"healSkill", account.charPerson.skills[i].healSkill},
				{"cooldownTime", account.charPerson.skills[i].cooldownTime.time_since_epoch().count()} // Converte para um tipo serializável
			};
		}
	}

	// Operador de extração para converter JSON para objeto da classe
	friend void from_json(const nlohmann::json& j, Client& account) {
		j.at("Login").get_to(account.Login);
		j.at("Pass").get_to(account.Pass);
		j.at("Email").get_to(account.Email);
		j.at("Nick").get_to(account.charPerson.Nick);
		//j.at("CharPerson").at().get_to(account.charPerson);
		if (j.contains("skills")) {
			auto& skillsArray = j.at("skills");
			for (int i = 0; i < 11 /*&& i < skillsArray.size()*/; ++i) {
				skillsArray[i].at("cooldown").get_to(account.charPerson.skills[i].cooldown);
				skillsArray[i].at("usable").get_to(account.charPerson.skills[i].bUsable);
				skillsArray[i].at("healSkill").get_to(account.charPerson.skills[i].healSkill);
				if (!skillsArray[i].contains("cooldownTime")) {
					account.charPerson.skills[i].cooldownTime = std::chrono::steady_clock::now();
				}
				else
				{
					account.charPerson.skills[i].cooldownTime = std::chrono::steady_clock::time_point(std::chrono::steady_clock::duration(skillsArray[i].at("cooldownTime").get<int64_t>())); // Converte de volta para time_point
				}
			}
		}
	}

	void move(int x, int y, int z);


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
	FindPos findFishingPos{ FindPos::down };
	std::chrono::steady_clock::time_point lastFishingMovement;
	std::chrono::steady_clock::time_point lastFirstFishingHabilit;
	std::chrono::steady_clock::time_point lastThirtFishingHabilit;
	std::chrono::steady_clock::time_point FishingHabilitsCooldown;
	int fishingFirstHabilitCount{ 0 };
	void NextFishingPosition();
	bool bFishingIncreseDecreseIndex{ true };




	//Wood Farm
	void FarmWood();
	std::vector<Waipoint> woodWaipont;
	FindPos farmPosition{ FindPos::down };
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



	void DisableAllFarmsFunctions();

	cv::Mat	oldPlayerPos;


	bool open_handle() noexcept;

	void close_handle() noexcept;

	template <typename T>
	constexpr T read(const std::uintptr_t& addr) noexcept {
		T value;
		// Call NtReadVirtualMemory to read the value from the target process
		NTSTATUS status = NtReadVirtualMemory(this->handle, reinterpret_cast<PVOID>(addr), &value, sizeof(T), 0);
		// Basic Error Handling
		if (status != 0) {
			std::cout << "ERROR - Failed to read value" << std::endl;
		}
		return value;
	}

	void findNearest(std::vector<Waipoint>& vectors, const Vector3& location, const MapperType mapperType);

	~Client() {
		oldPlayerPos.release();
		//close_handle();

	};


	void LoadWaipointConfig(const std::string& FileName, MapperType FarmType);

	//ui
	bool bEditPerson{ false };
	bool bAutoFishingPopup{ false };
	bool bAutoWoodFarmPopup{ false };
	bool bAutoOreFarmPopup{ false };
	bool bShowSavePopup{ false };

	//Client Memory
	double getHealth();
	double getMaxHealth();
	int getWoodInteraction();
	int getInvValue();
	int getAttacking();

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

inline std::vector<Client> Clients;
inline std::vector<Client*> ClientsFarm;

inline const char* mapperTypeIndex[]{
	"Wood", "WalkWood",
	"Fishi", "WalkFishi",
	"npc", "Ore", "OreWalk"
};
inline int mapperIndex{ 0 };

inline Mapper mapper;

#include <thread>
namespace ClientNamespace {
	void RunFarm();
	inline std::thread FarmThread(RunFarm);
}