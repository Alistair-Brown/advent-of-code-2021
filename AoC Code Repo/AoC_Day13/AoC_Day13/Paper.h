#pragma once
#include <vector>
#include <list>

namespace Paper
{
	struct Coordinate
	{
		int xPos;
		int yPos;
		Coordinate(int x, int y) : xPos{ x }, yPos{ y } {};
	};

	class DottedPaper
	{
	private:
		std::vector<std::vector<bool>> gridOfDots{};
		std::list<Coordinate> dotLocations{};
		void MaybeResizeGrid(Coordinate dotThatMustFit);
		void ShrinkGridToWidth(int newWidth);
		//void ShrinkGridToHeight(int newHeight);
		void InsertDotOnGrid(Coordinate dotCoord);
	public:
		void MakeDot(Coordinate dotCoord);
		void MakeFold(Coordinate foldLine);
		unsigned int NumberOfDots();
	};
}