#pragma once
#include <vector>
#include "grid_utils.h"

namespace Dumbo
{
	// Holds a grid of dumbo octopus energy levels, which will increase as time
	// passes. Time passing is modelled as steps of unit time. The flashing of an
	// octopus will increase the energy levels of adjacent octopuses further, which
	// can cause a chain reaction of flashes.
	class OctopusGrid
	{
	private:
		static const int levelRequiredToFlash{ 10 };
		unsigned int flashesOccuredSoFar{ 0 };
		unsigned int stepsTakenSoFar{ 0 };
		unsigned int firstSimultaneousFlash{ 0 };

		GridUtils::Grid<int> octopusEnergyLevels;

		void AdvanceSingleStep();
		bool AdjacentFlashTriggersOctopus(GridUtils::Grid<int>::GridCell& octopus);
	public:
		OctopusGrid(std::vector<std::vector<int>> startingEnergyLevels) : octopusEnergyLevels{ startingEnergyLevels } {};

		void AdvanceSteps(unsigned int numSteps);
		unsigned int NumberOfFlashesOccured() { return flashesOccuredSoFar; }
		unsigned int AdvanceUntilFirstSimultaneousFlash();
	};
}