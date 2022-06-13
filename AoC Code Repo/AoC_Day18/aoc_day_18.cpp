#include "Snailfish.h"
#include <iostream>
#include "puzzle_solvers.h"

PuzzleAnswerPair PuzzleSolvers::AocDayEighteenSolver(std::ifstream& puzzleInputFile)
{
	std::string firstNumAsString{};
	std::getline(puzzleInputFile, firstNumAsString);

	std::vector<std::string> snailNumbersAsString;
	snailNumbersAsString.push_back(firstNumAsString);
	Snail::SnailfishNumber *currentSnailNum = new Snail::SnailfishNumber(Snail::SplitStringIntoSnailfishPair(firstNumAsString));
	Snail::SnailfishNumber *nextSnailNum;

	std::vector<std::string> inputLines = Parsing::SeparateRemainingInputIntoLines(puzzleInputFile);
	for (std::string nextNumAsString : inputLines)
	{
		if (nextNumAsString.size() == 0) { break; }
		snailNumbersAsString.push_back(nextNumAsString);
		nextSnailNum = new Snail::SnailfishNumber(Snail::SplitStringIntoSnailfishPair(nextNumAsString));
		currentSnailNum = Snail::AddSnailfishNumbers(currentSnailNum, nextSnailNum);
	}

	int partOne = currentSnailNum->Magnitude();

	Snail::SnailfishNumber *numberOne;
	Snail::SnailfishNumber *numberTwo;
	Snail::SnailfishNumber *sumOfJustTwo;
	int maxMagnitude = 0;
	for (int ii = 0; ii < snailNumbersAsString.size(); ii++)
	{
		for (int jj = ii + 1; jj < snailNumbersAsString.size(); jj++)
		{
			numberOne = new Snail::SnailfishNumber(Snail::SplitStringIntoSnailfishPair(snailNumbersAsString[ii]));
			numberTwo = new Snail::SnailfishNumber(Snail::SplitStringIntoSnailfishPair(snailNumbersAsString[jj]));
			sumOfJustTwo = Snail::AddSnailfishNumbers(numberOne, numberTwo);
			int magnitude = sumOfJustTwo->Magnitude();
			if (magnitude > maxMagnitude) { maxMagnitude = magnitude; }
			delete sumOfJustTwo;
		}
	}

	return PuzzleAnswerPair{ std::to_string(partOne), std::to_string(maxMagnitude) };
}