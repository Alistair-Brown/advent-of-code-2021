#include "Parsing.h"
#include "Paper.h"
#include <iostream>

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

	// I'll parse this out properly once I've verified the rest of my code works
	int foldPos{};
	std::cout << "Enter first fold pos" << std::endl;
	std::cin >> foldPos;

	dottedPaper.MakeFold({ foldPos, 0 });
	std::cout << "Number of dots is " << dottedPaper.NumberOfDots() << std::endl;
}