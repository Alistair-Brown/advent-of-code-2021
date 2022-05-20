#include "Parsing.h"
#include "Paper.h"
#include <iostream>
#include <string>
#include "puzzle_solvers.h"

PuzzleAnswerPair PuzzleSolvers::AocDayThirteenSolver(std::ifstream& puzzleInputFile)
{
	Paper::DottedPaper dottedPaper{};

	std::vector<std::string> inputLines = Parsing::SeparateInputIntoLines(puzzleInputFile);
	unsigned int linesRead = 0;
	while (true)
	{
		if (inputLines[linesRead].size() == 0)
		{
			linesRead++;
			break;
		}

		Paper::Coordinate latestCoord{};
		Parsing::ParseStringIntoElements(
			inputLines[linesRead],
			std::regex{ "([0-9]+),([0-9]+)" },
			latestCoord.xPos,
			latestCoord.yPos
		);
		dottedPaper.MakeDot(latestCoord);
		linesRead++;
	}

	// Failing the while loop means we've hit the blank line, and the rest
	// will be fold instructions. Do the first fold separately to get part
	// 1, then run through the rest.
	Paper::Coordinate firstFold = Paper::ParseFoldInstruction(inputLines[linesRead++]);
	dottedPaper.MakeFold(firstFold);

	unsigned int dotsAfterFirstFold = dottedPaper.NumberOfDots();

	while (linesRead < inputLines.size())
	{
		Paper::Coordinate fold = Paper::ParseFoldInstruction(inputLines[linesRead]);
		dottedPaper.MakeFold(fold);
		linesRead++;
	}

	std::string printedWord = dottedPaper.FoldedPaper();

	return PuzzleAnswerPair{ std::to_string(dotsAfterFirstFold), printedWord };
}