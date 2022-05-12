#include "Dumbo_Octopus.h"

void Dumbo::OctopusGrid::AdvanceSingleStep()
{
	std::vector<GridUtils::Grid<int>::GridCell *> octosToFlash;

	for (GridUtils::Grid<int>::GridCell &octopus : octopusEnergyLevels)
	{
		octopus.value++;
		if (octopus.value == levelRequiredToFlash)
		{
			octosToFlash.push_back(&octopus);
		}
	}

	// Now go around triggering octopus flashes as long as there are any left
	// to trigger. An octopus goes to 0 after a flash and will not be incremented by other
	// flashes.
	while (octosToFlash.size() > 0)
	{
		GridUtils::Grid<int>::GridCell *octo = octosToFlash.back();
		octosToFlash.pop_back();
		octo->value = 0;
		flashesOccuredSoFar++;

		if (!octo->IsTopRow())
		{
			GridUtils::Grid<int>::GridCell &adjacentCell = octopusEnergyLevels[octo->Up().GetCoordinate()];
			if (AdjacentFlashTriggersOctopus(adjacentCell))
			{
				octosToFlash.push_back(&adjacentCell);
			}

			if (!octo->IsRightColumn())
			{
				GridUtils::Grid<int>::GridCell& adjacentCell = octopusEnergyLevels[octo->UpRight().GetCoordinate()];
				if (AdjacentFlashTriggersOctopus(adjacentCell))
				{
					octosToFlash.push_back(&adjacentCell);
				}
			}
			if (!octo->IsLeftColumn())
			{
				GridUtils::Grid<int>::GridCell& adjacentCell = octopusEnergyLevels[octo->UpLeft().GetCoordinate()];
				if (AdjacentFlashTriggersOctopus(adjacentCell))
				{
					octosToFlash.push_back(&adjacentCell);
				}
			}
		}
		if (!octo->IsBottomRow())
		{
			GridUtils::Grid<int>::GridCell& adjacentCell = octopusEnergyLevels[octo->Down().GetCoordinate()];
			if (AdjacentFlashTriggersOctopus(adjacentCell))
			{
				octosToFlash.push_back(&adjacentCell);
			}

			if (!octo->IsRightColumn())
			{
				GridUtils::Grid<int>::GridCell& adjacentCell = octopusEnergyLevels[octo->DownRight().GetCoordinate()];
				if (AdjacentFlashTriggersOctopus(adjacentCell))
				{
					octosToFlash.push_back(&adjacentCell);
				}
			}
			if (!octo->IsLeftColumn())
			{
				GridUtils::Grid<int>::GridCell& adjacentCell = octopusEnergyLevels[octo->DownLeft().GetCoordinate()];
				if (AdjacentFlashTriggersOctopus(adjacentCell))
				{
					octosToFlash.push_back(&adjacentCell);
				}
			}
		}
		if (!octo->IsLeftColumn())
		{
			GridUtils::Grid<int>::GridCell& adjacentCell = octopusEnergyLevels[octo->Left().GetCoordinate()];
			if (AdjacentFlashTriggersOctopus(adjacentCell))
			{
				octosToFlash.push_back(&adjacentCell);
			}
		}
		if (!octo->IsRightColumn())
		{
			GridUtils::Grid<int>::GridCell& adjacentCell = octopusEnergyLevels[octo->Right().GetCoordinate()];
			if (AdjacentFlashTriggersOctopus(adjacentCell))
			{
				octosToFlash.push_back(&adjacentCell);
			}
		}
	}

	stepsTakenSoFar++;
}

// We only increment that octopus if it hasn't already flashed (so isn't 0), and isn't
// already primed to flash (so isn't levelRequiredToFlash). If this octopus is now triggered to flash
// itself, return true.
bool Dumbo::OctopusGrid::AdjacentFlashTriggersOctopus(GridUtils::Grid<int>::GridCell &octopus)
{
	bool octopusTriggered{ false };
	int &octopusEnergy = octopus.value;
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

void Dumbo::OctopusGrid::AdvanceSteps(unsigned int numSteps)
{
	unsigned int flashesBeforeStep{ 0 };
	for (unsigned int ii = 0; ii < numSteps; ii++)
	{
		flashesBeforeStep = flashesOccuredSoFar;
		AdvanceSingleStep();
		if ((flashesOccuredSoFar - flashesBeforeStep) ==
			(octopusEnergyLevels.Width() * octopusEnergyLevels.Height()))
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
