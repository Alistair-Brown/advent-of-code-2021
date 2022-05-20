#pragma once
#include <vector>
#include <list>
#include <string>

namespace Paper
{
	struct Coordinate
	{
		unsigned int xPos;
		unsigned int yPos;
		Coordinate() : Coordinate(0, 0) {};
		Coordinate(unsigned int x, unsigned int y) : xPos{ x }, yPos{ y } {};
	};

	class DottedPaper
	{
	private:
		std::vector<std::vector<bool>> gridOfDots{};
		std::list<Coordinate> dotLocations{};
		void MaybeResizeGrid(Coordinate dotThatMustFit);
		void ShrinkGridToWidth(int newWidth);
		void ShrinkGridToHeight(int newHeight);
		void InsertDotOnGrid(Coordinate dotCoord);
	public:
		void MakeDot(Coordinate dotCoord);
		void MakeFold(Coordinate foldLine);
		unsigned int NumberOfDots();
		std::string FoldedPaper();
	};

	Coordinate ParseFoldInstruction(std::string instructionLine);
}