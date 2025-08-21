#include "policy.hpp"

int BaseDatePolicy::getPointByPolicy(const CountByWeekend& attendCount) {
	int result = 0;
	for (int date_id = 0; date_id < MAX_NUM_DATE; date_id++) {
		result += attendCount.count_of_date[date_id];
	}
	return result;
}

bool BaseDatePolicy::IsEjectableByPolicy(const const CountByWeekend& attendCount) {
	return true;
}

int WednesdayBonusPolicy::getPointByPolicy(const CountByWeekend& attendCount) {
	int base_bonus_point = attendCount.count_of_date[WEDNESDAY] * BASE_BONUS_WEDNESDAY;
	int attendance_count_wednesday = attendCount.count_of_date[WEDNESDAY];
	if (attendance_count_wednesday >= WEDNESDAY_BONUS_POINT_THRESHOLD)
		return base_bonus_point + WEDNESDAY_BONUS_PONINT;
	return base_bonus_point;
}

bool WednesdayBonusPolicy::IsEjectableByPolicy(const CountByWeekend& attendCount) {
	return attendCount.count_of_date[WEDNESDAY] <= 0;
}

int WeekendBonusPolicy::getPointByPolicy(const CountByWeekend& attendCount) {
	int attendance_count_weekend = attendCount.count_of_date[SATURDAY] + attendCount.count_of_date[SUNDAY];
	int base_bonus_point = attendance_count_weekend * BASE_BONUS_WEEKEND;

	if (attendance_count_weekend >= WEEKEND_BONUS_POINT_THRESHOLD)
		return base_bonus_point + WEEKEND_BONUS_PONINT;
	return base_bonus_point;
}

bool WeekendBonusPolicy::IsEjectableByPolicy(const CountByWeekend& attendCount) {
	return (attendCount.count_of_date[SATURDAY] <= 0 && attendCount.count_of_date[SUNDAY] <= 0);
}

int MultipleDateWorkPolicy::getPointByPolicy(const CountByWeekend& attendCount) {
	int resultPoint = 0;
	for (DateWorkPolicy* policy : policyList) {
		resultPoint += policy->getPointByPolicy(attendCount);
	}
	return resultPoint;
}

bool MultipleDateWorkPolicy::IsEjectableByPolicy(const CountByWeekend& attendCount) {
	for (DateWorkPolicy* policy : policyList) {
		if (false == policy->IsEjectableByPolicy(attendCount)) {
			return false;
		}
	}
	return true;
}

