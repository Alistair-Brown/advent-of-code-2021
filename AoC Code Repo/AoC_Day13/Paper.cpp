#include "Paper.h"
#include <cassert>
#include <iterator>
#include <regex>
#include "Parsing.h"

// Make a dot on the tracing paper. This will only result in a meaningful change to the
// viewed paper if there is a not already a dot in that position.
// We may need to resize the grid to make room for this new dot.
void Paper::DottedPaper::MakeDot(GridUtils::Coordinate dotCoord)
{
	MaybeResizeGrid(dotCoord);

	if (!gridOfDots[dotCoord.yPos][dotCoord.xPos])
	{
		dotLocations.push_back(dotCoord);
		gridOfDots[dotCoord.yPos][dotCoord.xPos] = true;
	}
}

// Fold this virtual piece of tracing paper along a given line. The puzzle guarantees that
// all folds will either be a fold to the left, or an upwards fold (i.e. the origin coordinate
// will never move, all folds are made towards it).
// After this fold, any dots that were in the region being folded will no longer exists
// at their old coordinates, but will have been translated to new positions, giving us a 
// new, smaller, pattern of dots.
void Paper::DottedPaper::MakeFold(FoldLine foldLine)
{
	foldLine.CheckValidity();

	// First shrink down the grid to remove the regions that will no longer exist under the
	// smaller, folded piece of paper. We still have a list of the dots that existed in
	// those regions, so we don't lose any information by doing this.
	(foldLine.axis == 'x') ? 
		ShrinkGridToWidth(foldLine.position) : ShrinkGridToHeight(foldLine.position);

	// For each coordinate which is currently 'dotted', check to see whether that dot has
	// been folded to a new location.
	for (std::list<GridUtils::Coordinate>::iterator coordIter = dotLocations.begin();
		coordIter != dotLocations.end();)
	{
		// The puzzle promised us that there would never be a dot actually ON the line being
		// folded.
		(foldLine.axis == 'x') ?
			assert(coordIter->xPos != foldLine.position) : assert(coordIter->yPos != foldLine.position);

		// If the dot was in the region we're folding, calculated the coordinate it has been
		// folded on to. Otherwise just move our iterator on to the next dot and leave this one be.
		GridUtils::Coordinate newDot;
		if ((foldLine.axis == 'x') && (coordIter->xPos > foldLine.position))
		{
			newDot = { (coordIter->xPos - ((coordIter->xPos - foldLine.position) * 2)), coordIter->yPos };
		}
		else if ((foldLine.axis == 'y') && (coordIter->yPos > foldLine.position))
		{
			newDot = { coordIter->xPos, coordIter->yPos - ((coordIter->yPos - foldLine.position) * 2) };
		}
		else
		{
			coordIter++;
			continue;
		}

		// If we've got this far, our old dot has been folded onto a new location. If there wasn't
		// already a dot at that coordinate, then update our existing dot with its new coordinates
		// and marked it as dotted on the grid. If there WAS already a dot here, then this new
		// dot does nothing to change that status, and so can just be deleted (noting that
		// deleting a list element returns an iterator to the next element).
		if (!gridOfDots[newDot.yPos][newDot.xPos])
		{
			*coordIter = newDot;
			gridOfDots[newDot.yPos][newDot.xPos] = true;
			coordIter++;
		}
		else
		{
			coordIter = dotLocations.erase(coordIter);
		}
	}
}

// Given the coordinate of a new dot we are about to make on the tracing paper, resize the
// grid if it is not currently large enough in either direction to accomodate this dot's
// position.
// Remember vectors being 0-indexed means the number of rows required in a given dimension
// will be 1-greater than the maximum coordinate in that dimension.
void Paper::DottedPaper::MaybeResizeGrid(GridUtils::Coordinate dotThatMustFit)
{
	if (gridOfDots.size() <= dotThatMustFit.yPos)
	{
		gridOfDots.resize(dotThatMustFit.yPos + 1);
	}

	int newWidth = (gridOfDots[0].size() <= (dotThatMustFit.xPos + 1)) ?
		(dotThatMustFit.xPos + 1) : gridOfDots[0].size();
	for (unsigned int rowNum = 0; rowNum < gridOfDots.size(); rowNum++)
	{
		gridOfDots[rowNum].resize(newWidth);
	}
}

// After a fold, resize the grid to contain only the cells required by the new width or
// height of the grid.
void Paper::DottedPaper::ShrinkGridToWidth(int newWidth)
{
	for (unsigned int rowNum = 0; rowNum < gridOfDots.size(); rowNum++)
	{
		gridOfDots[rowNum].resize(newWidth);
	}
}
void Paper::DottedPaper::ShrinkGridToHeight(int newHeight)
{
	gridOfDots.resize(newHeight);
}

// Returns the number of dots currently visible on the tracing paper.
unsigned int Paper::DottedPaper::NumberOfDots()
{
	return dotLocations.size();
}

// Prints out what a viewer of the tracing paper would currently see, using the '#' symbol
// for dots, and whitespace for empty space. The returned string begins with a new line, to
// ensure that the first line of the paper is correctly aligned with those below it.
std::string Paper::DottedPaper::FoldedPaperView()
{
	std::string output{};
	for (std::vector<bool> row : gridOfDots)
	{
		output += '\n';
		for (bool isDot : row)
		{
			if (isDot)
			{
				output += "#";
			}
			else
			{
				output += " ";
			}
		}
	}
	return output;
}
