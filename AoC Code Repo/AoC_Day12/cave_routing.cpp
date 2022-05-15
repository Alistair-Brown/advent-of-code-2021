#include "cave_routing.h"
#include <iostream>

// Create a link between this cave and another.
void CaveRouting::Cave::CreateCaveLink(std::weak_ptr<Cave const> caveToLink)
{
	assert(!caveToLink.expired());
	std::shared_ptr<Cave const> caveToLinkShdPtr{ caveToLink.lock() };
	linkedCaves[caveToLinkShdPtr->name] = caveToLink;
}

// Create a Cave in the CaveSystem if that cave (identified by its name) is not
// already known. Small caves have lower-case names, so we can do a simple char
// to int conversion to work out whether this is a small cave.
void CaveRouting::CaveSystem::MaybeCreateCave(std::string caveName)
{
	if (!cavesInSystem.contains(caveName))
	{
		bool isSmall = (int)caveName[0] >= 'a';
		cavesInSystem[caveName] = std::make_shared<Cave>(caveName, isSmall);

		if (caveName == StartName)
		{
			startCave = cavesInSystem[caveName];
		}
	}
}

// Create a 2-way link between two caves in the system, creating those caves first if either
// is not already known by the system.
void CaveRouting::CaveSystem::LinkCaves(std::string caveNameOne, std::string caveNameTwo)
{
	MaybeCreateCave(caveNameOne);
	MaybeCreateCave(caveNameTwo);
	cavesInSystem[caveNameOne]->CreateCaveLink(std::weak_ptr<Cave> {cavesInSystem[caveNameTwo]});
	cavesInSystem[caveNameTwo]->CreateCaveLink(std::weak_ptr<Cave> {cavesInSystem[caveNameOne]});
}

// Find the number of possible routes from this cave to the 'end' cave, given the route that has
// been taken so far (which is required to control which small caves the ongoing route cannot
// visit).
// This function will call itself recursively on caves adjacent to this one until it reaches
// the 'end' cave.
unsigned int CaveRouting::CaveSystem::NumRoutesFromCave(std::weak_ptr<const Cave> cave, RouteSoFar routeSoFar)
{
	assert(!cave.expired());
	std::shared_ptr<Cave const> caveShdPtr{ cave.lock() };
	
	RouteSoFar inputRoute = routeSoFar;

	// If this is the 'end' cave, then the route up to this point was a valid one, stop
	// recursing and return 1.
	if (caveShdPtr->name == EndName)
	{
		return 1;
	}

	// If we've got a cached result from having performed this exact function before, just return
	// that.
	if (cachedRoutesFromCaves.contains({ *caveShdPtr, routeSoFar }))
	{
		return cachedRoutesFromCaves[{ *caveShdPtr, routeSoFar }];
	}

	// If this is a small cave, then it is only valid to continue along this route
	// if we have either not visited this cave before (in which case we should now add
	// it to the ongoing route), or if this route is still allowed to revisit a single
	// small cave (in which case we need to mark that we are no longer allowed to do this
	// for the ongoign route). Note that 'start' is special in that it can never be
	// revisited.
	if (caveShdPtr->isSmall)
	{
		if (routeSoFar.smallCavesVisited.contains(caveShdPtr->name))
		{
			if ((!routeSoFar.doubleVisitRemaining) || (caveShdPtr->name == StartName)) { return 0; }
			else { routeSoFar.doubleVisitRemaining = false; }
		}
		else
		{
			routeSoFar.smallCavesVisited.insert(caveShdPtr->name);
		}
	}

	// If we've got this far then there are potentially valid ongoing routes via our adjacent
	// caves. Ask each of them how many possible routes there are to the end given our updated
	// route so far.
	unsigned int routesToReturn{ 0 };
	for (auto linkedCave : caveShdPtr->LinkedCaves())
	{
		routesToReturn += NumRoutesFromCave(linkedCave.second, routeSoFar);
	}

	// Cache the result of this function for this specific input so that we can save
	// ourselves repeating the same calculation elsewhere in the recursion.
	cachedRoutesFromCaves[{ *caveShdPtr, inputRoute }] = routesToReturn;
	return routesToReturn;
}

// Find the number of possible routes from 'start' to 'end' in this cave system. Small
// caves can generally only be visited once by a given route, although we can optionally
// allow each route to visit single one of the small caves along it twice.
unsigned int CaveRouting::CaveSystem::NumberOfRoutes(bool singleCaveDoubleVisitAllowed)
{
	RouteSoFar routeSoFar{ singleCaveDoubleVisitAllowed };
	return NumRoutesFromCave(startCave, routeSoFar);
}
