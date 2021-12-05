#pragma once
#include <vector>
#include <deque>

namespace Bingo
{
	class BingoGrid
	{
	private:
		static const int numbersOfRows{ 5 }, numbersOfColumns{ 5 }; // TODO, extract this from input
		int rows[numbersOfRows]{ 0 }, columns[numbersOfColumns]{ 0 };
		int valueOfUnmarkedNumbers;
		bool hasWon{ false };
	public:
		int GetValueOfUnmarkedNumbers() { return valueOfUnmarkedNumbers; }
		void InsertNumberIntoGrid(int numberValue) { valueOfUnmarkedNumbers += numberValue; }
		int CrossNumberOutFromGrid(int numberValue, int rowNumber, int columnNumber);
	};

	class NumberWithinGrid
	{
	private:
		int numberValue;
		BingoGrid *parentGrid;
		int rowNumber;
		int columnNumber;
	public:
		NumberWithinGrid(int numberValue, BingoGrid *parentGrid, int rowNumber, int columnNumber) :
			numberValue{ numberValue }, parentGrid{ parentGrid }, rowNumber{ rowNumber }, columnNumber{ columnNumber } {};
		int CrossSelfOutInParentGrid() { return parentGrid->CrossNumberOutFromGrid(numberValue, rowNumber, columnNumber); }
	};

	class BingoCaller
	{
	private:
		static const int maximumCallableNumber = 99; // TODO, extract this from input
		std::vector<NumberWithinGrid> callableNumbers[maximumCallableNumber + 1]; // 0 is callable
		std::deque<int> numbersToCall; // Numbers are added to the end, and called from the front
	public:
		void CreateNumberInGrid(int numberValue, BingoGrid *parentGrid, int rowNumber, int columnNumber);
		void QueueNumber(int numberValue) { numbersToCall.push_back(numberValue); }
		int CallNextNumber(int &numberOfWinners);
	};
}