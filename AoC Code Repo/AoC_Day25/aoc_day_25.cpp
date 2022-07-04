#include "Cucumber.h"
#include "Parsing.h"
#include "puzzle_solvers.h"

PuzzleAnswerPair PuzzleSolvers::AocDayTwentyFiveSolver(std::ifstream& puzzleInputFile)
{
	std::vector<std::string> inputGrid = Parsing::SeparateRemainingInputIntoLines(puzzleInputFile);

	Cucumber::Trench cucumberTrench{ inputGrid };

	unsigned int stepsToStop = cucumberTrench.MoveUntilAllBlocked();

	return PuzzleAnswerPair(std::to_string(stepsToStop), "n/a");
}