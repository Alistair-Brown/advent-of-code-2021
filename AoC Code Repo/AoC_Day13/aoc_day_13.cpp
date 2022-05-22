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
	while (true)
	{
		std::vector<unsigned int> coord = 
			Parsing::SplitNextLineOnDelimiter<unsigned int>(puzzleInputFile, Parsing::comma);
		if (coord.size() == 0)
		{
			break;
		}
		dottedPaper.MakeDot({ coord[0], coord[1] });
	}

	// The remaining lines of input will be fold instructions for us to apply to the
	// paper. After the first of these, part one of the puzzle wants to know how many
	// dots are now visible.
	std::vector<std::string> instructions = Parsing::SeparateRemainingInputIntoLines(puzzleInputFile);
	Paper::DottedPaper::FoldLine foldLine;
	bool firstFoldMade = false;
	unsigned int dotsAfterFirstFold{};
	for (const std::string &instruction : instructions)
	{
		Parsing::ParseStringIntoElements(
			instruction,
			std::regex{ "fold along (x|y)=([0-9]+)" },
			foldLine.axis,
			foldLine.position
		);
		dottedPaper.MakeFold(foldLine);

		if (!firstFoldMade)
		{
			firstFoldMade = true;
			dotsAfterFirstFold = dottedPaper.NumberOfDots();
		}
	}

	std::string printedWord = dottedPaper.FoldedPaperView();

	return PuzzleAnswerPair{ std::to_string(dotsAfterFirstFold), printedWord };
}