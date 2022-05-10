#pragma once
#include <vector>
#include <string>
#include <list>

namespace Syntax
{
	class Bracket
	{
	private:
		char openChar;
		char closeChar;
		int corruptedBracketValue;
		int completionBracketValue;
	public:
		static const int NotMatchingBracket{ 0 };
		static const int OpenBracket{ 1 };
		static const int CloseBracket{ 2 };
		Bracket(char openChar, char closeChar, int corruptedBracketValue, int completionBracketValue) :
			openChar{ openChar },
			closeChar{ closeChar },
			corruptedBracketValue{ corruptedBracketValue },
			completionBracketValue{ completionBracketValue }{};
		int DoesCharMatchAsBracket(char potentialBracket);
		int CorruptedBracketValue() { return corruptedBracketValue; }
		int CompletionBracketValue() { return completionBracketValue; }
	};

	class SyntaxChecker
	{
	private:
		std::vector<Bracket> bracketTypes;
		int totalCorruption{ 0 };
		std::list<unsigned long long int> completionCosts{};
	public:
		SyntaxChecker(std::vector<Bracket> bracketTypes) : bracketTypes{ bracketTypes } {};
		void TryToInsertLine(std::string line);
		int TotalCorruption() { return totalCorruption; }
		unsigned long long int MiddleCompletionCost();
	};
}