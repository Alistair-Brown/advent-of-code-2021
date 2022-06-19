#include "aoc_common_types.h"
#include "input_retrieval.h"
#include "puzzle_solvers.h"
#include <iostream>

// Based on user input either solve and print the result for a single day, or run through
// solving and printing the results for all puzzles.
int main()
{
	int dayToRun{ 0 };
	bool runAll{ false };
	while ((dayToRun <= 0) || (dayToRun > PuzzleSolvers::numberOfPuzzleSolvers))
	{
		std::cout << "Enter a number between 1 and " << PuzzleSolvers::numberOfPuzzleSolvers <<
			" to solve that day's puzzle, or \"all\" to solve all days: ";
		std::string dayAsString;

		std::cin >> dayAsString;
		if (dayAsString == "all")
		{
			runAll = true;
			break;
		}
		else
		{
			dayToRun = atoi(dayAsString.c_str());
		}
	}
	std::cout << std::endl;

	if (runAll)
	{
		// We are solving every puzzle. We have an array of puzzle solvers provided to us
		// by the PuzzleSolver namespace, so this is a simple case of iterating over
		// that array and printing readable results as we go.
		unsigned int day = 1;
		std::ifstream puzzleInput;
		for (PuzzleFunc puzzleSolver : PuzzleSolvers::puzzleSolvers)
		{
			puzzleInput = InputRetrieval::GetInputStreamForPuzzleInput(day);
			PuzzleAnswerPair answerPair = puzzleSolver(puzzleInput);
			std::cout << "Day " << day << ": " << answerPair.first << ", " << answerPair.second << std::endl;
			day++;
		}
	}
	else
	{
		// Just solve a single day's puzzle and print the result, with a comma separating
		// the result for part 1 and 2.
		// And of course, be careful of the fact that day numbering starts from 1, but
		// our array of puzzle solvers is 0-indexed.
		std::ifstream puzzleInput = InputRetrieval::GetInputStreamForPuzzleInput(dayToRun);
		PuzzleAnswerPair answerPair = PuzzleSolvers::puzzleSolvers[dayToRun - 1](puzzleInput);
		std::cout << "Day " << dayToRun << ": " << answerPair.first << ", " << answerPair.second << std::endl;
	}

	return 0;
}

// TODO: Delete this once I've actually incorporated number 19, I'm just skipping ahead
// to day 20 first.
PuzzleAnswerPair PuzzleSolvers::AocDayNineteenSolver(std::ifstream& puzzleInputFile)
{
	return PuzzleAnswerPair{ "not implemented", "not implemented" };
}