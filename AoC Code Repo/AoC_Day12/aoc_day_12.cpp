#include "Cave_System.h"
#include "Parsing.h"
#include <iostream>
#include "puzzle_solvers.h"
#include <utility>

// TODO: This code does the second step extrememly slowly, I should find out why
PuzzleAnswerPair PuzzleSolvers::AocDayTwelveSolver(std::ifstream& puzzleInputFile)
{
	Caves::CaveSystem caveSystem{};

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
		std::to_string(Caves::NumberOfRoutes(caveSystem, 1)),
		std::to_string(Caves::NumberOfRoutes(caveSystem, 2))
	};
}