#include "ImageEnhancement.h"
#include "Parsing.h"
#include <cassert>
#include "puzzle_solvers.h"

// Day 20 asks us to apply an "image enhancement algorithm" to an image composed of '.' and '#'.
// With each step of the enhancement algorithm, we produce a binary number for each cell in
// the image, based on the current setting of that cell and its adjacent cells. That binary number
// corresponds to a character on our input enhancement algorithm, which tells us the new value
// of that cell. e.g. if the enhancement algorithm was "#.##.#" and we had the binary number 101" for
// 5, we would take the 5th element of the enhancement algorithm, '#' as the new setting for that
// cell.
// Part one asks us to find how many cells are 'light' (meaning '#') after 2 steps of the enhancement
// process. Part two asks for the same, but after a further 48 steps (for 50 total steps).
PuzzleAnswerPair PuzzleSolvers::AocDayTwentySolver(std::ifstream& puzzleInputFile)
{
	// The first like of the input is our enhancement algorithm.
	std::string enhancementAlgorithmAsString{};
	std::getline(puzzleInputFile, enhancementAlgorithmAsString);
	Image::EnhancementAlgorithm enhancementAlgorithm{ enhancementAlgorithmAsString };
	
	// Then there should be a single blank line.
	std::string blankLine{};
	std::getline(puzzleInputFile, blankLine);
	assert(blankLine.size() == 0);

	// The remaining lines make up our initial image, which is a grid of '#' and '.'
	// characters.
	std::deque<std::string> blurryImage{};
	std::vector<std::string> imageLines = Parsing::SeparateRemainingInputIntoLines(puzzleInputFile);
	for (std::string const &imageLine : imageLines)
	{
		blurryImage.push_back(imageLine);
	}

	Image::Image imageBeingEnhanced{ blurryImage, enhancementAlgorithm };

	imageBeingEnhanced.UndergoRoundsOfEnhancement(2);
	unsigned int lightCellsAfter2Rounds = imageBeingEnhanced.NumberOfLightCells();

	imageBeingEnhanced.UndergoRoundsOfEnhancement(48);
	unsigned int lightCellsAfter48Rounds = imageBeingEnhanced.NumberOfLightCells();

	return PuzzleAnswerPair{ std::to_string(lightCellsAfter2Rounds), std::to_string(lightCellsAfter48Rounds) };
}