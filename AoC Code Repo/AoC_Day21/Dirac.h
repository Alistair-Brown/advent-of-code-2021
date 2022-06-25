#pragma once
#include <utility>
#include <vector>
#include <map>

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

	class DiracDice
	{
	private:
		int maxRollableValue;
		std::vector<int> possibleSumsPerTurn;

		std::vector<int> PossibleSumsForNRolls(int numRolls);
	public:
		DiracDice(int maxVal);
		std::vector<int> PossibleSumsPerTurn() { return possibleSumsPerTurn; }
	};

	class DiracGame
	{
	private:
		unsigned int playerOneStartPos;
		unsigned int playerTwoStartPos;
		int scoreToWin;
		DiracDice dice;
		// The pair used as a key holds the current position, and the required score.
		// Then the map stored as a value holds the number of ways to achieve each
		// possible score (i.e. what gets returned by NumberOfWaysToAchieveScore().
		// This cache means that NumberOfWaysToAchieveScore will only actually
		// have to do real work less than 200 times in the whole program.
		std::map<std::pair<int, int>, std::map<int, unsigned long long int>> cachedWaysToAchieveScores;
		std::map<std::pair<int, int>, std::map<int, unsigned long long int>> cachedExactWaysToAchieveScores;

		void IncreasePossibilitiesForTurn(
			std::map<int, unsigned long long int> &existingTurnToPossibilityMap,
			int turnNums,
			unsigned long long int extraPossibilities);
		int ScoreFromRoll(int currentPosition, int roll);
		std::map<int, unsigned long long int> NumberOfWaysToAchieveScore(int currentPosition, int requiredScore, bool exact);
		unsigned long long NumberOfWaysToNotReachScoreInTurns(int currentPosition, int scoreToMiss, int turnLimit);
	public:
		DiracGame(int scoreToWin, int diceMaxVal, unsigned int playerOnePos, unsigned int playerTwoPos) :
			scoreToWin{ scoreToWin },
			dice{ diceMaxVal },
			playerOneStartPos{playerOnePos},
			playerTwoStartPos{ playerTwoPos }{};
		unsigned long long int NumberOfUniversesBestPlayerWinsIn();
	};
}