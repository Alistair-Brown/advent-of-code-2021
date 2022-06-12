#pragma once
#include<utility>
#include<set>
#include<vector>

namespace Launcher
{
	// A ProbeLauncher is initialised with an area through which the probe must be fired.
	// The probe launched by this launcher will move in discrete steps, with velocity affected
	// by gravity and drag with each step. To have successfully passed through the target area,
	// the probe must finish one of these discrete steps within the area.
	// As required by the puzzle, the probe launcher can calculate:
	//  - The highest y coordinate a probe could be fired to and still pass through the
	//    target area
	//  - How many distinct starting velocities a probe could be fired with and still
	//    pass through the target area
	class ProbeLauncher
	{
	private:
		std::pair<std::pair<int, int>, std::pair<int, int>> targetRangeCoords;

		// To help make some of the later computation more efficient, the probe launcher
		// will calculate on construction the lower and upper bounds of velocity components
		// (x and y) with which a probe might be able to pass through the target area.
		int xVelocityLowerBound;
		int xVelocityUpperBound;
		int yVelocityLowerBound;
		int yVelocityUpperBound;

		std::set<int> XStepsToReachTarget(int initialXVelocity);
		std::set<int> YStepsToReachTarget(int initialYVelocity);
		unsigned int NumberOfVelocitiesWithOverlappingStepsToTarget(
			std::vector<std::set<int>> xStepsToReachTarget,
			std::vector<std::set<int>> yStepsToReachTarget
		);
	public:
		ProbeLauncher(std::pair<int, int> xRange, std::pair<int, int> yRange);

		int HighestYCoordinate();
		unsigned int NumberOfPossibleVelocities();
	};
}