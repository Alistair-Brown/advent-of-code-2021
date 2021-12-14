#pragma once
#include <string>
#include <unordered_map>
#include <utility>

namespace Polymer
{
	class ElementOccurences
	{
	private:
		char letter;
		unsigned int occurences{ 1 };
	public:
		ElementOccurences() : ElementOccurences{ 'A' } {};
		ElementOccurences(char letter) : letter{ letter } {};
		void Increment() { occurences++; }
		unsigned int Occurences() const { return occurences; }
	};

	class Polymerisation
	{
	private:
		std::unordered_map<std::string, std::string> insertionRules{};
		std::string polymerString;
		void MakeSingleInsertionStep();
		void IncrementElementOccurence(char element);
		std::unordered_map<char, ElementOccurences> elementOccurences{};
	public:
		Polymerisation(std::string polymerTemplate);
		void AddInsertionRule(std::pair<std::string, std::string> rule);
		void MakeInsertionSteps(int numSteps);
		std::string OutputPolymerString() { return polymerString; }
		unsigned int DifferenceBetweenMostAndLeastFrequentElement();
	};
}