#include "LanternFish.h"
#include "Parsing.h"
#include <iostream>
#include <vector>
#include "puzzle_solvers.h"

PuzzleAnswerPair PuzzleSolvers::AocDaySixSolver(std::ifstream &puzzleInputFile)
{
	std::vector<int> lanternFishInput =
		Parsing::ParseLineOnDelimiters<int>(puzzleInputFile, Parsing::comma);

	LanternFish::LanternFishTracker lanternFishTracker;

	for (int lantenFishDaysToBirth : lanternFishInput)
	{
		lanternFishTracker.AddALanternFish(lantenFishDaysToBirth);
	}

	PuzzleAnswerPair answer;
	lanternFishTracker.DaysHavePassed(80);
	answer.first = std::to_string(lanternFishTracker.TotalNumberOfLanterFish());
	lanternFishTracker.DaysHavePassed(176);
	answer.second = std::to_string(lanternFishTracker.TotalNumberOfLanterFish());

	return answer;
}