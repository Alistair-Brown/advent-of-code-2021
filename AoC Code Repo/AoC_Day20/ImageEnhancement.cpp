#include "ImageEnhancement.h"
#include "Parsing.h"
#include <cassert>

// The next value of a cell in the image after an enhancement step is formed by taking
// the square formed around that cell from its adjacent cells (including diagonal) and
// then reading the cells in that square from left to right and top to bottom to produce
// a 9 character string like "..#.##.#.". This string can be interpreted as a binary number
// where '#' (the light cells) are 1 and '.' (the dark cells) are 0. We then take the
// value of the character in the enhancement string indexed by that number as the new
// value of the cell.
char Image::EnhancementAlgorithm::NextValueOfCell(
	unsigned int row,
	unsigned int column,
	std::deque<std::string> const& image)
{
	// I do have a function in my Parsing library for going from binary to
	// int in a more general fashion, but this function is the golden path for
	// performance of this puzzle, so instead we will read the characters directly
	// from the grid rather than forming an intermediate string to convert to
	// actual binary.
	unsigned int intIndex{ 0 };
	if (image[row - 1][column - 1] == LightCell) { intIndex += 256; }
	if (image[row - 1][column] == LightCell) { intIndex += 128; }
	if (image[row - 1][column + 1] == LightCell) { intIndex += 64; }
	if (image[row][column - 1] == LightCell) { intIndex += 32; }
	if (image[row][column] == LightCell) { intIndex += 16; }
	if (image[row][column + 1] == LightCell) { intIndex += 8; }
	if (image[row + 1][column - 1] == LightCell) { intIndex += 4; }
	if (image[row + 1][column] == LightCell) { intIndex += 2; }
	if (image[row + 1][column + 1] == LightCell) { intIndex += 1; }

	return(algorithmString[intIndex]);
}

// Initialise the Image.
// According to the rules of the puzzle, all images are in fact infinite, with the region
// outside that we're actually interested in being initially composed of entirely dark
// cells.
// To represent this infinite region of darkness, we must:
//  - Track whether the infinite region beyond the image is currently light or dark (initially dark)
//  - Apply a buffer around our image, representing the small portion of the infinite that will
//    actually interact with our image during the enhancement process (i.e. the region which
//    contain the adjacent cells for the outer edge of the image).
Image::Image::Image(std::deque<std::string> unenhancedImage, EnhancementAlgorithm algorithm)
{
	image = unenhancedImage;
	enhancementAlgorithm = algorithm;
	outsideIsLight = false;
	ApplyInfinityBuffers();
}

// When applying the enhancement algorithm, the infinite region around the image
// will itself be enhanced. For the most part this is trivial, since every cell
// in the infinite region has identical cells for neighbours and so the entire region
// can be found from the same point on the enhancement algorithm. The nearest 2 'layers'
// of the infinite region to the image itself however will have their enhanced value
// affected by the values of the cells in the image. The best way to account for this
// is to simply add a 2-cell buffer around the image in every direction, so we can
// include those cells in each enhancement step automatically.
// The value of the cells to insert in those buffer regions depends on whether the
// region outside the image is currently light or dark.
void Image::Image::ApplyInfinityBuffers()
{
	std::string bufferRow;
	std::string edgeBuffer;

	if (outsideIsLight)
	{
		edgeBuffer = std::string(2, LightCell);
		bufferRow = std::string(image[0].size() + 4, LightCell);
	}
	else
	{
		edgeBuffer = std::string(2, DarkCell);
		bufferRow = std::string(image[0].size() + 4, DarkCell);
	}

	for (std::string &imageRow : image)
	{
		imageRow = edgeBuffer + imageRow + edgeBuffer;
	}
	for (int ii = 0; ii < 2; ii++)
	{
		image.push_front(bufferRow);
		image.push_back(bufferRow);
	}
}

void Image::Image::UndergoRoundsOfEnhancement(unsigned int numberOfRounds)
{
	for (unsigned int ii = 0; ii < numberOfRounds; ii++)
	{
		UndergoSingleRoundOfEnhancement();
	}
}

// When the image is enhanced, every cell is enhanced simultaneously, so the new value of
// each cell is only influenced by the existing values of its neighbours, not their newly
// enhanced values. This function handles this by constructing a new enhanced image and
// then simply reassigning the image member to use this new image once enhancement is
// complete.
void Image::Image::UndergoSingleRoundOfEnhancement()
{
	std::deque<std::string> enhancedImage{};

	// Infinity buffers should already have been applied to provide the portions of
	// the infite region that we need to take into account during enhancement.
	// Find the values of each cell in the enhanced image by looping around the entire
	// image apart from the very outer layer (those cells are too far from the image to
	// be influenced, and were only included in the infinity buffers so that they could
	// be used as the adjacent cells for the next layer in).
	for (unsigned int row = 0; row < image.size() - 2; row++)
	{
		enhancedImage.push_back("");
		for (unsigned int column = 0; column < image[0].size() - 2; column++)
		{
			enhancedImage[row] += enhancementAlgorithm.NextValueOfCell(row + 1, column + 1, image);
		}
	}
	image = enhancedImage;

	// Before reapplying infinity buffers to the enhanced image, determine the new value
	// of the infinite region. If the infinite region is currently dark, then every cell within
	// it should be enhanced to the position in the enhancement algorithm represented by
	// "........." (0), but if the infinite region is currently dark, they should be enhanced
	// to the position represented by "#########" (511).
	unsigned int positionToCheck = outsideIsLight ? 511 : 0;
	outsideIsLight = enhancementAlgorithm.CellAtPosition(positionToCheck) == LightCell;
	ApplyInfinityBuffers();
}

// Find the number of cells in the image that are currently light (i.e. '#').
// It is only valid to request the number of light cells when the region outside the
// the main image is dark, otherwise we have light cells stretching into infinity.
int Image::Image::NumberOfLightCells()
{
	assert(!outsideIsLight);

	int numberOfLightCells{ 0 };
	for (std::string const& imageLine : image)
	{
		for (char const& cell : imageLine)
		{
			if (cell == LightCell) { numberOfLightCells++; }
		}
	}

	return numberOfLightCells;
}
