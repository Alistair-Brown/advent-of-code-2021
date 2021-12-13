#include "Paper.h"
#include <cassert>
#include <iterator>
#include <regex>
#include "Parsing.h"

void Paper::DottedPaper::MaybeResizeGrid(Coordinate dotThatMustFit)
{
	if ((gridOfDots.size() == 0) || (gridOfDots.size() <= dotThatMustFit.yPos))
	{
		gridOfDots.resize(dotThatMustFit.yPos + 1);
	}

	int newWidth = (gridOfDots[0].size() <= (dotThatMustFit.xPos + 1)) ?
		(dotThatMustFit.xPos + 1) : gridOfDots[0].size();
	for (int rowNum = 0; rowNum < gridOfDots.size(); rowNum++)
	{
		gridOfDots[rowNum].resize(newWidth);
	}
}

void Paper::DottedPaper::ShrinkGridToWidth(int newWidth)
{
	for (int rowNum = 0; rowNum < gridOfDots.size(); rowNum++)
	{
		gridOfDots[rowNum].resize(newWidth);
	}
}

void Paper::DottedPaper::ShrinkGridToHeight(int newHeight)
{
	gridOfDots.resize(newHeight);
}

void Paper::DottedPaper::InsertDotOnGrid(Coordinate dotCoord)
{
	if (!gridOfDots[dotCoord.yPos][dotCoord.xPos])
	{
		dotLocations.push_back(dotCoord);
		gridOfDots[dotCoord.yPos][dotCoord.xPos] = true;
	}	
}

void Paper::DottedPaper::MakeDot(Coordinate dotCoord)
{
	MaybeResizeGrid(dotCoord);
	InsertDotOnGrid(dotCoord);
}

// Coordinate should be given where only one of the two axis is non zero, and represents
// the line to fold along.
void Paper::DottedPaper::MakeFold(Coordinate foldLine)
{
	// One, and only one, of the coordinate axes should be non-zero
	assert((foldLine.xPos == 0) != (foldLine.yPos == 0));

	// I'll just get the x fold working for now, since that's the first puzzle.
	std::list<Coordinate> newlyFoldedDots{};
	if (foldLine.xPos > 0)
	{
		int xFoldLine = foldLine.xPos;		
		ShrinkGridToWidth(xFoldLine);

		for (std::list<Coordinate>::iterator coordIter = dotLocations.begin();
			 coordIter != dotLocations.end();)
		{
			assert(coordIter->xPos != xFoldLine);

			if (coordIter->xPos > xFoldLine)
			{
				Coordinate newDot{ (coordIter->xPos - ((coordIter->xPos - xFoldLine) * 2)), coordIter->yPos };
				if (!gridOfDots[newDot.yPos][newDot.xPos])
				{
					newlyFoldedDots.push_back(newDot);
					gridOfDots[newDot.yPos][newDot.xPos] = true;
				}

				coordIter = dotLocations.erase(coordIter);
			}
			else
			{
				coordIter++;
			}
		}
	}
	else
	{
		int yFoldLine = foldLine.yPos;
		ShrinkGridToHeight(yFoldLine);

		for (std::list<Coordinate>::iterator coordIter = dotLocations.begin();
			coordIter != dotLocations.end();)
		{
			assert(coordIter->yPos != yFoldLine);

			if (coordIter->yPos > yFoldLine)
			{
				Coordinate newDot{ coordIter->xPos, coordIter->yPos - ((coordIter->yPos - yFoldLine) * 2) };
				if (!gridOfDots[newDot.yPos][newDot.xPos])
				{
					newlyFoldedDots.push_back(newDot);
					gridOfDots[newDot.yPos][newDot.xPos] = true;
				}

				coordIter = dotLocations.erase(coordIter);
			}
			else
			{
				coordIter++;
			}
		}
	}
	for (std::list<Coordinate>::iterator coordIter = newlyFoldedDots.begin();
		coordIter != newlyFoldedDots.end();
		coordIter++)
	{
		dotLocations.insert(dotLocations.end(), *coordIter);
	}
}

unsigned int Paper::DottedPaper::NumberOfDots()
{
	return dotLocations.size();
}

void Paper::DottedPaper::PrintDots()
{
	for (std::vector<bool> row : gridOfDots)
	{
		for (bool isDot : row)
		{
			if (isDot)
			{
				std::cout << "X";
			}
			else
			{
				std::cout << " ";
			}
		}
		std::cout << std::endl;
	}
}

Paper::Coordinate Paper::ParseFoldInstruction(std::string instructionLine)
{
	std::regex matchString{ "fold along (x|y)=([0-9]+)" };
	std::smatch matches;
	Coordinate parsedInstruction;

	std::regex_match(instructionLine, matches, matchString);
	int foldMagnitude = Parsing::ConvertStringToInt(matches[2]);
	if (matches[1] == "x")
	{
		parsedInstruction = Coordinate(foldMagnitude, 0);
	}
	else
	{
		assert(matches[1] == "y");
		parsedInstruction = Coordinate(0, foldMagnitude);
	}
	return parsedInstruction;
}
