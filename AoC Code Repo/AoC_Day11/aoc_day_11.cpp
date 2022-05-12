#include "Parsing.h"
#include "Dumbo_Octopus.h"
#include "puzzle_solvers.h"

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