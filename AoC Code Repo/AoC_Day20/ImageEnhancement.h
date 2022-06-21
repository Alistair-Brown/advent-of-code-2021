#pragma once
#include <string>
#include <vector>
#include <deque>
#include <cassert>

namespace Image
{
	const char LightCell{ '#' };
	const char DarkCell{ '.' };
	const unsigned int NumberOfNineDigitBinaryNumbers = 512;

	// The enhancement algorithm is a wrapper around a string like "#.##.#.", although the real string
	// must be 512 characters long, so that there is a character within it for every possible 9 digit
	// binary number.
	// The enhancement algorithm must be able to tell us what the new value of a given cell in the grid
	// should be after a single enhancement step.
	class EnhancementAlgorithm
	{
	private:
		std::string algorithmString{};
	public:
		EnhancementAlgorithm() {};
		EnhancementAlgorithm(std::string algorithmString) : algorithmString { algorithmString } 
		{
			assert(algorithmString.size() == NumberOfNineDigitBinaryNumbers);
		};

		char NextValueOfCell(
			unsigned int row,
			unsigned int column,
			std::deque<std::string> const& image);
		char CellAtPosition(unsigned int pos) { return algorithmString[pos]; }
	};

	// An Image is formed from an initial unenhanced image, and the enhancement algorithm
	// to apply for each enhancement step.
	// The Image must be able to undergo enhancement steps when instructed, and also
	// report the number of cells which are currently light at any point.
	class Image
	{
	private:
		std::deque<std::string> image;
		EnhancementAlgorithm enhancementAlgorithm;
		bool outsideIsLight{ false };
		void UndergoSingleRoundOfEnhancement();
		void ApplyInfinityBuffers();
	public:
		Image(std::deque<std::string> unenhancedImage, EnhancementAlgorithm algorithm);

		void UndergoRoundsOfEnhancement(unsigned int numberOfRounds);
		int NumberOfLightCells();
	};
}