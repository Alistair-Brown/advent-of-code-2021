#include "Polymers.h"

void Polymer::Polymerisation::MakeSingleInsertionStep()
{
	std::string newString = polymerString;
	std::string subString;
	int insertionsMade{ 0 };

	for (int ii = 0; ii < polymerString.size() - 1; ii++)
	{
		subString = polymerString.substr(ii, 2);
		if (insertionRules.find(subString) != insertionRules.end())
		{
			newString.insert(ii + 1 + insertionsMade, insertionRules[subString]);
			insertionsMade++;
		}
	}
	polymerString = newString;
}

void Polymer::Polymerisation::AddInsertionRule(std::pair<std::string, std::string> rule)
{
	insertionRules[rule.first] = rule.second;
}

void Polymer::Polymerisation::MakeInsertionSteps(int numSteps)
{
	for (int ii = 0; ii < numSteps; ii++)
	{
		MakeSingleInsertionStep();
	}
}
