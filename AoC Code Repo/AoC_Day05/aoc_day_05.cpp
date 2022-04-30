#include "SteamVents.h"
#include <iostream>
#include <string>
#include "puzzle_solvers.h"
#include "Parsing.h"

// Day 5 presents us with a whole bunch of lines given in the form "x1,y1 -> x2,y2",
// and ask us to calculate the number of coordinates which contain overlapping lines.
// Part 1 ignores all diagonal lines from the input, while part 2 takes the diagonal
// lines into account too.
PuzzleAnswerPair PuzzleSolvers::AocDayFiveSolver(std::ifstream &puzzleInputFile)
{
	Steam::WarningMap warningMap;
	std::vector<Steam::CoordinatePath> diagonalLines;

	// Parse every line from the puzzle input into the constituent coordinates of
	// its start and end position. For non-diagonal lines, insert them into the
	// overall 'map' of lines (map in the sense that it holds a 2D grid of coordinates,
	// not a map in the STL sense) as we go. Put diagonal lines to the side to add to
	// the map during part 2.
	std::vector<std::string> inputLines = Parsing::SeparateInputIntoLines(puzzleInputFile);
	std::regex matchString{ "([0-9]+),([0-9]+) -> ([0-9]+),([0-9]+)" };
	for (std::string inputLine : inputLines)
	{
		Steam::CoordinatePath coordPath;
		Parsing::ParseStringIntoElements(
			inputLine,
			matchString,
			coordPath.xStart,
			coordPath.yStart,
			coordPath.xFinish,
			coordPath.yFinish);
		if (!coordPath.IsDiagonalLine())
		{
			warningMap.InsertWarningPath(coordPath);
		}
		else
		{
			diagonalLines.push_back(coordPath);
		}
	}

	// The solution to part 1 is the number of coordinates in the map which have
	// overlapping lines after taking into account all non-diagonal lines.
	unsigned int overlapsWithoutDiagonals = warningMap.NumberOfOverlapCells();

	// Then for part 2 we just add in all of the diagonal lines we set aside earlier,
	// and re-query the number of coordinates with overlapping lines.
	for (Steam::CoordinatePath path : diagonalLines)
	{
		warningMap.InsertDiagonalWarningPath(path);
	}
	unsigned int overlapsIncludingDiagonals = warningMap.NumberOfOverlapCells();

	return PuzzleAnswerPair{ std::to_string(overlapsWithoutDiagonals), std::to_string(overlapsIncludingDiagonals) };
}