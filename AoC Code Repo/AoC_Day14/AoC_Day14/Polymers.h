#pragma once
#include <string>
#include <unordered_map>
#include <utility>

namespace Polymer
{
	class Polymerisation
	{
	private:
		std::unordered_map<std::string, std::pair<std::string, std::string>> insertionRules{};
		std::unordered_map<std::string, unsigned long long int> currentLetterPairTotals;
		std::unordered_map<std::string, unsigned long long int> zeroedLetterPairTotals;
		std::pair<char, char> firstAndLast;
		void MakeSingleInsertionStep();
		void IncreaseElementOccurences(
			char element,
			unsigned long long int numOccurences,
			std::unordered_map<char, unsigned long long int> &elementOccurences);
	public:
		Polymerisation(std::string polymerTemplate, std::vector<std::pair<std::string, std::string>> insertionRules);
		void AddInsertionRule(std::pair<std::string, std::string> rule);
		void MakeInsertionSteps(int numSteps);
		unsigned long long int DifferenceBetweenMostAndLeastFrequentElement();
	};
}