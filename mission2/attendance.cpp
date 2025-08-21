#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <algorithm>
#include "attendance.hpp"

using namespace std;

map<string, int> player_name_to_Id_map;
int max_player_id = 0;
int attendance_count_by_date[MAX_NUM_PLAYER_ID][MAX_NUM_DATE];
int player_point[MAX_NUM_PLAYER_ID];
Grade_e grade[MAX_NUM_PLAYER_ID];
string playerNames[MAX_NUM_PLAYER_ID];

bool IsValidDate(DATE_ID date_id) {
	return date_id != MAX_NUM_DATE;
}


int getPlayerIdFromName(const string& playerName) {
	if (player_name_to_Id_map.count(playerName) == 0) {
		player_name_to_Id_map.insert({ playerName, ++max_player_id });
		playerNames[max_player_id] = playerName;
	}
	return player_name_to_Id_map[playerName];
}

DATE_ID getDateIndexFromDateStr(const string& dateStr) {
	if (dateStr == "monday") {
		return MONDAY;
	}
	if (dateStr == "tuesday") {
		return TUSEDAY;
	}
	if (dateStr == "wednesday") {
		return WEDNESDAY;
	}
	if (dateStr == "thursday") {
		return THURSDAY;
	}
	if (dateStr == "friday") {
		return FRIDAY;
	}
	if (dateStr == "saturday") {
		return SATURDAY;
	}
	if (dateStr == "sunday") {
		return SUNDAY;
	}
	return MAX_NUM_DATE;
}

int getBasePointByDate(DATE_ID date_Id) {
	switch (date_Id) {
	case WEDNESDAY:
		return BASE_POINT_WEDNESDAY;
	case SATURDAY:
	case SUNDAY:
		return  BASE_POINT_WEEKEND;
	default:
		return BASE_POINT_DEFAULT;
	}
}

void updatePlayer(int player_Id, DATE_ID date_Id) {
	if (false == IsValidDate(date_Id)) return;

	player_point[player_Id] += getBasePointByDate(date_Id);
	attendance_count_by_date[player_Id][date_Id] += 1;
}


int getBonusPointByWendsDay(int attendance_count_wednesday) {
	if (attendance_count_wednesday >= WEDNESDAY_BONUS__POINT_THRESHOLD) 
		return WEDNESDAY_BONUS_PONINT;
	return 0;
}

int getBonusPointByWeekend(int attendance_count_weekend) {
	if (attendance_count_weekend >= WEEKEND_BONUS_POINT_THRESHOLD)
		return WEEKEND_BONUS_PONINT;
	return 0;
}

Grade_e getGradeFromPoint(int point) {
	if (point >= GOLD_GRADE_THRESHOLD) return GRADE_GOLD;
	if (point >= SILVER_GRADE_THRESHOLD) return GRADE_SILVER;
	return GRADE_NORMAL;
}

void summarizePlayers() {
	for (int player_id = 1; player_id <= max_player_id; player_id++) {
		player_point[player_id] += getBonusPointByWendsDay(attendance_count_by_date[player_id][WEDNESDAY]);

		int attendance_count_weekend = attendance_count_by_date[player_id][SATURDAY] + attendance_count_by_date[player_id][SUNDAY];
		player_point[player_id] += getBonusPointByWeekend(attendance_count_weekend);

		grade[player_id] = getGradeFromPoint(player_point[player_id]);
	}
}

const char* getGradeStr(int grade) {
	if (grade == GRADE_GOLD) return "GOLD";
	if (grade == GRADE_SILVER) return "SILVER";
	return "NORMAL";
}

const char* getGradeStrFromPlayerId(int player_id) {
	return getGradeStr(grade[player_id]);
}

const bool IsRemovedPlayer(Grade_e grade, int (&player_attendance_count_by_date)[MAX_NUM_DATE]) {
	if (grade == GRADE_GOLD || grade == GRADE_SILVER) return false;
	if (player_attendance_count_by_date[WEDNESDAY] > 0) return false;
	if (player_attendance_count_by_date[SATURDAY] > 0) return false;
	if (player_attendance_count_by_date[SUNDAY] > 0) return false;
	return true;
}

vector<int> getRemovedPlayerId(){
	vector<int> result;
	for (int player_id = 1; player_id <= max_player_id; player_id++) {
		if (IsRemovedPlayer(grade[player_id], attendance_count_by_date[player_id])) {
			result.push_back(player_id);
		}
	}
	return result;
}

void attendanceManageWeekdayDataFile(const char* weekdayDataFileName, int dataNum) {

	ifstream fin{ weekdayDataFileName };
	for (int i = 0; i < dataNum; i++) {
		WeekDayData weekdayData;
		fin >> weekdayData.playerName >> weekdayData.dateStr;
		updatePlayer(getPlayerIdFromName(weekdayData.playerName), getDateIndexFromDateStr(weekdayData.dateStr));
	}

	summarizePlayers();

	for (int player_id = 1; player_id <= max_player_id; player_id++) {
		cout << "NAME : " << playerNames[player_id] << ", ";
		cout << "POINT : " << player_point[player_id] << ", ";
		cout << "GRADE : " << getGradeStrFromPlayerId(player_id) << "\n";
	}

	std::cout << "\n";
	std::cout << "Removed player\n";
	std::cout << "==============\n";
	vector<int> removedPlayerId = getRemovedPlayerId();
	for (int player_Id : removedPlayerId) {
		std::cout << playerNames[player_Id] << "\n";
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
	attendanceManageWeekdayDataFile(weekdayDataFileName, WEEKDAY_DATA_NUM);
}

#endif