#include "Parsing.h"
#include "Polymers.h"
#include "puzzle_solvers.h"
#include <cassert>

// Day 14 gives us a 'polymer' (a string of elements/letters e.g. "ABCBDAB") and a series
// of 'polymerisation rules'. Each polymerisation rule is an instruction to insert a new
// element in the centre of each occurence of another pair of elements (e.g. insert 'C' in
// the centre of every "AB" to produce "ACB"). With every polymerisation step, all of these
// rules are applied simultaneously to produce an exponentially longer string as the steps
// go on.
// So for example with the polymer template "ABCBAB" and the rules "AB -> C", "CB -> A" and "BC -> A"
// the polymerisation process would go:
// Step 1: ACBACABACB
// Step 2: ACABACACBACAB
// This puzzle asks us to find the difference between the least and most frequently occuring
// 'element' (i.e. letter) in the polymer (i.e. string) after 10 and 40 steps.
PuzzleAnswerPair PuzzleSolvers::AocDayFourteenSolver(std::ifstream& puzzleInputFile)
{
	// The first line of the input is our polymer template (the starting string e.g. "ABCAB"),
	// followed by a blank line, and then all of the insertion rules (e.g. "AB -> C").
	std::string polymerTemplate;
	std::getline(puzzleInputFile, polymerTemplate);
	std::string emptyLine;
	std::getline(puzzleInputFile, emptyLine);

	std::vector<std::vector<std::string>> allRawInsertionRules;
	allRawInsertionRules = Parsing::ParseMultipleLinesWithDelimiters<std::string>(puzzleInputFile, " -> ");

	// Our 'raw' insertion rules were returned by the generic parsing function as a vector of
	// vectors, so let's break them down into a more understandable vector of pairs, where each
	// pair consists of a string of 2 letters that another must be inserted between, and then
	// that single character that will be inserted between them.
	std::pair<std::string, char> parsedInsertionRule;
	std::vector<std::pair<std::string, char>> allParsedInsertionRules;
	for (std::vector<std::string> rawInsertionRule : allRawInsertionRules)
	{
		assert(
			(rawInsertionRule.size() == 2) &&
			(rawInsertionRule[0].size() == 2) &&
			(rawInsertionRule[1].size() == 1)
		);

		parsedInsertionRule.first = rawInsertionRule[0];
		parsedInsertionRule.second = rawInsertionRule[1][0];

		allParsedInsertionRules.push_back(parsedInsertionRule);
	}

	// Initialise our polymerisation process with that input, and check the difference
	// between least and most frequent elements after 10 & 40 numbers of steps as required.
	Polymer::Polymerisation polymerisation{ polymerTemplate, allParsedInsertionRules };

	polymerisation.MakeInsertionSteps(10);
	ULLINT diffAfterTenSteps = polymerisation.DifferenceBetweenMostAndLeastFrequentElement();

	polymerisation.MakeInsertionSteps(30);
	ULLINT diffAfterFortySteps = polymerisation.DifferenceBetweenMostAndLeastFrequentElement();
	
	return PuzzleAnswerPair{ std::to_string(diffAfterTenSteps), std::to_string(diffAfterFortySteps) };
}