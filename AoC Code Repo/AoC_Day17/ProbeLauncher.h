#pragma once
#include<utility>

namespace Launcher
{
	class ProbeLauncher
	{
	private:
		std::pair<std::pair<int, int>, std::pair<int, int>> targetRangeCoords;
		int xVelocityLowerBound;
		int stepsRequiredForXLowerBound;
		int xVelocityUpperBound;
		int yVelocityLowerBound;
		int yVelocityUpperBound;
	public:
		ProbeLauncher(std::pair<int, int> xRange, std::pair<int, int> yRange);

		int HighestYCoordinate();
		int NumberOfPossibleVelocities();
	};
}