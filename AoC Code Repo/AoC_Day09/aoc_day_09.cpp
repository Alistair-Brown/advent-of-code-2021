#include "Parsing.h"
#include "Heightmap.h"
#include "puzzle_solvers.h"

// Day nine gives us a grid of integers representing the heights across a cavern.
// Part one requires us to find all low points (lower than adjacent positions) in the cavern
// and sum up the heights of these low points.
// Part two requires us to find the largest basin (area surrounded by height-9 locations or
// cavern edges), and do a product of all the individual heights within than basin.
PuzzleAnswerPair PuzzleSolvers::AocDayNineSolver(std::ifstream &puzzleInputFile)
{
	std::vector<std::vector<int>> gridOfDigits = Parsing::ParseWhitespacelessGridOfDigits(puzzleInputFile);

	HeightMap::HeightMap heightMap{ gridOfDigits };
	return PuzzleAnswerPair{
		std::to_string(heightMap.SumRiskLevels()),
		std::to_string(heightMap.ProductOfLargestBasins()) };
}