#pragma once
#include <vector>

namespace Chiton
{
	class ChitonCave
	{
	private:
		std::vector<std::vector<int>> riskGrid;
		std::vector<std::vector<int>> minimumRiskToReachPosition;
		std::pair<int, int> startPos{ 0,0 };
		std::pair<int, int> endPos;
		void FindMinimumRiskRoutes();
	public:
		ChitonCave(std::vector<std::vector<int>> inputRiskGrid);
		int MinimumRiskToEnd();
	};
}
