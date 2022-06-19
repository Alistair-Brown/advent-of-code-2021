#include "Snailfish.h"
#include <iostream>
#include "puzzle_solvers.h"

// Day 18 deals with the addition of 'snailfish numbers'. A snailfish number is two numbers in
// square brackets separated by a comma, where each of those constituent numbers can either be
// a regular integer number or another snailfish number, e.g. "[[8,1],6]".
// Most of this puzzle involves adding snailfish numbers together, which is done by simply
// making the operands the two sides of a new snailfish number e.g.
// [5,[9,1]] + [[4,5],[3,7]] = [[5,[9,1]],[[4,5],[3,7]]]
// There are also some special rules you have to apply to 'reduce' a number after addition, but
// those are described elsewhere in my solution.
// We are also asked to find the magnitude of snailfish numbers, which is done by adding 3 times
// the magnitude of left element to 2 times the magnitude of the right element (if either of those
// elements are themselves snailfish numbers, their individual magnitudes must be calculated first).
PuzzleAnswerPair PuzzleSolvers::AocDayEighteenSolver(std::ifstream& puzzleInputFile)
{
	std::vector<std::string> snailNumbersAsString = Parsing::SeparateRemainingInputIntoLines(puzzleInputFile);

	// Part one, go through the entire list of Snailfish Numbers adding them up in order. The magnitude
	// of the final SnailfishNumber after all this addition is our part one result.
	std::unique_ptr<Snail::SnailfishNumber> currentSnailNum{};
	for (std::string const &snailfishNumberAsString : snailNumbersAsString)
	{
		std::unique_ptr<Snail::SnailfishNumber> snailfishNumber = std::make_unique<Snail::SnailfishNumber>(
			Snail::SplitStringIntoSnailfishPair(snailfishNumberAsString));

		if (!currentSnailNum)
		{
			currentSnailNum = std::move(snailfishNumber);
		}
		else
		{
			currentSnailNum = Snail::AddSnailfishNumbers(std::move(currentSnailNum), std::move(snailfishNumber));
		}
	}
	int totalMagnitude = currentSnailNum->Magnitude();

	// Part two, find the largest possible magnitude resulting from the addition of 2 snailfish numbers
	// from our input snailfish numbers (bearing in mind that snailfish addition is not commutative
	// i.e x+y != y+x ). Most of the time running this program is spent here, I'm sure there's an
	// optimisation opportunity here.
	std::unique_ptr<Snail::SnailfishNumber> numberOne;
	std::unique_ptr<Snail::SnailfishNumber> numberTwo;
	std::unique_ptr<Snail::SnailfishNumber> sumOfJustTwo;
	int maxMagnitude = 0;
	for (unsigned int ii = 0; ii < snailNumbersAsString.size(); ii++)
	{
		for (unsigned int jj = 0; jj < snailNumbersAsString.size(); jj++)
		{
			if (ii == jj) { continue; }

			numberOne = std::make_unique<Snail::SnailfishNumber>(Snail::SplitStringIntoSnailfishPair(snailNumbersAsString[ii]));
			numberTwo = std::make_unique<Snail::SnailfishNumber>(Snail::SplitStringIntoSnailfishPair(snailNumbersAsString[jj]));
			sumOfJustTwo = Snail::AddSnailfishNumbers(std::move(numberOne), std::move(numberTwo));
			int magnitude = sumOfJustTwo->Magnitude();
			if (magnitude > maxMagnitude) { maxMagnitude = magnitude; }
		}
	}

	return PuzzleAnswerPair{ std::to_string(totalMagnitude), std::to_string(maxMagnitude) };
}