#include "Paper.h"
#include <cassert>
#include <iterator>

void Paper::DottedPaper::MaybeResizeGrid(Coordinate dotThatMustFit)
{
	if ((gridOfDots.size() == 0) || (gridOfDots.size() <= dotThatMustFit.yPos))
	{
		gridOfDots.resize(dotThatMustFit.yPos + 1);
	}

	int newWidth = (gridOfDots[0].size() <= (dotThatMustFit.xPos + 1)) ?
		(dotThatMustFit.xPos + 1) : gridOfDots[0].size();
	for (int rowNum = 0; rowNum < gridOfDots.size(); rowNum++)
	{
		gridOfDots[rowNum].resize(newWidth);
	}
}

void Paper::DottedPaper::ShrinkGridToWidth(int newWidth)
{
	for (int rowNum = 0; rowNum < gridOfDots.size(); rowNum++)
	{
		gridOfDots[rowNum].resize(newWidth);
	}
}

void Paper::DottedPaper::InsertDotOnGrid(Coordinate dotCoord)
{
	if (!gridOfDots[dotCoord.yPos][dotCoord.xPos])
	{
		dotLocations.push_back(dotCoord);
		gridOfDots[dotCoord.yPos][dotCoord.xPos] = true;
	}	
}

void Paper::DottedPaper::MakeDot(Coordinate dotCoord)
{
	MaybeResizeGrid(dotCoord);
	InsertDotOnGrid(dotCoord);
}

// Coordinate should be given where only one of the two axis is non zero, and represents
// the line to fold along.
void Paper::DottedPaper::MakeFold(Coordinate foldLine)
{
	// One, and only one, of the coordinate axes should be non-zero
	assert((foldLine.xPos == 0) != (foldLine.yPos == 0));

	// I'll just get the x fold working for now, since that's the first puzzle.
	if (foldLine.xPos > 0)
	{
		int xFoldLine = foldLine.xPos;
		std::list<Coordinate> newlyFoldedDots{};
		ShrinkGridToWidth(xFoldLine);

		for (std::list<Coordinate>::iterator coordIter = dotLocations.begin();
			 coordIter != dotLocations.end();)
		{
			assert(coordIter->xPos != xFoldLine);

			if (coordIter->xPos > xFoldLine)
			{
				Coordinate newDot{ (coordIter->xPos - ((coordIter->xPos - xFoldLine) * 2)), coordIter->yPos };
				if (!gridOfDots[newDot.yPos][newDot.xPos])
				{
					newlyFoldedDots.push_back(newDot);
					gridOfDots[newDot.yPos][newDot.xPos] = true;
				}

				coordIter = dotLocations.erase(coordIter);
			}
			else
			{
				coordIter++;
			}
		}

		for (std::list<Coordinate>::iterator coordIter = newlyFoldedDots.begin();
			coordIter != newlyFoldedDots.end();
			coordIter++)
		{
			dotLocations.insert(dotLocations.end(), *coordIter);
		}
	}
}

unsigned int Paper::DottedPaper::NumberOfDots()
{
	return dotLocations.size();
}
