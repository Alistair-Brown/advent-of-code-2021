#include "Bingo.h"
#include "Parsing.h"
#include <iostream>
#include <string>
#include "puzzle_solvers.h"

// Day 4 presents us with a set of bingo cards and an order in which numbers will be
// called. We need to find the first and last grid to complete a row or column (and then
// do a little bit of maths on each of those grids to produce a final couple of numbers).
PuzzleAnswerPair PuzzleSolvers::AocDayFourSolver(std::ifstream &puzzleInputFile)
{
	// The first line of input consists of all the bingo numbers which will be called, in
	// order. Queue these up on our bingo caller and then skip the following blank line
	// ready to parse the bingo grids themselves.
	std::vector<int> numbersToCall = Parsing::ParseLineOnDelimiters<int>(puzzleInputFile, Parsing::comma);
	Bingo::BingoCaller bingoCaller{ numbersToCall };
	Parsing::SeekNextLine(puzzleInputFile);
	
	// The remainder of the puzzle input is bingo grids, with a blank line between
	// each one. The way we handle these grids is quite nifty (I think so anyway).
	// The BingoGrids themselves will be given no knowledge of which numbers they
	// contain, instead the BingoCaller will be told which grids contain each number,
	// and which row and column the number is in for each grid. Then when the bingo caller
	// begins calling numbers, it can update each grid directly with the row and column
	// that have been hit, and all the grid needs to keep track off to know whether it has
	// won is the number of stamps in each row and column.
	std::vector<std::shared_ptr<Bingo::BingoGrid>> bingoGrids{};
	while (true)
	{		
		// Get the next grid from our input and work through its rows and columns creating
		// numbers ready to be crossed out when the bingo caller later calls those numbers.
		std::vector<std::vector<int>> parsedGrid = 
			Parsing::ParseMultipleLinesWithDelimiters<int>(puzzleInputFile, Parsing::space);
		if (parsedGrid.size() > 0)
		{
			std::shared_ptr<Bingo::BingoGrid> currentGrid{ std::make_shared<Bingo::BingoGrid>() };
			bingoGrids.push_back(currentGrid);
			int rowNumber = 0;
			int columnNumber = 0;
			for (std::vector<int> row : parsedGrid)
			{
				columnNumber = 0;
				for (int digit : row)
				{
					bingoCaller.CreateNumberInGrid(
						digit,
						std::weak_ptr<Bingo::BingoGrid>{currentGrid},
						rowNumber,
						columnNumber);
					columnNumber++;
				}
				rowNumber++;
			}
		}
		else
		{
			break;
		}
	}

	// Now get the bingo caller to start calling numbers out until every grid
	// has achieved at least a single row or column. We are only interested
	// in the first and last grids to do so.
	// The bingo caller will return a pair of results after each number called.
	// The first number in the pair is the number of grids that have now 'won' by
	// completing a row or column.
	// The second number in the pair is only populated if exactly one grid
	// completed a row or column this round, and is the product of the number that
	// was called and the sum of unstamped values remaining in the grid that
	// completed a row/column as a result of this number (as required by the puzzle).
	int numberOfWinners = 0;
	int firstWinnerScoreProduct = 0;
	int lastWinnerScoreProduct = 0;
	std::pair<unsigned int, unsigned int> resultPair;
	while (true)
	{
		resultPair = bingoCaller.CallNextNumber();
		if (resultPair.first > 0)
		{
			if (firstWinnerScoreProduct == 0)
			{
				assert(resultPair.first == 1);
				firstWinnerScoreProduct = resultPair.second; 
			}
			numberOfWinners += resultPair.first;
			if (numberOfWinners == bingoGrids.size())
			{
				assert(resultPair.first == 1);
				lastWinnerScoreProduct = resultPair.second;
				break;
			}
		}
	}

	return PuzzleAnswerPair{ std::to_string(firstWinnerScoreProduct), std::to_string(lastWinnerScoreProduct) };
}