#pragma once
#include "grid_utils.h"
#include "aoc_common_types.h"
#include <vector>
#include <set>
#include <functional>
#include <memory>

namespace Chiton
{
	class RiskCell
	{
	private:
		unsigned int timesRoutedTo{ 0 };

		unsigned int localRisk;
		ULLINT lowestRiskToReach { ULLONG_MAX };
		bool isEndCell{ false };
	public:
		RiskCell(int localRisk) : localRisk{ static_cast<unsigned int>(localRisk) } {};

		bool hasBeenRoutedFrom{ false };
		void MakeEndCell() { isEndCell = true; };
		void MakeStartCell() { lowestRiskToReach = 0; };

		ULLINT CurrentLowestRiskToReach() { return lowestRiskToReach; };
		bool HasBeenRoutedTo() { return lowestRiskToReach != ULLONG_MAX; };

		bool IsNewLowestRisk(ULLINT incomingRisk);
	};

	ULLINT LowestRiskPath(std::vector<std::vector<int>> const& riskMap);
	void AttemptRouteToAdjacentCells(
		GridUtils::Grid<RiskCell>& chitonCave,
		std::set < GridUtils::Grid<RiskCell>::GridCell,
		bool(*)(GridUtils::Grid<RiskCell>::GridCell, GridUtils::Grid<RiskCell>::GridCell)> &orderedCells,
		GridUtils::Grid<RiskCell>::GridCell &riskCell);
	void MaybeRouteToThisCell(
		std::set<GridUtils::Grid<RiskCell>::GridCell,
		bool(*)(GridUtils::Grid<RiskCell>::GridCell, GridUtils::Grid<RiskCell>::GridCell)> &orderedCells,
		GridUtils::Grid<RiskCell>::GridCell& riskCell,
		ULLINT incomingRisk);

	bool IsNewLowestRisk(GridUtils::Grid<RiskCell>::GridCell &riskCell, ULLINT incomingRisk);

	void MarkEndCell(GridUtils::Grid<RiskCell> riskGrid);
	RiskCell ReadEndCell(GridUtils::Grid<RiskCell> &riskGrid);

	inline bool CompareRisks(GridUtils::Grid<RiskCell>::GridCell first, GridUtils::Grid<RiskCell>::GridCell second) {
		return first.value.CurrentLowestRiskToReach() == second.value.CurrentLowestRiskToReach() ?
			first.GetCoordinate() < second.GetCoordinate() :
			first.value.CurrentLowestRiskToReach() < second.value.CurrentLowestRiskToReach(); 
	};
}