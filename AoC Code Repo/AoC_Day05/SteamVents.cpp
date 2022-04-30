#include "SteamVents.h"
#include <regex>
#include <cassert>
#include "Parsing.h"

// Potentially swap the start and finish coordinates to ensure that horizontal lines
// run left to right and vertical lines run low to high.
void Steam::CoordinatePath::OrientPath()
{
	if ((xStart == xFinish) || (yStart == yFinish))
	{
		if (xStart > xFinish)
		{
			int tempX = xStart;
			xStart = xFinish;
			xFinish = tempX;
		}
		else if (yStart > yFinish)
		{
			int tempY = yStart;
			yStart = yFinish;
			yFinish = tempY;
		}
	}
}

// Just a couple of simple helpers to get the current width or height of the
// warning map.
unsigned int Steam::WarningMap::mapWidth()
{
	if (map.size() == 0)
	{
		return 0;
	}
	else
	{
		return map[0].size();
	}
}
unsigned int Steam::WarningMap::mapHeight() { return map.size(); }

// Insert a horizontal or vertical line into the WarningMap, incrementing the map's count
// of overlaps wherever this causes 2 lines to cross the same cell for the first time.
void Steam::WarningMap::InsertWarningPath(CoordinatePath path)
{
	// This function can only deal with horiztontal or vertical lines, and ensures that
	// they run from left to right or low to high.
	assert(!path.IsDiagonalLine());
	path.OrientPath();

	// If the map isn't already big enough for the coordinates covered by this path, make
	// it so.
	MaybeExtendMapSize(path.xFinish + 1, path.yFinish + 1);

	// Work along each coordinate on the line, incrementing the count of number of lines which
	// have touched each position in the map as we go. Whenever that increment brings the
	// number of lines that have touched a coordinate to 2, that constitutes the first
	// time an overlap has occured on that position, so increment the total number of overlaps.
	if (path.IsHorizontalLine())
	{
		int rowNumber = path.yStart;
		for (int ii = path.xStart; ii <= path.xFinish; ii++)
		{
			map[rowNumber][ii]++;
			if (map[rowNumber][ii] == 2) { numberOfOverlaps++; }
		}
	}
	else if (path.IsVerticalLine())
	{
		int columnNumber = path.xStart;
		for (int ii = path.yStart; ii <= path.yFinish; ii++)
		{
			map[ii][columnNumber]++;
			if (map[ii][columnNumber] == 2) { numberOfOverlaps++; }
		}
	}
}

// Insert a diagonal line into the WarningMap, incrementing the map's count
// of overlaps wherever this causes 2 lines to cross the same cell for the first time.
void Steam::WarningMap::InsertDiagonalWarningPath(CoordinatePath path)
{
	// If the map isn't already big enough for the coordinates covered by this path, make
	// it so.
	MaybeExtendMapSize(
		path.xFinish > path.xStart ? path.xFinish + 1 : path.xStart + 1,
		path.yFinish > path.yStart ? path.yFinish + 1 : path.yStart + 1);

	// Lines are 45 degrees. Each step along the line will involve either a
	// step up or down in each direction.
	int xStep = (path.xFinish > path.xStart) ? 1 : -1;
	int yStep = (path.yFinish > path.yStart) ? 1 : -1;
	int lineLength = abs(path.xFinish - path.xStart);

	// Step along the line from its start, using the steps we just calculated, until we've
	// made as many steps as required to finish the line. Increment the count of number
	// of lines which have touched each position in the map as we go. Whenever that increment
	// brings the number of lines that have touched a coordinate to 2, that constitutes the first
	// time an overlap has occured on that position, so increment the total number of overlaps.
	for (int ii = 0; ii <= lineLength; ii++)
	{
		map[path.yStart + (yStep * ii)][path.xStart + (xStep * ii)]++;
		if (map[path.yStart + (yStep * ii)][path.xStart + (xStep * ii)] == 2)
		{
			numberOfOverlaps++;
		}
	}
}

// Given a required width and height, extend the map in either or both of those
// directions to ensure it can provide that width and height if it doesn't already.
// It would probably be more efficient to work out how big a map we need in the
// first place, but this doesn't seem to be costing us too much, so I'm happy enough
// for now.
void Steam::WarningMap::MaybeExtendMapSize(unsigned int width, unsigned int height)
{
	if (height > mapHeight())
	{
		map.resize(height);
	}

	for (std::vector<int> &row : map)
	{
		if (width > row.size())
		{
			row.resize(width, 0);
		}
	}
}
