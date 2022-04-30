#pragma once
#include <array>

namespace LanternFish
{
	class LanternFishTracker
	{
	private:
		static const int DaysToReproduce = 7;
		static const int DaysOfChildhood = 2;
		std::array<unsigned long long int, DaysToReproduce + DaysOfChildhood> lanternFishByDaysToReproduce{ 0 };
		void ModelPassingOfOneDay();
	public:
		void AddALanternFish(int daysToReproduce);
		void DaysHavePassed(int numberOfDays);
		unsigned long long int TotalNumberOfLanterFish();
	};
}