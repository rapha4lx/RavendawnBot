#pragma once

#include "../Client/Client.h"
#include <opencv2/opencv.hpp>


enum ImageType {
	street = 0,
	greenBar = 1,

	axieIcon = 2,
	axieWaiting = 11,

	interaction = 3,
	ColectionIcon = 4,
	EnemyDetected = 5,
	EnemyLocked = 6,
	fishiIcon = 7,
	fishingRood = 8,
	fishingWait = 9,
	cleanSlotInv = 10,


	error_fishing = 95,
	error_mining = 96,
	failSellText = 97,
	sellText = 98,
	sellButton = 99,





	death_screen = 120,
	restore_soul_screen = 121
};


inline std::vector<std::pair<std::string, ImageType>> Images{
{ "C:\\RavendawnBot\\axie_icon0.png", ImageType::axieIcon }, { "C:\\RavendawnBot\\axie_icon1.png", ImageType::axieIcon },
{ "C:\\RavendawnBot\\axie_icon2.png", ImageType::axieIcon },

{ "C:\\RavendawnBot\\axie_wait0.png", ImageType::axieWaiting }, { "C:\\RavendawnBot\\axie_wait2.png", ImageType::axieWaiting },
{ "C:\\RavendawnBot\\axie_wait2.png", ImageType::axieWaiting },

{ "C:\\RavendawnBot\\green bar 0.png", ImageType::greenBar }, { "C:\\RavendawnBot\\green bar 1.png", ImageType::greenBar },

//{ "C:\\RavendawnBot\\street.png", ImageType::street },
//{ "C:\\RavendawnBot\\street2.png", ImageType::street }, { "C:\\RavendawnBot\\street3.png", ImageType::street },
//{ "C:\\RavendawnBot\\street4.png", ImageType::street }, { "C:\\RavendawnBot\\street5.png", ImageType::street },
//{ "C:\\RavendawnBot\\street6.png", ImageType::street }, { "C:\\RavendawnBot\\street7.png", ImageType::street },
//{ "C:\\RavendawnBot\\street8.png", ImageType::street }, { "C:\\RavendawnBot\\street9.png", ImageType::street },
//{ "C:\\RavendawnBot\\street10.png", ImageType::street }, { "C:\\RavendawnBot\\street11.png", ImageType::street },
//{ "C:\\RavendawnBot\\street12.png", ImageType::street }, { "C:\\RavendawnBot\\street13.png", ImageType::street },
//{ "C:\\RavendawnBot\\street14.png", ImageType::street }, { "C:\\RavendawnBot\\street15.png", ImageType::street },
//{ "C:\\RavendawnBot\\street16.png", ImageType::street }, { "C:\\RavendawnBot\\street17.png", ImageType::street },
//{ "C:\\RavendawnBot\\street18.png", ImageType::street },
//
//{ "C:\\RavendawnBot\\ground0.png", ImageType::street }, { "C:\\RavendawnBot\\ground1.png", ImageType::street },
//{ "C:\\RavendawnBot\\ground2.png", ImageType::street }, { "C:\\RavendawnBot\\ground3.png", ImageType::street },
//{ "C:\\RavendawnBot\\ground4.png", ImageType::street }, { "C:\\RavendawnBot\\ground5.png", ImageType::street },
//{ "C:\\RavendawnBot\\ground6.png", ImageType::street }, /*{ "C:\\RavendawnBot\\ground7.png", ImageType::street },*/
//{ "C:\\RavendawnBot\\ground8.png", ImageType::street }, { "C:\\RavendawnBot\\ground9.png", ImageType::street },
//{ "C:\\RavendawnBot\\ground10.png", ImageType::street }, { "C:\\RavendawnBot\\ground11.png", ImageType::street },
//{ "C:\\RavendawnBot\\ground12.png", ImageType::street }, { "C:\\RavendawnBot\\ground13.png", ImageType::street },
///*{ "C:\\RavendawnBot\\ground14.png", ImageType::street },*/ { "C:\\RavendawnBot\\ground15.png", ImageType::street },
///*{ "C:\\RavendawnBot\\ground16.png", ImageType::street },*/ { "C:\\RavendawnBot\\ground17.png", ImageType::street },
//{ "C:\\RavendawnBot\\ground18.png", ImageType::street }, { "C:\\RavendawnBot\\ground19.png", ImageType::street },
//{ "C:\\RavendawnBot\\ground20.png", ImageType::street }, { "C:\\RavendawnBot\\ground21.png", ImageType::street },
//{ "C:\\RavendawnBot\\ground22.png", ImageType::street }, { "C:\\RavendawnBot\\ground23.png", ImageType::street },
//{ "C:\\RavendawnBot\\ground24.png", ImageType::street }, { "C:\\RavendawnBot\\ground25.png", ImageType::street },
//{ "C:\\RavendawnBot\\ground26.png", ImageType::street }, { "C:\\RavendawnBot\\ground27.png", ImageType::street },
//{ "C:\\RavendawnBot\\ground28.png", ImageType::street }, { "C:\\RavendawnBot\\ground29.png", ImageType::street },
//{ "C:\\RavendawnBot\\ground30.png", ImageType::street }, { "C:\\RavendawnBot\\ground31.png", ImageType::street },
//{ "C:\\RavendawnBot\\ground32.png", ImageType::street }, { "C:\\RavendawnBot\\ground33.png", ImageType::street },
//{ "C:\\RavendawnBot\\ground34.png", ImageType::street }, { "C:\\RavendawnBot\\ground35.png", ImageType::street },
//{ "C:\\RavendawnBot\\ground36.png", ImageType::street }, { "C:\\RavendawnBot\\ground37.png", ImageType::street },
//{ "C:\\RavendawnBot\\ground38.png", ImageType::street }, { "C:\\RavendawnBot\\ground39.png", ImageType::street },
//{ "C:\\RavendawnBot\\ground40.png", ImageType::street }, { "C:\\RavendawnBot\\ground41.png", ImageType::street },
//{ "C:\\RavendawnBot\\ground42.png", ImageType::street }, { "C:\\RavendawnBot\\ground43.png", ImageType::street },
//{ "C:\\RavendawnBot\\ground44.png", ImageType::street }, { "C:\\RavendawnBot\\ground45.png", ImageType::street },
//{ "C:\\RavendawnBot\\ground46.png", ImageType::street },

{ "C:\\RavendawnBot\\interact0.png", ImageType::interaction }, { "C:\\RavendawnBot\\interact1.png", ImageType::interaction },

{ "C:\\RavendawnBot\\coleting0.png", ImageType::ColectionIcon }, { "C:\\RavendawnBot\\coleting1.png", ImageType::ColectionIcon },


{ "C:\\RavendawnBot\\enemy_find.png", ImageType::EnemyDetected }, { "C:\\RavendawnBot\\enemy_locked.png", ImageType::EnemyLocked },

{ "C:\\RavendawnBot\\fishing_icon0.png", ImageType::fishiIcon },
/*, { "C:\\RavendawnBot\\fishing_icon1.png", ImageType::fishiIcon },
{ "C:\\RavendawnBot\\fishing_icon2.png", ImageType::fishiIcon },*/
{ "C:\\RavendawnBot\\fishing_rood.png", ImageType::fishingRood }, { "C:\\RavendawnBot\\fishing_wait0.png", ImageType::fishingWait },
{ "C:\\RavendawnBot\\fishing_wait1.png", ImageType::fishingWait }, { "C:\\RavendawnBot\\fishing_wait2.png", ImageType::fishingWait },
{ "C:\\RavendawnBot\\fishing_wait3.png", ImageType::fishingWait },
{ "C:\\RavendawnBot\\error_fishing0.png", ImageType::error_fishing},

{ "C:\\RavendawnBot\\donthavefishi.png", ImageType::failSellText },
{ "C:\\RavendawnBot\\sell_fish.png", ImageType::sellText },
{ "C:\\RavendawnBot\\sell_button.png", ImageType::sellButton },

{ "C:\\RavendawnBot\\inventory.png", ImageType::cleanSlotInv},

{ "C:\\RavendawnBot\\error_mining0.png", ImageType::error_mining}, { "C:\\RavendawnBot\\error_mining1.png", ImageType::error_mining},
{ "C:\\RavendawnBot\\error_mining2.png", ImageType::error_mining}, { "C:\\RavendawnBot\\error_mining3.png", ImageType::error_mining},

{ "C:\\RavendawnBot\\death_screen1.png", ImageType::death_screen}, { "C:\\RavendawnBot\\death_screen.png", ImageType::restore_soul_screen},


};



namespace Recognition {
	bool testefishi(HWND& hWnd, ImageType imageType, double threshold, bool debug);
	bool CustomRecognition(HWND& hWnd, ImageType imageType, double threshold, bool debug);
	bool CustomRecognition(HWND& hWnd, ImageType imageType, double threshold, POINT& pos, bool debug);
	bool CustomRecognition(HWND& hWnd, int posX, int posY, int sizeX, int sizeY, ImageType imageType, double threshold);
	bool CustomRecognition(HWND& hWnd, int posX, int posY, int sizeX, int sizeY, ImageType imageType, double threshold, POINT& pos);

	bool ComparOldPos(HWND& hWnd, cv::Mat& player);
	void UpdatePos(HWND& hWnd, cv::Mat& player);




	inline std::vector<cv::Point> bestWoodVec{};
	inline cv::Point bestWood;
	//Recognition Wood Farm
	bool WoodFarm(HWND& hWnd, bool debug);
	//bool WoodFarm(HWND& hWNd, POINT& pos, bool debug);

}