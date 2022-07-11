#include "Cucumber.h"
#include <cassert>
#include <iostream>

// Constructor for the Trench. First initialise a GridUtils::Grid based on the 2D vector
// of characters representing the initial state of the trench. Then scan the grid to find
// the coordinates of cucumbers that will be unblocked on step 1.
Cucumber::Trench::Trench(std::vector<std::vector<char>> gridIn) : cucumberGrid { gridIn }
{
	for (GridUtils::Grid<char>::GridCell const &cell : cucumberGrid)
	{
		// Because East-moving cucumbers move first each step, the only requirement for an
		// East-moving cucumber to be unblocked is that the space in front of it is empty.
		// But for a South-facing cucumber to be unblocked, we also need to check that an
		// East-cucumber will not move into that space before it gets a chance to move.
		if ((cell.value == EAST_CUCUMBER) &&
			(cell.RightOrWrap().value == EMPTY_SPACE))
		{
			unblockedEastCucumbers.push_back(cell.GetCoordinate());
		}
		else if ((cell.value == SOUTH_CUCUMBER) &&
			(cell.DownOrWrap().value == EMPTY_SPACE) &&
			(cell.DownOrWrap().LeftOrWrap().value != EAST_CUCUMBER))
		{
			unblockedSouthCucumbers.push_back(cell.GetCoordinate());
		}
	}
}

// Carry out movement steps until all cucumbers are unable to move. The final step number
// to return is the first on which no cucumbers can move, rather than the last in which
// movement takes place.
unsigned int Cucumber::Trench::StepsUntilAllBlocked()
{
	// Only carry out the movement steps if we haven't already found the result of this
	// function from a previous query (in which case it will be non-zero).
	if (stepsUntilAllBlocked == 0)
	{
		while ((unblockedEastCucumbers.size() > 0) || (unblockedSouthCucumbers.size() > 0))
		{
			CarryOutSingleMovementStep();
			stepsUntilAllBlocked++;
		}
		stepsUntilAllBlocked++;
	}
	return stepsUntilAllBlocked;
}

// Carry out a single movement step.
// The trench should already have a list of the coordinates of currently unblocked
// East- and South-moving cucumbers which we can work through rather than actually
// iterating over the grid. With each cucumber that moves though we need to both check
// if it will still be able to move next step, and whether it's own movement frees up
// any other cucumbers to move, and update the lists of unblocked cucumbers accordingly.
void Cucumber::Trench::CarryOutSingleMovementStep()
{
	// To avoid editing a list that we're actively iterating over, we will create new lists
	// for each direction as we go, and then replace the lists wholesale afterwards.
	std::list<GridUtils::Coordinate> newUnblockedEastCucumbers{};
	std::list<GridUtils::Coordinate> newUnblockedSouthCucumbers{};

	// East-moving cucumbers move first, so work through all of our East-moving cucumbers
	// before the South-moving ones.
	for (GridUtils::Coordinate eastCucumberOriginalPos : unblockedEastCucumbers)
	{
		// 'Move' our cucumber by resetting it's old position to be an empty space, and putting
		// an East Cucumber character in its new location.
		GridUtils::Coordinate eastCucumberNewPos = cucumberGrid[eastCucumberOriginalPos].RightOrWrap().GetCoordinate();
		assert(cucumberGrid[eastCucumberNewPos].value == EMPTY_SPACE);
		cucumberGrid[eastCucumberOriginalPos].value = EMPTY_SPACE;
		cucumberGrid[eastCucumberNewPos].value = EAST_CUCUMBER;

		// If there is now room for another cucumber to move into the space that this one just
		// vacated, update our lists accordingly. If there are two cucumbers able to move into
		// that space, the South-moving one gets dibs, since South-moving cucumbers are yet
		// to move this step.
		if (cucumberGrid[eastCucumberOriginalPos].UpOrWrap().value == SOUTH_CUCUMBER)
		{
			unblockedSouthCucumbers.push_back(cucumberGrid[eastCucumberOriginalPos].UpOrWrap().GetCoordinate());
		}
		else if (cucumberGrid[eastCucumberOriginalPos].LeftOrWrap().value == EAST_CUCUMBER)
		{
			newUnblockedEastCucumbers.push_back(cucumberGrid[eastCucumberOriginalPos].LeftOrWrap().GetCoordinate());
		}

		// If there is also a space in front of this cucumber's new position, and a South-moving
		// cucumber is not about to move into it, place this cucumber on the new list of
		// unblocked East-moving cucumbers ready for the next step.
		GridUtils::Coordinate nextPotentialCoordinate = cucumberGrid[eastCucumberNewPos].RightOrWrap().GetCoordinate();
		if ((cucumberGrid[nextPotentialCoordinate].value == EMPTY_SPACE) &&
		    (cucumberGrid[nextPotentialCoordinate].UpOrWrap().value != SOUTH_CUCUMBER))
		{
			newUnblockedEastCucumbers.push_back(eastCucumberNewPos);
		}
	}
	unblockedEastCucumbers = newUnblockedEastCucumbers;

	// East-moving cucumbers are now all done, with the list of unblocked South-moving cucumbers
	// updated to reflect any South-moving cucumbers that can now move. So proceed with the
	// South-moving cucumbers' half of this movement step.
	for (GridUtils::Coordinate southCucumberOriginalPos : unblockedSouthCucumbers)
	{
		// 'Move' our cucumber by resetting it's old position to be an empty space, and putting
		// a South Cucumber character in its new location.
		GridUtils::Coordinate southCucumberNewPos = cucumberGrid[southCucumberOriginalPos].DownOrWrap().GetCoordinate();
		assert(cucumberGrid[southCucumberNewPos].value == EMPTY_SPACE);
		cucumberGrid[southCucumberOriginalPos].value = EMPTY_SPACE;
		cucumberGrid[southCucumberNewPos].value = SOUTH_CUCUMBER;

		// If there is now room for another cucumber to move into the space that this one just
		// vacated, update our lists accordingly. If there are two cucumbers able to move into
		// that space, the East-moving one gets dibs, since East-moving cucumbers will move first
		// next step.
		if (cucumberGrid[southCucumberOriginalPos].LeftOrWrap().value == EAST_CUCUMBER)
		{
			unblockedEastCucumbers.push_back(cucumberGrid[southCucumberOriginalPos].LeftOrWrap().GetCoordinate());
		}
		else if (cucumberGrid[southCucumberOriginalPos].UpOrWrap().value == SOUTH_CUCUMBER)
		{
			newUnblockedSouthCucumbers.push_back(cucumberGrid[southCucumberOriginalPos].UpOrWrap().GetCoordinate());
		}

		// If there is also a space in front of this cucumber's new position, and a East-moving
		// cucumber is not about to move into it, place this cucumber on the new list of
		// unblocked South-moving cucumbers ready for the next step.
		GridUtils::Coordinate nextPotentialCoordinate = cucumberGrid[southCucumberNewPos].DownOrWrap().GetCoordinate();
		if ((cucumberGrid[nextPotentialCoordinate].value == EMPTY_SPACE) &&
			(cucumberGrid[nextPotentialCoordinate].LeftOrWrap().value != EAST_CUCUMBER))
		{
			newUnblockedSouthCucumbers.push_back(southCucumberNewPos);
		}
	}
	unblockedSouthCucumbers = newUnblockedSouthCucumbers;
}

// Diagnostic helper function to print out the current state of the trench.
void Cucumber::Trench::PrintTrenchToScreen()
{
	std::cout << '\n';
	for (unsigned int yy = cucumberGrid.Height() - 1; ; yy--)
	{
		for (unsigned int xx = 0; xx < cucumberGrid.Width(); xx++)
		{
			std::cout << cucumberGrid[{xx, yy}].value;
		}
		if (yy == 0) { break; }
		std::cout << '\n';
	}
	std::cout << std::endl;
}