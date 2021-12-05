#include "Navigation.h"
#include "Parsing.h"
#include <iostream>
#include <string>

int main()
{
	std::string directionStr;
	int magnitude;
	Navigation::Direction direction;
	Navigation::Position simpleSubmarinePosition;
	Navigation::Position complexSubmarinePosition;

	std::cout << "Enter directions followed by 'end'" << std::endl;

	while (true)
	{
		directionStr = Parsing::ParseStringFromConsole();
		if (directionStr == "end") { break; }

		direction = Navigation::GetDirectionFromString(directionStr);
		magnitude = Parsing::ParseIntFromConsole();

		simpleSubmarinePosition.ApplyMovementWithAim(Navigation::MovementCommand(direction, magnitude));
		complexSubmarinePosition.ApplyMovementWithAim(Navigation::MovementCommand(direction, magnitude));
	}

	std::cout << "Simple product was " << simpleSubmarinePosition.GetProductOfPositionsInvertingDepth() << std::endl;
	std::cout << "Complex product was " << complexSubmarinePosition.GetProductOfPositionsInvertingDepth() << std::endl;
}