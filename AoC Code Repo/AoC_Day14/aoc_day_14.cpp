#include "Parsing.h"
#include "Polymers.h"
#include "puzzle_solvers.h"
#include <cassert>

PuzzleAnswerPair PuzzleSolvers::AocDayFourteenSolver(std::ifstream& puzzleInputFile)
{
	std::string polymerTemplate;
	std::getline(puzzleInputFile, polymerTemplate);

	// Remove the next blank line
	std::string emptyLine;
	std::getline(puzzleInputFile, emptyLine);

	std::vector<std::vector<std::string>> allRawInsertionRules;
	std::pair<std::string, std::string> parsedInsertionRule;
	std::vector<std::pair<std::string, std::string>> allParsedInsertionRules;
	allRawInsertionRules = Parsing::ParseMultipleLinesWithDelimiters<std::string>(puzzleInputFile, " -> ");

	for (std::vector<std::string> rawInsertionRule : allRawInsertionRules)
	{
		assert(
			(rawInsertionRule.size() == 2) &&
			(rawInsertionRule[0].size() == 2) &&
			(rawInsertionRule[1].size() == 1)
		);

		parsedInsertionRule.first = rawInsertionRule[0];
		parsedInsertionRule.second = rawInsertionRule[1];

		allParsedInsertionRules.push_back(parsedInsertionRule);
	}

	Polymer::Polymerisation polymerisation{ polymerTemplate, allParsedInsertionRules };

	polymerisation.MakeInsertionSteps(10);
	ULLINT partOne = polymerisation.DifferenceBetweenMostAndLeastFrequentElement();

	polymerisation.MakeInsertionSteps(30);
	ULLINT partTwo = polymerisation.DifferenceBetweenMostAndLeastFrequentElement();
	
	return PuzzleAnswerPair{ std::to_string(partOne), std::to_string(partTwo) };
}