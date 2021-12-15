#include "Parsing.h"
#include "Chiton.h"
#include <iostream>

std::vector<std::vector<int>> EnlargeGridBy5(std::vector<std::vector<int>> originalGrid);

int main()
{
	std::cout << "Enter risk grid" << std::endl;

	std::vector<std::vector<int>> originalGrid = Parsing::ParseWhitespacelessGridOfDigits();
	Chiton::ChitonCave chitonCave{ originalGrid };

	std::cout << "Minimum risk route to end is " << chitonCave.MinimumRiskToEnd() << std::endl;

	std::vector<std::vector<int>> enlargedGrid = EnlargeGridBy5(originalGrid);
	Chiton::ChitonCave bigChitonCave{ enlargedGrid };

	std::cout << "Minimum risk route to end on bigger grid is " << bigChitonCave.MinimumRiskToEnd() << std::endl;
}

std::vector<std::vector<int>> EnlargeGridBy5(std::vector<std::vector<int>> originalGrid)
{
	std::vector<std::vector<int>> newGrid = originalGrid;
	for (unsigned int ii = 0; ii < originalGrid.size(); ii++)
	{
		std::vector<int> inputRow = originalGrid[ii];
		for (int jj = 0; jj < 4; jj++)
		{
			for (int elem = 0; elem < inputRow.size(); elem++)
			{
				if (inputRow[elem] < 9) { inputRow[elem]++; }
				else { inputRow[elem] = 1; }
			}

			newGrid[ii].insert(newGrid[ii].end(), inputRow.begin(), inputRow.end());
		}
	}

	std::vector<std::vector<int>> middleGrid = newGrid;

	for (int ii = 0; ii < 4; ii++)
	{
		for (std::vector<int> &row : middleGrid)
		{
			for (int &elem : row)
			{
				if (elem < 9) { elem++; }
				else { elem = 1; }
			}
		}

		for (unsigned int jj = 0; jj < originalGrid.size(); jj++)
		{
			newGrid.push_back(middleGrid[jj]);
		}
	}

	return newGrid;
}