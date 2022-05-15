#include "cave_routing.h"
#include <iostream>

void CaveRouting::Cave::CreateCaveLink(std::weak_ptr<Cave const> caveToLink)
{
	assert(!caveToLink.expired());
	std::shared_ptr<Cave const> caveToLinkShdPtr{ caveToLink.lock() };
	linkedCaves[caveToLinkShdPtr->name] = caveToLink;
}

void CaveRouting::CaveSystem::CreateCave(std::string caveName)
{
	if (cavesInSystem.find(caveName) == cavesInSystem.end())
	{
		bool isSmall = (int)caveName[0] >= 'a';
		cavesInSystem[caveName] = std::make_shared<Cave>(caveName, isSmall);

		if (caveName == StartName)
		{
			startCave = cavesInSystem[caveName];
		}
	}
}

void CaveRouting::CaveSystem::LinkCaves(std::string caveNameOne, std::string caveNameTwo)
{
	CreateCave(caveNameOne);
	CreateCave(caveNameTwo);
	cavesInSystem[caveNameOne]->CreateCaveLink(std::weak_ptr<Cave> {cavesInSystem[caveNameTwo]});
	cavesInSystem[caveNameTwo]->CreateCaveLink(std::weak_ptr<Cave> {cavesInSystem[caveNameOne]});
}

unsigned int CaveRouting::CaveSystem::NumRoutesFromCave(std::weak_ptr<const Cave> cave, RouteSoFar routeSoFar)
{
	assert(!cave.expired());
	std::shared_ptr<Cave const> caveShdPtr{ cave.lock() };
	
	RouteSoFar inputRoute = routeSoFar;

	if (cachedRoutesFromCaves.contains({ *caveShdPtr, routeSoFar }))
	{
		return cachedRoutesFromCaves[{ *caveShdPtr, routeSoFar }];
	}

	if (caveShdPtr->name == EndName)
	{
		return 1;
	}

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

	unsigned int routesToReturn{ 0 };
	for (auto linkedCave : caveShdPtr->LinkedCaves())
	{
		routesToReturn += NumRoutesFromCave(linkedCave.second, routeSoFar);
	}

	cachedRoutesFromCaves[{ *caveShdPtr, inputRoute }] = routesToReturn;

	return routesToReturn;
}

unsigned int CaveRouting::CaveSystem::NumberOfRoutes(bool singleCaveDoubleVisitAllowed)
{
	RouteSoFar routeSoFar{ singleCaveDoubleVisitAllowed };
	return NumRoutesFromCave(startCave, routeSoFar);
}
