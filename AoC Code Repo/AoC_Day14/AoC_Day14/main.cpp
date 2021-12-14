#include "Parsing.h"
#include "Polymers.h"

int main()
{
	std::cout << "Enter input string followed by insertion rules" << std::endl;
	std::string polymerTemplate;
	std::getline(std::cin, polymerTemplate);

	Polymer::Polymerisation polymerisation{ polymerTemplate };

	// Remove the next blank line
	std::string emptyLine;
	std::getline(std::cin, emptyLine);

	std::vector<std::vector<std::string>> rawishInsertionRule;
	std::pair<std::string, std::string> insertionRule;
	while (true)
	{
		rawishInsertionRule = Parsing::ParseGroupsOfString({ 2 }, " -> ", "");
		if (rawishInsertionRule.size() == 0) { break; }

		insertionRule.first = rawishInsertionRule[0][0];
		insertionRule.second = rawishInsertionRule[0][1];
		polymerisation.AddInsertionRule(insertionRule);
	}

	polymerisation.MakeInsertionSteps(10);

}