#pragma once
#include <map>
#include <vector>
#include <string>
#include <set>

namespace Caves
{
	const std::string StartName{ "start" };
	const std::string EndName{ "end" };

	class Cave
	{
	private:
		std::string name;
		bool isSmall;
		std::map<std::string, Cave const *> linkedCaves{};
		std::vector<Cave const *> linkedCavesAsVector{};
	public:
		Cave(std::string name, bool isSmall) : name{ name }, isSmall { isSmall } {};
		bool IsSmall() const { return isSmall; }
		bool IsEnd() { return name == EndName; }
		std::string Name() const { return name; }
		void CreateCaveLink(std::string caveName, Cave const *caveToLink);
		std::vector<Cave const *> LinkedCaves() const { return linkedCavesAsVector; }
	};

	class CaveSystem
	{
	private:
		std::map<std::string, Cave*> cavesInSystem{};
		void CreateCave(std::string caveName);
	public:

		// I need a destructor to free off the Cave objects that this class will create.
		// I don't think I'll ever be passing instances of this class around, so I'll hold off
		// on the Copy and Move constructors for now.
		~CaveSystem();
		Cave const *StartCave();		
		void LinkCaves(std::string caveNameOne, std::string caveNameTwo);
	};

	struct CaveVisitTracker
	{
		int timesVisited{ 0 };
		Cave const *cave{};
		CaveVisitTracker() {};
		CaveVisitTracker(Cave const *cave) : cave{ cave } {};
		CaveVisitTracker(Cave const *cave, int initialVisits) : cave{ cave }, timesVisited{ initialVisits } {};
	};

	class Route
	{
	private:
		std::map<std::string, CaveVisitTracker> visitedCaves{};
		std::vector<std::string> visitedCavesInOrder{};
		Cave const * currentCave;
		bool haveDoubleVisited{ false };
	public:
		Route(std::string startingCaveName, Cave const *startingCave);
		bool AttemptToMoveToCave(std::string caveName, Cave const *cave, int maxVisitsToSmallCave);
		void PrintCompleteRoute();
		Cave const * CurrentCave() { return currentCave; }
	};

	unsigned int NumberOfRoutes(CaveSystem &caveSystem, int visitsAllowedPerSmallCave);
}