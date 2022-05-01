#pragma once
#include <array>
#include "aoc_common_types.h"

namespace LanternFish
{
	// The LanternFishTracker keeps track of all the lanternfish in the current population,
	// by how each has until it produces a new lanternfish. Keeping track of every individual
	// fish would be a terrible idea though, all we actually need to keep track of here is
	// how many fish have 0 days left to reproduce, 1 days left to reproduce etc.
	class LanternFishTracker
	{
	private:
		// An adult lanterfish takes 7 days to reproduce into a new lanternfish. A newly
		// born lanternfish has an additional 2 days of childhood before it enters that
		// repeating 7-day cycle.
		static constexpr int DaysToReproduce = 7;
		static constexpr int DaysOfChildhood = 2;

		// Keep track of how many lanternfish there are with each of the possible number
		// of days left to reproduce.
		std::array<ULLINT, DaysToReproduce + DaysOfChildhood> lanternFishByDaysToReproduce{ 0 };

		void ModelPassingOfOneDay();
	public:
		void AddALanternFish(int daysToReproduce);
		void DaysHavePassed(int numberOfDays);
		ULLINT TotalNumberOfLanterFish();
	};
}