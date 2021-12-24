#include "Dirac.h"
#include "Parsing.h"
#include <iostream>
#include <regex>
#include <cassert>

int main()
{
	std::regex playerPosPattern{ "Player [0-9] starting position: ([0-9]+)" };
	Dirac::DeterministicDice dice{ 100 };
	std::smatch matches;
	std::string inputLine;
	int playerOneStartingPos;
	int playerTwoStartingPos;

	std::cout << "Enter input" << std::endl;

	std::getline(std::cin, inputLine);
	assert(std::regex_search(inputLine, matches, playerPosPattern));
	playerOneStartingPos = Parsing::ConvertStringToInt(matches[1]);

	std::getline(std::cin, inputLine);
	assert(std::regex_search(inputLine, matches, playerPosPattern));
	playerTwoStartingPos = Parsing::ConvertStringToInt(matches[1]);

	Dirac::DeterministicGame deterministicGame{ dice, 3, 10, 1000, playerOneStartingPos, playerTwoStartingPos };
	std::pair<int, int> finalScores = deterministicGame.FinalScores();
	int loserScore = finalScores.first > finalScores.second ? finalScores.second : finalScores.first;
	int diceRolls = dice.NumberOfTimesRolled();

	std::cout << "Solution for part one is " << loserScore * diceRolls << std::endl;

	Dirac::DiracGame diracGame{ 10, 21, 3, 3, playerOneStartingPos, playerTwoStartingPos };

	std::cout << "Solution for part two is " << diracGame.NumberOfUniversesBestPlayerWinsIn() << std::endl;
}