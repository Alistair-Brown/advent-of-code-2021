#pragma once
#include <string>
#include <unordered_map>
#include <utility>

namespace Polymer
{
	class Polymerisation
	{
	private:
		std::unordered_map<std::string, std::string> insertionRules{};
		std::string polymerString;
		void MakeSingleInsertionStep();
	public:
		Polymerisation(std::string polymerTemplate) : polymerString{ polymerTemplate } {};
		void AddInsertionRule(std::pair<std::string, std::string> rule);
		void MakeInsertionSteps(int numSteps);
		std::string OutputPolymerString() { return polymerString; }
	};
}