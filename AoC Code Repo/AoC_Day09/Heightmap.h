#pragma once
#include <vector>
#include "grid_utils.h"

namespace HeightMap
{
	// Inteprets a grid of integers as if each int represents the height at a given point
	// over an area.
	// From this 'height map', we can determine where low points and low regions (basins)
	// lie.
	class HeightMap
	{
	private:
		// My custom GridUtils library is used to make working with a grid of integers
		// easier.
		GridUtils::Grid<int> heightMap;

		// The key features of the height map are low points (defined as points which are
		// lower than all of their adjacent points) and basins (regions of the map which are
		// completed bounded by a mix of height-9 points and the edge of the map).
		std::vector<GridUtils::Grid<int>::GridCell> lowPoints{};
		bool LocationIsLowPoint(GridUtils::Grid<int>::GridCell location) const;
		int GetBasinSize(GridUtils::Grid<int>::GridCell coordinate) const;
	public:
		HeightMap(std::vector<std::vector<int>> heightMap) : heightMap{ heightMap } {};
		int SumLowPointRiskLevels();
		int ProductOfLargestBasins() const;
	};
}