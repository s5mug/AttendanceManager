#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <algorithm>

using namespace std;

struct WeekDayData {
	string nameStr;
	string dateStr;
};

map<string, int> id1;
int id_cnt = 0;

//dat[사용자ID][요일]
int attendance_count_by_date[100][100];
int points[100];
int grade[100];
string names[100];

int attendance_wendsday[100];
int attendance_weekend[100];

void input2(const WeekDayData &weekdayData) {
	//ID 부여
	if (id1.count(weekdayData.nameStr) == 0) {
		id1.insert({ weekdayData.nameStr, ++id_cnt });

		if (weekdayData.nameStr == "Daisy") {
			int debug = 1;
		}

		names[id_cnt] = weekdayData.nameStr;
	}
	int id2 = id1[weekdayData.nameStr];

	//디버깅용
	if (weekdayData.nameStr == "Daisy") {
		int debug = 1;
	}


	int attendance_point = 0;
	int index = 0;
	if (weekdayData.dateStr == "monday") {
		index = 0;
		attendance_point++;
	}
	if (weekdayData.dateStr == "tuesday") {
		index = 1;
		attendance_point++;
	}
	if (weekdayData.dateStr == "wednesday") {
		index = 2;
		attendance_point += 3;
		attendance_wendsday[id2] += 1;
	}
	if (weekdayData.dateStr == "thursday") {
		index = 3;
		attendance_point++;
	}
	if (weekdayData.dateStr == "friday") {
		index = 4;
		attendance_point++;
	}
	if (weekdayData.dateStr == "saturday") {
		index = 5;
		attendance_point += 2;
		attendance_weekend[id2] += 1;
	}
	if (weekdayData.dateStr == "sunday") {
		index = 6;
		attendance_point += 2;
		attendance_weekend[id2] += 1;
	}

	//사용자ID별 요일 데이터에 1씩 증가
	attendance_count_by_date[id2][index] += 1;
	points[id2] += attendance_point;
}

void input() {
	ifstream fin{ "attendance_weekday_500.txt" }; //500개 데이터 입력
	for (int i = 0; i < 500; i++) {
		WeekDayData weekdayData;
		fin >> weekdayData.nameStr >> weekdayData.dateStr;
		input2(weekdayData);
	}

	for (int i = 1; i <= id_cnt; i++) {
		if (attendance_count_by_date[i][2] > 9) {
			points[i] += 10;
		}

		if (attendance_count_by_date[i][5] + attendance_count_by_date[i][6] > 9) {
			points[i] += 10;
		}

		if (points[i] >= 50) {
			grade[i] = 1;
		}
		else if (points[i] >= 30) {
			grade[i] = 2;
		}
		else {
			grade[i] = 0;
		}

		cout << "NAME : " << names[i] << ", ";
		cout << "POINT : " << points[i] << ", ";
		cout << "GRADE : ";

		if (grade[i] == 1) {
			cout << "GOLD" << "\n";
		}
		else if (grade[i] == 2) {
			cout << "SILVER" << "\n";
		}
		else {
			cout << "NORMAL" << "\n";
		}
	}

	std::cout << "\n";
	std::cout << "Removed player\n";
	std::cout << "==============\n";
	for (int i = 1; i <= id_cnt; i++) {

		if (grade[i] != 1 && grade[i] != 2 && attendance_wendsday[i] == 0 && attendance_weekend[i] == 0) {
			std::cout << names[i] << "\n";
		}
	}
}

int main() {
	input();
}