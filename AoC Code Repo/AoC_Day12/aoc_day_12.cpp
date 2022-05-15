#include "Parsing.h"
#include <iostream>
#include "puzzle_solvers.h"
#include <utility>
#include "cave_routing.h"

PuzzleAnswerPair PuzzleSolvers::AocDayTwelveSolver(std::ifstream& puzzleInputFile)
{
	CaveRouting::CaveSystem caveSystem{};

	std::vector<std::string> inputLines = Parsing::SeparateInputIntoLines(puzzleInputFile);
	std::regex matchString{ "([a-zA-Z]+)-([a-zA-Z]+)" };
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

	return PuzzleAnswerPair{
		std::to_string(caveSystem.NumberOfRoutes(false)),
		std::to_string(caveSystem.NumberOfRoutes(true))
	};
}