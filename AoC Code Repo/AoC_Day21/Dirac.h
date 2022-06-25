#pragma once
#include <utility>
#include <vector>
#include <map>

namespace Dirac
{
	class DeterministicDice
	{
	private:
		int maxRollableNumber;
		int nextNumberToRoll{ 1 };
		int numberOfTimesRolled{ 0 };
	public:
		DeterministicDice(int maxRollableNumber) : maxRollableNumber{ maxRollableNumber } {};
		int NumberOfTimesRolled() { return numberOfTimesRolled; }
		int SumOfNextNRolls(int numberOfRolls);
	};

	class DeterministicGame
	{
	private:
		int maximumPosition;
		int scoreToWin;
		int diceRollsPerMove;
		DeterministicDice &dice;

		int playerOnePosition;
		int playerTwoPosition;
		int playerOneScore{ 0 };
		int playerTwoScore{ 0 };

		void UpdatePlayerScoreAndPosition(int diceRolls, int &playerPos, int &playerScore);
	public:
		DeterministicGame(DeterministicDice &dice,
			  	  int diceRolls,
			  	  int maxPos,
				  int scoreToWin,
				  int playerOneStartingPosition,
				  int playerTwoStartingPosition) :
			dice{ dice },
			diceRollsPerMove {diceRolls},
			maximumPosition{ maxPos },
			scoreToWin{ scoreToWin },
			playerOnePosition{ playerOneStartingPosition },
			playerTwoPosition{ playerTwoStartingPosition }{};
		std::pair<int, int> FinalScores();
	};

	class DiracDice
	{
	private:
		int maxRollableValue;
		int rollsPerTurn;
		std::vector<int> possibleSumsPerTurn;

		std::vector<int> PossibleSumsForNRolls(int numRolls);
	public:
		DiracDice(int maxVal, int rolls);
		std::vector<int> PossibleSumsPerTurn() { return possibleSumsPerTurn; }
	};

	class DiracGame
	{
	private:
		int playerOneStartPos;
		int playerTwoStartPos;
		int maximumPosition;
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
		DiracGame(int maxPos, int scoreToWin, int diceRolls, int diceMaxVal, int playerOnePos, int playerTwoPos) :
			maximumPosition{ maxPos },
			scoreToWin{ scoreToWin },
			dice{ diceMaxVal, diceRolls },
			playerOneStartPos{playerOnePos},
			playerTwoStartPos{ playerTwoPos }{};
		unsigned long long int NumberOfUniversesBestPlayerWinsIn();
	};
}