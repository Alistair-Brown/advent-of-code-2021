#pragma once
#include <vector>
#include "grid_utils.h"

namespace Dumbo
{
	class OctopusGrid
	{
	private:
		static const int levelRequiredToFlash{ 10 };
		GridUtils::Grid<int> octopusEnergyLevels;
		unsigned int flashesOccuredSoFar{ 0 };
		unsigned int stepsTakenSoFar{ 0 };
		unsigned int firstSimultaneousFlash{ 0 };
		void AdvanceSingleStep();
		bool AdjacentFlashTriggersOctopus(GridUtils::Grid<int>::GridCell& octopus);
	public:
		OctopusGrid(std::vector<std::vector<int>> startingEnergyLevels) : octopusEnergyLevels{ startingEnergyLevels } {};
		void AdvanceSteps(unsigned int numSteps);
		unsigned int NumberOfFlashesOccured() { return flashesOccuredSoFar; }
		unsigned int AdvanceUntilFirstSimultaneousFlash();
	};
}