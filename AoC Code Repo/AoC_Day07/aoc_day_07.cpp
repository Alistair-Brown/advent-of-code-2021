#include "Parsing.h"
#include "CrabSubmarines.h"
#include <iostream>
#include "puzzle_solvers.h"

PuzzleAnswerPair PuzzleSolvers::AocDaySevenSolver(std::ifstream &puzzleInputFile)
{
	std::vector<int> crabPositionList =
		Parsing::ParseLineOnDelimiters<int>(puzzleInputFile, Parsing::comma);

	CrabSubmarines::CrabArmada crabArmada;
	for (int crabPosition : crabPositionList)
	{
		crabArmada.AddCrabSumbarine(crabPosition);
	}

	return PuzzleAnswerPair{
		std::to_string(crabArmada.MinimalConvergence()),
		std::to_string(crabArmada.MoreComplicatedMinimumConvergence()) };
}