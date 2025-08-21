#include <string>
#include <vector>

using namespace std;


struct WeekDayData {
	string playerName;
	string dateStr;
};

enum DATE_ID {
	MONDAY = 0,
	TUSEDAY = 1,
	WEDNESDAY = 2,
	THURSDAY = 3,
	FRIDAY = 4,
	SATURDAY = 5,
	SUNDAY = 6,
	MAX_NUM_DATE = 7
};

enum Grade_e {
	GRADE_NORMAL = 0,
	GRADE_GOLD = 1,
	GRADE_SILVER = 2
};

const int MAX_NUM_PLAYER_ID = 100;
const int BASE_POINT_WEDNESDAY = 3;
const int BASE_POINT_WEEKEND = 2;
const int BASE_POINT_DEFAULT = 1;

const int WEDNESDAY_BONUS__POINT_THRESHOLD = 10;
const int WEDNESDAY_BONUS_PONINT = 10;
const int WEEKEND_BONUS_POINT_THRESHOLD = 10;
const int WEEKEND_BONUS_PONINT = 10;

const int GOLD_GRADE_THRESHOLD = 50;
const int SILVER_GRADE_THRESHOLD = 30;


bool IsValidDate(DATE_ID date_id);

extern int getPlayerIdFromName(const string& playerName);
extern DATE_ID getDateIndexFromDateStr(const string& dateStr);
extern int getBasePointByDate(DATE_ID date_Id);
extern void updatePlayer(int player_Id, DATE_ID date_Id);

extern int getBonusPointByWendsDay(int attendance_count_wednesday);
extern int getBonusPointByWeekend(int attendance_count_weekend);
extern Grade_e getGradeFromPoint(int point);
extern void summarizePlayers();

extern const char* getGradeStr(int grade);
extern const char* getGradeStrFromPlayerId(int player_id);
extern const bool IsRemovedPlayer(Grade_e grade, int(&player_attendance_count_by_date)[MAX_NUM_DATE]);
extern vector<int> getRemovedPlayerId();

extern void attendanceManageWeekdayDataFile(const char* weekdayDataFileName, int dataNum);