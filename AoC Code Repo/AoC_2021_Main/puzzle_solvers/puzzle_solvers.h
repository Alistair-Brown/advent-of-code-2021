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
	PuzzleAnswerPair AocDayThreeSolver(std::ifstream &puzzleInputFile);
	PuzzleAnswerPair AocDayFourSolver(std::ifstream &puzzleInputFile);
	PuzzleAnswerPair AocDayFiveSolver(std::ifstream &puzzleInputFile);
	PuzzleAnswerPair AocDaySixSolver(std::ifstream &puzzleInputFile);
	PuzzleAnswerPair AocDaySevenSolver(std::ifstream &puzzleInputFile);
	PuzzleAnswerPair AocDayEightSolver(std::ifstream &puzzleInputFile);
	PuzzleAnswerPair AocDayNineSolver(std::ifstream &puzzleInputFile);
	PuzzleAnswerPair AocDayTenSolver(std::ifstream& puzzleInputFile);
	PuzzleAnswerPair AocDayElevenSolver(std::ifstream& puzzleInputFile);
	PuzzleAnswerPair AocDayTwelveSolver(std::ifstream& puzzleInputFile);
	PuzzleAnswerPair AocDayThirteenSolver(std::ifstream& puzzleInputFile);
	PuzzleAnswerPair AocDayFourteenSolver(std::ifstream& puzzleInputFile);
	PuzzleAnswerPair AocDayFifteenSolver(std::ifstream& puzzleInputFile);
	PuzzleAnswerPair AocDaySixteenSolver(std::ifstream& puzzleInputFile);
	PuzzleAnswerPair AocDaySeventeenSolver(std::ifstream& puzzleInputFile);
	PuzzleAnswerPair AocDayEighteenSolver(std::ifstream& puzzleInputFile);
	PuzzleAnswerPair AocDayNineteenSolver(std::ifstream& puzzleInputFile);
	PuzzleAnswerPair AocDayTwentySolver(std::ifstream& puzzleInputFile);

	constexpr unsigned int numberOfPuzzleSolvers = 20;
	constexpr PuzzleFunc puzzleSolvers[numberOfPuzzleSolvers] = { 
		AocDayOneSolver,
		AocDayTwoSolver,
		AocDayThreeSolver,
		AocDayFourSolver,
		AocDayFiveSolver,
		AocDaySixSolver,
		AocDaySevenSolver,
		AocDayEightSolver,
		AocDayNineSolver,
		AocDayTenSolver,
		AocDayElevenSolver,
		AocDayTwelveSolver,
		AocDayThirteenSolver,
		AocDayFourteenSolver,
		AocDayFifteenSolver,
		AocDaySixteenSolver,
		AocDaySeventeenSolver,
		AocDayEighteenSolver,
		AocDayNineteenSolver,
		AocDayTwentySolver
	};
}