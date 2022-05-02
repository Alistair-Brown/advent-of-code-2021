#pragma once
#include <vector>
#include <deque>
#include <memory>
#include <cassert>

namespace Bingo
{
	// A BingoGrid keeps track of how many numbers have been marked off
	// in each of its rows and columns. Once either a row or column has had 5 numbers
	// marked off in it, it has been completed, and this bingo grid has become
	// a winner (but not necessarily the first winner).
	class BingoGrid
	{
	private:
		static const int numbersOfRows{ 5 }, numbersOfColumns{ 5 };
		int rows[numbersOfRows]{ 0 }, columns[numbersOfColumns]{ 0 };
		int valueOfUnmarkedNumbers;
		bool hasWon{ false };
	public:
		int ValueOfUnmarkedNumbers() const { return valueOfUnmarkedNumbers; }
		void GridContainsNumber(int numberValue);
		int CrossNumberOutFromGrid(int numberValue, int rowNumber, int columnNumber);
	};

	// A number within a grid knows what grid it exists in, and in which column and row.
	// It is able to tell its grid that it has been crossed out.
	class NumberWithinGrid
	{
	private:
		int numberValue;
		std::weak_ptr<BingoGrid> parentGrid;
		int rowNumber;
		int columnNumber;
	public:
		// As part of constructing a number within a grid, the grid the number exists within
		// must be informed of its existence so that it can update its tracker of the total
		// value of the numbers within it.
		NumberWithinGrid(int numberValue, std::weak_ptr<BingoGrid> parentGrid, int rowNumber, int columnNumber) :
			numberValue{ numberValue }, parentGrid{ parentGrid }, rowNumber{ rowNumber }, columnNumber{ columnNumber } 
		{
			auto parentGridShd = parentGrid.lock();
			assert(parentGridShd);
			parentGridShd->GridContainsNumber(numberValue);
		};
		// If crossing this number our resulted in the parent grid being completed, this function
		// will return the remaining value of unmarked numbers in that grid.
		int CrossSelfOutInParentGrid() const {
			auto parentGridShd = parentGrid.lock();
			assert(parentGridShd);
			return parentGridShd->CrossNumberOutFromGrid(numberValue, rowNumber, columnNumber); }
	};

	// The BingoCaller is constructed with a vector of numbers to call, in the order in which
	// they should be called. Prior to actually starting to call them, it should also be told
	// of every instance of a number which exists within a bingo grid, so that on subseqently calling
	// any given number, it can tell each instance of that number within a grid to go and cross
	// itself out.
	class BingoCaller
	{
	private:
		static const int maximumCallableNumber = 99;
		std::vector<NumberWithinGrid> callableNumbers[maximumCallableNumber + 1];
		std::deque<int> numbersToCall;
	public:
		BingoCaller(std::vector<int> numbersIn)
		{
			for (int numberIn : numbersIn) { numbersToCall.push_back(numberIn); }
		}
		void CreateNumberInGrid(int numberValue, std::weak_ptr<BingoGrid> parentGrid, int rowNumber, int columnNumber);
		std::pair<unsigned int, unsigned int> CallNextNumber();
	};
}