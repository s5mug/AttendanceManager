#include <vector>
#include <string>
#include <algorithm>

using namespace std;

class Grade {
public:
	virtual int getPointThreshold() const = 0;
	virtual const char* getStr() const = 0;
	virtual bool IsEjectableGrade() const = 0;
};

class NormalGrade : public Grade {
public:
	static const int POINT_THRESHOLD = 0;
	static constexpr const char* NORMAL_STR = "NORMAL";

	int getPointThreshold() const override { return POINT_THRESHOLD; };
	const char* getStr() const override { return NORMAL_STR; };
	bool IsEjectableGrade() const override { return  true; }

	static NormalGrade* getInstance() {
		if (nullptr == _instance) _instance = new NormalGrade();
		return _instance;
	}

	static bool IsNormalGrade(Grade* grade) {
		return grade == _instance;
	}
private:
	NormalGrade() = default;
	static NormalGrade* _instance;
};

class SilverGrade : public Grade {
public:
	static const int POINT_THRESHOLD = 30;
	static constexpr const char* SILVER_STR = "SILVER";

	int getPointThreshold() const override { return POINT_THRESHOLD; };
	const char* getStr() const override { return SILVER_STR; };
	bool IsEjectableGrade() const override { return  false; }

	static SilverGrade* getInstance() {
		if (nullptr == _instance) _instance = new SilverGrade();
		return _instance;
	}

	static bool IsSilverGrade(Grade* grade) {
		return grade == _instance;
	}
private:
	SilverGrade() = default;
	static SilverGrade* _instance;
};

class GoldGrade : public Grade {
public:
	static const int POINT_THRESHOLD = 50;
	static constexpr const char* GOLD_STR = "GOLD";

	int getPointThreshold() const override { return POINT_THRESHOLD; };
	const char* getStr() const override { return GOLD_STR; };
	bool IsEjectableGrade() const override { return  false; }

	static bool IsGoldGrade(Grade* grade) {
		return grade == _instance;
	}

	static GoldGrade* getInstance() {
		if (nullptr == _instance) _instance = new GoldGrade();
		return _instance;
	}
private:
	GoldGrade() = default;
	static GoldGrade* _instance;
};

class GradeChecker {
public:
	GradeChecker(initializer_list<Grade*> _gradeList) : gradeList(_gradeList) {
		sort(gradeList.begin(), gradeList.end(), [](Grade* a, Grade* b) {
			return a->getPointThreshold() >= b->getPointThreshold();
			});
	}

	Grade* getGradeFromPoint(int point) {
		for (Grade* grade : gradeList) {
			if (point >= grade->getPointThreshold())
				return grade;
		}
		return nullptr;
	}

private:
	vector<Grade*> gradeList;
};
