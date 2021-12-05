#include "SteamVents.h"
#include <iostream>
#include <string>

int main()
{
	std::cout << "Begin" << std::endl;

	std::string inputLine;
	Steam::WarningMap warningMap;
	std::vector<Steam::WarningPath> diagonalLines;

	while (true)
	{
		std::getline(std::cin, inputLine);
		if (inputLine.size() == 0) { break; }

		Steam::WarningPath path{ inputLine };
		if (path.IsStraightLine())
		{
			warningMap.InsertWarningPath(path);
		}
		else
		{
			diagonalLines.push_back(path);
		}
	}

	std::cout << "Number of overlaps without diagonals is " << warningMap.NumberOfOverlapCells() << std::endl;

	for (Steam::WarningPath path : diagonalLines)
	{
		warningMap.InsertDiagonalWarningPath(path);
	}

	std::cout << "Number of overlaps including diagonals is " << warningMap.NumberOfOverlapCells() << std::endl;
}