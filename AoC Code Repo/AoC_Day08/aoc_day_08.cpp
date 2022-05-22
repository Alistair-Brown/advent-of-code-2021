#include "Parsing.h"
#include "SevenSegment.h"
#include "puzzle_solvers.h"

// Day 8 gives us a collection of muddled seven-segment-displays. From each line of input
// we must work out which of the letters a-g represent each segment in the display, and
// use this knowledge to calculate which numbers are being printed by each display.
PuzzleAnswerPair PuzzleSolvers::AocDayEightSolver(std::ifstream &puzzleInputFile)
{
	std::array<std::string, SevenSeg::numLeftHandElements> leftHandElements{};
	std::vector<std::string> rightHandElements{ SevenSeg::numRightHandElements };

	int partOneTotal{ 0 };
	int totalRightHandValue{ 0 };

	// Each line of input consists of 10 elements representing the numbers 0-9 in any order,
	// followed by 4 elements representing a 4 digit number e.g 3602 or 0359. From the representation
	// of the numbers 0-9, we can work out which of the letters a-g represent each segment
	// and then use this to parse the 4-digit number on the right hand side of the input.
	std::vector<std::string> inputLines = Parsing::SeparateRemainingInputIntoLines(puzzleInputFile);
	std::regex matchString{
		"([a-z]+) ([a-z]+) ([a-z]+) ([a-z]+) ([a-z]+) ([a-z]+) ([a-z]+) ([a-z]+) ([a-z]+) ([a-z]+) \\| "
		"([a-z]+) ([a-z]+) ([a-z]+) ([a-z]+)" };
	for (std::string inputLine : inputLines)
	{
		Parsing::ParseStringIntoElements(
			inputLine,
			matchString,
			leftHandElements[0],
			leftHandElements[1],
			leftHandElements[2],
			leftHandElements[3],
			leftHandElements[4],
			leftHandElements[5],
			leftHandElements[6],
			leftHandElements[7],
			leftHandElements[8],
			leftHandElements[9],
			rightHandElements[0],
			rightHandElements[1],
			rightHandElements[2],
			rightHandElements[3]
		);

		// Part 1 just requires us to count up the number of right hand elements
		// that could represent a 1 (2 segments), 4 (4 segments), 7 (3 segments)
		// or 8 (7 segments). So do a quick check for how many of the right hand
		// elements meet any of those options on this line and add that to our
		// running total.
		for (std::string element : rightHandElements)
		{
			if ((element.size() == 2) ||
				(element.size() == 3) ||
				(element.size() == 4) ||
				(element.size() == 7))
			{
				partOneTotal++;
			}
		}

		// Each Seven Segment Display is constructed using the left hand elements from its
		// line of input. From this, it will be able to deduce the number represented by
		// the right hand elements of the input. Part 2 asks for the sum of all of these
		// right hand elements.
		SevenSeg::SevenSegDisplay currentDisplay{ leftHandElements };
		totalRightHandValue += currentDisplay.GetValueOfSegments(rightHandElements);
	}

	return PuzzleAnswerPair{ std::to_string(partOneTotal), std::to_string(totalRightHandValue) };
}