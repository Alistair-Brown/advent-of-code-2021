#include "ImageEnhancement.h"
#include "Parsing.h"
#include <cassert>

bool Image::EnhancementAlgorithm::CellIsLight(std::string binaryIndex)
{
	for (char &cell : binaryIndex)
	{
		if (cell == LightCell) { cell = '1'; }
		else if (cell == DarkCell) { cell = '0'; }
		else { assert(false); }
	}
	int intIndex = Parsing::ConvertBinaryToInt(binaryIndex);
	char cellAtIndex = algorithmString[intIndex];

	if (cellAtIndex == LightCell)
	{
		return true;
	}
	else
	{
		assert(cellAtIndex == DarkCell);
		return false;
	}
}

void Image::Image::UndergoSingleRoundOfEnhancement()
{
	std::deque<std::string> enhancedImage{};

	// With the buffers of cells around the edge representing what extends into
	// infinity, there's no need to do any special maths there, we can just iterate
	// over our whole image, buffers and all. The only thing we need to do is
	// make sure we stop short of trying to access cells off the edge of the grid.
	for (int row = 0; row < image.size() - 3; row++)
	{
		enhancedImage.push_back("");
		for (int column = 0; column < image[0].size() - 3; column++)
		{
			std::string algorithmIndex =
				image[row].substr(column, 3) +
				image[row + 1].substr(column, 3) +
				image[row + 2].substr(column, 3);
			if (enhancementAlgorithm.CellIsLight(algorithmIndex))
			{
				enhancedImage[row].push_back(LightCell);
			}
			else
			{
				enhancedImage[row].push_back(DarkCell);
			}
		}
	}
	image = enhancedImage;

	// Find out whether our new infinity is light or dark, and apply the new infinity buffers.
	char currentOuterCell = outsideIsLight ? LightCell : DarkCell;
	std::string infinityIndex = std::string(9, currentOuterCell);
	outsideIsLight = enhancementAlgorithm.CellIsLight(infinityIndex);
	ApplyInfinityBuffers();
}

Image::Image::Image(std::deque<std::string> unenhancedImage, EnhancementAlgorithm algorithm, bool outsideBeginsLight)
{
	image = unenhancedImage;
	enhancementAlgorithm = algorithm;
	outsideIsLight = outsideBeginsLight;
	ApplyInfinityBuffers();
}

void Image::Image::ApplyInfinityBuffers()
{
	// We need to create an image that accounts for the infiniteness around the
	// edges of the image, by a three character buffer in all directions, and then
	// explicitly recording whether the cells outside the bounds of the image
	// are light or dark.
	std::string bufferRow;
	std::string edgeBuffer;
	if (outsideIsLight)
	{
		edgeBuffer = std::string(3, LightCell);
		bufferRow = std::string(image[0].size() + 6, LightCell);
	}
	else
	{
		edgeBuffer = std::string(3, DarkCell);
		bufferRow = std::string(image[0].size() + 6, DarkCell);
	}
	for (std::string &imageRow : image)
	{
		imageRow = edgeBuffer + imageRow + edgeBuffer;
	}
	for (int ii = 0; ii < 3; ii++)
	{
		image.push_front(bufferRow);
		image.push_back(bufferRow);
	}
}

void Image::Image::UndergoRoundsOfEnhancement(int numberOfRounds)
{
	for (int ii = 0; ii < numberOfRounds; ii++)
	{
		UndergoSingleRoundOfEnhancement();
	}
}

int Image::Image::NumberOfLightCells()
{
	if (outsideIsLight) { return -1; }

	int numberOfLightCells{ 0 };
	for (int row = 0; row < image.size(); row++)
	{
		for (int column = 0; column < image[0].size(); column++)
		{
			if (image[row][column] == LightCell) { numberOfLightCells++; }
		}
	}
	return numberOfLightCells;
}
