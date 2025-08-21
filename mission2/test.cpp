#include "gmock/gmock.h"
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <algorithm>
#include "attendance.hpp"


using namespace testing;
using namespace std;

extern int max_player_id;;
extern map<string, int> player_name_to_Id_map;
extern int player_point[MAX_NUM_PLAYER_ID];
extern int attendance_count_by_date[MAX_NUM_PLAYER_ID][MAX_NUM_DATE];

class AttendanceMangerFixture : public Test {
public:
	void SetUp() override {
		InitManger();
		ANY_PLAYER_ID = getPlayerIdFromName(ANY_NAME);
	}

	void InitManger() {
		max_player_id = 0;
		player_name_to_Id_map.clear();
		memset(player_point, 0, sizeof(player_point));
		memset(attendance_count_by_date, 0, sizeof(attendance_count_by_date));
	}

	int TestBasePointIncrease(DATE_ID date) {
		int old_player_point = player_point[ANY_PLAYER_ID];
		updatePlayer(ANY_PLAYER_ID, date);
		int new_player_point = player_point[ANY_PLAYER_ID];
		return new_player_point - old_player_point;
	}

	int TestBonusPointIncreaseAfterSummarize() {
		int old_player_point = player_point[ANY_PLAYER_ID];
		summarizePlayers();
		int new_player_point = player_point[ANY_PLAYER_ID];
		return new_player_point - old_player_point;
	}

	string getSampleOutput() {
		ifstream fin{ "sample_output.txt" };
		std::stringstream sstr;
		while (fin >> sstr.rdbuf());
		return sstr.str();
	}

	string ANY_NAME = "ANY";
	string OTHER_NAME = "OTHER";
	int ANY_PLAYER_ID = 0;
};

TEST_F(AttendanceMangerFixture, ValidDateTest) {
	vector<DATE_ID> validDates{ MONDAY, TUSEDAY , WEDNESDAY , THURSDAY , FRIDAY , SATURDAY , SUNDAY };
	for (auto validDate : validDates) {
		EXPECT_TRUE(IsValidDate(validDate));
	}

	EXPECT_FALSE(IsValidDate(MAX_NUM_DATE));
}


TEST_F(AttendanceMangerFixture, PlayerId_NewPlayerTest) {
	int any_player_Id = getPlayerIdFromName(ANY_NAME);
	int other_player_Id = getPlayerIdFromName(OTHER_NAME);
	EXPECT_NE(any_player_Id, other_player_Id);
}

TEST_F(AttendanceMangerFixture, PlayerId_SamePlayerTest) {
	int any_player_Id = getPlayerIdFromName(ANY_NAME);
	int any_player_Id2 = getPlayerIdFromName(ANY_NAME);
	EXPECT_EQ(any_player_Id, any_player_Id2);
}

TEST_F(AttendanceMangerFixture, DateStrToDateIndex) {
	EXPECT_EQ(MONDAY, getDateIndexFromDateStr("monday"));
	EXPECT_EQ(TUSEDAY, getDateIndexFromDateStr("tuesday"));
	EXPECT_EQ(WEDNESDAY, getDateIndexFromDateStr("wednesday"));
	EXPECT_EQ(THURSDAY, getDateIndexFromDateStr("thursday"));
	EXPECT_EQ(FRIDAY, getDateIndexFromDateStr("friday"));
	EXPECT_EQ(SATURDAY, getDateIndexFromDateStr("saturday"));
	EXPECT_EQ(SUNDAY, getDateIndexFromDateStr("sunday"));
	EXPECT_EQ(MAX_NUM_DATE, getDateIndexFromDateStr("ERROR"));
}

TEST_F(AttendanceMangerFixture, UpdatePlayerWithDefaultBase) {
	EXPECT_EQ(BASE_POINT_DEFAULT, TestBasePointIncrease(MONDAY));
	EXPECT_EQ(BASE_POINT_DEFAULT, TestBasePointIncrease(TUSEDAY));
	EXPECT_EQ(BASE_POINT_DEFAULT, TestBasePointIncrease(THURSDAY));
	EXPECT_EQ(BASE_POINT_DEFAULT, TestBasePointIncrease(FRIDAY));

	EXPECT_EQ(BASE_POINT_WEDNESDAY, TestBasePointIncrease(WEDNESDAY));

	EXPECT_EQ(BASE_POINT_WEEKEND, TestBasePointIncrease(SATURDAY));
	EXPECT_EQ(BASE_POINT_WEEKEND, TestBasePointIncrease(SUNDAY));

	EXPECT_EQ(0, TestBasePointIncrease(MAX_NUM_DATE));
}

TEST_F(AttendanceMangerFixture, BonusByWednesDay) {
	int WENDSDAY_LESS_THAN_THRESHOLD = WEDNESDAY_BONUS__POINT_THRESHOLD - 1;
	for (int i = 0; i < WENDSDAY_LESS_THAN_THRESHOLD; i++) {
		updatePlayer(ANY_PLAYER_ID, WEDNESDAY);
	}

	EXPECT_EQ(0, TestBonusPointIncreaseAfterSummarize());

	for (int i = 0; i < WEDNESDAY_BONUS__POINT_THRESHOLD; i++) {
		updatePlayer(ANY_PLAYER_ID, WEDNESDAY);
	}

	EXPECT_EQ(WEDNESDAY_BONUS_PONINT, TestBonusPointIncreaseAfterSummarize());
}

TEST_F(AttendanceMangerFixture, BonusByWeekend) {
	int WEEKEND_LESS_THAN_THRESHOLD = WEEKEND_BONUS_POINT_THRESHOLD - 1;
	for (int i = 0; i < WEEKEND_LESS_THAN_THRESHOLD; i++) {
		updatePlayer(ANY_PLAYER_ID, SUNDAY);
	}

	EXPECT_EQ(0, TestBonusPointIncreaseAfterSummarize());

	for (int i = 0; i < WEEKEND_BONUS_POINT_THRESHOLD; i++) {
		updatePlayer(ANY_PLAYER_ID, SUNDAY);
	}

	EXPECT_EQ(WEEKEND_BONUS_PONINT, TestBonusPointIncreaseAfterSummarize());
}

TEST_F(AttendanceMangerFixture, GradeFromPoint) {
	EXPECT_EQ(GRADE_NORMAL, getGradeFromPoint(0));
	EXPECT_EQ(GRADE_NORMAL, getGradeFromPoint(SILVER_GRADE_THRESHOLD - 1));
	EXPECT_EQ(GRADE_SILVER, getGradeFromPoint(SILVER_GRADE_THRESHOLD));
	EXPECT_EQ(GRADE_SILVER, getGradeFromPoint(GOLD_GRADE_THRESHOLD - 1));
	EXPECT_EQ(GRADE_GOLD, getGradeFromPoint(GOLD_GRADE_THRESHOLD));
}

TEST_F(AttendanceMangerFixture, getGradeStr) {
	EXPECT_EQ("GOLD", getGradeStr(GRADE_GOLD));
	EXPECT_EQ("SILVER", getGradeStr(GRADE_SILVER));
	EXPECT_EQ("NORMAL", getGradeStr(GRADE_NORMAL));
}

TEST_F(AttendanceMangerFixture, ReadFromFilesTest) {
	InitManger();
	stringstream actualOutput;
	std::cout.rdbuf(actualOutput.rdbuf());
	auto originalBuf = std::cout.rdbuf();


	string sampleOutput = getSampleOutput();
	const char* weekdayDataFileName = "attendance_weekday_500.txt";
	const int WEEKDAY_DATA_NUM = 500;
	attendanceManageWeekdayDataFile(weekdayDataFileName, WEEKDAY_DATA_NUM);


	EXPECT_EQ(sampleOutput, actualOutput.str());
	std::cout.rdbuf(originalBuf);
}