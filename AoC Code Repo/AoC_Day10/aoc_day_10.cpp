#include "Bracket_Syntax.h"
#include <iostream>
#include "puzzle_solvers.h"
#include "Parsing.h"

// Day 10 is a syntax checker and autocomplete tool for a series of brackets.
// Part 1 asks us to find the first syntax error in a line (closing bracket not matching
// the correct opening bracket).
// Part 2 asks us to complete all of the lines which didn't contain syntax errors in part
// 1, by adding the correct closing brackets in the right order.
PuzzleAnswerPair PuzzleSolvers::AocDayTenSolver(std::ifstream& puzzleInputFile)
{
	// Initialise our syntax checker with the syntax and autocomplete scores from the
	// puzzle instructions for each bracket type.
	Syntax::SyntaxChecker syntaxChecker{ {
		Syntax::Bracket{'(', ')', 3, 1},
		Syntax::Bracket{'[', ']', 57, 2},
		Syntax::Bracket{'{', '}', 1197, 3},
		Syntax::Bracket{'<', '>', 25137, 4}
		} };

	// Feeding each of the input lines into the syntax checker leaves it able
	// to return the two 'scores' requested by this puzzle, the first being
	// a 'corruption' score for syntax errors, the second being the median
	// of all of the completion costs for completing incomplete lines.
	std::vector<std::string> inputLines = Parsing::SeparateRemainingInputIntoLines(puzzleInputFile);
	for (std::string inputLine : inputLines)
	{
		syntaxChecker.ParseLine(inputLine);
	}
	return PuzzleAnswerPair{
		std::to_string(syntaxChecker.TotalCorruption()),
		std::to_string(syntaxChecker.MedianCompletionCost())
	};
}