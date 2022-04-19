#pragma once 
#include <string>
#include <fstream>
#include <utility>
#include "aoc_common_types.h"

// Provides the means to access the puzzle solver functions for each day.
// An array of puzzle solvers allows for iteration over all days, or access
// to a specific day's solver.
namespace PuzzleSolvers
{
	PuzzleAnswerPair AocDayOneSolver(std::ifstream &puzzleInputFile);
	PuzzleAnswerPair AocDayTwoSolver(std::ifstream &puzzleInputFile);

	constexpr unsigned int numberOfPuzzleSolvers = 2;
	constexpr PuzzleFunc puzzleSolvers[numberOfPuzzleSolvers] = { AocDayOneSolver, AocDayTwoSolver };
}