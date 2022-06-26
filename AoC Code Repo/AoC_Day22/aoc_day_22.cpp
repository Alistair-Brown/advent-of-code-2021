#include "Reactor.h"
#include "Parsing.h"
#include <iostream>
#include <regex>
#include <cassert>
#include <deque>
#include "puzzle_solvers.h"

PuzzleAnswerPair PuzzleSolvers::AocDayTwentyTwoSolver(std::ifstream& puzzleInputFile)
{
	std::vector<std::string> inputLines = Parsing::SeparateRemainingInputIntoLines(puzzleInputFile);
	std::vector<std::pair<bool, Reactor::InstructionVolume>> instructions;
	for (std::string &instruction : inputLines)
	{
		std::regex instructionPattern{
			"([a-z]+) x=(-{0,1}[0-9]+)..(-{0,1}[0-9]+),y=(-{0,1}[0-9]+)..(-{0,1}[0-9]+),z=(-{0,1}[0-9]+)..(-{0,1}[0-9]+)"
		};
		std::string onOrOff;
		std::pair<int, int> xRange;
		std::pair<int, int> yRange;
		std::pair<int, int> zRange;
		Parsing::ParseStringIntoElements(
			instruction,
			instructionPattern,
			onOrOff,
			xRange.first,
			xRange.second,
			yRange.first,
			yRange.second,
			zRange.first,
			zRange.second);

		assert((onOrOff == "on") || (onOrOff == "off"));
		bool settingIsOn = (onOrOff == "on");

		instructions.push_back({ settingIsOn, { xRange, yRange, zRange } });
	}

	Reactor::CompleteReactor initializationZone{ instructions, {-50,50}, {-50,50}, {-50,50} };
	Reactor::CompleteReactor completeReactor{ instructions };

	return PuzzleAnswerPair{
		std::to_string(initializationZone.NumberOfOnCubes()),
		std::to_string(completeReactor.NumberOfOnCubes())
	};
}