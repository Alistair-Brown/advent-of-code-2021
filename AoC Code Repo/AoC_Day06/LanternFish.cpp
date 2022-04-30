#include "LanternFish.h"
#include <cassert>

void LanternFish::LanternFishTracker::ModelPassingOfOneDay()
{
	std::array<unsigned long long int, DaysToReproduce + DaysOfChildhood> newRecordOfLanternFish{ 0 };

	for (int daysToBirth = 0; daysToBirth < (DaysToReproduce + DaysOfChildhood); daysToBirth++)
	{
		unsigned long long int numberOfLanternFish = lanternFishByDaysToReproduce[daysToBirth];
		if (daysToBirth == 0)
		{
			// Remember 0-indexing
			newRecordOfLanternFish[DaysToReproduce - 1] += numberOfLanternFish;
			newRecordOfLanternFish[DaysToReproduce + DaysOfChildhood - 1] += numberOfLanternFish;
		}
		else
		{
			newRecordOfLanternFish[daysToBirth - 1] += numberOfLanternFish;
		}
	}
	lanternFishByDaysToReproduce = newRecordOfLanternFish;
}

void LanternFish::LanternFishTracker::AddALanternFish(int daysToReproduce)
{
	assert(daysToReproduce < (DaysToReproduce + DaysOfChildhood));
	lanternFishByDaysToReproduce[daysToReproduce]++;
}

void LanternFish::LanternFishTracker::DaysHavePassed(int numberOfDays)
{
	for (int ii = 0; ii < numberOfDays; ii++)
	{
		ModelPassingOfOneDay();
	}
}

unsigned long long int LanternFish::LanternFishTracker::TotalNumberOfLanterFish()
{
	unsigned long long int totalFish{ 0 };
	for (unsigned long long int numberOfFish : lanternFishByDaysToReproduce)
	{
		totalFish += numberOfFish;
	}
	return totalFish;
}
