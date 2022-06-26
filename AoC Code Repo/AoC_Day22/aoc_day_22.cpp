#include "Reactor.h"
#include "Parsing.h"
#include <iostream>
#include <regex>
#include <cassert>
#include <deque>
#include "puzzle_solvers.h"

PuzzleAnswerPair PuzzleSolvers::AocDayTwentyTwoSolver(std::ifstream& puzzleInputFile)
{
	Reactor::InitializationZone initializationZone{ false, {-50, 50}, {-50, 50}, {-50, 50} };
	std::vector<std::pair<bool, Reactor::InstructionVolume>> instructions;

	std::vector<std::string> inputLines = Parsing::SeparateRemainingInputIntoLines(puzzleInputFile);
	for (std::string &instruction : inputLines)
	{
		std::regex instructionPattern{
			"([a-z]+) x=(-{0,1}[0-9]+)..(-{0,1}[0-9]+),y=(-{0,1}[0-9]+)..(-{0,1}[0-9]+),z=(-{0,1}[0-9]+)..(-{0,1}[0-9]+)" };
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

		initializationZone.SetCubesInRange(settingIsOn, xRange, yRange, zRange);

		instructions.push_back({ settingIsOn, { xRange, yRange, zRange } });
	}

	Reactor::CompleteReactor completeReactor{ instructions };

	return PuzzleAnswerPair{
		std::to_string(initializationZone.GetNumberOfOnCubes()),
		std::to_string(completeReactor.NumberOfOnCubes())
	};
}