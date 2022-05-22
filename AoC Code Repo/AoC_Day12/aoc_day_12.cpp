#include "Parsing.h"
#include <iostream>
#include "puzzle_solvers.h"
#include <utility>
#include "cave_routing.h"

// Day 12 asks us to find the total number of possible routes from start to finish in a
// network of small and large caves, where large caves can be visited as many times as
// you like, but small caves can only be visited once. Part two switches that up a bit by
// asking us to find the number of possible routes where any given route is instead allowed
// to visit one (and only one) of the small caves twice.
PuzzleAnswerPair PuzzleSolvers::AocDayTwelveSolver(std::ifstream& puzzleInputFile)
{
	std::vector<std::string> inputLines = Parsing::SeparateRemainingInputIntoLines(puzzleInputFile);
	std::regex matchString{ "([a-zA-Z]+)-([a-zA-Z]+)" };

	// Piece together our cave system from the individual links between caves given
	// to us by the puzzle input.
	CaveRouting::CaveSystem caveSystem{};
	std::pair<std::string, std::string> cavePair{};
	for (std::string inputLine : inputLines)
	{
		Parsing::ParseStringIntoElements(
			inputLine,
			matchString,
			cavePair.first,
			cavePair.second
		);
		caveSystem.LinkCaves(cavePair.first, cavePair.second);
	}

	// For part 1, visiting a small cave twice is not allowed. For part 2, visiting
	// a single small cave twice as part of a given route is allowed.
	return PuzzleAnswerPair{
		std::to_string(caveSystem.NumberOfRoutes(false)),
		std::to_string(caveSystem.NumberOfRoutes(true))
	};
}