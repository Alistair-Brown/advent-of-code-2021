#include "SteamVents.h"
#include <regex>
#include <cassert>
#include "Parsing.h"

Steam::WarningPath::WarningPath(std::string pathAsString)
{
	std::regex matchString{ "([0-9]+),([0-9]+) -> ([0-9]+),([0-9]+)" };
	std::smatch matches;

	std::regex_search(pathAsString, matches, matchString);
	assert(matches.size() == 5);

	int xStart = Parsing::ConvertStringToInt(std::string(matches[1]));
	int yStart = Parsing::ConvertStringToInt(std::string(matches[2]));
	int xFinish = Parsing::ConvertStringToInt(std::string(matches[3]));
	int yFinish = Parsing::ConvertStringToInt(std::string(matches[4]));
	
	// The WarningMap makes a lazy assumption that all straight lines
	// run from low to high in whatever direction. That won't necessarily
	// work for diagonals though, so only do that change if we're dealing
	// with a straight line.
	if ((xStart == xFinish) || (yStart == yFinish))
	{
		path = CoordinatePath{
			((xStart > xFinish) ? xFinish : xStart),
			((yStart > yFinish) ? yFinish : yStart),
			((xStart > xFinish) ? xStart : xFinish),
			((yStart > yFinish) ? yStart : yFinish) };
	}
	else
	{
		path = CoordinatePath{ xStart, yStart, xFinish, yFinish };
	}
}

int Steam::WarningMap::mapWidth()
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
int Steam::WarningMap::mapHeight() { return map.size(); }

void Steam::WarningMap::InsertWarningPath(WarningPath path)
{
	assert(path.IsStraightLine());
	WarningPath::CoordinatePath rawPath = path.GetCoordinatePath();
	MaybeExtendMapSize(rawPath.xFinish + 1, rawPath.yFinish + 1);

	if (path.IsHorizontalLine())
	{
		std::vector<int> &row = map[rawPath.yStart];
		for (int ii = rawPath.xStart; ii <= rawPath.xFinish; ii++)
		{
			row[ii]++;
		}
	}
	else if (path.IsVerticalLine())
	{
		int columnNumber = rawPath.xStart;
		for (int ii = rawPath.yStart; ii <= rawPath.yFinish; ii++)
		{
			map[ii][columnNumber]++;
		}
	}
}

void Steam::WarningMap::InsertDiagonalWarningPath(WarningPath path)
{
	WarningPath::CoordinatePath rawPath = path.GetCoordinatePath();
	MaybeExtendMapSize(
		rawPath.xFinish > rawPath.xStart ? rawPath.xFinish + 1 : rawPath.xStart + 1,
		rawPath.yFinish > rawPath.yStart ? rawPath.yFinish + 1 : rawPath.yStart + 1);

	// Lines are 45 degrees. Each cell will be either a step up or down
	// in each direction.
	int xStep = (rawPath.xFinish > rawPath.xStart) ? 1 : -1;
	int yStep = (rawPath.yFinish > rawPath.yStart) ? 1 : -1;
	int lineLength = abs(rawPath.xFinish - rawPath.xStart);

	for (int ii = 0; ii <= lineLength; ii++)
	{
		map[rawPath.yStart + (yStep * ii)][rawPath.xStart + (xStep * ii)]++;
	}
}

int Steam::WarningMap::NumberOfOverlapCells()
{
	int numberOfOverlaps{ 0 };
	for (std::vector<int> row : map)
	{
		for (int cell : row)
		{
			if (cell > 1)
			{
				numberOfOverlaps++;
			}
		}
	}
	return numberOfOverlaps;
}

void Steam::WarningMap::MaybeExtendMapSize(int width, int height)
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