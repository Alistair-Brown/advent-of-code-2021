#pragma once
#include <string>
#include <memory>
#include <map>
#include <cassert>
#include <set>

namespace CaveRouting
{
	const std::string StartName{ "start" };
	const std::string EndName{ "end" };

	class Cave
	{
	private:
		std::map<std::string, std::weak_ptr<Cave const>> linkedCaves{};
	public:
		std::string const name;
		bool IsEnd() const { return name == EndName; }
		bool const isSmall;

		Cave(std::string name, bool isSmall) : name{ name }, isSmall{ isSmall } {};

		void CreateCaveLink(std::weak_ptr<Cave const> caveToLink);
		std::map<std::string, std::weak_ptr<Cave const>> const& LinkedCaves() const { return linkedCaves; };

		bool operator==(const Cave& otherCave) const { return name == otherCave.name; };
		bool operator!=(const Cave& otherCave) const { return name != otherCave.name; };
		bool operator<(const Cave& otherCave) const { return name < otherCave.name; };
		bool operator>(const Cave& otherCave) const { return name > otherCave.name; };
	};

	struct RouteSoFar
	{
		bool doubleVisitRemaining;
		std::set<std::string> smallCavesVisited{};

		RouteSoFar(bool doubleVisitAllowed) : doubleVisitRemaining{ doubleVisitAllowed } {};

		bool operator==(const RouteSoFar& otherRoute) const {
			return (
				(doubleVisitRemaining == otherRoute.doubleVisitRemaining) &&
				(smallCavesVisited == otherRoute.smallCavesVisited)
				);
		};
		bool operator!=(const RouteSoFar& otherRoute) const { return !(*this == otherRoute); }
		bool operator<(const RouteSoFar& otherRoute) const {
			return (
				smallCavesVisited != otherRoute.smallCavesVisited ?
					smallCavesVisited < otherRoute.smallCavesVisited :
					doubleVisitRemaining < otherRoute.doubleVisitRemaining
				);
		};
		bool operator>(const RouteSoFar& otherRoute) const {
			return (
				smallCavesVisited != otherRoute.smallCavesVisited ?
				smallCavesVisited > otherRoute.smallCavesVisited :
				doubleVisitRemaining > otherRoute.doubleVisitRemaining
				);
		};
	};

	class CaveSystem
	{
	private:
		std::map<std::string, std::shared_ptr<Cave>> cavesInSystem{};
		std::shared_ptr<Cave> startCave{};

		std::map<std::pair<Cave, RouteSoFar>, unsigned int> cachedRoutesFromCaves{};

		unsigned int NumRoutesFromCave(std::weak_ptr<const Cave> cave, RouteSoFar routeSoFar);
		void CreateCave(std::string caveName);
	public:
		void LinkCaves(std::string caveNameOne, std::string caveNameTwo);
		unsigned int NumberOfRoutes(bool singleCaveDoubleVisitAllowed);
	};
}