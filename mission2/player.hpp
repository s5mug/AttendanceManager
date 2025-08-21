#include "policy.hpp"
#include "grade.hpp"

class Player {
public:
	Player(string _name = "", int _id = 0) :
		point(0), grade(nullptr), name(_name), Id(_id) {
	}

	void updatePoint(DateWorkPolicy* policy) {
		point = policy->getPointByPolicy(attendCount);
	}

	void updateNewDateWork(DATE_ID date_Id) {
		if (false == IsValidDate(date_Id)) return;
		attendCount.count_of_date[date_Id] += 1;
	}

	bool IsRemovedPlayer(DateWorkPolicy* policy) {
		if (false == grade->IsEjectableGrade()) return false;
		if (false == policy->IsEjectableByPolicy(attendCount)) return false;
		return true;
	}

	string& getName() {
		return name;
	}

	int getPoint() {
		return point;
	}

	void setGrade(Grade* new_grade) {
		grade = new_grade;
	}

	Grade* getGrade() {
		return grade;
	}
private:

	CountByWeekend attendCount;
	int point;
	int Id;
	Grade* grade;
	string name;
};