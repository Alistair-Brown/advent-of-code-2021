#include "aoc_common_types.h"
#include "input_retrieval.h"
#include "puzzle_solvers.h"
#include <iostream>

int main()
{
	// To test the architecture, temporarily just call the puzzle solver for day
	// 1 and print the output for manual comparison with the known answer.
	std::ifstream myIfStream = InputRetrieval::GetInputStreamForPuzzleInput(1);
	PuzzleAnswerPair dayOneAnswers = PuzzleSolvers::puzzleSolvers[0](myIfStream);
	std::cout << dayOneAnswers.first << ", " << dayOneAnswers.second << std::endl;
	return 0;
}