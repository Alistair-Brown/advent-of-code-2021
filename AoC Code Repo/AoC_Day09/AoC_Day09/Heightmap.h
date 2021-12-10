#pragma once
#include <vector>

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
			class CoordinateHash
			{
			public:
				size_t operator()(const Coordinate &coord) const
				{
					return coord.xPos;
				}
			};
		};
		std::vector<std::vector<int>> heightMap;
		std::vector<Coordinate> lowPoints{};
		bool LocationIsLowPoint(unsigned int xPos, unsigned int yPos);
		int GetBasinSize(Coordinate coordinate);
	public:
		HeightMap(std::vector<std::vector<int>> heightMap) : heightMap{ heightMap } {};
		int SumRiskLevels();
		int ProductOfLargestBasins();
	};
}