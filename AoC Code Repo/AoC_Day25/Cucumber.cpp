#include "Cucumber.h"
#include <cassert>

// Returns true if at least one cucumber moved
bool Cucumber::Trench::CarryOutSingleMovementTurn()
{
	// We first work out which cucumbers can move, and then move that whole direction
	// at once, so need a couple of intermediate structures, one to hold cucumbers which
	// are about to move, and another to be a snapshot of the unblocked cucumbers in a direction,
	// since the actual unblocked set may change as cucumbers discover they are actually blocked
	// while working out if they can move.
	std::vector<SeaCucumber *>cucumbersToMove;
	std::set<SeaCucumber *>unblockedCucumbers = unblockedEastCucumbers;
	bool atLeastOneCucumberHasMoved{ false };
	for (SeaCucumber *cucumber : unblockedCucumbers)
	{
		if (cucumber->PossibleToMove())
		{
			atLeastOneCucumberHasMoved = true;
			cucumbersToMove.push_back(cucumber);
		}
	}
	for (SeaCucumber *cucumber : cucumbersToMove)
	{
		cucumber->CarryOutMove();
	}

	unblockedCucumbers = unblockedSouthCucumbers;
	cucumbersToMove.clear();
	for (SeaCucumber *cucumber : unblockedCucumbers)
	{
		if (cucumber->PossibleToMove())
		{
			atLeastOneCucumberHasMoved = true;
			cucumbersToMove.push_back(cucumber);
		}
	}
	for (SeaCucumber *cucumber : cucumbersToMove)
	{
		cucumber->CarryOutMove();
	}

	return atLeastOneCucumberHasMoved;
}

Cucumber::Trench::Trench(std::vector<std::string> trenchLayout)
{
	// Initialise the trench with null pointers
	trenchOfCucumbers = std::vector<std::vector<SeaCucumber *>>(trenchLayout.size(), { trenchLayout[0].size(), nullptr });

	// As we iterate over our input to find each cucumber, we need to know whether a cucumber should
	// go on the blocked or unblocked list. And we need to know whether that cucmber is blocking another,
	// so that we can unblock that other cucumber once the current cucumber moves.
	for (unsigned int row = 0; row < trenchLayout.size(); row++)
	{
		for (unsigned int column = 0; column < trenchLayout[0].size(); column++)
		{
			char gridElement = trenchLayout[row][column];
			assert((gridElement == SouthMovingCucumber) ||
				(gridElement == EastMovingCucumber) ||
				(gridElement == EmptySpace));

			if (gridElement != EmptySpace)
			{
				SeaCucumber *thisCucumber;
				char spaceToMoveTo;
				SeaCucumber *cucumberNorth;
				SeaCucumber *cucumberWest;

				if (gridElement == SouthMovingCucumber)
				{
					thisCucumber = new SeaCucumber({ row, column }, CucumberDirection::South, this);

					spaceToMoveTo = (row == (trenchLayout.size() - 1)) ?
						trenchLayout[0][column] : trenchLayout[row + 1][column];
					cucumberWest = (column == 0) ?
						trenchOfCucumbers[row][trenchLayout[0].size() - 1] :
						trenchOfCucumbers[row][column - 1];
					cucumberNorth = (row == 0) ?
						trenchOfCucumbers[trenchLayout.size() - 1][column] :
						trenchOfCucumbers[row - 1][column];

					if (spaceToMoveTo == EmptySpace)
					{
						unblockedSouthCucumbers.insert(thisCucumber);
					}
					else
					{
						blockedSouthCucumbers.insert(thisCucumber);

						// If we're wrapping from bottom to top, the blocking cucumber won't
						// have known it was blocking when it was instantiated, so tell it
						// now.
						if (row == trenchOfCucumbers.size() - 1)
						{
							trenchOfCucumbers[0][column]->AddBlockingCucumber(thisCucumber);
						}
					}
				}
				else
				{
					thisCucumber = new SeaCucumber({ row, column }, CucumberDirection::East, this);

					spaceToMoveTo = (column == (trenchLayout[0].size() - 1)) ?
						trenchLayout[row][0] : trenchLayout[row][column + 1];
					cucumberWest = (column == 0) ?
						trenchOfCucumbers[row][trenchLayout[0].size() - 1] :
						trenchOfCucumbers[row][column - 1];
					cucumberNorth = (row == 0) ?
						trenchOfCucumbers[trenchLayout.size() - 1][column] :
						trenchOfCucumbers[row - 1][column];

					if (spaceToMoveTo == EmptySpace)
					{
						unblockedEastCucumbers.insert(thisCucumber);
					}
					else
					{
						blockedEastCucumbers.insert(thisCucumber);

						// If we're wrapping from right to left, the blocking cucumber won't
						// have known it was blocking when it was instantiated, so tell it
						// now.
						if (column == trenchOfCucumbers[0].size() - 1)
						{
							trenchOfCucumbers[row][0]->AddBlockingCucumber(thisCucumber);
						}
					}
				}

				trenchOfCucumbers[row][column] = thisCucumber;
				if ((cucumberNorth != nullptr) && (cucumberNorth->MovementDirection() == CucumberDirection::South))
				{
					thisCucumber->AddBlockingCucumber(cucumberNorth);
				}
				if ((cucumberWest != nullptr) && (cucumberWest->MovementDirection() == CucumberDirection::East))
				{
					thisCucumber->AddBlockingCucumber(cucumberWest);
				}
			}
		}
	}
}

Cucumber::Trench::~Trench()
{
	for (std::vector<SeaCucumber *>trenchRow : trenchOfCucumbers)
	{
		for (SeaCucumber *cucumber : trenchRow)
		{
			delete cucumber;
		}
	}

}

// If it's possible to move, return the new location, taking wrapping across the edges of the
// grid into account. If movement isn't possible, return the current location back.
std::pair<int, int> Cucumber::Trench::PossibleToMove(std::pair<int, int> currentLocation, CucumberDirection direction)
{
	std::pair<int, int> desiredLocation;
	if (direction == CucumberDirection::East)
	{
		desiredLocation = (currentLocation.second == (trenchOfCucumbers[0].size() - 1)) ?
			std::pair<int,int>{currentLocation.first, 0} :
			std::pair<int, int>{currentLocation.first, currentLocation.second + 1};
	}
	else
	{
		desiredLocation = (currentLocation.first == (trenchOfCucumbers.size() - 1)) ?
			std::pair<int, int>{0, currentLocation.second} :
			std::pair<int, int>{ currentLocation.first + 1, currentLocation.second };
	}

	if (trenchOfCucumbers[desiredLocation.first][desiredLocation.second] == nullptr)
	{
		return desiredLocation;
	}
	else
	{
		return currentLocation;
	}
}

unsigned long long int Cucumber::Trench::MoveUntilAllBlocked()
{
	int numberOfMoves{ 0 };

	while (true)
	{
		// The first turn in which no cucumbers move is itself counted as a turn, so
		// increment before attempting the move.
		numberOfMoves++;
		if (!CarryOutSingleMovementTurn())
		{
			break;
		}
	}
	
	return numberOfMoves;
}

void Cucumber::Trench::CucumberHasMovedToLocation(
	SeaCucumber * cucumber,
	std::pair<int, int> newLocation,
	std::pair<int, int> oldLocation)
{
	assert(trenchOfCucumbers[oldLocation.first][oldLocation.second] == cucumber);
	assert(trenchOfCucumbers[newLocation.first][newLocation.second] == nullptr);

	trenchOfCucumbers[oldLocation.first][oldLocation.second] = nullptr;
	trenchOfCucumbers[newLocation.first][newLocation.second] = cucumber;
}

void Cucumber::Trench::CucumberHasBecomeUnblocked(SeaCucumber * cucumber)
{
	if (cucumber->MovementDirection() == CucumberDirection::East)
	{
		assert(blockedEastCucumbers.find(cucumber) != blockedEastCucumbers.end());
		blockedEastCucumbers.erase(cucumber);
		unblockedEastCucumbers.insert(cucumber);
	}
	else
	{
		assert(blockedSouthCucumbers.find(cucumber) != blockedSouthCucumbers.end());
		blockedSouthCucumbers.erase(cucumber);
		unblockedSouthCucumbers.insert(cucumber);
	}
}

void Cucumber::Trench::CucumberHasBecomeBlocked(SeaCucumber * cucumber)
{
	if (cucumber->MovementDirection() == CucumberDirection::East)
	{
		assert(unblockedEastCucumbers.find(cucumber) != unblockedEastCucumbers.end());
		unblockedEastCucumbers.erase(cucumber);
		blockedEastCucumbers.insert(cucumber);

		SeaCucumber *blockingCucumber = (cucumber->Location().second == (trenchOfCucumbers[0].size() - 1)) ?
			trenchOfCucumbers[cucumber->Location().first][0] :
			trenchOfCucumbers[cucumber->Location().first][cucumber->Location().second + 1];
		assert(blockingCucumber != nullptr);
		blockingCucumber->AddBlockingCucumber(cucumber);
	}
	else
	{
		assert(unblockedSouthCucumbers.find(cucumber) != unblockedSouthCucumbers.end());
		unblockedSouthCucumbers.erase(cucumber);
		blockedSouthCucumbers.insert(cucumber);

		SeaCucumber *blockingCucumber = (cucumber->Location().first == (trenchOfCucumbers.size() - 1)) ?
			trenchOfCucumbers[0][cucumber->Location().second] :
			trenchOfCucumbers[cucumber->Location().first + 1][cucumber->Location().second];
		assert(blockingCucumber != nullptr);
		blockingCucumber->AddBlockingCucumber(cucumber);
	}
}

bool Cucumber::SeaCucumber::PossibleToMove()
{
	bool possibleToMove{ false };

	std::pair<int, int> newLocation = trench->PossibleToMove(location, movementDirection);
	if (newLocation != location)
	{
		possibleToMove = true;
	}
	else
	{
		trench->CucumberHasBecomeBlocked(this);
	}

	return possibleToMove;
}

void Cucumber::SeaCucumber::CarryOutMove()
{
	std::pair<int, int> newLocation = trench->PossibleToMove(location, movementDirection);
	assert(location != newLocation);
	std::pair<int, int> oldLocation = location;
	location = newLocation;

	trench->CucumberHasMovedToLocation(this, location, oldLocation);
	for (SeaCucumber *cucumber : blockingCucumbers)
	{
		trench->CucumberHasBecomeUnblocked(cucumber);
	}
	blockingCucumbers.clear();
}
