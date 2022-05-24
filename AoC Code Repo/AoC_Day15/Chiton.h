#pragma once
#include <vector>

namespace Chiton
{
	// Represents a single node in a route trying to find the lowest risk route.
	// To save recalculations, this node knows:
	//  - The minimum risk it took any route to reach it
	//  - All connecting nodes
	class RiskNode
	{
	private:
		int minimumRiskToReach{ INT16_MAX };
		int localRisk{};
		std::pair<int, int> gridLocation{};
		std::vector<RiskNode *> nextNodes{};
	public:
		bool hasBeenRoutedFrom{ false };
		void MakeStartingCell() { minimumRiskToReach = 0; }

		RiskNode() : RiskNode({ 0,0 }, 0) {};
		RiskNode(std::pair<int, int> position, int risk) : gridLocation{ position }, localRisk{ risk } {};

		std::pair<int, int> UpNodePos() { return{ gridLocation.first + 1, gridLocation.second }; }
		std::pair<int, int> DownNodePos() { return{ gridLocation.first - 1, gridLocation.second }; }
		std::pair<int, int> LeftNodePos() { return{ gridLocation.first, gridLocation.second - 1 }; }
		std::pair<int, int> RightNodePos() { return{ gridLocation.first, gridLocation.second + 1 }; }

		void AssignNextNode(RiskNode *node);
		void NoLongerNextNode(RiskNode *node);
		bool IsThisTheNewMinimum(int inboundRisk, RiskNode *inboundNode);
		int MinimumRiskToReach() { return minimumRiskToReach; }
	};

	class ChitonCave
	{
	private:
		std::vector<std::vector<int>> riskGrid;
		std::vector<std::vector<RiskNode *>> minimumRiskToReachPosition;
		std::pair<int, int> startPos{ 0,0 };
		std::pair<int, int> endPos;
		void FindMinimumRiskRoutes();
	public:
		ChitonCave(std::vector<std::vector<int>> inputRiskGrid);
		~ChitonCave();
		int MinimumRiskToEnd();
	};
}
