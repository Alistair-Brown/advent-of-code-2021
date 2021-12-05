#pragma once
#include <string>
#include <vector>

namespace Steam
{
	class WarningPath
	{
	public:
		struct CoordinatePath
		{
			int xStart;
			int yStart;
			int xFinish;
			int yFinish;
			CoordinatePath(int xStart, int yStart, int xFinish, int yFinish) :
				xStart{ xStart }, yStart{ yStart }, xFinish{ xFinish }, yFinish{ yFinish } {};
			bool IsHorizontalLine() { return (yStart == yFinish); }
			bool IsVerticalLine() { return (xStart == xFinish); }
			bool IsStraightLine() { return (IsHorizontalLine() || IsVerticalLine()); }
		};
		WarningPath(int xStart, int yStart, int xFinish, int yFinish) :
			path{ xStart, yStart, xFinish, yFinish } {};
		WarningPath(std::string pathAsString); // e.g. "1,7 -> 3,7"
		bool IsHorizontalLine() { return path.IsHorizontalLine(); }
		bool IsVerticalLine() { return path.IsVerticalLine(); }
		bool IsStraightLine() { return path.IsStraightLine(); }
		CoordinatePath GetCoordinatePath() { return path; }

	private:
		WarningPath::CoordinatePath path { 0,0,0,0 };
	};

	class WarningMap
	{
	private:
		std::vector<std::vector<int>> map;
		int mapWidth();
		int mapHeight();
	public:
		void InsertWarningPath(WarningPath path);
		void InsertDiagonalWarningPath(WarningPath path);
		int NumberOfOverlapCells();
		void MaybeExtendMapSize(int width, int height);
	};
}