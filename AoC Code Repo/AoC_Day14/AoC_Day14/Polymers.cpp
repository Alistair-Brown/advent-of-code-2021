#include "Polymers.h"

void Polymer::Polymerisation::MakeSingleInsertionStep()
{
	std::string newString = polymerString;
	std::string subString;
	int insertionsMade{ 0 };

	for (unsigned int ii = 0; ii < polymerString.size() - 1; ii++)
	{
		subString = polymerString.substr(ii, 2);
		if (insertionRules.find(subString) != insertionRules.end())
		{
			newString.insert(ii + 1 + insertionsMade, insertionRules[subString]);
			// Shameless assumption that the substring only has one element.
			IncrementElementOccurence(insertionRules[subString][0]);
			insertionsMade++;
		}
	}
	polymerString = newString;
}

void Polymer::Polymerisation::IncrementElementOccurence(char element)
{
	if (elementOccurences.find(element) == elementOccurences.end())
	{
		elementOccurences[element] = ElementOccurences{ element };
	}
	else
	{
		elementOccurences[element].Increment();
	}
}

Polymer::Polymerisation::Polymerisation(std::string polymerTemplate)
{
	polymerString = polymerTemplate;

	for (char element : polymerTemplate)
	{
		IncrementElementOccurence(element);
	}
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

unsigned int Polymer::Polymerisation::DifferenceBetweenMostAndLeastFrequentElement()
{
	ElementOccurences leastFrequent;
	ElementOccurences mostFrequent;

	bool firstElement{ true };
	for (auto const& element : elementOccurences)
	{
		if (firstElement)
		{
			leastFrequent = element.second;
			mostFrequent = element.second;
			firstElement = false;
		}
		else
		{
			if (element.second.Occurences() < leastFrequent.Occurences()) 
			{
				leastFrequent = element.second;
			}
			else if (element.second.Occurences() > mostFrequent.Occurences())
			{
				mostFrequent = element.second;
			}
		}
	}
	return mostFrequent.Occurences() - leastFrequent.Occurences();
}
