#include "Bingo.h"
#include "Parsing.h"
#include <iostream>
#include <string>

int main()
{
	std::cout << "Begin" << std::endl;

	Bingo::BingoCaller bingoCaller;

	// Parse the numbers to call up from input, separating out the commas. This should go into
	// my parsing library later.
	std::vector<int> numbersToCall = Parsing::ParseLineOfInts(Parsing::comma);
	for (int numberToCall : numbersToCall)
	{
		bingoCaller.QueueNumber(numberToCall);
	}
	
	std::string nextLine;
	std::getline(std::cin, nextLine); // Clear new line
	
	std::vector<Bingo::BingoGrid *> bingoGrids; // Just need these so I can free them later
	while (true)
	{		
		std::getline(std::cin, nextLine);
		if (nextLine.length() > 0)
		{
			Bingo::BingoGrid *currentGrid = new Bingo::BingoGrid();
			bingoGrids.push_back(currentGrid);
			int rowNumber = 0;
			int columnNumber = 0;
			int currentNumber;
			while (true)
			{
				columnNumber = 0;

				while (true)
				{
					// Remove a leading space if needed
					while (nextLine.substr(0, 1) == Parsing::space)
					{
						nextLine.erase(0, 1);
					}

					currentNumber = strtol(nextLine.substr(0, nextLine.find(Parsing::space)).c_str(), nullptr, 10);
					bingoCaller.CreateNumberInGrid(currentNumber, currentGrid, rowNumber, columnNumber);
					nextLine.erase(0, nextLine.find(Parsing::space) + 1);
					// Remove a leading space if needed
					while (nextLine.substr(0, 1) == Parsing::space)
					{
						nextLine.erase(0, 1);
					}

					columnNumber++;
					if (nextLine.find(Parsing::space) == std::string::npos)
					{


						currentNumber = strtol(nextLine.c_str(), nullptr, 10);
						bingoCaller.CreateNumberInGrid(currentNumber, currentGrid, rowNumber, columnNumber);
						break;
					}
				}
				rowNumber++;
				std::getline(std::cin, nextLine);
				if (nextLine.length() == 0) { break; }
			}
		}
		else
		{
			break;
		}
	}

	std::cout << "All Grids populated" << std::endl;

	int numberOfWinners = 0;
	int thisNumberWinners = 0;
	int result;
	while (true)
	{
		result = bingoCaller.CallNextNumber(thisNumberWinners);
		if (result >= 0)
		{
			numberOfWinners += thisNumberWinners;
			if (numberOfWinners == bingoGrids.size())
			{
				std::cout << "Result is " << result;
				break;
			}
		}
	}

	for (Bingo::BingoGrid *grid : bingoGrids)
	{
		delete grid;
	}
}