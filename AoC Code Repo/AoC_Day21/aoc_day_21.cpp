#include "Dirac.h"
#include "Parsing.h"
#include <iostream>
#include <regex>
#include <cassert>
#include "puzzle_solvers.h"

// Day 21 asks us to play a couple of games of Dirac Dice (see header comment in Dirac.h
// for the rules). In these games, each player is given a random starting position, which will
// be used for both games (these starting positions are the puzzle input).
// For part one, the game is played with a deterministic dice, so the result is pre-determined.
// We need to find the product of the loser's final score and the number of times the
// deterministic dice was rolled during the game.
// For part 2, we play with a quantum dice which splits the universe into a number of possible
// realities each time it is rolled. We are required to find out which player wins in more universes,
// and return the number of universes in which that player wins. 
PuzzleAnswerPair PuzzleSolvers::AocDayTwentyOneSolver(std::ifstream& puzzleInputFile)
{
	// Parse out the starting position of each player from our puzzle input.
	std::vector<std::string> inputLines = Parsing::SeparateRemainingInputIntoLines(puzzleInputFile);
	std::regex playerPosPattern{ "Player [0-9] starting position: ([0-9]+)" };
	unsigned int playerOneStartingPos;
	unsigned int playerTwoStartingPos;
	Parsing::ParseStringIntoElements(inputLines[0], playerPosPattern, playerOneStartingPos);
	Parsing::ParseStringIntoElements(inputLines[1], playerPosPattern, playerTwoStartingPos);

	// We solve part one by playing a deterministic game with a DeterministicDice which is local
	// to this function and provided as a reference. After the game we can query this dice for
	// the number of times it was rolled, and multiply this with the loser's score.
	// The deterministic game in part 1 is played with the following conditions:
	//  - The deterministic dice has a maximum value of 100
	//  - A score of 1000 is required to win
	Dirac::DeterministicDice dice{ 100 };
	Dirac::DeterministicGame deterministicGame{ dice, 1000, playerOneStartingPos, playerTwoStartingPos };
	std::pair<unsigned int, unsigned int> finalScores = deterministicGame.FinalScores();
	unsigned int loserScore = finalScores.first > finalScores.second ? finalScores.second : finalScores.first;
	unsigned int diceRolls = dice.NumberOfTimesRolled();
	unsigned int partOne = loserScore * diceRolls;

	// Part 2 requires us to determine which player wins in the most universes, and how many universes
	// that player wins in.
	// The Dirac Game splits the universe into multiple realities each time the dice is rolled, and
	// is played with the following conditions:
	//  - The deterministic dice has a maximum value of 3
	//  - A score of 21 is required to win
	Dirac::DiracGame diracGame{ 21, 3, playerOneStartingPos, playerTwoStartingPos };
	ULLINT partTwo = diracGame.NumberOfUniversesBestPlayerWinsIn();
	
	return PuzzleAnswerPair{ std::to_string(partOne), std::to_string(partTwo) };
}