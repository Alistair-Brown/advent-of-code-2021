#include "Heightmap.h"
#include "Wrangling.h"
#include <unordered_set>

bool HeightMap::HeightMap::LocationIsLowPoint(GridUtils::Coordinate const& location) const
{
	bool northHigher, southHigher, eastHigher, westHigher;
	GridUtils::Grid<int>::GridCell locationCell = heightMap[location];
	int currentHeight = locationCell.value;
	northHigher = locationCell.IsTopRow() ?
		true : locationCell.Up().value > currentHeight;
	southHigher = (locationCell.IsBottomRow()) ?
		true : locationCell.Down().value > currentHeight;
	eastHigher = locationCell.IsRightColumn() ?
		true : locationCell.Right().value > currentHeight;
	westHigher = locationCell.IsLeftColumn() ?
		true : locationCell.Left().value > currentHeight;

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
int HeightMap::HeightMap::GetBasinSize(Coordinate coordinate) const
{
	auto coordHash = [](Coordinate const & coord) -> size_t { return coord.xPos; };
	std::unordered_set<Coordinate, decltype(coordHash)> exploredPositions{ 0, coordHash };
	std::vector<Coordinate> positionsToExploreFrom{ coordinate };
	
	while (positionsToExploreFrom.size() > 0)
	{
		Coordinate positionToExploreFrom = positionsToExploreFrom.back();
		positionsToExploreFrom.pop_back();

		if (!((positionToExploreFrom.xPos == (heightMap.Width() - 1)) ||
			(heightMap[GridUtils::Coordinate{ positionToExploreFrom.xPos + 1, positionToExploreFrom.yPos }].value == 9)))
		{
			if (exploredPositions.find({ positionToExploreFrom.xPos + 1, positionToExploreFrom.yPos }) == exploredPositions.end())
			{
				positionsToExploreFrom.push_back({ positionToExploreFrom.xPos + 1, positionToExploreFrom.yPos });
			}
		}
		if (!((positionToExploreFrom.xPos == 0) ||
			(heightMap[GridUtils::Coordinate{ positionToExploreFrom.xPos - 1, positionToExploreFrom.yPos }].value == 9)))
		{
			if (exploredPositions.find({ positionToExploreFrom.xPos - 1, positionToExploreFrom.yPos }) == exploredPositions.end())
			{
				positionsToExploreFrom.push_back({ positionToExploreFrom.xPos - 1, positionToExploreFrom.yPos });
			}
		}
		if (!((positionToExploreFrom.yPos == (heightMap.Height() -1)) ||
			(heightMap[GridUtils::Coordinate{ positionToExploreFrom.xPos, positionToExploreFrom.yPos + 1 }].value == 9)))
		{
			if (exploredPositions.find({ positionToExploreFrom.xPos, positionToExploreFrom.yPos + 1 }) == exploredPositions.end())
			{
				positionsToExploreFrom.push_back({ positionToExploreFrom.xPos, positionToExploreFrom.yPos + 1});
			}
		}
		if (!((positionToExploreFrom.yPos == 0) ||
			(heightMap[GridUtils::Coordinate{ positionToExploreFrom.xPos, positionToExploreFrom.yPos - 1 }].value == 9)))
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
	
	for (unsigned int yy = 0; yy < heightMap.Height(); yy++)
	{
		for (unsigned int xx = 0; xx < heightMap.Width(); xx++)
		{
			if (LocationIsLowPoint(GridUtils::Coordinate{ xx, yy }))
			{
				sumOfRiskLevels += heightMap[GridUtils::Coordinate{ xx,yy }].value + 1;
				lowPoints.push_back({ xx, yy });
			}
		}
	}
	return sumOfRiskLevels;
}

int HeightMap::HeightMap::ProductOfLargestBasins() const
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
