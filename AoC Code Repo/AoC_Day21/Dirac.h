#pragma once
#include <utility>
#include <vector>
#include <map>
#include "aoc_common_types.h"

// Dirac dice is played on a circular board with spaces numbered 1 to 10 inclusive, in order. Each
// player has a pawn on this board, and they take turns to roll a dice 3 times, and then move their
// pawn a number of spaces equal to the sum of the rolls. Their score is then increased by the number
// of the space they land on. A game continues until one player has reached the required score to win,
// at which point the game ends immediately.
namespace Dirac
{
	const unsigned int SPACES_ON_DIRAC_BOARD = 10;
	const unsigned int DICE_ROLLS_PER_TURN = 3;

	// A deterministic dice is guaranteed to initially roll a 1, then a 2, then a 3, and so
	// on, up until it reaches its maximum rollable number, at which point it will wrap back
	// to 1 again. This deterministic dice also keeps track of the total number of times it has
	// been rolled.
	class DeterministicDice
	{
	private:
		unsigned int maxRollableNumber;
		unsigned int nextNumberToRoll{ 1 };
		unsigned int numberOfTimesRolled{ 0 };
	public:
		DeterministicDice(unsigned int maxRollableNumber) : maxRollableNumber{ maxRollableNumber } {};
		unsigned int NumberOfTimesRolled() { return numberOfTimesRolled; }
		unsigned int SumOfNextNRolls(unsigned int numberOfRolls);
	};

	// A deterministic game of dirac dice is played with a deterministic dice, for which any given
	// roll is pre-determined. This class will play a game of Dirac dice (following rules in namespace
	// header comment) with such a dice, reporting back the final scores once a player has won.
	class DeterministicGame
	{
	private:
		unsigned int scoreToWin;
		DeterministicDice &dice;

		unsigned int playerOnePosition;
		unsigned int playerTwoPosition;
		bool gamePlayed{ false };
		unsigned int playerOneScore{ 0 };
		unsigned int playerTwoScore{ 0 };

		void PlayGame();
		void UpdatePlayerScoreAndPosition(
			unsigned int spacesToMove,
			unsigned int &playerPos,
			unsigned int &playerScore);
	public:
		DeterministicGame(DeterministicDice &dice,
				unsigned int scoreToWin,
				unsigned int playerOneStartingPosition,
				unsigned int playerTwoStartingPosition) :
			dice{ dice },
			scoreToWin{ scoreToWin },
			playerOnePosition{ playerOneStartingPosition },
			playerTwoPosition{ playerTwoStartingPosition }{};

		std::pair<unsigned int, unsigned int> FinalScores();
	};

	// The Dirac dice can provide a vector containing each of the possible values that
	// could result from it on a given turn. The dice is rolled 3 times on a turn (namespace
	// constant DICE_ROLLS_PER_TURN) and takes its maximum rollable value from the maxVal
	// construction parameter.
	class DiracDice
	{
	private:
		unsigned int maxRollableValue;
		std::vector<unsigned int> possibleSumsPerTurn;

		std::vector<unsigned int> PossibleSumsForNRolls(unsigned int numRolls);
	public:
		DiracDice(unsigned int maxVal);
		std::vector<unsigned int> PossibleSumsPerTurn() { return possibleSumsPerTurn; }
	};

	// The truly 'Dirac' version of Dirac Dice is played with a dice that splits the universe
	// into multiple realities each time it is rolled (one reality for each rollable value).
	// This class is capable of working out which of the 2 players would win in more of these
	// realities than the other, and return the number of realities in which that player one.
	class DiracGame
	{
	private:
		unsigned int playerOneStartPos;
		unsigned int playerTwoStartPos;
		unsigned int scoreToWin;
		DiracDice dice;

		// Much of the legwork of this solution involves asking the question "Given a current
		// position and a number of remaining required points, how many ways could this point total
		// be reached?". So we can optimise enormously by caching the result of this query,
		// so that NumberOfWaysToAchieveScore will only have to do meaningful work around 200 times
		// in the lifetime of the program.
		// The pair used as a key holds the current position, and the required score. Then the map
		// stored as a value holds the number of ways to achieve each possible score in different 
		// numbers of turns (i.e. what gets returned by NumberOfWaysToAchieveScore()).
		std::map<std::pair<unsigned int, unsigned int>, std::map<unsigned int, ULLINT>> cachedWaysToAchieveScores;
		std::map<std::pair<unsigned int, unsigned int>, std::map<unsigned int, ULLINT>> cachedExactWaysToAchieveScores;

		std::map<unsigned int, ULLINT> NumberOfWaysToAchieveScore(
			unsigned int currentPosition,
			unsigned int requiredScore,
			bool exactScore);
		ULLINT NumberOfWaysToNotReachScoreInTurns(
			unsigned int currentPosition,
			unsigned int scoreToMiss,
			unsigned int turnLimit);
		void IncreasePossibilitiesForTurn(
			std::map<unsigned int, ULLINT> &existingTurnToPossibilityMap,
			unsigned int turnNums,
			ULLINT extraPossibilities);

		unsigned int PositionAfterRoll(unsigned int currentPosition, unsigned int roll);

	public:
		DiracGame(unsigned int scoreToWin, unsigned int diceMaxVal, unsigned int playerOnePos, unsigned int playerTwoPos) :
			scoreToWin{ scoreToWin },
			dice{ diceMaxVal },
			playerOneStartPos{playerOnePos},
			playerTwoStartPos{ playerTwoPos }{};
		ULLINT NumberOfUniversesBestPlayerWinsIn();
	};
}