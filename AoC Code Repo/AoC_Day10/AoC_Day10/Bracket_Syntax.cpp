#include "Bracket_Syntax.h"
#include "Wrangling.h"

int Syntax::Bracket::DoesCharMatchAsBracket(char potentialBracket)
{
	if (potentialBracket == openChar) { return OpenBracket; }
	else if (potentialBracket == closeChar) { return CloseBracket; }
	else { return NotMatchingBracket; }
}

void Syntax::SyntaxChecker::TryToInsertLine(std::string line)
{
	std::vector<char> openBracketStack{};
	int corruptedBracketValue{ 0 };
	unsigned long long int completionCost{ 0 };
	
	for (char nextBracket : line)
	{
		for (Bracket bracketType : bracketTypes)
		{
			int bracketMatch = bracketType.DoesCharMatchAsBracket(nextBracket);
			if (bracketMatch == Bracket::OpenBracket)
			{
				openBracketStack.push_back(nextBracket);
				break;
			}
			else if (bracketMatch == Bracket::CloseBracket)
			{
				if (bracketType.DoesCharMatchAsBracket(openBracketStack.back())
					== Bracket::OpenBracket)
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
		if (corruptedBracketValue > 0)
		{
			totalCorruption += corruptedBracketValue;
			break;
		}
	}

	if (corruptedBracketValue == 0)
	{
		while (openBracketStack.size() > 0)
		{
			char nextBracket = openBracketStack.back();
			openBracketStack.pop_back();
			for (Bracket bracketType : bracketTypes)
			{
				int bracketMatch = bracketType.DoesCharMatchAsBracket(nextBracket);
				if (bracketMatch == Bracket::OpenBracket)
				{
					unsigned long long int lastCompletionCost = completionCost;
					completionCost = (completionCost * 5) + bracketType.CompletionBracketValue();
					if (lastCompletionCost > completionCost)
					{
						int ii = 5;
					}
				}
			}
		}
		Wrangling::InsertIntoOrderedList<unsigned long long int>(completionCosts, completionCost);
	}
}

unsigned long long int Syntax::SyntaxChecker::MiddleCompletionCost()
{
	// Let's just assume an odd number of element
	int middlePosition = (completionCosts.size() / 2) + 1;
	std::list<unsigned long long int>::iterator iter = completionCosts.begin();
	for (int ii = 1; ii < middlePosition; ii++)
	{
		iter++;
	}
	return *iter;
}
