#include "ProbeLauncher.h"
#include "Parsing.h"
#include <cassert>
#include <iostream>
#include <regex>
#include "puzzle_solvers.h"

PuzzleAnswerPair PuzzleSolvers::AocDaySeventeenSolver(std::ifstream& puzzleInputFile)
{
	std::regex targetZonePattern{ "target area: x=([0-9]+)..([0-9]+), y=(-[0-9]+)..(-[0-9]+)" };
	std::string inputLine = Parsing::SeparateRemainingInputIntoLines(puzzleInputFile)[0];
	
	int positions[4];
	Parsing::ParseStringIntoElements(
		inputLine,
		targetZonePattern,
		positions[0],
		positions[1],
		positions[2],
		positions[3]
	);

	Launcher::ProbeLauncher probeLauncher{ 
		{positions[0], positions[1]},
		{positions[2], positions[3]}};

	return PuzzleAnswerPair{
		std::to_string(probeLauncher.HighestYCoordinate()),
		std::to_string(probeLauncher.NumberOfPossibleVelocities())
	};
}