#pragma once
#include <utility>
#include <vector>
#include <string>
#include <set>

namespace Cucumber
{
	enum CucumberDirection {East, South};
	const char SouthMovingCucumber = 'v';
	const char EastMovingCucumber = '>';
	const char EmptySpace = '.';

	class Trench;

	class SeaCucumber
	{
	private:
		std::pair<int, int> location;
		Trench *trench;
		CucumberDirection movementDirection;
		// Blocking here refers to cucumbers that *this* cucumber is blocking from moving.
		std::vector<SeaCucumber *>blockingCucumbers;
	public:
		SeaCucumber(std::pair<int, int> location, CucumberDirection direction, Trench *trench) :
			location{ location },
			movementDirection{ direction },
			trench{ trench }{};

		CucumberDirection MovementDirection() { return movementDirection; }
		std::pair<int, int> Location() { return location; }
		bool PossibleToMove();
		void CarryOutMove();
		void AddBlockingCucumber(SeaCucumber *cucumber) { blockingCucumbers.push_back(cucumber); }
	};

	class Trench
	{
	private:
		std::vector<std::vector<SeaCucumber *>>trenchOfCucumbers;
		std::set<SeaCucumber *>unblockedEastCucumbers;
		std::set<SeaCucumber *>unblockedSouthCucumbers;
		std::set<SeaCucumber *>blockedEastCucumbers;
		std::set<SeaCucumber *>blockedSouthCucumbers;

		bool CarryOutSingleMovementTurn();
	public:
		Trench(std::vector<std::string> trenchLayout);
		~Trench();

		std::pair<int, int> PossibleToMove(std::pair<int, int> currentLocation, CucumberDirection direction);
		unsigned long long int MoveUntilAllBlocked();
		void CucumberHasMovedToLocation(SeaCucumber *cucumber, std::pair<int, int> newLocation, std::pair<int, int> oldLocation);
		void CucumberHasBecomeUnblocked(SeaCucumber *cucumber);
		void CucumberHasBecomeBlocked(SeaCucumber *cucumber);
	};
}