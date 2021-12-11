#pragma once

namespace GridUtils
{
	struct Coordinate
	{
		int x;
		int y;
		int gridWidth;
		int gridHeight;
		Coordinate(int xIn, int yIn, int width, int height) : 
			x{ xIn }, y{ yIn }, gridWidth{ width }, gridHeight{ height }{};

		Coordinate Up() { return Coordinate(x, y + 1, gridWidth, gridHeight); }
		Coordinate Down() { return Coordinate(x, y - 1, gridWidth, gridHeight); }
		Coordinate Right() { return Coordinate(x + 1, y, gridWidth, gridHeight); }
		Coordinate Left() { return Coordinate(x - 1, y, gridWidth, gridHeight); }
		Coordinate UpRight() { return Coordinate(x + 1, y + 1, gridWidth, gridHeight); }
		Coordinate DownRight() { return Coordinate(x + 1, y - 1, gridWidth, gridHeight); }
		Coordinate UpLeft() { return Coordinate(x - 1, y + 1, gridWidth, gridHeight); }
		Coordinate DownLeft() { return Coordinate(x - 1, y - 1, gridWidth, gridHeight); }

		// These functions assume a grid with origin of 0,0
		bool IsTopRow() { return (y == (gridHeight - 1)); }
		bool IsBottomRow() { return (y == 0); }
		bool IsRightColumn() { return (x == (gridWidth - 1)); }
		bool IsLeftColumn() { return (x == 0); }
	};
}