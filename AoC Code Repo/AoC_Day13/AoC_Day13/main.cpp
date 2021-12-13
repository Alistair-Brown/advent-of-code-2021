#include "Parsing.h"
#include "Paper.h"
#include <iostream>
#include <string>

int main()
{
	std::cout << "Enter grid locations" << std::endl;
	Paper::DottedPaper dottedPaper{};

	std::vector<int> parsedInts{};
	while (true)
	{
		parsedInts = Parsing::ParseLineOfInts(Parsing::comma);
		if (parsedInts.size() == 0) { break; }

		Paper::Coordinate latestCoord{ parsedInts[0], parsedInts[1] };
		dottedPaper.MakeDot(latestCoord);
	}

	// Failing the while loop means we've hit the blank line, and the rest
	// will be fold instructions. Do the first fold separately to get part
	// 1, then run through the rest.
	std::string inputLine;
	std::getline(std::cin, inputLine);
	Paper::Coordinate firstFold = Paper::ParseFoldInstruction(inputLine);
	dottedPaper.MakeFold(firstFold);
	std::cout << "Number of dots is " << dottedPaper.NumberOfDots() << std::endl;

	while (true)
	{
		std::getline(std::cin, inputLine);
		if (inputLine.size() == 0) { break; }
		Paper::Coordinate fold = Paper::ParseFoldInstruction(inputLine);
		dottedPaper.MakeFold(fold);
	}

	// Now let's print our results
	dottedPaper.PrintDots();
}