#pragma once 
#include <string>
#include <fstream>
#include <utility>
#include "aoc_day_01.h"
#include "aoc_common_types.h"

// Provides the means to access the puzzle solver functions for each day.
// An array of puzzle solvers allows for iteration over all days, or access
// to a specific day's solver.
namespace PuzzleSolvers
{
	constexpr unsigned int numberOfPuzzleSolvers = 1;
	constexpr PuzzleFunc puzzleSolvers[numberOfPuzzleSolvers] = { AocDayOne::aocDayOneSolver };
}