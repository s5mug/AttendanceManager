
void Player::updatePoint(DateWorkPolicy* policy) {
	point = policy->getPointByPolicy(attendCount);
}

void Player::updateNewDateWork(DATE_ID date_Id) {
	if (false == IsValidDate(date_Id)) return;

	attendCount.count_of_date[date_Id] += 1;
}

bool Player::IsRemovedPlayer(DateWorkPolicy* policy) {
	if (false == grade->IsEjectableGrade()) return false;
	if (false == policy->IsEjectableByPolicy(attendCount)) return false;
	return true;
}

