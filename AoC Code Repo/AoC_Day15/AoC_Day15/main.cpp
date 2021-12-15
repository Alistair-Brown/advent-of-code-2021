#include "Parsing.h"
#include "Chiton.h"
#include <iostream>

int main()
{
	std::cout << "Enter risk grid" << std::endl;

	Chiton::ChitonCave chitonCave{ Parsing::ParseWhitespacelessGridOfDigits() };

	std::cout << "Minimum risk route to end is " << chitonCave.MinimumRiskToEnd() << std::endl;
}