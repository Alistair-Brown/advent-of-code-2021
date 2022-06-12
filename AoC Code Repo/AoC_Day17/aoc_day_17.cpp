#include "ProbeLauncher.h"
#include "Parsing.h"
#include <cassert>
#include <iostream>
#include <regex>
#include "puzzle_solvers.h"

// Day 17 involves a probe launcher that can launch a probe with any number of starting velocities
// with integer x and y components. The probe moves in discrete steps, and is affected by gravity
// and drag. Part 1 asks to find the highest velocity the probe could be fired to and still spend
// at least one step in a target region of coordinates. Part 2 asks us to find the number of
// distinct starting velocities that would have the probe spend at least one step in the target
// region.
PuzzleAnswerPair PuzzleSolvers::AocDaySeventeenSolver(std::ifstream& puzzleInputFile)
{
	std::regex targetZonePattern{ "target area: x=([0-9]+)..([0-9]+), y=(-[0-9]+)..(-[0-9]+)" };
	std::string inputLine = Parsing::SeparateRemainingInputIntoLines(puzzleInputFile)[0];
	std::pair<int, int> xRange{};
	std::pair<int, int> yRange{};
	Parsing::ParseStringIntoElements(
		inputLine,
		targetZonePattern,
		xRange.first,
		xRange.second,
		yRange.first,
		yRange.second
	);

	Launcher::ProbeLauncher probeLauncher{ xRange, yRange };

	return PuzzleAnswerPair{
		std::to_string(probeLauncher.HighestYCoordinate()),
		std::to_string(probeLauncher.NumberOfPossibleVelocities())
	};
}