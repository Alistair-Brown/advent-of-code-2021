#include "ProbeLauncher.h"
#include "Parsing.h"
#include <cassert>
#include <iostream>
#include <regex>

int main()
{
	std::regex targetZonePattern{ "target area: x=([0-9]+)..([0-9]+), y=-([0-9]+)..-([0-9]+)" };
	std::smatch matches;
	std::string inputLine;

	std::cout << "Enter input" << std::endl;

	std::getline(std::cin, inputLine);
	
	assert(std::regex_search(inputLine, matches, targetZonePattern));

	Launcher::ProbeLauncher probeLauncher{ 
		{Parsing::ConvertStringToInt(matches[1]), Parsing::ConvertStringToInt(matches[2])},
		{-Parsing::ConvertStringToInt(matches[3]), -Parsing::ConvertStringToInt(matches[4])} };

	std::cout << "Highest y position is " << probeLauncher.HighestYCoordinate() << std::endl;
	std::cout << "Number of possible velocities is " << probeLauncher.NumberOfPossibleVelocities() << std::endl;
}