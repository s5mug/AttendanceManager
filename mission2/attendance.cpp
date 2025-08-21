#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <algorithm>
#include "attendance.hpp"

using namespace std;

int AttendanceManager::getPlayerIdFromName(const string& playerName) {
	if (player_name_to_Id_map.count(playerName) == 0) {
		int newId = players.size() + 1;
		players.emplace_back(playerName, newId);
		player_name_to_Id_map.emplace(playerName, newId);
		return newId;
	}
	return player_name_to_Id_map[playerName];
}

Player& AttendanceManager::getPlayerFromId(int player_Id) {
	return players[player_Id - 1];
}

void AttendanceManager::updatePlayerPoint() {
	for (Player& player : players) {
		player.updatePoint(&dateWorkPolicy);
	}
}

void AttendanceManager::updatePlayerGrade() {
	for (Player& player : players) {
		Grade* grade = gradeChecker.getGradeFromPoint(player.getPoint());
		player.setGrade(grade);
	}
}

vector<Player*> AttendanceManager::getRemovedPlayerId(){
	vector<Player*> result;
	for (Player& player : players) {
		if (player.IsRemovedPlayer(&dateWorkPolicy)) {
			result.push_back(&player);
		}
	}
	return result;
}

void AttendanceManager::ManageWeekdayDataFile(const char* weekdayDataFileName, int dataNum) {

	ifstream fin{ weekdayDataFileName };
	for (int i = 0; i < dataNum; i++) {
		WeekDayData weekdayData;
		fin >> weekdayData.playerName >> weekdayData.dateStr;

		Player& player = getPlayerFromId(getPlayerIdFromName(weekdayData.playerName));
		player.updateNewDateWork(getDateIndexFromDateStr(weekdayData.dateStr));
	}

	updatePlayerPoint();
	updatePlayerGrade();
	printSummary();
}

void AttendanceManager::printSummary() {
	for (Player& player : players) {
		cout << "NAME : " << player.getName() << ", ";
		cout << "POINT : " << player.getPoint() << ", ";
		cout << "GRADE : " << player.getGrade()->getStr() << "\n";
	}

	std::cout << "\n";
	std::cout << "Removed player\n";
	std::cout << "==============\n";
	vector<Player*> removedPlayers = getRemovedPlayerId();
	for (Player* player : removedPlayers) {
		std::cout << player->getName() << "\n";
	}
}


#include"gmock/gmock.h"
#ifdef _DEBUG

int main() {
	testing::InitGoogleMock();
	return RUN_ALL_TESTS();
}

#else 

int main() {
	const char* weekdayDataFileName = "attendance_weekday_500.txt";
	const int WEEKDAY_DATA_NUM = 500;


	BaseDatePolicy basePolicy;
	WeekendBonusPolicy weekendPolicy;
	WednesdayBonusPolicy wendneesdayPolicy;
	MultipleDateWorkPolicy datePolicy{ {&basePolicy, &weekendPolicy, &wendneesdayPolicy} };
	GradeChecker gradeChecker{ NormalGrade::getInstance(), SilverGrade::getInstance(), GoldGrade::getInstance() };

	AttendanceManager manger{ datePolicy , gradeChecker };
	manger.ManageWeekdayDataFile(weekdayDataFileName, WEEKDAY_DATA_NUM);
}

#endif