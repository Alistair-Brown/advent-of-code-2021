#include "LanternFish.h"
#include "Parsing.h"
#include <iostream>
#include <vector>

int main()
{
	std::cout << "Enter initial lanternfish numbers" << std::endl;
	std::vector<int> lanternFishInput = Parsing::ParseLineOfInts(Parsing::comma);

	LanternFish::LanternFishTracker lanternFishTracker;

	for (int lantenFishDaysToBirth : lanternFishInput)
	{
		lanternFishTracker.AddALanternFish(lantenFishDaysToBirth);
	}

	lanternFishTracker.DaysHavePassed(80);
	std::cout << "Total fish after 80 days is " << lanternFishTracker.TotalNumberOfLanterFish() << std::endl;
	lanternFishTracker.DaysHavePassed(176);
	std::cout << "Total fish after 256 is " << lanternFishTracker.TotalNumberOfLanterFish() << std::endl;
}