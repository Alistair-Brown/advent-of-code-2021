#include "Cave_System.h"
#include <cassert>
#include <iostream>

Caves::CaveSystem::~CaveSystem()
{
	// Delete each Cave instance that was created.
	for (auto kvPair : cavesInSystem)
	{
		delete kvPair.second;
	}
}

Caves::Cave const * Caves::CaveSystem::StartCave()
{
	auto startCave = cavesInSystem.find(StartName);
	assert(startCave != cavesInSystem.end());

	return ((&(*(startCave->second))));
}

void Caves::CaveSystem::LinkCaves(std::string caveNameOne, std::string caveNameTwo)
{
	// Create the caves if they don't already exist
	CreateCave(caveNameOne);
	CreateCave(caveNameTwo);
	cavesInSystem[caveNameOne]->CreateCaveLink(caveNameTwo, cavesInSystem[caveNameTwo]);
	cavesInSystem[caveNameTwo]->CreateCaveLink(caveNameOne, cavesInSystem[caveNameOne]);
}

void Caves::CaveSystem::CreateCave(std::string caveName)
{
	if (cavesInSystem.find(caveName) == cavesInSystem.end())
	{
		bool isSmall = (int)caveName[0] >= 'a';
		Cave *newCave = new Cave(caveName, isSmall);
		cavesInSystem[caveName] = newCave;
	}
}

void Caves::Cave::CreateCaveLink(std::string caveName, Cave const *caveToLink)
{
	if (linkedCaves.find(caveName) == linkedCaves.end())
	{
		linkedCaves[caveName] = caveToLink;
	}
	linkedCavesAsVector.push_back(caveToLink);
}

Caves::Route::Route(std::string startingCaveName, Cave const *startingCave)
{
	currentCave = startingCave;
	visitedCaves[startingCaveName] = startingCave;
	visitedCavesInOrder.push_back(startingCaveName);
}

// TODO: The logic for max visits to a small cave is horrible, all the variables are named
// like you can visit any small cave up to that many times, but the code (correctly), only lets
// you visit one small cave more than once per route.
bool Caves::Route::AttemptToMoveToCave(std::string caveName, Cave const *cave, int maxVisitsToSmallCave)
{
	auto visitedCave = visitedCaves.find(caveName);
	bool success{ false };

	if (visitedCave == visitedCaves.end())
	{
		visitedCaves[caveName] = CaveVisitTracker{ cave, 1 };
		visitedCavesInOrder.push_back(caveName);
		currentCave = cave;
		success = true;
	}
	else if ((!(visitedCave->second.cave->IsSmall())) ||
		     ((visitedCave->second.cave->Name() != StartName) &&
		     (visitedCave->second.timesVisited < maxVisitsToSmallCave) &&
		      !haveDoubleVisited))
	{
		visitedCaves[caveName].timesVisited++;
		visitedCavesInOrder.push_back(caveName);
		currentCave = cave;
		success = true;

		if ((visitedCaves[caveName].timesVisited == 2) &&
			visitedCaves[caveName].cave->IsSmall())
		{
			haveDoubleVisited = true;
		}

	}

	return success;
}

void Caves::Route::PrintCompleteRoute()
{
	for (std::string caveName : visitedCavesInOrder)
	{
		std::cout << caveName << ",";
	}
	std::cout << std::endl;
}

unsigned int Caves::NumberOfRoutes(CaveSystem &caveSystem, int visitsAllowedPerSmallCave)
{
	std::vector<Route> routesToTry{};
	unsigned int numberOfRoutes{ 0 };

	Route currentRoute{ caveSystem.StartCave()->Name(), caveSystem.StartCave() };

	while (true)
	{
		if (currentRoute.CurrentCave()->Name() == EndName)
		{
			//currentRoute.PrintCompleteRoute();
			numberOfRoutes++;

			if (routesToTry.size() == 0) { break; }
			else
			{
				currentRoute = routesToTry.back();
				routesToTry.pop_back();
				continue;
			}
		}

		bool triedARoute{ false };
		Route snapshotRoute = currentRoute;
		for (Cave const*linkedCave : currentRoute.CurrentCave()->LinkedCaves())
		{
			if (!triedARoute)
			{
				if (currentRoute.AttemptToMoveToCave(linkedCave->Name(), linkedCave, visitsAllowedPerSmallCave))
				{
					triedARoute = true;
				}
			}
			else
			{
				Route newPotentialRoute = snapshotRoute;
				if (newPotentialRoute.AttemptToMoveToCave(linkedCave->Name(), linkedCave, visitsAllowedPerSmallCave))
				{
					routesToTry.push_back(newPotentialRoute);
				}
			}
		}
		if (!triedARoute)
		{
			if (routesToTry.size() == 0) { break; }
			else
			{
				currentRoute = routesToTry.back();
				routesToTry.pop_back();
				continue;
			}
		}
	}

	return numberOfRoutes;
}
