#include "Dirac.h"

// Roll the deterministic dice the required number of times and return the sum of those
// rolls.
// The Deterministic dice simply returns the next number in an incrementing sequence
// with each roll, wrapping back to 1 once it reaches its maximum value.
// The Dice keeps track of how many times it has been rolled.
unsigned int Dirac::DeterministicDice::SumOfNextNRolls(unsigned int numberOfRolls)
{
	unsigned int sumOfRolls{ 0 };
	for (unsigned int ii = 0; ii < numberOfRolls; ii++)
	{
		sumOfRolls += nextNumberToRoll;
		numberOfTimesRolled++;
		nextNumberToRoll = (nextNumberToRoll == maxRollableNumber) ?
			1: nextNumberToRoll + 1;
	}
	return sumOfRolls;
}

// Return the final scores of the two players after a fully deterministic game of Dirac
// Dice. If the game has not yet been played, play it first to determine those scores.
std::pair<unsigned int, unsigned int> Dirac::DeterministicGame::FinalScores()
{
	if (!gamePlayed)
	{
		PlayGame();
	}

	return { playerOneScore, playerTwoScore };
}

// Play a deterministic game of Dirac Dice. Do this by taking turns for each player to roll
// the deterministic dice the permitted number of times, and move their piece to increase their
// scode as per the rules of the game. The game ends immediately when either player reaches
// the required score to win.
void Dirac::DeterministicGame::PlayGame()
{
	unsigned int nextDiceRoll;
	while (true)
	{
		nextDiceRoll = dice.SumOfNextNRolls(DICE_ROLLS_PER_TURN);
		UpdatePlayerScoreAndPosition(nextDiceRoll, playerOnePosition, playerOneScore);
		if (playerOneScore >= scoreToWin) { break; }

		nextDiceRoll = dice.SumOfNextNRolls(DICE_ROLLS_PER_TURN);
		UpdatePlayerScoreAndPosition(nextDiceRoll, playerTwoPosition, playerTwoScore);
		if (playerTwoScore >= scoreToWin) { break; }
	}
	gamePlayed = true;
}

// Move a player the required number of spaces around the Dirac board, accounting the circular
// nature of the board when handling wrapping past the maximum position. The player's score is
// incremented by the number of the position they land on.
void Dirac::DeterministicGame::UpdatePlayerScoreAndPosition(
	unsigned int spacesToMove,
	unsigned int& playerPos,
	unsigned int& playerScore)
{
	playerPos += spacesToMove;

	while (playerPos > SPACES_ON_DIRAC_BOARD)
	{
		playerPos -= SPACES_ON_DIRAC_BOARD;
	}
	playerScore += playerPos;
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

Dirac::DiracDice::DiracDice(int maxVal)
{
	maxRollableValue = maxVal;

	possibleSumsPerTurn = PossibleSumsForNRolls(DICE_ROLLS_PER_TURN);
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

	while (newPosition > SPACES_ON_DIRAC_BOARD)
	{
		newPosition -= SPACES_ON_DIRAC_BOARD;
	}
	return newPosition;
}

// Find all the scores you might get from rolling a dice at the current position.
// For any of those that don't reach the required score, subtract the score we got
// and use recursion to get the results from *that* score.
// Ways that we reached the score in a single turn are returned as ways to reach the
// score in one turn. For results we had to call recursively for, just add 1 to
// represent our own dice roll and return that.
std::map<int, unsigned long long int> Dirac::DiracGame::NumberOfWaysToAchieveScore(
	int currentPosition, int requiredScore, bool exact)
{
	std::map<int, unsigned long long int> numberOfWaysToAchieveInNumberOfTurns;

	std::map<std::pair<int,int>,std::map<int, unsigned long long int>>::iterator returnedCachedExactValue =
		cachedExactWaysToAchieveScores.find({ currentPosition, requiredScore });
	std::map<std::pair<int, int>, std::map<int, unsigned long long int>>::iterator returnedCachedNonExactValue =
		cachedWaysToAchieveScores.find({ currentPosition, requiredScore });
	if (exact && (returnedCachedExactValue != cachedExactWaysToAchieveScores.end()))
	{
		numberOfWaysToAchieveInNumberOfTurns = returnedCachedExactValue->second;
	}
	else if (!exact && (returnedCachedNonExactValue != cachedWaysToAchieveScores.end()))
	{
		numberOfWaysToAchieveInNumberOfTurns = returnedCachedNonExactValue->second;
	}
	else
	{
		std::vector<int> possibleRolls = dice.PossibleSumsPerTurn();
		for (int roll : possibleRolls)
		{
			int scoreFromRoll = ScoreFromRoll(currentPosition, roll);
			if ((scoreFromRoll == requiredScore) ||
				(!exact && (scoreFromRoll > requiredScore)))
			{
				IncreasePossibilitiesForTurn(numberOfWaysToAchieveInNumberOfTurns, 1, 1);
			}
			else if (scoreFromRoll < requiredScore)
			{
				std::map<int, unsigned long long int> waysToAchieveRemainingScore =
					NumberOfWaysToAchieveScore(scoreFromRoll, requiredScore - scoreFromRoll, exact);
				for (auto const &mapping : waysToAchieveRemainingScore)
				{
					IncreasePossibilitiesForTurn(numberOfWaysToAchieveInNumberOfTurns, (mapping.first + 1), mapping.second);
				}
			}
		}

		if (exact)
		{
			cachedExactWaysToAchieveScores[{ currentPosition, requiredScore }] =
				numberOfWaysToAchieveInNumberOfTurns;
		}
		else
		{
			cachedWaysToAchieveScores[{ currentPosition, requiredScore }] =
				numberOfWaysToAchieveInNumberOfTurns;
		}
	}

	return numberOfWaysToAchieveInNumberOfTurns;
}

// Number of ways a player can spend a certain number of turns without reaching
// a particular score.
// The number of ways they could do this is equal to the number of ways they
// could achieve a lower score in that number of turns.
unsigned long long Dirac::DiracGame::NumberOfWaysToNotReachScoreInTurns(int currentPosition, int scoreToMiss, int turnLimit)
{
	unsigned long long int numberOfWaysToFail{ 0 };
	for (int possibleScore = 1; possibleScore < scoreToMiss; possibleScore++)
	{
		numberOfWaysToFail +=
			NumberOfWaysToAchieveScore(currentPosition, possibleScore, true)[turnLimit];
	}
	return numberOfWaysToFail;
}

unsigned long long int Dirac::DiracGame::NumberOfUniversesBestPlayerWinsIn()
{
	std::map<int, unsigned long long int> playerOnePlayPossibilities =
		NumberOfWaysToAchieveScore(playerOneStartPos, scoreToWin, false);
	std::map<int, unsigned long long int> playerTwoPlayPossibilities =
		NumberOfWaysToAchieveScore(playerTwoStartPos, scoreToWin, false);

	unsigned long long int playerOneWins{ 0 };
	unsigned long long int playerTwoWins{ 0 };
	for (std::pair<int, unsigned long long int> playerOneTurnPossibilities : playerOnePlayPossibilities)
	{
		int playerOneTurnsTaken = playerOneTurnPossibilities.first;
		unsigned long long int playerOneWaysToSpendTurns = playerOneTurnPossibilities.second;

		playerOneWins += playerOneWaysToSpendTurns *
			NumberOfWaysToNotReachScoreInTurns(playerTwoStartPos, scoreToWin, playerOneTurnsTaken - 1);
	}
	for (std::pair<int, unsigned long long int> playerTwoTurnPossibilities : playerTwoPlayPossibilities)
	{
		int playerTwoTurnsTaken = playerTwoTurnPossibilities.first;
		unsigned long long int playerTwoWaysToSpendTurns = playerTwoTurnPossibilities.second;

		playerTwoWins += playerTwoWaysToSpendTurns *
			NumberOfWaysToNotReachScoreInTurns(playerOneStartPos, scoreToWin, playerTwoTurnsTaken);
	}

	return ((playerOneWins > playerTwoWins) ? playerOneWins : playerTwoWins);
}
