#include "Chiton.h"

Chiton::ChitonCave::ChitonCave(std::vector<std::vector<int>> inputRiskGrid)
{
	riskGrid = inputRiskGrid;
	endPos = { inputRiskGrid[0].size() - 1, inputRiskGrid.size() - 1 };

	minimumRiskToReachPosition = std::vector<std::vector<int>>(inputRiskGrid.size(), std::vector<int>(inputRiskGrid[0].size(), INT16_MAX));
}

int Chiton::ChitonCave::MinimumRiskToEnd()
{
	FindMinimumRiskRoutes();
	// Don't get fooled by the fact that endPos is given as x,y coords, but
	// the 2D vector has y as the first element when indexing.
	return minimumRiskToReachPosition[endPos.second][endPos.first];
}

// General approach here is:
// - Start at the start position, with a total risk on this route so far of 0
// - Then look at each position around us, and if the total risk of the route after
//   moving there would be less than the current minimum we have stored to reach that
//   position, put a new potential route on the bottom of our queue of potential
//   routes that represents us moving there. Make the minimum to reach to that position
//   the total risk of that route.
// - Rinse and repeat until we're out of queue.
void Chiton::ChitonCave::FindMinimumRiskRoutes()
{
	// Our stack of potential routes contains a position (in form 'x,y', which is worth
	// remembering so you don't get tripped up indexing grid positions), and a total
	// risk we've encountered so far on that route.
	std::vector<std::pair<std::pair<int, int>, int>> potentialRoutes{ {{0,0},0} };

	while (potentialRoutes.size() > 0)
	{
		std::pair<std::pair<int, int>, int> currentRoute = potentialRoutes.back();
		potentialRoutes.pop_back();

		int newRisk;
		int currXPos = currentRoute.first.first;
		int currYPos = currentRoute.first.second;
		int currRisk = currentRoute.second;
		std::pair<int, int> newPosition;

		if (currXPos > 0)
		{
			newPosition = { currXPos - 1, currYPos };
			newRisk = currRisk + riskGrid[newPosition.second][newPosition.first];
			if (newRisk < minimumRiskToReachPosition[newPosition.second][newPosition.first])
			{
				minimumRiskToReachPosition[newPosition.second][newPosition.first] = newRisk;
				potentialRoutes.push_back({ newPosition, newRisk });
			}
		}
		if (currXPos < riskGrid[0].size() - 1)
		{
			newPosition = { currXPos + 1, currYPos };
			newRisk = currRisk + riskGrid[newPosition.second][newPosition.first];
			if (newRisk < minimumRiskToReachPosition[newPosition.second][newPosition.first])
			{
				minimumRiskToReachPosition[newPosition.second][newPosition.first] = newRisk;
				potentialRoutes.push_back({ newPosition, newRisk });
			}
		}
		if (currYPos > 0)
		{
			newPosition = { currXPos, currYPos - 1 };
			newRisk = currRisk + riskGrid[newPosition.second][newPosition.first];
			if (newRisk < minimumRiskToReachPosition[newPosition.second][newPosition.first])
			{
				minimumRiskToReachPosition[newPosition.second][newPosition.first] = newRisk;
				potentialRoutes.push_back({ newPosition, newRisk });
			}
		}
		if (currYPos < riskGrid.size() - 1)
		{
			newPosition = { currXPos, currYPos + 1 };
			newRisk = currRisk + riskGrid[newPosition.second][newPosition.first];
			if (newRisk < minimumRiskToReachPosition[newPosition.second][newPosition.first])
			{
				minimumRiskToReachPosition[newPosition.second][newPosition.first] = newRisk;
				potentialRoutes.push_back({ newPosition, newRisk });
			}
		}
	}
}
