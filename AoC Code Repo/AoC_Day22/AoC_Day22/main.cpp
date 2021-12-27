#include "Reactor.h"
#include "Parsing.h"
#include <iostream>
#include <regex>
#include <cassert>
#include <deque>

int main()
{
	Reactor::InitializationZone initializationZone{ false, {-50, 50}, {-50, 50}, {-50, 50} };
	std::vector<std::pair<bool, Reactor::InstructionVolume>> instructions;
	std::regex inputMatchString {
		"([a-z]+) x=(-{0,1}[0-9]+)..(-{0,1}[0-9]+),y=(-{0,1}[0-9]+)..(-{0,1}[0-9]+),z=(-{0,1}[0-9]+)..(-{0,1}[0-9]+)" };
	std::smatch matches;

	std::cout << "Enter reactor instructions" << std::endl;

	std::string inputLine;
	while (true)
	{
		std::getline(std::cin, inputLine);
		if (std::regex_match(inputLine, matches, inputMatchString))
		{
			assert((matches[1] == "on") || (matches[1] == "off"));
			bool settingIsOn = (matches[1] == "on");
			std::pair<int, int> xRange{ Parsing::ConvertStringToInt(matches[2]), Parsing::ConvertStringToInt(matches[3]) };
			std::pair<int, int> yRange{ Parsing::ConvertStringToInt(matches[4]), Parsing::ConvertStringToInt(matches[5]) };
			std::pair<int, int> zRange{ Parsing::ConvertStringToInt(matches[6]), Parsing::ConvertStringToInt(matches[7]) };

			initializationZone.SetCubesInRange(settingIsOn, xRange, yRange, zRange);

			instructions.push_back({ settingIsOn, { xRange, yRange, zRange } });

			//std::cout << "After step there are " << initializationZone.GetNumberOfOnCubes() << " 'on' cubes" << std::endl;
		}
		else
		{
			break;
		}
	}

	std::cout << "There are " << initializationZone.GetNumberOfOnCubes() << " 'on' cubes in the initialisation zone" << std::endl;

	Reactor::CompleteReactor completeReactor{ instructions };

	std::cout << "There are " << completeReactor.NumberOfOnCubes() << " 'on' cubes in the complete reactor" << std::endl;
}