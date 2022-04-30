#pragma once
#include <string>
#include <vector>

namespace Steam
{
	// The coordinate path just bundles up the coordinates describing a line between
	// two points, with a couple of helper function.
	struct CoordinatePath
	{
		int xStart{ 0 };
		int yStart{ 0 };
		int xFinish{ 0 };
		int yFinish{ 0 };

		void OrientPath();
		bool IsHorizontalLine() { return (yStart == yFinish); }
		bool IsVerticalLine() { return (xStart == xFinish); }
		bool IsDiagonalLine() { return !(IsHorizontalLine() || IsVerticalLine()); }
	};

	// The warning map is a growable map of coordinates, into which lines between given
	// coordinates can be inserted. The map will keep track of the number of coordinates
	// which have at least 2 lines overlapping on them, which can be queried.
	class WarningMap
	{
	private:
		std::vector<std::vector<int>> map;
		unsigned int mapWidth();
		unsigned int mapHeight();
		unsigned int numberOfOverlaps{ 0 };
	public:
		void InsertWarningPath(CoordinatePath path);
		void InsertDiagonalWarningPath(CoordinatePath path);
		unsigned int NumberOfOverlapCells() { return numberOfOverlaps; }
		void MaybeExtendMapSize(unsigned int width, unsigned int height);
	};
}