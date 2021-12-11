#pragma once
#include <vector>
#include "GridUtils.h"

namespace Dumbo
{
	class OctopusGrid
	{
	private:
		static const int levelRequiredToFlash{ 10 };
		std::vector<std::vector<int>> octopusEnergyLevels;
		unsigned int flashesOccuredSoFar{ 0 };
		unsigned int stepsTakenSoFar{ 0 };
		unsigned int firstSimultaneousFlash{ 0 };
		void AdvanceSingleStep();
		bool AdjacentFlashTriggersOctopus(GridUtils::Coordinate coord);
	public:
		OctopusGrid(std::vector<std::vector<int>> startingEnergyLevels) : octopusEnergyLevels{ startingEnergyLevels } {};
		void AdvanceSteps(int numSteps);
		unsigned int NumberOfFlashesOccured() { return flashesOccuredSoFar; }
		unsigned int AdvanceUntilFirstSimultaneousFlash();
	};
}