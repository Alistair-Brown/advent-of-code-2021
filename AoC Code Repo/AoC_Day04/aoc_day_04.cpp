#include "Bingo.h"
#include "Parsing.h"
#include <iostream>
#include <string>
#include "puzzle_solvers.h"

PuzzleAnswerPair PuzzleSolvers::AocDayFourSolver(std::ifstream &puzzleInputFile)
{
	Bingo::BingoCaller bingoCaller;

	// Parse the numbers to call up from input, separating out the commas. This should go into
	// my parsing library later.
	std::vector<int> numbersToCall = Parsing::ParseLineOnDelimiters<int>(puzzleInputFile, Parsing::comma);
	for (int numberToCall : numbersToCall)
	{
		bingoCaller.QueueNumber(numberToCall);
	}
	
	Parsing::SeekNextLine(puzzleInputFile);
	
	std::vector<Bingo::BingoGrid *> bingoGrids; // Just need these so I can free them later
	while (true)
	{		
		std::vector<std::vector<int>> parsedGrid = 
			Parsing::ParseMultipleLinesWithDelimiters<int>(puzzleInputFile, Parsing::space);

		if (parsedGrid.size() > 0)
		{
			Bingo::BingoGrid *currentGrid = new Bingo::BingoGrid();
			bingoGrids.push_back(currentGrid);
			int rowNumber = 0;
			int columnNumber = 0;
			for (std::vector<int> row : parsedGrid)
			{
				columnNumber = 0;
				for (int digit : row)
				{
					bingoCaller.CreateNumberInGrid(digit, currentGrid, rowNumber, columnNumber);
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

	std::cout << "All Grids populated" << std::endl;

	int numberOfWinners = 0;
	int thisNumberWinners = 0;
	int firstWinner = 0;
	int result;
	while (true)
	{
		result = bingoCaller.CallNextNumber(thisNumberWinners);
		if (result >= 0)
		{
			if (firstWinner == 0) { firstWinner = result; }
			numberOfWinners += thisNumberWinners;
			if (numberOfWinners == bingoGrids.size())
			{
				break;
			}
		}
	}

	for (Bingo::BingoGrid *grid : bingoGrids)
	{
		delete grid;
	}

	return PuzzleAnswerPair{ std::to_string(firstWinner), std::to_string(result) };
}