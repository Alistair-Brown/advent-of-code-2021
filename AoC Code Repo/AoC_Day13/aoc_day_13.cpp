#include "Parsing.h"
#include "Paper.h"
#include <iostream>
#include <string>
#include "puzzle_solvers.h"

// Day 13 gives us a virtual piece of tracing paper with numerous dots on it, and asks us
// to work out what the pattern of visible dots would be after this tracing paper was folded
// in on itself several times. The resulting paper should display a series of capital letters.
PuzzleAnswerPair PuzzleSolvers::AocDayThirteenSolver(std::ifstream& puzzleInputFile)
{
	Paper::DottedPaper dottedPaper{};

	// Up until the first blank line, our puzzle input consists of coordinates to mark as
	// dots on the tracing paper.
	std::vector<std::string> inputLines = Parsing::SeparateInputIntoLines(puzzleInputFile);
	unsigned int linesRead = 0;
	while (true)
	{
		if (inputLines[linesRead].size() == 0)
		{
			linesRead++;
			break;
		}

		GridUtils::Coordinate latestCoord{};
		Parsing::ParseStringIntoElements(
			inputLines[linesRead],
			std::regex{ "([0-9]+),([0-9]+)" },
			latestCoord.xPos,
			latestCoord.yPos
		);
		dottedPaper.MakeDot(latestCoord);
		linesRead++;
	}

	// The remaining lines of input will be fold instructions for us to apply to the
	// paper. After the first of these, part one of the puzzle wants to know how many
	// dots are now visible.
	Paper::DottedPaper::FoldLine foldLine;
	bool firstFoldMade = false;
	unsigned int dotsAfterFirstFold{};
	while (linesRead < inputLines.size())
	{
		Parsing::ParseStringIntoElements(
			inputLines[linesRead],
			std::regex{ "fold along (x|y)=([0-9]+)" },
			foldLine.axis,
			foldLine.position
		);
		dottedPaper.MakeFold(foldLine);
		linesRead++;

		if (!firstFoldMade)
		{
			firstFoldMade = true;
			dotsAfterFirstFold = dottedPaper.NumberOfDots();
		}
	}

	std::string printedWord = dottedPaper.FoldedPaperView();

	return PuzzleAnswerPair{ std::to_string(dotsAfterFirstFold), printedWord };
}