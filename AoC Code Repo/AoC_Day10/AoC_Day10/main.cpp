#include "Bracket_Syntax.h"
#include <iostream>

int main()
{
	Syntax::SyntaxChecker syntaxChecker{ {
		Syntax::Bracket{'(', ')', 3, 1},
		Syntax::Bracket{'[', ']', 57, 2},
		Syntax::Bracket{'{', '}', 1197, 3},
		Syntax::Bracket{'<', '>', 25137, 4}
		} };

	std::cout << "Ready for input" << std::endl;

	while (true)
	{
		std::string nextLine{};
		std::getline(std::cin, nextLine);

		if (nextLine.size() == 0) { break; }
		syntaxChecker.TryToInsertLine(nextLine);
	}

	std::cout << "Total corruption is " << syntaxChecker.TotalCorruption() << std::endl;
	std::cout << "Middle completion is " << syntaxChecker.MiddleCompletionCost() << std::endl;

	return 0;
}