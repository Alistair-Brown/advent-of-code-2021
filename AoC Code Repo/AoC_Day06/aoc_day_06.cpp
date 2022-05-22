#include "LanternFish.h"
#include "Parsing.h"
#include <iostream>
#include <vector>
#include "puzzle_solvers.h"

// Day six covers an exponentially growing population of lanternfish. We start with a bunch
// of lanternfish of various ages, and the knowledge that each lanternfish will produce a
// new lanternfish every 7 days. Based on the initial collection of lanternfish, we need
// to find out how many there will be after first 80 and the 256 days.
PuzzleAnswerPair PuzzleSolvers::AocDaySixSolver(std::ifstream &puzzleInputFile)
{
	// Puzzle input is a comma separated list of how many days each current lanternfish
	// will take to produce a new one.
	std::vector<int> lanternFishInput =
		Parsing::SplitNextLineOnDelimiter<int>(puzzleInputFile, Parsing::comma);

	// Feed this initial collection of lanternfish tracker ready to start incrementing
	// days.
	LanternFish::LanternFishTracker lanternFishTracker;
	for (int lantenFishDaysToBirth : lanternFishInput)
	{
		lanternFishTracker.AddALanternFish(lantenFishDaysToBirth);
	}

	// The puzzle requires us to determine the number of lanternfish after 80 days and after 256
	// days total.
	PuzzleAnswerPair answer;
	lanternFishTracker.DaysHavePassed(80);
	answer.first = std::to_string(lanternFishTracker.TotalNumberOfLanterFish());
	lanternFishTracker.DaysHavePassed(176);
	answer.second = std::to_string(lanternFishTracker.TotalNumberOfLanterFish());

	return answer;
}