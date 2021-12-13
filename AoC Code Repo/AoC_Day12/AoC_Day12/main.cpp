#include "Cave_System.h"
#include "Parsing.h"
#include <iostream>

// TODO: This code does the second step extrememly slowly, I should find out why
int main()
{
	Caves::CaveSystem caveSystem{};
	std::cout << "Enter cave input" << std::endl;

	while (true)
	{
		std::vector<std::vector<std::string>> parsedLine = Parsing::ParseGroupsOfString({ 2 }, "-", "");
		if (parsedLine.size() == 0)
		{
			break;
		}
		else
		{
			std::vector<std::string> connectedCaves = parsedLine[0];
			caveSystem.LinkCaves(connectedCaves[0], connectedCaves[1]);
		}
	}

	std::cout << "There are " << Caves::NumberOfRoutes(caveSystem, 1) << " routes if you can visit each small cave once" << std::endl;
	std::cout << "There are " << Caves::NumberOfRoutes(caveSystem, 2) << " routes if you can visit each small cave twice" << std::endl;
}