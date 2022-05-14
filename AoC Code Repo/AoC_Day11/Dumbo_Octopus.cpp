#include "Dumbo_Octopus.h"

// Advancing a single time step causes all octopuses to increase their energy level,
// maybe reaching the point where they flash, which further increases the energy level
// of adjacent octopuses, maybe causing further flashes.
void Dumbo::OctopusGrid::AdvanceSingleStep()
{
	// Start by simply increasing the energy levels of all octopuses and creating a vector
	// of those which will now flash as a result.
	std::vector<std::reference_wrapper<GridUtils::Grid<int>::GridCell>> octosToFlash;
	for (GridUtils::Grid<int>::GridCell &octopus : octopusEnergyLevels)
	{
		octopus.value++;
		if (octopus.value == levelRequiredToFlash)
		{
			octosToFlash.push_back(octopus);
		}
	}

	// Now for each octopus which flashes, also increase the energy levels of adjacent
	// octopuses. This may cause this adjacent octopus to flash, in which case we add
	// it to the list of octopuses left to handle the flash for this step.
	while (octosToFlash.size() > 0)
	{
		GridUtils::Grid<int>::GridCell &octo = octosToFlash.back();
		octosToFlash.pop_back();
		octo.value = 0;
		flashesOccuredSoFar++;

		// When setting up a vector of adjacent octopuses which we should increment the
		// energy levels for as a result of this octopus's flash, account for the possibility
		// of this octopus being on a grid edge by just placing this octopus itself into the
		// list for that adjacent location, and then just skip over any 'adjacent' octopuses
		// which are in fact just this octopus (hence don't exist).
		std::vector<std::reference_wrapper<GridUtils::Grid<int>::GridCell>> adjacentOctos{
			octo.IsTopRow() ? octo : octo.Up(),
			octo.IsTopRow() || octo.IsRightColumn() ? octo : octo.UpRight(),
			octo.IsTopRow() || octo.IsLeftColumn() ? octo : octo.UpLeft(),
			octo.IsBottomRow() ? octo : octo.Down(),
			octo.IsBottomRow() || octo.IsRightColumn() ? octo : octo.DownRight(),
			octo.IsBottomRow() || octo.IsLeftColumn() ? octo : octo.DownLeft(),
			octo.IsLeftColumn() ? octo : octo.Left(),
			octo.IsRightColumn() ? octo : octo.Right()
		};
		for (GridUtils::Grid<int>::GridCell& adjacentOcto : adjacentOctos)
		{
			if ((octo != adjacentOcto) && AdjacentFlashTriggersOctopus(adjacentOcto))
			{
				octosToFlash.push_back(adjacentOcto);
			}
		}
	}

	stepsTakenSoFar++;
}

// Called when an octopus adjacent to this one has flashed, causing an increase in this
// octopus's energy level too, unless this octopus has already flashed this step, or
// is already primed to flash (has energy of levelRequiredToFlash). Returns true if
// this octopus is now primed to flash when it wasn't before.
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

// Advance time by a given number of steps. If any of these steps causes
// all of the octopuses to flash simultaneously for the first time, note
// down the total number of steps it took to reach this simultaneous flash
// (including any steps already taken by the octopus grid prior to this
// series of steps).
void Dumbo::OctopusGrid::AdvanceSteps(unsigned int numSteps)
{
	unsigned int flashesBeforeStep{ 0 };
	for (unsigned int ii = 0; ii < numSteps; ii++)
	{
		flashesBeforeStep = flashesOccuredSoFar;
		AdvanceSingleStep();

		// If the difference between the number of flashes that had ever occured
		// prior to that step, and the number of flashes that have now ever occured,
		// is equal to the size of the grid, this was a simultaneous flash of all
		// octopuses.
		if ((flashesOccuredSoFar - flashesBeforeStep) ==
			(octopusEnergyLevels.Width() * octopusEnergyLevels.Height()))
		{
			firstSimultaneousFlash = firstSimultaneousFlash == 0 ?
				stepsTakenSoFar : firstSimultaneousFlash;
		}
	}
}

// Advance time until the method variable firstSimultaneousFlash is non-zero,
// indicating that all of the octopuses have flashed on the same time step
// for the first time. Return the total number of flashes (including any
// that took place before this function call) that it took for this first
// simultaneous flash to occur.
unsigned int Dumbo::OctopusGrid::AdvanceUntilFirstSimultaneousFlash()
{
	while (firstSimultaneousFlash == 0)
	{
		AdvanceSteps(1);
	}
	return firstSimultaneousFlash;
}
