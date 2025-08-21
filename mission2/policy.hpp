#include <string>
#include <vector>

using namespace std;


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


struct CountByWeekend {
	CountByWeekend() {
		memset(count_of_date, 0, sizeof(count_of_date));
	}

	int count_of_date[MAX_NUM_DATE];
};

inline bool IsValidDate(DATE_ID date_id) {
	return date_id != MAX_NUM_DATE;
}

inline DATE_ID getDateIndexFromDateStr(const string& dateStr) {
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


class DateWorkPolicy {
public:
	virtual int getPointByPolicy(const CountByWeekend& attendCount) = 0;
	virtual bool IsEjectableByPolicy(const CountByWeekend& attendCount) = 0;
};

class BaseDatePolicy : public DateWorkPolicy {
public:
	static const int BASE_POINT_DEFAULT = 1;
	int getPointByPolicy(const CountByWeekend& attendCount) override;
	bool IsEjectableByPolicy(const CountByWeekend& attendCount) override;
};

class WednesdayBonusPolicy : public DateWorkPolicy {
public:
	static  const int BASE_BONUS_WEDNESDAY = 2;
	static  const int WEDNESDAY_BONUS_POINT_THRESHOLD = 10;
	static  const int WEDNESDAY_BONUS_PONINT = 10;
	int getPointByPolicy(const CountByWeekend& attendCount) override;
	bool IsEjectableByPolicy(const CountByWeekend& attendCount) override;
};

class WeekendBonusPolicy : public DateWorkPolicy {
public:
	static  const int BASE_BONUS_WEEKEND = 1;
	static  const int WEEKEND_BONUS_POINT_THRESHOLD = 10;
	static  const int WEEKEND_BONUS_PONINT = 10;
	int getPointByPolicy(const CountByWeekend& attendCount) override;
	bool IsEjectableByPolicy(const CountByWeekend& attendCount) override;
};

class MultipleDateWorkPolicy : public DateWorkPolicy {
public:
	MultipleDateWorkPolicy(vector<DateWorkPolicy*> _policyList) : policyList{ _policyList } {}

	int getPointByPolicy(const CountByWeekend& attendCount);
	bool IsEjectableByPolicy(const CountByWeekend& attendCount);

private:
	vector<DateWorkPolicy*> policyList;
};