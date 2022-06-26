#include "Dirac.h"

// Roll the deterministic dice the required number of times and return the sum of those
// rolls.
// The Deterministic dice simply returns the next number in an incrementing sequence
// with each roll, wrapping back to 1 once it reaches its maximum value.
// The Dice also keeps track of how many times it has been rolled.
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
// score as per the rules of the game. The game ends immediately when either player reaches
// the score required to win.
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

// Move a player the required number of spaces around the Dirac board, accounting for the fact that
// the board is circular and wraps back to 1 after reaching the maximum space. The player's score is
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

// As part of initialising the DiracDice, set up the vector of possible values it could
// produce on a given turn, to save us calculating this on each call.
Dirac::DiracDice::DiracDice(unsigned int maxVal)
{
	maxRollableValue = maxVal;
	possibleSumsPerTurn = PossibleSumsForNRolls(DICE_ROLLS_PER_TURN);
}

// Return a vector of the possible totals this dice could produce in N rolls. e.g.
// a dice that can only roll 1 or 2, could produce with 2 rolls the numbers: 2 (1+1),
// 3 (1+2), 3 (2+1) or 4 (2+2).
// This function is recursive. It first calls itself to find the possible sum for n-1 rolls
// Then for each element in that vector, it adds each of the possible rollable numbers to
// produce a larger vector of the possible sums after n rolls.
std::vector<unsigned int> Dirac::DiracDice::PossibleSumsForNRolls(unsigned int numRolls)
{
	std::vector<unsigned int> possibleNumsToReturn;
	std::vector<unsigned int> numsAfterOneFewerRoll;

	// If we're calculating the 1 roll case, then we only have one possible starting point
	// for this nth roll, which is 0 from 0 rolls so far. Otherwise, we need to find the
	// possible sums for n-1, to extend for this nth roll.
	if (numRolls == 1)
	{
		numsAfterOneFewerRoll.push_back(0);
	}
	else
	{
		numsAfterOneFewerRoll = PossibleSumsForNRolls(numRolls - 1);
	}

	// For each of the results we could have got from the previous n-1 rolls, add each
	// possible rollable value to produce the possible sums after this nth roll.
	for (unsigned int previousResult : numsAfterOneFewerRoll)
	{
		for (unsigned int ii = 1; ii <= maxRollableValue; ii++)
		{
			possibleNumsToReturn.push_back(previousResult + ii);
		}
	}

	return possibleNumsToReturn;
}

// For this universe-splitting game of Dirac Dice, find out which player wins in the most
// of the possible realities. Return the number of realities this player wins in.
ULLINT Dirac::DiracGame::NumberOfUniversesBestPlayerWinsIn()
{
	// Each player's map of possibilities maps a number of turns it would be possible for them to
	// reach the required score in, to the number of ways they could reach that required score (or
	// higher) in exactly that many turns.
	std::map<unsigned int, ULLINT> playerOnePlayPossibilities =
		NumberOfWaysToAchieveScore(playerOneStartPos, scoreToWin, false);
	std::map<unsigned int, ULLINT> playerTwoPlayPossibilities =
		NumberOfWaysToAchieveScore(playerTwoStartPos, scoreToWin, false);

	// Having calculated every possible way each player could reach the required score, we now
	// need to convert this information into a number of possible wins for each player:
	//  - For each number of turns a given player could take to reach the required score, find
	//    the number of ways the other player could *fail* to reach that required score.
	//  - Note that player 1 goes first, so if player 1 takes n turns to reach the score, player
	//    2 will only have had n-1 turns by that point (as the game ends immediately when a
	//    player reaches the required score.
	//  - Multiply the number of ways the winning player could reach that score in that number
	//    of turns by the number of ways the other player could fail to reach that score, and
	//    we have the total number of universes in which the winning player could take that
	//    many turns to reach the required score and win as a result.
	//  - Repeat for each number of turns that player could take to reach the required score.
	ULLINT playerOneWins{ 0 };
	ULLINT playerTwoWins{ 0 };
	for (std::pair<unsigned int, ULLINT> const &playerOneTurnPossibilities : playerOnePlayPossibilities)
	{
		unsigned int playerOneTurnsTaken = playerOneTurnPossibilities.first;
		ULLINT playerOneWaysToSpendTurns = playerOneTurnPossibilities.second;

		playerOneWins += playerOneWaysToSpendTurns *
			NumberOfWaysToNotReachScoreInTurns(playerTwoStartPos, scoreToWin, playerOneTurnsTaken - 1);
	}
	for (std::pair<unsigned int, ULLINT> const &playerTwoTurnPossibilities : playerTwoPlayPossibilities)
	{
		unsigned int playerTwoTurnsTaken = playerTwoTurnPossibilities.first;
		ULLINT playerTwoWaysToSpendTurns = playerTwoTurnPossibilities.second;

		playerTwoWins += playerTwoWaysToSpendTurns *
			NumberOfWaysToNotReachScoreInTurns(playerOneStartPos, scoreToWin, playerTwoTurnsTaken);
	}

	return ((playerOneWins > playerTwoWins) ? playerOneWins : playerTwoWins);
}

// Given a current position, find the number of ways a player could reach a required score.
// The 'exactScore' parameter determines whether the player must hit *exactly* that score, or
// or whether it is acceptable to exceed that score on the turn that it is reached.
// Return value is a map containing each number of turns it could take to reach that score as
// the key, and the number of ways that score could be reached in that many turns as the value.
std::map<unsigned int, ULLINT> Dirac::DiracGame::NumberOfWaysToAchieveScore(
	unsigned int currentPosition, unsigned int requiredScore, bool exactScore)
{
	// First check our cached results in case we have already been asked this exact query before
	// and can return without recalculating.
	std::map<std::pair<unsigned int, unsigned int>, std::map<unsigned int, ULLINT>>::iterator returnedCachedExactValue =
		cachedExactWaysToAchieveScores.find({ currentPosition, requiredScore });
	std::map<std::pair<unsigned int, unsigned int>, std::map<unsigned int, ULLINT>>::iterator returnedCachedNonExactValue =
		cachedWaysToAchieveScores.find({ currentPosition, requiredScore });
	if (exactScore && (returnedCachedExactValue != cachedExactWaysToAchieveScores.end()))
	{
		return returnedCachedExactValue->second;
	}
	else if (!exactScore && (returnedCachedNonExactValue != cachedWaysToAchieveScores.end()))
	{
		return returnedCachedNonExactValue->second;
	}

	// The process for finding the number of ways to achieve a given score is as follows:
	//  - Take every possible result from the sum of dice rolls for a single turn
	//  - For any of those results that meet or exceed the required score, increment
	//    our map to reflect a single way in which the result could be achieved in one turn.
	//    If we are only counting results that achieve the score *exactly*, then any rolls
	//    that exceed the score are discarded.
	//  - For any roll that came in below the remaining score, call ourselves recursively
	//    to find the number of ways to achieve the remaining score from the new position.
	//    To convert the map returned by that call into the equivalent elements in our own
	//    map of possibilities, we simply need to add 1 to each key to account for the extra
	//    turn we just took in addition to each turn taken in that map.
	std::map<unsigned int, ULLINT> numberOfWaysToAchieveInNumberOfTurns;
	std::vector<unsigned int> possibleRolls = dice.PossibleSumsPerTurn();
	for (unsigned int roll : possibleRolls)
	{
		unsigned int scoreFromRoll = PositionAfterRoll(currentPosition, roll);
		if ((scoreFromRoll == requiredScore) ||
			(!exactScore && (scoreFromRoll > requiredScore)))
		{
			IncreasePossibilitiesForTurn(numberOfWaysToAchieveInNumberOfTurns, 1, 1);
		}
		else if (scoreFromRoll < requiredScore)
		{
			std::map<unsigned int, ULLINT> waysToAchieveRemainingScore =
				NumberOfWaysToAchieveScore(scoreFromRoll, requiredScore - scoreFromRoll, exactScore);
			for (auto const& mapping : waysToAchieveRemainingScore)
			{
				IncreasePossibilitiesForTurn(numberOfWaysToAchieveInNumberOfTurns, (mapping.first + 1), mapping.second);
			}
		}
	}

	// Cache our result before returning so that we don't need to re-calculate the same query
	// again in future.
	if (exactScore)
	{
		cachedExactWaysToAchieveScores[{ currentPosition, requiredScore }] =
			numberOfWaysToAchieveInNumberOfTurns;
	}
	else
	{
		cachedWaysToAchieveScores[{ currentPosition, requiredScore }] =
			numberOfWaysToAchieveInNumberOfTurns;
	}
	return numberOfWaysToAchieveInNumberOfTurns;
}

// The number of ways that a player could spend a certain number of turns without reaching
// a particular score is equal to the number of ways they could achieve a lower score in that
// number of turns.
ULLINT Dirac::DiracGame::NumberOfWaysToNotReachScoreInTurns(
	unsigned int currentPosition,
	unsigned int scoreToMiss,
	unsigned int turnLimit)
{
	ULLINT numberOfWaysToFail{ 0 };

	// Since we are considering each possible 'lower than target' score separately, it's important
	// that we only consider ways to achieve *exactly* that score on each loop, otherwise
	// we will end up double counting.
	for (unsigned int possibleScore = 1; possibleScore < scoreToMiss; possibleScore++)
	{
		numberOfWaysToFail +=
			NumberOfWaysToAchieveScore(currentPosition, possibleScore, true)[turnLimit];
	}
	return numberOfWaysToFail;
}

// Given an existing map of "number of turns" to "number of ways a score could be achieved in
// that many turns", increase the number of possibilities for a given turn number. If there
// isn't already an entry for that number of turns, add one.
void Dirac::DiracGame::IncreasePossibilitiesForTurn(
	std::map<unsigned int, ULLINT>& existingTurnToPossibilityMap,
	unsigned int turnNums,
	ULLINT extraPossibilities)
{
	if (existingTurnToPossibilityMap.contains(turnNums))
	{
		existingTurnToPossibilityMap[turnNums] += extraPossibilities;
	}
	else
	{
		existingTurnToPossibilityMap[turnNums] = extraPossibilities;
	}
}

// Given a starting position and roll, move 'roll' number of spaces accounting for the fact that
// the board is circular and wraps back to 1 after reaching the maximum space. Return the new
// position.
unsigned int Dirac::DiracGame::PositionAfterRoll(unsigned int currentPosition, unsigned int roll)
{
	unsigned int newPosition = currentPosition + roll;

	while (newPosition > SPACES_ON_DIRAC_BOARD)
	{
		newPosition -= SPACES_ON_DIRAC_BOARD;
	}
	return newPosition;
}
