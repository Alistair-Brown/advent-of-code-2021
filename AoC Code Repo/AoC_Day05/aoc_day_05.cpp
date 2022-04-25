#include "SteamVents.h"
#include <iostream>
#include <string>
#include "puzzle_solvers.h"
#include "Parsing.h"

PuzzleAnswerPair PuzzleSolvers::AocDayFiveSolver(std::ifstream &puzzleInputFile)
{
	Steam::WarningMap warningMap;
	std::vector<Steam::WarningPath> diagonalLines;

	std::vector<std::string> inputLines = Parsing::SeparateInputIntoLines(puzzleInputFile);

	for (std::string inputLine : inputLines)
	{
		Steam::WarningPath path{ inputLine };
		if (path.IsStraightLine())
		{
			warningMap.InsertWarningPath(path);
		}
		else
		{
			diagonalLines.push_back(path);
		}
	}

	unsigned int overlapsPartOne = warningMap.NumberOfOverlapCells();

	for (Steam::WarningPath path : diagonalLines)
	{
		warningMap.InsertDiagonalWarningPath(path);
	}

	unsigned int overlapsPartTwo = warningMap.NumberOfOverlapCells();

	return PuzzleAnswerPair{ std::to_string(overlapsPartOne), std::to_string(overlapsPartTwo) };
}