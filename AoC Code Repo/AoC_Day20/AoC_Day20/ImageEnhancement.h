#pragma once
#include <string>
#include <vector>
#include <deque>

namespace Image
{
	const char LightCell{ '#' };
	const char DarkCell{ '.' };

	class EnhancementAlgorithm
	{
	private:
		std::string algorithmString{};
	public:
		EnhancementAlgorithm() {};
		EnhancementAlgorithm(std::string algorithmString) : algorithmString { algorithmString } {};
		bool CellIsLight(std::string binaryIndex);
	};

	class Image
	{
	private:
		std::deque<std::string> image;
		EnhancementAlgorithm enhancementAlgorithm;
		bool outsideIsLight{ false };
		void UndergoSingleRoundOfEnhancement();
		void ApplyInfinityBuffers();
	public:
		Image(std::deque<std::string> unenhancedImage, EnhancementAlgorithm algorithm) : Image(unenhancedImage, algorithm, false) {};
		Image(std::deque<std::string> unenhancedImage, EnhancementAlgorithm algorithm, bool outsideBeginsLight);
		void UndergoRoundsOfEnhancement(int numberOfRounds);
		int NumberOfLightCells();
	};
}