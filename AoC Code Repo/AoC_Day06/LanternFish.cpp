#include "LanternFish.h"
#include <cassert>

// Given the number of lanternfish in each 'days left to reproduce' bucket today, create
// the new set of lanternfish for the next day. This involves:
//  - For each number of days to reproduce other than 0, shift each bucket of lanternfish
//    to the 'left', so that e.g. if we had n lanterfish with 4 days left to reproduce
//    today, there will be n lanternfish with 3 days left to reproduce tomorrow.
//  - Each lanternfish which had 0 days left to reproduce produces a new lanternfish
//    and then restarts the 7-day cycle. So for n lanternfish with 0 days left to
//    reproduce today, tomorrow there will be n lanterfish with 6 days left to reproduce
//    (representing the lanternfish which just reproduced), and n with 8 days left (representing
//    the newly born lanternfish).
void LanternFish::LanternFishTracker::ModelPassingOfOneDay()
{
	std::array<ULLINT, DaysToReproduce + DaysOfChildhood> newRecordOfLanternFish{ 0 };

	for (int daysToBirth = 0; daysToBirth < (DaysToReproduce + DaysOfChildhood); daysToBirth++)
	{
		ULLINT numberOfLanternFish = lanternFishByDaysToReproduce[daysToBirth];
		if (daysToBirth == 0)
		{
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

// Insert a lanternfish with a given number of days left to reproduce into the
// tracker.
void LanternFish::LanternFishTracker::AddALanternFish(int daysToReproduce)
{
	assert(daysToReproduce < (DaysToReproduce + DaysOfChildhood));
	lanternFishByDaysToReproduce[daysToReproduce]++;
}

// A call to increment the number of days by a certain amount just requires that
// many calls to the function which models the passing of a single day.
void LanternFish::LanternFishTracker::DaysHavePassed(int numberOfDays)
{
	for (int ii = 0; ii < numberOfDays; ii++)
	{
		ModelPassingOfOneDay();
	}
}

// Total number of lanterfish can be retrieved just by summing over the array
// of lanternfish by days left to reproduce.
ULLINT LanternFish::LanternFishTracker::TotalNumberOfLanterFish()
{
	ULLINT totalFish{ 0 };
	for (ULLINT numberOfFish : lanternFishByDaysToReproduce)
	{
		totalFish += numberOfFish;
	}
	return totalFish;
}
