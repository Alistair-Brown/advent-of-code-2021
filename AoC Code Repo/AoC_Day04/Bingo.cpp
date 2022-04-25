#include "Bingo.h"
#include <cassert>

// Called during setup to inform this grid that it contains a given number.
// BingoGrid doesn't keep track of what numbers are in each row and column, the
// only thing we need to do with the knowledge that a grid contains a certain number
// somewhere is increase the total value of the unmarked numbers in the grid.
void Bingo::BingoGrid::GridContainsNumber(int numberValue)
{
	valueOfUnmarkedNumbers += numberValue;
}

// Crossing out a number from a grid means that:
// - The total value of unmarked numbers decreases accordingly
// - The number of hits in each row and column is increased.
// 
// If a row or column has been hit enough to indicate that it was completed,
// then this function will return the value of unmarked numbers remaining in the
// grid, otherwise it will return -1.
int Bingo::BingoGrid::CrossNumberOutFromGrid(int numberValue, int rowNumber, int columnNumber)
{
	int returnValue = -1;
	valueOfUnmarkedNumbers -= numberValue;
	rows[rowNumber]++;
	columns[columnNumber]++;
	if ((rows[rowNumber] == numbersOfColumns) || (columns[columnNumber] == numbersOfRows))
	{
		if (!hasWon)
		{
			returnValue = valueOfUnmarkedNumbers;
			hasWon = true;
		}
	}
	return returnValue;
}

// Creating a number within a grid simply requires the caller to create the instance of
// this number and add it to the list of NumberWithinGrid's that must be crossed off
// when a given callable number is later called.
// The constructor for NumberWithinGrid handles alerting the parent grid to that number's
// existence.
void Bingo::BingoCaller::CreateNumberInGrid(
	int numberValue,
	std::weak_ptr<BingoGrid> parentGrid,
	int rowNumber,
	int columnNumber)
{
	callableNumbers[numberValue].emplace_back(NumberWithinGrid(numberValue, parentGrid, rowNumber, columnNumber));
}

// 'Call' a number, causing it to be crossed off in all grids containing it. The number of
// grids that complete a row or column for the first time as a result of this number being
// called is returned as the first element in the answer pair. If exactly one grid was
// completed in this way, the second element of the answer pair will return the total value
// of uncrossed numbers in that grid, multiplied by the number that was just called (as
// is required by the puzzle).
std::pair<unsigned int, unsigned int> Bingo::BingoCaller::CallNextNumber()
{
	// Give that the numbers to call were appended to the END of numbersToCall in the order
	// in which they should be called, we need to retrieve the next number from the FRONT
	// of that deque.
	int calledNumber = *(numbersToCall.begin());
	numbersToCall.pop_front();

	int unmarkedValuesInCompleteGrid;
	int productOfNumberAndUnmarkedValues{ 0 };
	unsigned int numberOfWinners{ 0 };
	for (NumberWithinGrid numberWithinGrid : callableNumbers[calledNumber])
	{
		unmarkedValuesInCompleteGrid = numberWithinGrid.CrossSelfOutInParentGrid();
		if (unmarkedValuesInCompleteGrid >= 0)
		{
			numberOfWinners++;
			if (numberOfWinners == 1)
			{
				productOfNumberAndUnmarkedValues = calledNumber * unmarkedValuesInCompleteGrid;
			}
		}
	}

	std::pair<unsigned int, unsigned int> returnPair;
	returnPair.first = numberOfWinners;
	returnPair.second = numberOfWinners == 1 ? productOfNumberAndUnmarkedValues : 0;

	return returnPair;
}
