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

		void IncreasePossibilitiesForTurn(
			std::map<int, unsigned long long int> &existingTurnToPossibilityMap,
			int turnNums,
			unsigned long long int extraPossibilities);
		int ScoreFromRoll(int currentPosition, int roll);
		std::map<int, unsigned long long int> NumberOfWaysToAchieveScore(int currentPosition, int requiredScore);
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