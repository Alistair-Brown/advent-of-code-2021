#include "Bingo.h"
#include <cassert>

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

void Bingo::BingoCaller::CreateNumberInGrid(int numberValue, BingoGrid *parentGrid, int rowNumber, int columnNumber)
{
	callableNumbers[numberValue].push_back(NumberWithinGrid(numberValue, parentGrid, rowNumber, columnNumber));
	parentGrid->InsertNumberIntoGrid(numberValue);
}

int Bingo::BingoCaller::CallNextNumber(int &numberOfWinners)
{
	int calledNumber = *(numbersToCall.begin());
	numbersToCall.pop_front();
	int unmarkedValuesInCompleteGrid;
	int returnValue = -1;
	numberOfWinners = 0;
	for (NumberWithinGrid numberWithinGrid : callableNumbers[calledNumber])
	{
		unmarkedValuesInCompleteGrid = numberWithinGrid.CrossSelfOutInParentGrid();
		if (unmarkedValuesInCompleteGrid >= 0)
		{
			numberOfWinners++;
			returnValue = unmarkedValuesInCompleteGrid;
		}
	}

	if (returnValue >= 0)
	{
		return returnValue * calledNumber;
	}
	else
	{
		return -1;
	}
}
