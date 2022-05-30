#include "Parsing.h"
#include "Chiton.h"
#include <iostream>
#include "puzzle_solvers.h"
#include "grid_utils.h"

void EnlargeGridBy5(std::vector<std::vector<int>> &originalGrid);

// Day 15 asks us to find the lowest 'risk' from the top left to bottom right of a grid
// of integers, where each element in the grid has a risk (between 1 and 9) of moving
// into it. For part 2, the input grid is enlarged by 5 in each direction, to force
// us to use a sensible algorithm that solves the problem in our lifetime.
PuzzleAnswerPair PuzzleSolvers::AocDayFifteenSolver(std::ifstream& puzzleInputFile)
{
	std::vector<std::vector<int>> gridOfInts = Parsing::ParseWhitespacelessGridOfDigits(puzzleInputFile);

	// First find the path through our initial grid of integers, then enlarge by 5
	// and go again.
	ULLINT partOneRisk = Chiton::LowestRiskPath(gridOfInts);
	EnlargeGridBy5(gridOfInts);
	ULLINT partTwoRisk = Chiton::LowestRiskPath(gridOfInts);

	return PuzzleAnswerPair{ std::to_string(partOneRisk), std::to_string(partTwoRisk) };
}

// Enlarge a grid of integers by 5 in both dimensions. The enlarged grid should be formed by
// duplicating the original grid 4 times in each direction, but where the values in the duplicated
// grid are increased by 1 for every step down or to the right that that copy of the grid is
// from the original. Any value that would exceed 9 as a result is wrapped back around to 1.
void EnlargeGridBy5(std::vector<std::vector<int>> &gridToEnlarge)
{
	// Start off by duplicating the original grid 4 times to the right, to form a grid
	// that is the same height as the original, but 4 times as wide. Each row is expanded
	// in one continuous loop that takes advantage of the fact that by the time we are on
	// to the second duplication, the elements from the first duplication will already be
	// in place for us to copy from, with values only needing to be incremented by 1
	// further in order for them to have been increased by 2 relative to the original grid.
	// And so on, until we have added enough elements to produce rows that are 5 times
	// as wide as they were.
	for (auto &row : gridToEnlarge)
	{
		unsigned int originalWidth = row.size();
		for (unsigned int ii = 0; ii < 4 * originalWidth; ii++)
		{
			int newValue = row[ii];
			if (newValue == 9) { newValue = 1; }
			else { newValue += 1; }				
			row.push_back(newValue);
		}
	}

	// Now work our way down to create a grid that is 5 times as high as the original. We will
	// use the same trick we did when widening the rows to allow ourselves to do this in a single
	// loop, where the value each element added only needs to be increased by 1 more than the
	// value in the row it is being copied from.
	unsigned int originalHeight = gridToEnlarge.size();
	for (unsigned int ii = 0; ii < 4 * originalHeight; ii++)
	{
		std::vector<int> row = gridToEnlarge[ii];
		for (int &element : row)
		{
			if (element == 9) { element = 1; }
			else { element += 1; }
		}

		gridToEnlarge.push_back(row);
	}
}