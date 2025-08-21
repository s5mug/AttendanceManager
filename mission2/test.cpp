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

class AttendanceMangerFixture : public Test {
public:
	AttendanceMangerFixture() :
		gradeChecker{NormalGrade::getInstance(), SilverGrade::getInstance(), GoldGrade::getInstance()},
		defaultPolicy{ {&basePolicy, &weekendPolicy, &wendnesdayPolicy} },
		attendanceManger{ defaultPolicy, gradeChecker }
	{

	}


	void SetUp() override {
		InitManger();
	}

	void InitManger() {

	}

	int TestPointIncreaseByDate(DATE_ID date, DateWorkPolicy* policy, int date_cnt = 1) {
		Player player{};
		int old_player_point = player.getPoint();
		for (int i = 0; i < date_cnt; i++) {
			player.updateNewDateWork(date);
		}
		player.updatePoint(policy);
		int new_player_point = player.getPoint();
		return new_player_point - old_player_point;
	}

	string getSampleOutput() {
		ifstream fin{ "sample_output.txt" };
		std::stringstream sstr;
		while (fin >> sstr.rdbuf());
		return sstr.str();
	}

	Player& GetAnyPlayer() {
		int any_player_Id = attendanceManger.getPlayerIdFromName(ANY_NAME);
		return attendanceManger.getPlayerFromId(any_player_Id);
	}


	BaseDatePolicy basePolicy;
	WeekendBonusPolicy weekendPolicy;
	WednesdayBonusPolicy wendnesdayPolicy;
	GradeChecker gradeChecker;
	MultipleDateWorkPolicy defaultPolicy;
	AttendanceManager attendanceManger;
	string ANY_NAME = "ANY";
	string OTHER_NAME = "OTHER";

};


TEST_F(AttendanceMangerFixture, PlayerId_NewPlayerTest) {
	int AnyPlayer = attendanceManger.getPlayerIdFromName(ANY_NAME);
	int OtherPlayer = attendanceManger.getPlayerIdFromName(OTHER_NAME);
	EXPECT_NE(AnyPlayer, OtherPlayer);
}

TEST_F(AttendanceMangerFixture, PlayerId_SamePlayerTest) {
	int AnyPlayer = attendanceManger.getPlayerIdFromName(ANY_NAME);
	int AnyPlayer2 = attendanceManger.getPlayerIdFromName(ANY_NAME);
	EXPECT_EQ(AnyPlayer, AnyPlayer2);
}


TEST_F(AttendanceMangerFixture, ValidDateTest) {
	vector<DATE_ID> validDates{ MONDAY, TUSEDAY , WEDNESDAY , THURSDAY , FRIDAY , SATURDAY , SUNDAY };
	for (auto validDate : validDates) {
		EXPECT_TRUE(IsValidDate(validDate));
	}

	EXPECT_FALSE(IsValidDate(MAX_NUM_DATE));
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
	EXPECT_EQ(BaseDatePolicy::BASE_POINT_DEFAULT, TestPointIncreaseByDate(MONDAY, &basePolicy));
	EXPECT_EQ(BaseDatePolicy::BASE_POINT_DEFAULT, TestPointIncreaseByDate(TUSEDAY, &basePolicy));
	EXPECT_EQ(BaseDatePolicy::BASE_POINT_DEFAULT, TestPointIncreaseByDate(THURSDAY, &basePolicy));
	EXPECT_EQ(BaseDatePolicy::BASE_POINT_DEFAULT, TestPointIncreaseByDate(FRIDAY, &basePolicy));
	EXPECT_EQ(BaseDatePolicy::BASE_POINT_DEFAULT, TestPointIncreaseByDate(WEDNESDAY, &basePolicy));
	EXPECT_EQ(BaseDatePolicy::BASE_POINT_DEFAULT, TestPointIncreaseByDate(SATURDAY, &basePolicy));
	EXPECT_EQ(BaseDatePolicy::BASE_POINT_DEFAULT, TestPointIncreaseByDate(SUNDAY, &basePolicy));

	EXPECT_EQ(0, TestPointIncreaseByDate(MAX_NUM_DATE, &basePolicy));
}


TEST_F(AttendanceMangerFixture, BonusByWednesDay) {
	int lessThresholdWorkCnt = WednesdayBonusPolicy::WEDNESDAY_BONUS_POINT_THRESHOLD - 1;
	int expected_less_threshold = lessThresholdWorkCnt * WednesdayBonusPolicy::BASE_BONUS_WEDNESDAY;
	EXPECT_EQ(expected_less_threshold, TestPointIncreaseByDate(WEDNESDAY, &wendnesdayPolicy, lessThresholdWorkCnt));


	int overThresholdWorkCnt = WednesdayBonusPolicy::WEDNESDAY_BONUS_POINT_THRESHOLD;
	int expected_over_threshold = WednesdayBonusPolicy::BASE_BONUS_WEDNESDAY * overThresholdWorkCnt + WednesdayBonusPolicy::WEDNESDAY_BONUS_PONINT;
	EXPECT_EQ(expected_over_threshold, TestPointIncreaseByDate(WEDNESDAY, &wendnesdayPolicy, overThresholdWorkCnt));
}

TEST_F(AttendanceMangerFixture, BonusByWeekend) {
	int lessThresholdWorkCnt = WeekendBonusPolicy::WEEKEND_BONUS_POINT_THRESHOLD - 1;
	int expected_less_threshold = lessThresholdWorkCnt * WeekendBonusPolicy::BASE_BONUS_WEEKEND;
	EXPECT_EQ(expected_less_threshold, TestPointIncreaseByDate(SATURDAY, &weekendPolicy, lessThresholdWorkCnt));
	EXPECT_EQ(expected_less_threshold, TestPointIncreaseByDate(SUNDAY, &weekendPolicy, lessThresholdWorkCnt));

	int overThresholdWorkCnt = WeekendBonusPolicy::WEEKEND_BONUS_POINT_THRESHOLD;
	int expected_over_threshold = WeekendBonusPolicy::BASE_BONUS_WEEKEND * overThresholdWorkCnt + WeekendBonusPolicy::WEEKEND_BONUS_PONINT;
	EXPECT_EQ(expected_over_threshold, TestPointIncreaseByDate(SATURDAY, &weekendPolicy, overThresholdWorkCnt));
	EXPECT_EQ(expected_less_threshold, TestPointIncreaseByDate(SUNDAY, &weekendPolicy, lessThresholdWorkCnt));
}

TEST_F(AttendanceMangerFixture, GradeFromPoint) {
	EXPECT_EQ(nullptr, gradeChecker.getGradeFromPoint(-1));
	EXPECT_TRUE(NormalGrade::IsNormalGrade(gradeChecker.getGradeFromPoint(0)));
	EXPECT_TRUE(NormalGrade::IsNormalGrade(gradeChecker.getGradeFromPoint(SilverGrade::POINT_THRESHOLD - 1)));
	EXPECT_TRUE(SilverGrade::IsSilverGrade(gradeChecker.getGradeFromPoint(SilverGrade::POINT_THRESHOLD)));
	EXPECT_TRUE(SilverGrade::IsSilverGrade(gradeChecker.getGradeFromPoint(GoldGrade::POINT_THRESHOLD - 1)));
	EXPECT_TRUE(GoldGrade::IsGoldGrade(gradeChecker.getGradeFromPoint(GoldGrade::POINT_THRESHOLD)));
}

TEST_F(AttendanceMangerFixture, ReadFromFilesTest) {
	InitManger();
	stringstream actualOutput;
	std::cout.rdbuf(actualOutput.rdbuf());
	auto originalBuf = std::cout.rdbuf();


	string sampleOutput = getSampleOutput();
	const char* weekdayDataFileName = "attendance_weekday_500.txt";
	const int WEEKDAY_DATA_NUM = 500;
	attendanceManger.ManageWeekdayDataFile(weekdayDataFileName, WEEKDAY_DATA_NUM);


	EXPECT_EQ(sampleOutput, actualOutput.str());
	std::cout.rdbuf(originalBuf);
}