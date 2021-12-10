#include "Heightmap.h"
#include "Wrangling.h"
#include <unordered_set>

bool HeightMap::HeightMap::LocationIsLowPoint(unsigned int xPos, unsigned int yPos)
{
	bool northHigher, southHigher, eastHigher, westHigher;
	int currentHeight = heightMap[yPos][xPos];
	northHigher = (yPos == (heightMap.size() - 1)) ?
		true : heightMap[yPos + 1][xPos] > currentHeight;
	southHigher = (yPos == 0) ?
		true : heightMap[yPos - 1][xPos] > currentHeight;
	eastHigher = (xPos == (heightMap[0].size() - 1)) ?
		true : heightMap[yPos][xPos + 1] > currentHeight;
	westHigher = (xPos == 0) ?
		true : heightMap[yPos][xPos - 1] > currentHeight;

	if (northHigher && southHigher && eastHigher && westHigher)
	{
		return true;
	}
	else
	{
		return false;
	}
}

// Could really do with some refactoring, but this function is effectively doing a
// breadth-first-search-esque expansion in every direction, finding how many tiles
// it comes across before being totally walled in by edges or 9-value tiles.
int HeightMap::HeightMap::GetBasinSize(Coordinate coordinate)
{
	std::unordered_set<Coordinate, Coordinate::CoordinateHash> exploredPositions{};
	std::vector<Coordinate> positionsToExploreFrom{ coordinate };
	
	while (positionsToExploreFrom.size() > 0)
	{
		Coordinate positionToExploreFrom = positionsToExploreFrom.back();
		positionsToExploreFrom.pop_back();

		if (!((positionToExploreFrom.xPos == (heightMap[0].size() - 1)) ||
			(heightMap[positionToExploreFrom.yPos][positionToExploreFrom.xPos + 1] == 9)))
		{
			if (exploredPositions.find({ positionToExploreFrom.xPos + 1, positionToExploreFrom.yPos }) == exploredPositions.end())
			{
				positionsToExploreFrom.push_back({ positionToExploreFrom.xPos + 1, positionToExploreFrom.yPos });
			}
		}
		if (!((positionToExploreFrom.xPos == 0) ||
			(heightMap[positionToExploreFrom.yPos][positionToExploreFrom.xPos - 1] == 9)))
		{
			if (exploredPositions.find({ positionToExploreFrom.xPos - 1, positionToExploreFrom.yPos }) == exploredPositions.end())
			{
				positionsToExploreFrom.push_back({ positionToExploreFrom.xPos - 1, positionToExploreFrom.yPos });
			}
		}
		if (!((positionToExploreFrom.yPos == (heightMap.size() - 1)) ||
			(heightMap[positionToExploreFrom.yPos + 1][positionToExploreFrom.xPos] == 9)))
		{
			if (exploredPositions.find({ positionToExploreFrom.xPos, positionToExploreFrom.yPos + 1 }) == exploredPositions.end())
			{
				positionsToExploreFrom.push_back({ positionToExploreFrom.xPos, positionToExploreFrom.yPos + 1});
			}
		}
		if (!((positionToExploreFrom.yPos == 0) ||
			(heightMap[positionToExploreFrom.yPos - 1][positionToExploreFrom.xPos] == 9)))
		{
			if (exploredPositions.find({ positionToExploreFrom.xPos, positionToExploreFrom.yPos - 1 }) == exploredPositions.end())
			{
				positionsToExploreFrom.push_back({ positionToExploreFrom.xPos, positionToExploreFrom.yPos - 1 });
			}
		}

		exploredPositions.insert(positionToExploreFrom);
	}
	return exploredPositions.size();
}

int HeightMap::HeightMap::SumRiskLevels()
{
	int sumOfRiskLevels{ 0 };
	
	for (unsigned int yy = 0; yy < heightMap.size(); yy++)
	{
		for (unsigned int xx = 0; xx < heightMap[0].size(); xx++)
		{
			if (LocationIsLowPoint(xx, yy))
			{
				sumOfRiskLevels += heightMap[yy][xx] + 1;
				lowPoints.push_back({ xx, yy });
			}
		}
	}
	return sumOfRiskLevels;
}

int HeightMap::HeightMap::ProductOfLargestBasins()
{
	std::list<int> basinSizes{};
	for (Coordinate lowPoint : lowPoints)
	{
		int basinSize = GetBasinSize(lowPoint);
		Wrangling::InsertIntoOrderedList(basinSizes, basinSize);
	}

	int productOfTopThree{ 1 };
	std::list<int>::reverse_iterator iter = basinSizes.rbegin();
	for (int ii = 0; ii < 3; ii++)
	{
		productOfTopThree *= *iter;
		iter++;
	}
	return productOfTopThree;
}
