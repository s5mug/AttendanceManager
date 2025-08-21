#include <string>
#include <vector>
#include <map>
#include "player.hpp"

using namespace std;

struct WeekDayData {
	string playerName;
	string dateStr;
};

class AttendanceManager {
public:
	AttendanceManager(DateWorkPolicy& _dataworkPolicy, GradeChecker& _gradeChecker)
		: dateWorkPolicy{ _dataworkPolicy }, gradeChecker(_gradeChecker){}

	int getPlayerIdFromName(const string& playerName);
	Player& getPlayerFromId(int PlayerId);

	void updatePlayerPoint();
	void updatePlayerGrade();
	void printSummary();
	vector<Player*> getRemovedPlayerId(void);

	void ManageWeekdayDataFile(const char* weekdayDataFileName, int dataNum);
private:
	map<string, int> player_name_to_Id_map;
	vector<Player> players;
	DateWorkPolicy& dateWorkPolicy;
	vector<Grade*> sorted_grade_list;

	GradeChecker& gradeChecker;

};