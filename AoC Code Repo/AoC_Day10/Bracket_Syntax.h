#pragma once
#include <vector>
#include <string>
#include <set>
#include "aoc_common_types.h"

namespace Syntax
{
	// Defines a type of bracket, with the characters that are used to open and close that
	// bracket type e.g. '(' and ')', and the values assigned to detection of an unexpcceted
	// instance of this bracket (corruption) and autocompletion of this bracket, for the
	// purposes of puzzle result.
	class Bracket
	{
	private:
		char openChar;
		char closeChar;
		unsigned int corruptedBracketValue;
		unsigned int completionBracketValue;
	public:
		Bracket(char openChar, char closeChar, unsigned int corruptedBracketValue, unsigned int completionBracketValue) :
			openChar{ openChar },
			closeChar{ closeChar },
			corruptedBracketValue{ corruptedBracketValue },
			completionBracketValue{ completionBracketValue }{};

		// A given character will either match as the Open or Close bracket for this
		// Bracket, if it matches at all.
		enum BracketType { Not, Open, Close };
		BracketType DoesCharMatchAsBracket(char potentialBracket);

		unsigned int CorruptedBracketValue() { return corruptedBracketValue; }
		unsigned int CompletionBracketValue() { return completionBracketValue; }
	};

	// Syntax checker can parse lines consisting exclusively of bracket characters matching the
	// bracket types it is intialised with. It can detect syntax errors (when we reach a close
	// bracket that doesn't match the corresponding open bracket), keeping track of a total
	// 'corruption' score for all of the lines it has parsed. It can also work out the
	// cost to complete incompleted (but syntactically correct) lines which are missing closing
	// brackets.
	class SyntaxChecker
	{
	private:
		std::vector<Bracket> bracketTypes;

		// As lines are parsed by the syntax checker, they will either be detected as corrupted,
		// increasing the total corruption experienced by this syntax checker, or as incomplete,
		// in which cases the cost to autocomplete them will be added to the set of completion costs.
		unsigned int totalCorruption{ 0 };
		std::multiset<ULLINT> completionCosts{};
	public:
		SyntaxChecker(std::vector<Bracket> bracketTypes) : bracketTypes{ bracketTypes } {};
		void ParseLine(std::string line);

		// Corruption is just a total, but completion costs are stored as an ordered set, from
		// which the puzzle is interested in the median value.
		unsigned int TotalCorruption() { return totalCorruption; }
		ULLINT MedianCompletionCost();
	};
}