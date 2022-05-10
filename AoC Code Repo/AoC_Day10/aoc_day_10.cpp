#include "Bracket_Syntax.h"
#include <iostream>
#include "puzzle_solvers.h"
#include "Parsing.h"

PuzzleAnswerPair PuzzleSolvers::AocDayTenSolver(std::ifstream& puzzleInputFile)
{
	Syntax::SyntaxChecker syntaxChecker{ {
		Syntax::Bracket{'(', ')', 3, 1},
		Syntax::Bracket{'[', ']', 57, 2},
		Syntax::Bracket{'{', '}', 1197, 3},
		Syntax::Bracket{'<', '>', 25137, 4}
		} };

	std::vector<std::string> inputLines = Parsing::SeparateInputIntoLines(puzzleInputFile);
	for (std::string inputLine : inputLines)
	{
		syntaxChecker.TryToInsertLine(inputLine);
	}

	return PuzzleAnswerPair{
		std::to_string(syntaxChecker.TotalCorruption()),
		std::to_string(syntaxChecker.MiddleCompletionCost())
	};
}