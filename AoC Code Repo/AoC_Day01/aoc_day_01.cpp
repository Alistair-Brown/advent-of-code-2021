#include <iostream>
#include <cassert>
#include "Parsing.h"
#include "puzzle_solvers.h"

// Day 1 was a simple case of comparing the relative size of subsequent elements in a list
// of ints, with part 2 instead comparing the relative size of subsequent groups of ints.
PuzzleAnswerPair PuzzleSolvers::AocDayOneSolver(std::ifstream &puzzleInputFile)
{
	PuzzleAnswerPair puzzleAnswerPair{};
	int totalGreaterThans{ 0 };

	// Puzzle input for today is a whole load of ints, each on its own line, so
	// let's turn that into a vector.
	std::vector<int> parsedValues = Parsing::SeparateOnWhitespace<int>(puzzleInputFile);
	assert(parsedValues.size() > 1);

	// Part 1, how many times is an element larger than the previous element.
	for (unsigned int ii = 1; ii < parsedValues.size(); ii++)
	{
		if (parsedValues[ii] > parsedValues[ii - 1])
		{
			totalGreaterThans++;
		}
	}
	puzzleAnswerPair.first = std::to_string(totalGreaterThans);

	// Although what we're really comparing in part 2 is the relative sizes of groups of
	// 3 elements, there are two elements of overlap between each group of 3, so we can
	// simplify the problem a little by just calculating how many times an element is
	// greater than that 3 spaces before it.
	totalGreaterThans = 0;
	for (unsigned int ii = 3; ii < parsedValues.size(); ii++)
	{
		if (parsedValues[ii] > parsedValues[ii - 3])
		{
			totalGreaterThans++;
		}
	}
	puzzleAnswerPair.second = std::to_string(totalGreaterThans);

	puzzleInputFile.close();

	return puzzleAnswerPair;
}