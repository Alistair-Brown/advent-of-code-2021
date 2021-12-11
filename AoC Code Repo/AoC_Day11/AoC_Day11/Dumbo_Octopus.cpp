#include "Dumbo_Octopus.h"
#include "GridUtils.h"

void Dumbo::OctopusGrid::AdvanceSingleStep()
{
	std::vector<GridUtils::Coordinate> coordsToFlash;

	// First increment energy levels of all octopuses
	for (unsigned int yy = 0; yy < octopusEnergyLevels.size(); yy++)
	{
		for (unsigned int xx = 0; xx < octopusEnergyLevels[0].size(); xx++)
		{
			octopusEnergyLevels[yy][xx]++;
			if (octopusEnergyLevels[yy][xx] == levelRequiredToFlash)
			{
				coordsToFlash.push_back(GridUtils::Coordinate(xx, yy, octopusEnergyLevels[0].size(), octopusEnergyLevels.size()));
			}
		}
	}

	// Now go around triggering octopus flashes as long as there are any left
	// to trigger. An octopus goes to 0 after a flash and will not be incremented by other
	// flashes.
	while (coordsToFlash.size() > 0)
	{
		GridUtils::Coordinate currentFlashCoord = coordsToFlash.back();
		coordsToFlash.pop_back();
		octopusEnergyLevels[currentFlashCoord.y][currentFlashCoord.x] = 0;
		flashesOccuredSoFar++;

		if (!currentFlashCoord.IsTopRow())
		{
			GridUtils::Coordinate adjacentCell{ currentFlashCoord.Up() };
			if (AdjacentFlashTriggersOctopus(adjacentCell))
			{
				coordsToFlash.push_back(adjacentCell);
			}

			if (!currentFlashCoord.IsRightColumn())
			{
				GridUtils::Coordinate adjacentCell{ currentFlashCoord.UpRight() };
				if (AdjacentFlashTriggersOctopus(adjacentCell))
				{
					coordsToFlash.push_back(adjacentCell);
				}
			}
			if (!currentFlashCoord.IsLeftColumn())
			{
				GridUtils::Coordinate adjacentCell{ currentFlashCoord.UpLeft() };
				if (AdjacentFlashTriggersOctopus(adjacentCell))
				{
					coordsToFlash.push_back(adjacentCell);
				}
			}
		}
		if (!currentFlashCoord.IsBottomRow())
		{
			GridUtils::Coordinate adjacentCell{ currentFlashCoord.Down() };
			if (AdjacentFlashTriggersOctopus(adjacentCell))
			{
				coordsToFlash.push_back(adjacentCell);
			}

			if (!currentFlashCoord.IsRightColumn())
			{
				GridUtils::Coordinate adjacentCell{ currentFlashCoord.DownRight() };
				if (AdjacentFlashTriggersOctopus(adjacentCell))
				{
					coordsToFlash.push_back(adjacentCell);
				}
			}
			if (!currentFlashCoord.IsLeftColumn())
			{
				GridUtils::Coordinate adjacentCell{ currentFlashCoord.DownLeft() };
				if (AdjacentFlashTriggersOctopus(adjacentCell))
				{
					coordsToFlash.push_back(adjacentCell);
				}
			}
		}
		if (!currentFlashCoord.IsLeftColumn())
		{
			GridUtils::Coordinate adjacentCell{ currentFlashCoord.Left() };
			if (AdjacentFlashTriggersOctopus(adjacentCell))
			{
				coordsToFlash.push_back(adjacentCell);
			}
		}
		if (!currentFlashCoord.IsRightColumn())
		{
			GridUtils::Coordinate adjacentCell{ currentFlashCoord.Right() };
			if (AdjacentFlashTriggersOctopus(adjacentCell))
			{
				coordsToFlash.push_back(adjacentCell);
			}
		}
	}

	stepsTakenSoFar++;
}

// We only increment that octopus if it hasn't already flashed (so isn't 0), and isn't
// already primed to flash (so isn't levelRequiredToFlash). If this octopus is now triggered to flash
// itself, return true.
bool Dumbo::OctopusGrid::AdjacentFlashTriggersOctopus(GridUtils::Coordinate coord)
{
	bool octopusTriggered{ false };
	int &octopusEnergy = octopusEnergyLevels[coord.y][coord.x];
	if ((octopusEnergy != 0) && (octopusEnergy != levelRequiredToFlash))
	{
		octopusEnergy++;
		if (octopusEnergy == levelRequiredToFlash)
		{
			octopusTriggered = true;
		}
	}
	return octopusTriggered;
}

void Dumbo::OctopusGrid::AdvanceSteps(int numSteps)
{
	unsigned int flashesBeforeStep{ 0 };
	for (int ii = 0; ii < numSteps; ii++)
	{
		flashesBeforeStep = flashesOccuredSoFar;
		AdvanceSingleStep();
		if ((flashesOccuredSoFar - flashesBeforeStep) ==
			(octopusEnergyLevels.size() * octopusEnergyLevels[0].size()))
		{
			firstSimultaneousFlash = firstSimultaneousFlash == 0 ?
				stepsTakenSoFar : firstSimultaneousFlash;
		}
	}
}

unsigned int Dumbo::OctopusGrid::AdvanceUntilFirstSimultaneousFlash()
{
	while (firstSimultaneousFlash == 0)
	{
		AdvanceSteps(1);
	}
	return firstSimultaneousFlash;
}
