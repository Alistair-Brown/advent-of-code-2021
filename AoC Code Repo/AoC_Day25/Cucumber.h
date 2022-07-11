#pragma once
#include "grid_utils.h"
#include <list>

namespace Cucumber
{
	constexpr char EAST_CUCUMBER = '>';
	constexpr char SOUTH_CUCUMBER = 'v';
	constexpr char EMPTY_SPACE = '.';

	// The Trench is initialised with a 2D array of characters where each cell is either
	// empty ('.'), or contains an East moving ('>') or South moving ('v') cucumber.
	// It can be queried to find how many time steps it takes before all of the cucumbers
	// are blocked from further movement.
	class Trench
	{
	private:
		// The trench is represented as a Grid of characters. But to avoid iterating over the whole
		// grid each step, all we actually need to keep track of is the coordinates of each currently
		// unblocked cucumber.
		GridUtils::Grid<char> cucumberGrid;
		std::list<GridUtils::Coordinate> unblockedEastCucumbers;
		std::list<GridUtils::Coordinate> unblockedSouthCucumbers;

		unsigned int stepsUntilAllBlocked{ 0 };
		void CarryOutSingleMovementStep();
		void PrintTrenchToScreen();
	public:
		Trench(std::vector<std::vector<char>> gridIn);
		unsigned int StepsUntilAllBlocked();
	};
}