#pragma once
#include <vector>
#include <cassert>

// A fair few of the advent of code problems involve working with a grid of values.
// This namespace provides utilities for working with such grids, including a grid
// class that holds a 2D-vector of values and provides methods that let a user
// move around the cells within as well as reading/writing values.
namespace GridUtils
{
	// A location in a 2D grid can be described as an x,y coordinate. Using a dedicated
	// struct for this means that users can just think in terms of such a coordinate rather
	// than worrying about putting the right parts of the coordinate into the right array
	// access operators when reading the grid.
	struct Coordinate
	{
		unsigned int x;
		unsigned int y;
		Coordinate(unsigned int xIn, unsigned int yIn) : x{ xIn }, y{ yIn }{};

		/*
		TODO: Delete these commented out lines once I've refactored all the old puzzles
		that used to use them.
		unsigned int gridWidth;
		unsigned int gridHeight;
		Coordinate(unsigned int xIn, unsigned int yIn, unsigned int width, unsigned int height) :
			x{ xIn }, y{ yIn }, gridWidth{ width }, gridHeight{ height }{};

		bool operator==(Coordinate const& otherCoord) const
		{
			if ((otherCoord.x == x) && (otherCoord.y == y)) { return true; }
			else { return false; }
		}

		// These functions assume they are being used to return a valid coordinate
		// i.e. you must check that a coordinate is not on the top row before
		// trying to get the coordinate above it.
		Coordinate Up() const { return Coordinate(x, y + 1, gridWidth, gridHeight); }
		Coordinate Down() const { return Coordinate(x, y - 1, gridWidth, gridHeight); }
		Coordinate Right() const { return Coordinate(x + 1, y, gridWidth, gridHeight); }
		Coordinate Left() const { return Coordinate(x - 1, y, gridWidth, gridHeight); }
		Coordinate UpRight() const { return Coordinate(x + 1, y + 1, gridWidth, gridHeight); }
		Coordinate DownRight() const { return Coordinate(x + 1, y - 1, gridWidth, gridHeight); }
		Coordinate UpLeft() const { return Coordinate(x - 1, y + 1, gridWidth, gridHeight); }
		Coordinate DownLeft() const { return Coordinate(x - 1, y - 1, gridWidth, gridHeight); }

		// These functions assume a grid with origin of 0,0.
		bool IsTopRow() const { return (y == (gridHeight - 1)); }
		bool IsBottomRow() const { return (y == 0); }
		bool IsRightColumn() const { return (x == (gridWidth - 1)); }
		bool IsLeftColumn() const { return (x == 0); }
		*/
	};

	// A 2D-vector of any required type. This grid does not hold those types directly, but
	// instead holds a 2D-vector of 'grid cells', from the underlying value can be accessed,
	// but which also provide methods for navigating the grid more easily than using
	// increments/decrements on coordinates.
	template<typename T>
	class Grid
	{
	public:
		class GridCell;

	private:
		std::vector<std::vector<GridCell>> grid;

	public:
		Grid(std::vector<std::vector<T>> gridIn);

		GridCell operator[] (Coordinate const &coord) const { return grid[coord.y][coord.x]; }
		GridCell &operator[] (Coordinate const &coord) { return grid[coord.y][coord.x]; }

		unsigned int Height() const { return grid.size(); }
		unsigned int Width() const { return grid[0].size(); }
	};

	// A single cells within a grid of type T. Provides utility functions for accessing
	// adjacent cells, and a publicly read/writable cell value.
	template <typename T>
	class Grid<T>::GridCell
	{
	public:
		T value;

	private:
		const Grid<T> &parentGrid;
		const unsigned int x;
		const unsigned int y;

	public:
		Grid<T>::GridCell(unsigned int xIn, unsigned int yIn, Grid<T> const& grid, T valueIn) :
			x{ xIn }, y{ yIn }, parentGrid{ grid }, value{ valueIn }{};

		// Functions for determining whether a cell lies on one of the edges of the
		// grid. Assume a grid with an origin of 0,0.
		bool IsTopRow() const { return (y == (parentGrid.Height() - 1)); }
		bool IsBottomRow() const { return (y == 0); }
		bool IsRightColumn() const { return (x == (parentGrid.Width() - 1)); }
		bool IsLeftColumn() const { return (x == 0); }

		// Functions for returning the adjacent cell in a given direction.
		// Requires that there is actually a cell in that direction.
		GridCell Up() const {
			assert(!IsTopRow());
			return parentGrid[Coordinate(x, y + 1)]; 
		}
		GridCell Down() const { 
			assert(!IsBottomRow());
			return parentGrid[Coordinate(x, y - 1)];
		}
		GridCell Right() const {
			assert(!IsRightColumn());
			return  parentGrid[Coordinate(x + 1, y)];
		}
		GridCell Left() const {
			assert(!IsLeftColumn());
			return  parentGrid[Coordinate(x - 1, y)];
		}
		GridCell UpRight() const {
			assert(!IsTopRow() && !IsRightColumn());
			return  parentGrid[Coordinate(x + 1, y + 1)];
		}
		GridCell DownRight() const {
			assert(!IsBottomRow() && !IsRightColumn());
			return  parentGrid[Coordinate(x + 1, y - 1)];
		}
		GridCell UpLeft() const {
			assert(!IsTopRow() && !IsLeftColumn());
			return  parentGrid[Coordinate(x - 1, y + 1)];
		}
		GridCell DownLeft() const {
			assert(!IsBottomRow() && !IsLeftColumn());
			return  parentGrid[Coordinate(x - 1, y - 1)];
		}
	};

	// We need to define the constructor for a Grid down here, since it requires knowledge
	// of the GridCell constructor.
	// Given a 2D-vector of values, create a new 2D-vector which wrap up each of those
	// values in a GridCell.
	template <typename T>
	Grid<T>::Grid(std::vector<std::vector<T>> gridIn)
	{
		for (unsigned int row = 0; row < gridIn.size(); row++)
		{
			std::vector<GridCell> newLine{};
			std::vector<T> lineIn = gridIn[row];
			for (unsigned int column = 0; column < lineIn.size(); column++)
			{
				newLine.push_back(GridCell{column, row, *this, lineIn[column]});
			}
			grid.push_back(newLine);
		}
	}
}