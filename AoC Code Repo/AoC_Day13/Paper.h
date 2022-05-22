#pragma once
#include <vector>
#include <list>
#include <string>
#include "grid_utils.h"
#include <cassert>

namespace Paper
{
	// Represents a piece of tracing paper with a number of dots made on it at positions
	// defined by integer coordinates. This virtual piece of paper can be folder up to create
	// an effective smaller piece of paper where dots in the folded region are now effectively
	// dots at a new location, because, well, that's how tracing paper works.
	// This class lets you make dots on a sheet of tracing paper, perform a number
	// of folds, and then print out the resulting pattern of dots that someone viewing
	// the tracing paper would now see.
	class DottedPaper
	{
	private:
		std::vector<std::vector<bool>> gridOfDots{};
		std::list<GridUtils::Coordinate> dotLocations{};
		void MaybeResizeGrid(GridUtils::Coordinate dotThatMustFit);
		void ShrinkGridToWidth(int newWidth);
		void ShrinkGridToHeight(int newHeight);
	public:
		struct FoldLine;
		void MakeDot(GridUtils::Coordinate dotCoord);
		void MakeFold(FoldLine foldLine);
		unsigned int NumberOfDots();
		std::string FoldedPaperView();
	};

	// Describes a line to fold the paper along (e.g. the line x=5). It is up the the user of the
	// struct to call CheckValidity() before attempting to use this structure to carry out a fold,
	// to check that the line is along a valid axis, and has a real position (the entire paper
	// exists in the positive quadrant of 2D space, so a fold along a 0-position line is not
	// possible).
	struct DottedPaper::FoldLine
	{
		char axis;
		unsigned int position;

		void CheckValidity()
		{
			assert((axis == 'x') || (axis == 'y'));
			assert(position > 0);
		};
	};
}