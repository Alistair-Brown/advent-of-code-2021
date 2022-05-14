#include "Parsing.h"
#include "Dumbo_Octopus.h"
#include "puzzle_solvers.h"

// Day 11 gives us a grid of numbers representing the energy levels of some 'dumbo
// octopuses'. With every step of unit time, each octopus increases in energy, flashing
// once it reaches energy 10, in a reaction which also increases the energy levels
// of adjacent octopuses. Part 1 asks us to find out how many flashes will have occured
// after 100 time steps, and part 2 asks to find the number of steps before all
// octopuses flash simulataneously for the first time.
PuzzleAnswerPair PuzzleSolvers::AocDayElevenSolver(std::ifstream& puzzleInputFile)
{
	Dumbo::OctopusGrid octupusEnergyGrid{ Parsing::ParseWhitespacelessGridOfDigits(puzzleInputFile) };

	octupusEnergyGrid.AdvanceSteps(100);
	unsigned int flashesAfterHundredSteps = octupusEnergyGrid.NumberOfFlashesOccured();
	unsigned int stepsToFirstSimultaneousFlash = octupusEnergyGrid.AdvanceUntilFirstSimultaneousFlash();

	return PuzzleAnswerPair{
		std::to_string(flashesAfterHundredSteps),
		std::to_string(stepsToFirstSimultaneousFlash)
	};
}