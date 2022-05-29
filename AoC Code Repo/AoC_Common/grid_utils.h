#pragma once
#include <vector>
#include <cassert>
#include <memory>

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
		unsigned int xPos;
		unsigned int yPos;
		Coordinate(unsigned int xIn, unsigned int yIn) : xPos{ xIn }, yPos{ yIn }{};
		Coordinate() : Coordinate(0, 0) {};

		bool operator==(Coordinate const& otherCoord) const
		{
			if ((otherCoord.xPos == xPos) && (otherCoord.yPos == yPos)) { return true; }
			else { return false; }
		}
		bool operator!=(Coordinate const& otherCoord) const
		{
			return !(*this == otherCoord);
		}
		bool operator<(Coordinate const& otherCoord) const
		{
			return (otherCoord.xPos == xPos) ?
				(yPos < otherCoord.yPos) : (xPos < otherCoord.xPos);
		}
	};

	// A 2D-vector of any required type. This grid does not hold those types directly, but
	// instead holds a 2D-vector of 'grid cells', from the underlying value can be accessed,
	// but which also provide methods for navigating the grid more easily than using
	// increments/decrements on raw coordinates.
	template<typename T>
	class Grid
	{
	public:
		class GridCell;		

	private:
		std::vector<std::vector<GridCell>> grid;

	public:
		// We declare the Grid constructor here but have to hold off on the definition until
		// the GridCell nested class has been defined so that we can construct GridCells.
		Grid(std::vector<std::vector<T>> gridIn);

		// Turn a 2D-vector of one type into a Grid of another, by applying a function to each
		// element in the input grid. Useful for puzzles where we might have a grid of integers
		// as input, but want to use a user-defined class to actually represent each element
		// in that Grid.
		template <typename TypeIn>
		static Grid GridFactory(std::vector<std::vector<TypeIn>> gridIn, T(*cellConstructor)(TypeIn));

		unsigned int Height() const { return grid.size(); }
		unsigned int Width() const { return grid[0].size(); }

		// Cells within the grid can be accessed by read-only value, or read/write reference.
		GridCell operator[] (Coordinate const &coord) const { return grid[coord.yPos][coord.xPos]; }
		GridCell &operator[] (Coordinate const &coord) { return grid[coord.yPos][coord.xPos]; }

		// The Grid provides begin() and end() functions that can be used to easily iterate
		// over every value in it.
		class Iterator;
		Iterator begin();
		Iterator end();
	};

	// A single cells within a grid of type T. Provides utility functions for accessing
	// adjacent cells, and a publicly read/writable cell value.
	template <typename T>
	class Grid<T>::GridCell
	{
	private:
		Grid<T> &parentGrid;
		const unsigned int x;
		const unsigned int y;

	public:
		T value;

		Grid<T>::GridCell(unsigned int xIn, unsigned int yIn, Grid<T> & grid, T valueIn) :
			x{ xIn }, y{ yIn }, parentGrid{ grid }, value{ valueIn }{};
		Coordinate GetCoordinate() const { return Coordinate{ x, y }; }

		// Functions for determining whether a cell lies on one of the edges of the
		// grid. Assume a grid with an origin of 0,0.
		bool IsTopRow() const { return (y == (parentGrid.Height() - 1)); }
		bool IsBottomRow() const { return (y == 0); }
		bool IsRightColumn() const { return (x == (parentGrid.Width() - 1)); }
		bool IsLeftColumn() const { return (x == 0); }

		// Functions for returning the adjacent cell in a given direction.
		// Requires that there is actually a cell in that direction.
		// Can either return the adjacent cell as read-only value (const version
		// of the function) or read-write reference (non-const version of function);
		GridCell Up() const {
			assert(!IsTopRow());
			return parentGrid[Coordinate(x, y + 1)]; 
		}
		GridCell &Up() {
			assert(!IsTopRow());
			return parentGrid[Coordinate(x, y + 1)];
		}
		GridCell Down() const { 
			assert(!IsBottomRow());
			return parentGrid[Coordinate(x, y - 1)];
		}
		GridCell &Down() {
			assert(!IsBottomRow());
			return parentGrid[Coordinate(x, y - 1)];
		}
		GridCell Right() const {
			assert(!IsRightColumn());
			return  parentGrid[Coordinate(x + 1, y)];
		}
		GridCell &Right() {
			assert(!IsRightColumn());
			return  parentGrid[Coordinate(x + 1, y)];
		}
		GridCell Left() const {
			assert(!IsLeftColumn());
			return  parentGrid[Coordinate(x - 1, y)];
		}
		GridCell &Left() {
			assert(!IsLeftColumn());
			return  parentGrid[Coordinate(x - 1, y)];
		}
		GridCell UpRight() const {
			assert(!IsTopRow() && !IsRightColumn());
			return  parentGrid[Coordinate(x + 1, y + 1)];
		}
		GridCell &UpRight() {
			assert(!IsTopRow() && !IsRightColumn());
			return  parentGrid[Coordinate(x + 1, y + 1)];
		}
		GridCell DownRight() const {
			assert(!IsBottomRow() && !IsRightColumn());
			return  parentGrid[Coordinate(x + 1, y - 1)];
		}
		GridCell &DownRight() {
			assert(!IsBottomRow() && !IsRightColumn());
			return  parentGrid[Coordinate(x + 1, y - 1)];
		}
		GridCell UpLeft() const {
			assert(!IsTopRow() && !IsLeftColumn());
			return  parentGrid[Coordinate(x - 1, y + 1)];
		}
		GridCell &UpLeft() {
			assert(!IsTopRow() && !IsLeftColumn());
			return  parentGrid[Coordinate(x - 1, y + 1)];
		}
		GridCell DownLeft() const {
			assert(!IsBottomRow() && !IsLeftColumn());
			return  parentGrid[Coordinate(x - 1, y - 1)];
		}
		GridCell &DownLeft() {
			assert(!IsBottomRow() && !IsLeftColumn());
			return  parentGrid[Coordinate(x - 1, y - 1)];
		}

		friend bool operator==(const GridCell& cell1, const GridCell& cell2)
		{
			return cell1.GetCoordinate() == cell2.GetCoordinate();
		}
		friend bool operator!=(const GridCell& cell1, const GridCell& cell2)
		{
			return cell1.GetCoordinate() != cell2.GetCoordinate();
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

	// Turn a 2D-vector of one type into a Grid of another, by applying a function to each
	// element in the input grid. Useful for puzzles where we might have a grid of integers
	// as input, but want to use a user-defined class to actually represent each element
	// in that Grid.
	// We accomplish this by creating a new 2D-vector by calling the cell constructor function
	// on each element of the original 2D-vector, and then pass this 2D-vector into the
	// regular Grid constructor to wrap it up as a Grid like usual.
	template <typename T>
	template <typename TypeIn>
	Grid<T> Grid<T>::GridFactory(std::vector<std::vector<TypeIn>> gridIn, T(* cellConstructor)(TypeIn))
	{
		std::vector<std::vector<T>> transformedGrid;
		for (std::vector<TypeIn> rowIn : gridIn)
		{
			std::vector<T> newLine{};
			for (TypeIn cellIn : rowIn)
			{
				newLine.push_back(cellConstructor(cellIn));
			}
			transformedGrid.push_back(newLine);
		}
		return Grid{transformedGrid};
	}

	// Iterator for the Grid class. Holds a pointer to a GridCell. Incrementing
	// the iterator will either:
	//  - Move it to the next element on the same row of the grid if we're not
	//    the end of a row already.
	//  - Otherwise, move it to the first element in the next row, if we're not
	//    also on the last row of the grid.
	//  - Otherwise, set the GridCell pointer to nullptr, since we have been
	//    iterated off the end of the Grid and should no longer be accessed.
	template <typename T>
	class Grid<T>::Iterator
	{
	private:
		Grid &grid;
		GridCell *cellPtr;

		// Move to point to the next element in the grid, whether that be the
		// next element in the current row, the first element in the next row,
		// or just nullptr if we're out of grid to iterate over.
		void IncrementCell()
		{
			assert(cellPtr != nullptr);
			Coordinate newCoord = cellPtr->GetCoordinate();
			if (cellPtr->IsRightColumn())
			{
				if (cellPtr->IsTopRow())
				{
					cellPtr = nullptr;
					return;
				}
				else
				{
					newCoord.xPos = 0;
					newCoord.yPos++;
				}
			}
			else
			{
				newCoord.xPos++;
			}
			cellPtr = &grid[newCoord];
		}
	public:
		// An Iterator can be constructed from a Grid reference to point to
		// the cells within, and coordinate to start the iteration from.
		// Constructing without a coordinate will create an Iterator with a
		// nullptr, which should be used to create the end iterator.
		Iterator(Grid &gridIn, Coordinate coord) : grid { gridIn }
		{
			cellPtr = &grid[coord];
		}
		Iterator(Grid& gridIn) : grid{ gridIn }
		{
			cellPtr = nullptr;
		}

		GridCell& operator*() { return *cellPtr; }
		GridCell* operator->() { return cellPtr; }

		// Pre- and post-fix increments.
		Iterator& operator++() { 
			IncrementCell();
			return *this;
		}
		Iterator operator++(int)
		{
			Iterator tempItr = *this;
			IncrementCell();
			return tempItr;
		}

		friend bool operator==(const Iterator &itr1, const Iterator &itr2)
		{
			return (itr1.cellPtr == itr2.cellPtr);
		}
		friend bool operator!=(const Iterator &itr1, const Iterator &itr2)
		{
			return (itr1.cellPtr != itr2.cellPtr);
		}
	};

	// Begin and End iterators for iterating over a grid.
	// The Begin iterator starts at the origin of the grid. The End iterator
	// is constructed without passing in a coordinate, which causes the Iterator
	// class to construct itself to have a null pointer rather than pointer to
	// a valid GridCell, in the same way that it will set its pointer to
	// nullptr when it detects that it is being iterated out of the bounds
	// of the Grid.
	template <typename T>
	typename Grid<T>::Iterator Grid<T>::begin()
	{
		return Iterator{ *this, Coordinate{ 0, 0 } };
	}
	template <typename T>
	typename Grid<T>::Iterator Grid<T>::end()
	{
		return Iterator{ *this };
	}
}