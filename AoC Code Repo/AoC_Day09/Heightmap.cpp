#include "Heightmap.h"
#include <unordered_set>
#include <set>

// A location is a low point if the height of the adjacent cells in every cardinal direction
// is greater than itself.
bool HeightMap::HeightMap::LocationIsLowPoint(GridUtils::Grid<int>::GridCell location) const
{
	bool northHigher, southHigher, eastHigher, westHigher;
	int locationHeight = location.value;
	northHigher = location.IsTopRow() ?
		true : location.Up().value > locationHeight;
	southHigher = (location.IsBottomRow()) ?
		true : location.Down().value > locationHeight;
	eastHigher = location.IsRightColumn() ?
		true : location.Right().value > locationHeight;
	westHigher = location.IsLeftColumn() ?
		true : location.Left().value > locationHeight;

	return (northHigher && southHigher && eastHigher && westHigher) ?
		true : false;
}

// From a given low point, find the size of the basin the low point resides in, where a
// basin includes all cells up to (but not including) a cell with a height of 9, or the
// edge of the map.
// Calculates the basin size using a breadth-first-search-esque expansion from the low
// point.
int HeightMap::HeightMap::GetBasinSize(GridUtils::Grid<int>::GridCell lowPoint) const
{
	auto coordHash = [](GridUtils::Grid<int>::GridCell const & coord) -> size_t { return coord.GetCoordinate().xPos; };
	std::unordered_set<GridUtils::Grid<int>::GridCell, decltype(coordHash)> exploredPositions{ 0, coordHash };
	std::vector<GridUtils::Grid<int>::GridCell> positionsToExploreFrom{ lowPoint };
	
	// We start off with a single position to explore from, our starting low point. On each
	// loop we get the next position to explore from and look in each cardinal direction. If
	// there is a cell in that direction (i.e. not the edge of the map) which is neither height
	// 9 nor already explored, add that cell to the list of positions we still need to explore
	// from. If we do this until there are no unexplored cells left we will have found the size
	// of the basin bounded be map-edge and/or height-9 cells.
	while (positionsToExploreFrom.size() > 0)
	{
		GridUtils::Grid<int>::GridCell positionToExploreFrom = positionsToExploreFrom.back();
		positionsToExploreFrom.pop_back();

		if (!(positionToExploreFrom.IsRightColumn() || (positionToExploreFrom.Right().value == 9)))
		{
			if (exploredPositions.find(positionToExploreFrom.Right()) == exploredPositions.end())
			{
				positionsToExploreFrom.push_back(positionToExploreFrom.Right());
			}
		}
		if (!(positionToExploreFrom.IsLeftColumn() || (positionToExploreFrom.Left().value == 9)))
		{
			if (exploredPositions.find(positionToExploreFrom.Left()) == exploredPositions.end())
			{
				positionsToExploreFrom.push_back(positionToExploreFrom.Left());
			}
		}
		if (!(positionToExploreFrom.IsTopRow() || (positionToExploreFrom.Up().value == 9)))
		{
			if (exploredPositions.find(positionToExploreFrom.Up()) == exploredPositions.end())
			{
				positionsToExploreFrom.push_back(positionToExploreFrom.Up());
			}
		}
		if (!(positionToExploreFrom.IsBottomRow() || (positionToExploreFrom.Down().value == 9)))
		{
			if (exploredPositions.find(positionToExploreFrom.Down()) == exploredPositions.end())
			{
				positionsToExploreFrom.push_back(positionToExploreFrom.Down());
			}
		}

		exploredPositions.insert(positionToExploreFrom);
	}
	return exploredPositions.size();
}

// The 'risk level' of a point is defined as its height plus one.
// Part 1 of the puzzle requires us to sum up the risk levels of all low
// points. This function is non-const in the way it calculates this, since
// it will also cache the list of low points discovered in this way for
// the basin calculations in part 2.
int HeightMap::HeightMap::SumLowPointRiskLevels()
{
	int sumOfRiskLevels{ 0 };
	
	for (auto itr = heightMap.begin(); itr != heightMap.end(); itr++)
	{
		auto location = *itr;
		if (LocationIsLowPoint(location))
		{
			sumOfRiskLevels += location.value + 1;
			lowPoints.push_back(location);
		}
	}
	return sumOfRiskLevels;
}

// For part 2, we need to find the 3 largest basins, and return the product
// of their sizes. For this we use the list of low points calculated by part
// 1, and the knowledge that every basin contains exactly one low point.
int HeightMap::HeightMap::ProductOfLargestBasins() const
{
	// Start by finding the size of every basin. Every basin contains exactly one
	// low point, so we can simply find the basin that each of our low points resides
	// in, and be sure that we will neither miss or double count any basin.
	std::set<unsigned int> basinSizes{};
	for (auto const &lowPoint : lowPoints)
	{
		int basinSize = GetBasinSize(lowPoint);
		basinSizes.insert(basinSize);
	}

	// Our basin sizes were listed in order by the previous step, so just take
	// the back 3 elements of the list.
	int productOfTopThree{ 1 };
	auto iter = basinSizes.rbegin();
	for (int ii = 0; ii < 3; ii++)
	{
		productOfTopThree *= *iter;
		iter++;
	}
	return productOfTopThree;
}
