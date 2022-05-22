#include "Polymers.h"

void Polymer::Polymerisation::MakeSingleInsertionStep()
{
	std::unordered_map<std::string, unsigned long long int> newLetterPairTotals = zeroedLetterPairTotals;

	for (auto const& letterPair : currentLetterPairTotals)
	{
		newLetterPairTotals[insertionRules[letterPair.first].first] += letterPair.second;
		newLetterPairTotals[insertionRules[letterPair.first].second] += letterPair.second;
	}

	currentLetterPairTotals = newLetterPairTotals;
}

Polymer::Polymerisation::Polymerisation(
	std::string polymerTemplate,
	std::vector<std::pair<std::string, std::string>> insertionRules)
{
	for (std::pair<std::string, std::string> rule : insertionRules)
	{
		AddInsertionRule(rule);
	}
	
	currentLetterPairTotals = zeroedLetterPairTotals;
	for (unsigned int ii = 0; ii < polymerTemplate.size() - 1; ii++)
	{
		std::string elementPair = polymerTemplate.substr(ii, 2);
		currentLetterPairTotals[elementPair]++;
	}

	firstAndLast = std::pair<char, char>{ polymerTemplate[0], polymerTemplate[polymerTemplate.size() - 1] };
}

void Polymer::Polymerisation::AddInsertionRule(std::pair<std::string, std::string> rule)
{
	insertionRules[rule.first] = std::pair<std::string, std::string>{ 
		std::string{rule.first[0] + rule.second}, std::string{rule.second + rule.first[1]} };
	zeroedLetterPairTotals[rule.first] = 0;
}

void Polymer::Polymerisation::MakeInsertionSteps(int numSteps)
{
	for (int ii = 0; ii < numSteps; ii++)
	{
		MakeSingleInsertionStep();
	}
}

void Polymer::Polymerisation::IncreaseElementOccurences(
	char element,
	unsigned long long int numOccurences,
	std::unordered_map<char, unsigned long long int> &elementOccurences)
{
	if (elementOccurences.find(element) == elementOccurences.end())
	{
		elementOccurences[element] = numOccurences;
	}
	else
	{
		elementOccurences[element] += numOccurences;
	}
}

unsigned long long int Polymer::Polymerisation::DifferenceBetweenMostAndLeastFrequentElement()
{
	unsigned long long int leastFrequent;
	unsigned long long int mostFrequent;
	std::unordered_map<char, unsigned long long int> elementOccurences{};

	for (auto const& letterPair : currentLetterPairTotals)
	{
		IncreaseElementOccurences(letterPair.first[0], letterPair.second, elementOccurences);
		IncreaseElementOccurences(letterPair.first[1], letterPair.second, elementOccurences);
	}

	bool firstElement{ true };
	for (auto const& element : elementOccurences)
	{
		unsigned long long int elementCount = element.second;
		// Account for the fact that every element was counted twice, except the 2 on the end
		if ((element.first == firstAndLast.first) || (element.first == firstAndLast.second))
		{
			elementCount += 1;
		}
		elementCount /= 2;

		if (firstElement)
		{
			leastFrequent = elementCount;
			mostFrequent = elementCount;
			firstElement = false;
		}
		else
		{
			if (elementCount < leastFrequent)
			{
				leastFrequent = elementCount;
			}
			else if (elementCount > mostFrequent)
			{
				mostFrequent = elementCount;
			}
		}
	}
	return mostFrequent - leastFrequent;
}
