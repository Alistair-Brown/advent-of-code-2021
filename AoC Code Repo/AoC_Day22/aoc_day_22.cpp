#include "Reactor.h"
#include "Parsing.h"
#include <iostream>
#include <regex>
#include <cassert>
#include <deque>
#include "puzzle_solvers.h"

// Day 22 asks us to "initialize a reactor". The link between the theme and the actual problem is
// pretty tenuous with this one, what it really asks us to do is apply a series of instructions to
// switch a region of 3D space 'on' or 'off'. The entirety of 3D space starts 'off' and each
// instruction describes a cuboid shape to switch on or off, e.g:
// "on x=-36..14,y=-35..14,z=-24..28"
// After applying a number of these instructions, we must report how many of the unit cubes in
// 3D space are 'on'.
// For part 1 we only consider those instructions which refer to regions within a 100x100x100
// cube centred on (0,0,0). This is the 'initialisation zone' of the reactor.
// For part 2 we consider all of the instructions.
PuzzleAnswerPair PuzzleSolvers::AocDayTwentyTwoSolver(std::ifstream& puzzleInputFile)
{
	// Use a regex to parse each instruction into whether it is an 'on' or 'off'
	// instruction, and the 3 ranges (x,y,z) that describe the space the instruction acts on.
	std::vector<std::string> inputLines = Parsing::SeparateRemainingInputIntoLines(puzzleInputFile);
	std::vector<std::pair<bool, Reactor::InstructionVolume>> instructions;
	for (std::string &instruction : inputLines)
	{
		std::regex instructionPattern{
			"^([a-z]+) x=(-{0,1}[0-9]+)..(-{0,1}[0-9]+),y=(-{0,1}[0-9]+)..(-{0,1}[0-9]+),z=(-{0,1}[0-9]+)..(-{0,1}[0-9]+)$"
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

	// Our part one reactor should only consider instructions that fall in the 100-unit cube
	// centred on (0,0,0). This is the range -50 to 50 on each axis.
	// Part 2 covers the complete reactor and uses all instructions.
	Reactor::Reactor initializationZone{ instructions, { -50, 50 }, { -50, 50 }, { -50, 50 } };
	Reactor::Reactor completeReactor{ instructions };

	// For the initialization zone in part one, and for the complete reactor in part two,
	// we are required to find the number of coordinates in the reactor which are now 'on'.
	return PuzzleAnswerPair{
		std::to_string(initializationZone.NumberOfOnCubes()),
		std::to_string(completeReactor.NumberOfOnCubes())
	};
}