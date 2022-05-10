#include "Bracket_Syntax.h"
#include <cassert>

// Compare a single character with the open and close characters for this bracket
// type and return an enum representing which of these that character matches,
// if either.
Syntax::Bracket::BracketType Syntax::Bracket::DoesCharMatchAsBracket(char potentialBracket)
{
	if (potentialBracket == openChar) { return BracketType::Open; }
	else if (potentialBracket == closeChar) { return BracketType::Close; }
	else { return BracketType::Not; }
}

// Parse a line consisting only of bracket characters. If the line is syntactically
// incorrect, calculate the corruption score according to the puzzle rules and
// add it to the total corruption detected by this syntax checker so far. Otherwise
// work out the cost required to complete this line, again according to the puzzle
// rules, and add it to set of completion costs for lines this checker has encountered.
void Syntax::SyntaxChecker::ParseLine(std::string line)
{
	std::vector<char> openBracketStack{};
	unsigned int corruptedBracketValue{ 0 };
	ULLINT completionCost{ 0 };
	
	// Work along the line, doing the following for each character:
	// - If the character is an open bracket, add it to the top of the stack of
	//   open brackets.
	// - Otherwise if the the character is a close bracket matching the open bracket
	//   currently at the top of the stack we have validly closed a bracket, pop the
	//   open bracket off the top of the stack and continue.
	// - Otherwise we have a close bracket which doesn't match the current open
	//   bracket, this is a corrupted line.
	for (char nextBracket : line)
	{
		for (Bracket bracketType : bracketTypes)
		{
			Bracket::BracketType bracketMatch = bracketType.DoesCharMatchAsBracket(nextBracket);
			if (bracketMatch == Bracket::BracketType::Open)
			{
				openBracketStack.push_back(nextBracket);
				break;
			}
			else if (bracketMatch == Bracket::BracketType::Close)
			{
				if (bracketType.DoesCharMatchAsBracket(openBracketStack.back())
					== Bracket::BracketType::Open)
				{
					openBracketStack.pop_back();
				}
				else
				{
					corruptedBracketValue = bracketType.CorruptedBracketValue();
				}
				break;
			}
		}

		// If we've detected a corrupted line, add the corruption value to the total
		// for the syntax checker and break out of this parsing loop.
		if (corruptedBracketValue > 0)
		{
			totalCorruption += corruptedBracketValue;
			break;
		}
	}

	// If we got all the way along the line without detecting any corruption,
	// all that's left to do is to complete the line with close brackets matching
	// each open bracket remaining in the stack we've built up from parsing the
	// line so far. We use the puzzles rules for calculating the completion cost,
	// where each new close bracket added causes us to multiply the completion cost
	// so far by 5 and then add the completion cost of that particular close-bracket.
	if (corruptedBracketValue == 0)
	{
		while (openBracketStack.size() > 0)
		{
			char nextBracket = openBracketStack.back();
			openBracketStack.pop_back();
			for (Bracket bracketType : bracketTypes)
			{
				Bracket::BracketType bracketMatch = bracketType.DoesCharMatchAsBracket(nextBracket);
				if (bracketMatch == Bracket::BracketType::Open)
				{
					completionCost = (completionCost * 5) + bracketType.CompletionBracketValue();
				}
			}
		}
		completionCosts.insert(completionCost);
	}
}

// Get the median value from our list of completion costs. The puzzle input
// guarantees there are an odd number of lines, so there will be a median
// value.
ULLINT Syntax::SyntaxChecker::MedianCompletionCost()
{
	assert(completionCosts.size() % 2 == 1);
	unsigned int middlePosition = (completionCosts.size() / 2) + 1;
	std::multiset<ULLINT>::iterator iter = completionCosts.begin();
	for (unsigned int ii = 1; ii < middlePosition; ii++)
	{
		iter++;
	}
	return *iter;
}
