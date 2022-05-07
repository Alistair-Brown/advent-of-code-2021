#pragma once
#include <vector>
#include "grid_utils.h"

namespace HeightMap
{
	class HeightMap
	{
	private:
		struct Coordinate
		{
			unsigned int xPos, yPos;
			Coordinate(unsigned int xPos, unsigned int yPos) : xPos{ xPos }, yPos{ yPos }{};
			bool operator==(Coordinate const& otherCoord) const
			{
				if ((otherCoord.xPos == xPos) && (otherCoord.yPos == yPos)) { return true; }
				else { return false; }
			}
		};

		GridUtils::Grid<int> heightMap;
		std::vector<Coordinate> lowPoints{};
		bool LocationIsLowPoint(GridUtils::Coordinate const& location) const;
		int GetBasinSize(Coordinate coordinate) const;
	public:
		HeightMap(std::vector<std::vector<int>> heightMap) : heightMap{ heightMap } {};
		int SumRiskLevels();
		int ProductOfLargestBasins() const;
	};
}