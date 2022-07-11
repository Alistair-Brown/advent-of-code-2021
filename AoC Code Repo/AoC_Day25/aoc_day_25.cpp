#include "Cucumber.h"
#include "Parsing.h"
#include "puzzle_solvers.h"

// Day 25 presents us with a grid of sea cucumbers, all of which are either East-moving or
// South-moving. With every time step, every East-moving cucumber will move one step East
// if there is currently a space available, and then every South-moving sea cucumber will
// move South if there is a space available. If a cucumber would move off the edge of the
// grid, it will instead appear on the opposite end of that row/column (i.e. the grid wraps
// horizontally and vertically).
// This puzzle has only one part, which is to find how many time steps pass before every
// sea cucumber is blocked and all movement stops.
PuzzleAnswerPair PuzzleSolvers::AocDayTwentyFiveSolver(std::ifstream& puzzleInputFile)
{
	std::vector<std::string> inputGrid = Parsing::SeparateRemainingInputIntoLines(puzzleInputFile);
	std::vector<std::vector<char>> inputGridAsChars;
	
	// The CucumberTrench (or more accurately my GridUtils library) expects the 2D vector to be
	// provided such that position 0,0 is at the bottom left of the grid. We read the input lines
	// in order, so the origin is currently at the top left. To correct for that, at the same time
	// that we're converting our strings into character vectors, also reorder the input lines
	// into the expected order.
	for (auto gridItr = inputGrid.end() - 1; ; gridItr--)
	{
		std::vector<char> inputLineAsChars { gridItr->begin(), gridItr->end() };
		inputGridAsChars.push_back(inputLineAsChars);
		if (gridItr == inputGrid.begin()) { break; }
	}

	Cucumber::Trench cucumberTrench{ inputGridAsChars };
	unsigned int stepsToStop = cucumberTrench.StepsUntilAllBlocked();

	return PuzzleAnswerPair(std::to_string(stepsToStop), "n/a");
}