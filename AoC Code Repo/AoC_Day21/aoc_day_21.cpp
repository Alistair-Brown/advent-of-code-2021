#include "Dirac.h"
#include "Parsing.h"
#include <iostream>
#include <regex>
#include <cassert>
#include "puzzle_solvers.h"

PuzzleAnswerPair PuzzleSolvers::AocDayTwentyOneSolver(std::ifstream& puzzleInputFile)
{
	int playerOneStartingPos;
	int playerTwoStartingPos;

	std::vector<std::string> inputLines = Parsing::SeparateRemainingInputIntoLines(puzzleInputFile);
	std::regex playerPosPattern{ "Player [0-9] starting position: ([0-9]+)" };
	Parsing::ParseStringIntoElements(inputLines[0], playerPosPattern, playerOneStartingPos);
	Parsing::ParseStringIntoElements(inputLines[1], playerPosPattern, playerTwoStartingPos);

	Dirac::DeterministicDice dice{ 100 };
	Dirac::DeterministicGame deterministicGame{ dice, 3, 10, 1000, playerOneStartingPos, playerTwoStartingPos };
	std::pair<int, int> finalScores = deterministicGame.FinalScores();
	int loserScore = finalScores.first > finalScores.second ? finalScores.second : finalScores.first;
	int diceRolls = dice.NumberOfTimesRolled();
	int partOne = loserScore * diceRolls;

	Dirac::DiracGame diracGame{ 10, 21, 3, 3, playerOneStartingPos, playerTwoStartingPos };
	ULLINT partTwo = diracGame.NumberOfUniversesBestPlayerWinsIn();
	
	return PuzzleAnswerPair{ std::to_string(partOne), std::to_string(partTwo) };
}