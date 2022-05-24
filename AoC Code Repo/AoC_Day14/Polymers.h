#pragma once
#include <string>
#include <unordered_map>
#include <utility>
#include "aoc_common_types.h"

namespace Polymer
{
	// The 'polymerisation' process in this puzzle has us start with a string of letters e.g. "ABCDA"
	// and a series of rules like "AB->C", which would mean that on every polymerisation step the letter
	// C should be inserted between A and B in every instance of the letter pair "AB".
	// This class can be initialised with such a string and set of rules, and then put through as many
	// polymerisation steps as required.
	// To solve the puzzle, the difference between the most and least frequent character at any given
	// time can be queried.
	class Polymerisation
	{
	private:
		// Each insertion rule is given as a letter to insert in between each instance
		// of a certain pair of other letters. But this is effectively the same as saying that each
		// instance of that certain pair of letters will be replaced with a new 'pair of pairs'. e.g
		// if we must insert 'C' in the centre of every 'AB', we will effectively replace every 'AB'
		// with an 'AC' and 'CB'. That is what is represented by this mapping of initial string to
		// a pair of new strings.
		std::unordered_map<std::string, std::pair<std::string, std::string>> insertionRules{};

		// Rather than keeping track of the full polymer string, we can make the program run in my lifetime
		// by just keeing track of how many instances of each letter pair there are in the string.
		// e.g. the string "ABAB" has 2 instances of the pair "AB", and 1 of the pair "BA".
		std::unordered_map<std::string, ULLINT> currentLetterPairTotals;
		char lastCharacter;

		void AddInsertionRule(std::pair<std::string, char> rule);
		void MakeSingleInsertionStep();
	public:
		Polymerisation(std::string polymerTemplate, std::vector<std::pair<std::string, char>> insertionRules);
		void MakeInsertionSteps(unsigned int numSteps);
		ULLINT DifferenceBetweenMostAndLeastFrequentElement();
	};
}