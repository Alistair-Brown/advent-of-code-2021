#pragma once
#include "grid_utils.h"
#include "aoc_common_types.h"
#include <vector>
#include <set>

// Forward declaration to allow us to type alias our OrderedRiskCells set.
namespace Chiton {
	class RiskCell;
};

// Our set of ordered risk cells needs to use a custom compare function, and that makes this
// a long enough type to be worth type aliasing.
using OrderedRiskCells = std::set<
	GridUtils::Grid<Chiton::RiskCell>::GridCell,
	bool(*)(GridUtils::Grid<Chiton::RiskCell>::GridCell const&, GridUtils::Grid<Chiton::RiskCell>::GridCell const&)
>;

namespace Chiton
{
	// A RiskCell represents a single location in our grid representing a cave of chitons.
	// A single cell has a local risk that represents the cost of moving into it, and also
	// keeps a record of the cheapest route we have found to reach it from the starting node.
	class RiskCell
	{
	private:
		unsigned short int localRisk;
		ULLINT lowestRiskToReach { ULLONG_MAX };

	public:
		RiskCell(int localRisk) : localRisk{ static_cast<unsigned short int>(localRisk) } {};				
		void MakeStartCell() { lowestRiskToReach = 0; };
		
		// Cells are initialised with maximum risk to reach. So we can tell whether we
		// have yet routed to this cell by checking whether risk has been brought
		// lower than this max.
		bool HasBeenRoutedTo() const { return lowestRiskToReach != ULLONG_MAX; };
		bool hasBeenRoutedFrom{ false };

		ULLINT LowestRiskToReach() const { return lowestRiskToReach; };
		void IsNewLowestRisk(ULLINT incomingRisk);
	};

	// Finds the cheapest (or 'lowest risk') path through a cave of chitons,
	// represented as a grid of integers where each element is the risk of moving
	// through that cell. Uses Djikstra's algorithm.
	ULLINT LowestRiskPath(std::vector<std::vector<int>> const& riskMap);

	void RouteFromNextCheapestCell(
		GridUtils::Grid<RiskCell>& chitonCave,
		OrderedRiskCells& orderedCells);
	void UpdateCellWithCheapestRoute(
		OrderedRiskCells &orderedCells,
		GridUtils::Grid<RiskCell>::GridCell& riskCell,
		ULLINT incomingRisk);

	RiskCell ReadEndCell(GridUtils::Grid<RiskCell> const &riskGrid);

	// Used for ordering risk cells according to which can be reached from the starting node with
	// the lowest total risk. Ties are broken by simply looking at the coordinates of the cells
	// which are guaranteed to be unique within a given grid.
	inline bool CompareRisks(
		GridUtils::Grid<RiskCell>::GridCell const &first,
		GridUtils::Grid<RiskCell>::GridCell const &second) 
	{
		return first.value.LowestRiskToReach() == second.value.LowestRiskToReach() ?
			first.GetCoordinate() < second.GetCoordinate() :
			first.value.LowestRiskToReach() < second.value.LowestRiskToReach(); 
	};
}