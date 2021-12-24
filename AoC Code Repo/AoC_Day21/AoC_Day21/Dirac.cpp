#include "Dirac.h"

int Dirac::DeterministicDice::SumOfNextNRolls(int numberOfRolls)
{
	int sumOfRolls{ 0 };
	for (int ii = 0; ii < numberOfRolls; ii++)
	{
		sumOfRolls += nextNumberToRoll;
		numberOfTimesRolled++;
		nextNumberToRoll = (nextNumberToRoll == maxRollableNumber) ?
			1: nextNumberToRoll + 1;
	}
	return sumOfRolls;
}

void Dirac::DeterministicGame::UpdatePlayerScoreAndPosition(int diceRolls, int & playerPos, int & playerScore)
{
	playerPos += diceRolls;

	while (playerPos > maximumPosition)
	{
		playerPos -= maximumPosition;
	}
	playerScore += playerPos;
}

std::pair<int, int> Dirac::DeterministicGame::FinalScores()
{
	int nextDiceRoll;
	while (true)
	{
		nextDiceRoll = dice.SumOfNextNRolls(diceRollsPerMove);
		UpdatePlayerScoreAndPosition(nextDiceRoll, playerOnePosition, playerOneScore);
		if (playerOneScore >= scoreToWin) { break; }

		nextDiceRoll = dice.SumOfNextNRolls(diceRollsPerMove);
		UpdatePlayerScoreAndPosition(nextDiceRoll, playerTwoPosition, playerTwoScore);
		if (playerTwoScore >= scoreToWin) { break; }
	}

	return { playerOneScore, playerTwoScore };
}

std::vector<int> Dirac::DiracDice::PossibleSumsForNRolls(int numRolls)
{
	std::vector<int> possibleNumsToReturn;
	std::vector<int> numsAfterOneFewerRoll;

	if (numRolls == 1)
	{
		numsAfterOneFewerRoll.push_back(0);
	}
	else
	{
		numsAfterOneFewerRoll = PossibleSumsForNRolls(numRolls - 1);
	}

	for (int previousResult : numsAfterOneFewerRoll)
	{
		for (int ii = 1; ii <= maxRollableValue; ii++)
		{
			possibleNumsToReturn.push_back(previousResult + ii);
		}
	}

	return possibleNumsToReturn;
}

Dirac::DiracDice::DiracDice(int maxVal, int rolls)
{
	maxRollableValue = maxVal;
	rollsPerTurn = rolls;

	possibleSumsPerTurn = PossibleSumsForNRolls(rolls);
}

void Dirac::DiracGame::IncreasePossibilitiesForTurn(
	std::map<int, unsigned long long int>& existingTurnToPossibilityMap,
	int turnNums,
	unsigned long long int extraPossibilities)
{
	if (existingTurnToPossibilityMap.find(turnNums) == existingTurnToPossibilityMap.end())
	{
		existingTurnToPossibilityMap[turnNums] = extraPossibilities;
	}
	else
	{
		existingTurnToPossibilityMap[turnNums] += extraPossibilities;
	}
}

int Dirac::DiracGame::ScoreFromRoll(int currentPosition, int roll)
{
	int newPosition = currentPosition + roll;

	while (newPosition > maximumPosition)
	{
		newPosition -= maximumPosition;
	}
	return newPosition;
}

// Find all the scores you might get from rolling a dice at the current position.
// For any of those that don't reach the required score, subtract the score we got
// and use recursion to get the results from *that* score.
// Ways that we reached the score in a single turn are returned as ways to reach the
// score in one turn. For results we had to call recursively for, just add 1 to
// represent our own dice roll and return that.
std::map<int, unsigned long long int> Dirac::DiracGame::NumberOfWaysToAchieveScore(int currentPosition, int requiredScore)
{
	std::map<int, unsigned long long int> numberOfWaysToAchieveInNumberOfTurns;

	std::vector<int> possibleRolls = dice.PossibleSumsPerTurn();
	for (int roll : possibleRolls)
	{
		int scoreFromRoll = ScoreFromRoll(currentPosition, roll);
		if (scoreFromRoll >= requiredScore)
		{
			IncreasePossibilitiesForTurn(numberOfWaysToAchieveInNumberOfTurns, 1, 1);
		}
		else
		{
			std::map<int, unsigned long long int> waysToAchieveRemainingScore =
				NumberOfWaysToAchieveScore(scoreFromRoll, requiredScore - scoreFromRoll);
			for (auto const &mapping : waysToAchieveRemainingScore)
			{
				IncreasePossibilitiesForTurn(numberOfWaysToAchieveInNumberOfTurns, (mapping.first + 1), mapping.second);
			}
		}
	}

	return numberOfWaysToAchieveInNumberOfTurns;
}

// TODO: Lots of opportunities for caching being missed out on throughtout the stack below
// this point, which should speed up my program substantially.
unsigned long long int Dirac::DiracGame::NumberOfUniversesBestPlayerWinsIn()
{
	std::map<int, unsigned long long int> playerOnePlayPossibilities =
		NumberOfWaysToAchieveScore(playerOneStartPos, scoreToWin);
	std::map<int, unsigned long long int> playerTwoPlayPossibilities =
		NumberOfWaysToAchieveScore(playerTwoStartPos, scoreToWin);

	unsigned long long int playerOneWins{ 0 };
	unsigned long long int playerTwoWins{ 0 };
	for (std::pair<int, unsigned long long int> playerOneTurnPossibilities : playerOnePlayPossibilities)
	{
		int playerOneTurnsTaken = playerOneTurnPossibilities.first;
		unsigned long long int playerOneWaysToSpendTurns = playerOneTurnPossibilities.second;
		for (std::pair<int, unsigned long long int> playerTwoTurnPossibilities : playerOnePlayPossibilities)
		{
			int playerTwoTurnsTaken = playerTwoTurnPossibilities.first;
			unsigned long long int playerTwoWaysToSpendTurns = playerTwoTurnPossibilities.second;
			if (playerOneTurnsTaken <= playerTwoTurnsTaken)
			{
				playerOneWins += playerOneWaysToSpendTurns * playerTwoWaysToSpendTurns;
			}
			else
			{
				playerTwoWins += playerOneWaysToSpendTurns * playerTwoWaysToSpendTurns;
			}
		}
	}

	return ((playerOneWins > playerTwoWins) ? playerOneWins : playerTwoWins);
}
