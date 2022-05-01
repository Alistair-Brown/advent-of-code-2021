#include "Parsing.h"
#include "SevenSegment.h"
#include "puzzle_solvers.h"

PuzzleAnswerPair PuzzleSolvers::AocDayEightSolver(std::ifstream &puzzleInputFile)
{
	std::vector<std::string> leftHandElements{ 10 };
	std::vector<std::string> rightHandElements{ 4 };

	int totalHits{ 0 };
	int totalRightHandValue{ 0 };

	std::vector<std::string> inputLines = Parsing::SeparateInputIntoLines(puzzleInputFile);
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

		for (std::string element : rightHandElements)
		{
			if ((element.size() == 2) ||
				(element.size() == 3) ||
				(element.size() == 4) ||
				(element.size() == 7))
			{
				totalHits++;
			}
		}

		SevenSeg::SevenSegDisplay currentDisplay{ leftHandElements };
		int incrementBy = currentDisplay.GetValueOfSegments(rightHandElements);
		totalRightHandValue += incrementBy;
	}

	return PuzzleAnswerPair{ std::to_string(totalHits), std::to_string(totalRightHandValue) };
}