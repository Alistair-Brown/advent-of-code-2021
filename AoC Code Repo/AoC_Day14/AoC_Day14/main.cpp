#include "Parsing.h"
#include "Polymers.h"

int main()
{
	std::cout << "Enter input string followed by insertion rules" << std::endl;
	std::string polymerTemplate;
	std::getline(std::cin, polymerTemplate);

	// Remove the next blank line
	std::string emptyLine;
	std::getline(std::cin, emptyLine);

	std::vector<std::vector<std::string>> rawishInsertionRule;
	std::pair<std::string, std::string> singleInsertionRule;
	std::vector<std::pair<std::string, std::string>> insertionRules;
	while (true)
	{
		rawishInsertionRule = Parsing::ParseGroupsOfString({ 2 }, " -> ", "");
		if (rawishInsertionRule.size() == 0) { break; }

		singleInsertionRule.first = rawishInsertionRule[0][0];
		singleInsertionRule.second = rawishInsertionRule[0][1];

		insertionRules.push_back(singleInsertionRule);
	}

	Polymer::Polymerisation polymerisation{ polymerTemplate, insertionRules };

	polymerisation.MakeInsertionSteps(10);
	std::cout << "Difference between most and least common after 10 steps is " <<
		polymerisation.DifferenceBetweenMostAndLeastFrequentElement() << std::endl;

	polymerisation.MakeInsertionSteps(30);
	std::cout << "Difference between most and least common after another 30 is " <<
		polymerisation.DifferenceBetweenMostAndLeastFrequentElement() << std::endl;
}