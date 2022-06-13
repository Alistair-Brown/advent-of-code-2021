#include "Snailfish.h"
#include <iostream>
#include "puzzle_solvers.h"

PuzzleAnswerPair PuzzleSolvers::AocDayEighteenSolver(std::ifstream& puzzleInputFile)
{
	std::string firstNumAsString{};
	std::getline(puzzleInputFile, firstNumAsString);

	std::vector<std::string> snailNumbersAsString;
	snailNumbersAsString.push_back(firstNumAsString);
	std::unique_ptr<Snail::SnailfishNumber> currentSnailNum =
		std::make_unique<Snail::SnailfishNumber>(Snail::SplitStringIntoSnailfishPair(firstNumAsString));
	std::unique_ptr<Snail::SnailfishNumber> nextSnailNum;

	std::vector<std::string> inputLines = Parsing::SeparateRemainingInputIntoLines(puzzleInputFile);
	for (std::string nextNumAsString : inputLines)
	{
		if (nextNumAsString.size() == 0) { break; }
		snailNumbersAsString.push_back(nextNumAsString);
		nextSnailNum = std::make_unique<Snail::SnailfishNumber>(
			Snail::SplitStringIntoSnailfishPair(nextNumAsString));
		currentSnailNum = Snail::AddSnailfishNumbers(std::move(currentSnailNum), std::move(nextSnailNum));
	}

	int partOne = currentSnailNum->Magnitude();

	std::unique_ptr<Snail::SnailfishNumber> numberOne;
	std::unique_ptr<Snail::SnailfishNumber> numberTwo;
	std::unique_ptr<Snail::SnailfishNumber> sumOfJustTwo;
	int maxMagnitude = 0;
	for (int ii = 0; ii < snailNumbersAsString.size(); ii++)
	{
		for (int jj = ii + 1; jj < snailNumbersAsString.size(); jj++)
		{
			numberOne = std::make_unique<Snail::SnailfishNumber>(Snail::SplitStringIntoSnailfishPair(snailNumbersAsString[ii]));
			numberTwo = std::make_unique<Snail::SnailfishNumber>(Snail::SplitStringIntoSnailfishPair(snailNumbersAsString[jj]));
			sumOfJustTwo = Snail::AddSnailfishNumbers(std::move(numberOne), std::move(numberTwo));
			int magnitude = sumOfJustTwo->Magnitude();
			if (magnitude > maxMagnitude) { maxMagnitude = magnitude; }
		}
	}

	return PuzzleAnswerPair{ std::to_string(partOne), std::to_string(maxMagnitude) };
}