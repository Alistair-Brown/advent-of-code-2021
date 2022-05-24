#include "Polymers.h"

// Constructor takes the starting string, or 'polymer template', and a series of insertion rules,
// expressed in the form of a letter pair (e.g. "AB"), and the letter to insert in the centre of
// each instance of that pair with each polymerisation step.
Polymer::Polymerisation::Polymerisation(
	std::string polymerTemplate,
	std::vector<std::pair<std::string, char>> insertionRules)
{
	for (std::pair<std::string, char> const &rule : insertionRules)
	{
		AddInsertionRule(rule);
	}
	
	// As well as storing the number of occurences of each pair of letters currently
	// present in string, we will save off the last character of the string (this will
	// make some later work counting character occurences slightly easier).
	for (unsigned int ii = 0; ii < polymerTemplate.size() - 1; ii++)
	{
		std::string elementPair = polymerTemplate.substr(ii, 2);
		currentLetterPairTotals[elementPair]++;
	}
	lastCharacter = polymerTemplate[polymerTemplate.size() - 1];
}

// Instertion rules are initially provided as a letter to insert in the centre of each
// instance of a given pair of letters. But for convenience of computation we want to
// instead store this insertion rule as the two new letter pairs that will result from
// this insertion (e.g. inserting "C" into every "AB" will result in "AC" and "CB").
void Polymer::Polymerisation::AddInsertionRule(std::pair<std::string, char> rule)
{
	insertionRules[rule.first] = std::pair<std::string, std::string>{
		std::string{rule.first[0] + std::string{rule.second}},
		std::string{std::string{rule.second} + rule.first[1]} };
}

// Just a little helper function to reduce the level of indentation required
// for looping around several insertion steps.
void Polymer::Polymerisation::MakeInsertionSteps(unsigned int numSteps)
{
	for (unsigned int ii = 0; ii < numSteps; ii++)
	{
		MakeSingleInsertionStep();
	}
}

// Each insertion step requires us to simultaneously carry out all of the insertion rules
// (simultaneously just means that none of the insertion steps will be considered to have
// created new letter pairs for the purposes of this round of insertion).
// This can be done by looking at each type of letter pair (e.g. "AB") that currently exists
// in the string, and the corresponding insertion rule telling us which new letter pairs
// will be created from this one by the insertion. The number of instances of that
// letter pair currently in the string is then the number of each of those 2 new letter
// pairs that will exist in the new string.
// NOTE: This assumes that every pair of possible letters will have a corresponding insertion
// rule, which is true for this puzzle and saves us some computation.
void Polymer::Polymerisation::MakeSingleInsertionStep()
{
	std::unordered_map<std::string, ULLINT> newLetterPairTotals{};
	for (auto const& [letterPair, currentTotal] : currentLetterPairTotals)
	{
		newLetterPairTotals[insertionRules[letterPair].first] += currentTotal;
		newLetterPairTotals[insertionRules[letterPair].second] += currentTotal;
	}
	currentLetterPairTotals = newLetterPairTotals;
}

// For the current form of the polymer string (which is held internally by a series of
// letter pairs), find the difference between the most and least frequently occuring element
// (where an element is just a letter).
ULLINT Polymer::Polymerisation::DifferenceBetweenMostAndLeastFrequentElement()
{
	ULLINT leastFrequent{};
	ULLINT mostFrequent{};
	std::unordered_map<char, ULLINT> elementOccurences{};

	// Count up the total occurences of each element by looking at only the first element/letter
	// in each letter pair, since pairs overlap. And then add on one occurence of the final character
	// in the string, being the only character not to appear as the first letter in a letter
	// pair.
	for (auto const& [letterPair, currentTotal] : currentLetterPairTotals)
	{
		elementOccurences[letterPair[0]] += currentTotal;
	}
	elementOccurences[lastCharacter] += 1;

	// Work through the counts of each element, find the most and least commonly occuring
	// element and return the difference between their two counts.
	bool firstElement{ true };
	for (auto const& [element, occurences] : elementOccurences)
	{
		if (firstElement)
		{
			leastFrequent = occurences;
			mostFrequent = occurences;
			firstElement = false;
		}
		else
		{
			if (occurences < leastFrequent)
			{
				leastFrequent = occurences;
			}
			else if (occurences > mostFrequent)
			{
				mostFrequent = occurences;
			}
		}
	}
	return mostFrequent - leastFrequent;
}