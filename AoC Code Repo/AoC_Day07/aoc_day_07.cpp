#include "Parsing.h"
#include "CrabSubmarines.h"
#include <iostream>
#include "puzzle_solvers.h"

// Day 7 gives us a list of positions of "crab submarines", and asks us to find the position
// that all of these submarines could move to using the least amount of collective fuel.
// In part 1, the rule for fuel consumption is simply that each space moved costs 1 unit of
// fuel. In part 2, the amount of fuel required for each step increases the further a submarine
// moves from its starting position.
PuzzleAnswerPair PuzzleSolvers::AocDaySevenSolver(std::ifstream &puzzleInputFile)
{
	// Puzzle input is a comma separated list of initial crab submarine positions.
	std::vector<int> crabPositionList =
		Parsing::ParseLineOnDelimiters<int>(puzzleInputFile, Parsing::comma);

	// Create our armada of crab submarines, inserting a submarine at each position
	// specified by the puzzle input.
	CrabSubmarines::CrabArmada crabArmada;
	for (int crabPosition : crabPositionList)
	{
		crabArmada.AddCrabSumbarine(crabPosition);
	}

	// The puzzle requires us to find the position that the crabs could all move
	// to with the least collective fuel, and then return how many units of fuel
	// that convergence would cost. The rules for fuel consumption are different
	// between part 1 and part 2.
	return PuzzleAnswerPair{
		std::to_string(crabArmada.PartOneMinimalConvergenceCost()),
		std::to_string(crabArmada.PartTwoMinimalConvergenceCost()) };
}