#include "Parsing.h"
#include "Chiton.h"
#include <iostream>
#include "puzzle_solvers.h"

std::vector<std::vector<int>> EnlargeGridBy5(std::vector<std::vector<int>> originalGrid);

PuzzleAnswerPair PuzzleSolvers::AocDayFifteenSolver(std::ifstream& puzzleInputFile)
{
	std::vector<std::vector<int>> originalGrid = Parsing::ParseWhitespacelessGridOfDigits(puzzleInputFile);
	Chiton::ChitonCave chitonCave{ originalGrid };

	int partOne = chitonCave.MinimumRiskToEnd();

	std::vector<std::vector<int>> enlargedGrid = EnlargeGridBy5(originalGrid);
	Chiton::ChitonCave bigChitonCave{ enlargedGrid };

	int partTwo = bigChitonCave.MinimumRiskToEnd();

	return PuzzleAnswerPair{ std::to_string(partOne), std::to_string(partTwo) };
}

std::vector<std::vector<int>> EnlargeGridBy5(std::vector<std::vector<int>> originalGrid)
{
	std::vector<std::vector<int>> newGrid = originalGrid;
	for (unsigned int ii = 0; ii < originalGrid.size(); ii++)
	{
		std::vector<int> inputRow = originalGrid[ii];
		for (int jj = 0; jj < 4; jj++)
		{
			for (int elem = 0; elem < inputRow.size(); elem++)
			{
				if (inputRow[elem] < 9) { inputRow[elem]++; }
				else { inputRow[elem] = 1; }
			}

			newGrid[ii].insert(newGrid[ii].end(), inputRow.begin(), inputRow.end());
		}
	}

	std::vector<std::vector<int>> middleGrid = newGrid;

	for (int ii = 0; ii < 4; ii++)
	{
		for (std::vector<int> &row : middleGrid)
		{
			for (int &elem : row)
			{
				if (elem < 9) { elem++; }
				else { elem = 1; }
			}
		}

		for (unsigned int jj = 0; jj < originalGrid.size(); jj++)
		{
			newGrid.push_back(middleGrid[jj]);
		}
	}

	return newGrid;
}