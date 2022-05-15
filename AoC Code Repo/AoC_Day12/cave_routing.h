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

	// A cave in the network has a name and a size, and can be linked to other caves in
	// the network.
	class Cave
	{
	private:
		std::map<std::string, std::weak_ptr<Cave const>> linkedCaves{};
	public:
		std::string const name;
		bool const isSmall;

		Cave(std::string name, bool isSmall) : name{ name }, isSmall{ isSmall } {};

		void CreateCaveLink(std::weak_ptr<Cave const> caveToLink);
		std::map<std::string, std::weak_ptr<Cave const>> const& LinkedCaves() const { return linkedCaves; };

		// These cave objects are inserted in a various containers which require comparison
		// operators to be implemented. Comparison of caves can be done lexicographically with
		// their name (which also encapsulates their size via upper/lower case-ness).
		bool operator==(const Cave& otherCave) const { return name == otherCave.name; };
		bool operator!=(const Cave& otherCave) const { return name != otherCave.name; };
		bool operator<(const Cave& otherCave) const { return name < otherCave.name; };
		bool operator>(const Cave& otherCave) const { return name > otherCave.name; };
	};

	// When finding different routes, the route taken up to a given point can be uniquely
	// defined by the number of small caves visited, and whether we still have the option
	// to visit a small cave for a second time (in the case of part 1, this will always
	// be false, but for part 2, this is true until a small cave is first 'double-visited'
	// by a route.
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

	// A CaveSystem is put together as a series of linked cave pairings. Once all
	// of the linked caves have been fed into the system, it can be queries to find
	// the number of possible routes from 'start' to 'end', with or without the
	// ability to visit a single small cave twice rather than the usual limit of
	// one visit per small cave.
	class CaveSystem
	{
	private:
		std::map<std::string, std::shared_ptr<Cave>> cavesInSystem{};
		std::shared_ptr<Cave> startCave{};

		// For a given 'route taken so far' (which only needs to care about the small
		// caves visited, since large caves can be visited any number of times), the
		// number of routes from any given cave will always be the same. We can therefore
		// get a massive perf improvement by caching previous knowledge of how many routes
		// there are from a given cave and given route taken so far.
		std::map<std::pair<Cave, RouteSoFar>, unsigned int> cachedRoutesFromCaves{};

		unsigned int NumRoutesFromCave(std::weak_ptr<const Cave> cave, RouteSoFar routeSoFar);
		void MaybeCreateCave(std::string caveName);
	public:
		void LinkCaves(std::string caveNameOne, std::string caveNameTwo);
		unsigned int NumberOfRoutes(bool singleCaveDoubleVisitAllowed);
	};
}