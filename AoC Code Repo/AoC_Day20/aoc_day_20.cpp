#include "ImageEnhancement.h"
#include "Parsing.h"
#include <cassert>
#include "puzzle_solvers.h"

PuzzleAnswerPair PuzzleSolvers::AocDayTwentySolver(std::ifstream& puzzleInputFile)
{
	std::string enhancementAlgorithmAsString{};
	std::getline(puzzleInputFile, enhancementAlgorithmAsString);
	Image::EnhancementAlgorithm enhancementAlgorithm{ enhancementAlgorithmAsString };
	
	std::string blankLine{};
	std::getline(puzzleInputFile, blankLine);
	assert(blankLine.size() == 0);

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