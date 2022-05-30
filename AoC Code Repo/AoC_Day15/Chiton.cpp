#include "chiton.h"
#include <cassert>

// Use Djikstra's algorithm to find the cheapest path from the top left to the bottom
// right of a grid of integers, where each integer represents the cost of moving to
// that point (or the 'risk' of moving to that point in the chiton cave, to use the
// puzzle flavour).
ULLINT Chiton::LowestRiskPath(std::vector<std::vector<int>> const &riskMap)
{
    GridUtils::Grid<RiskCell> chitonCave =
        GridUtils::Grid<RiskCell>::GridFactory<int>(
            riskMap,
            [](int risk)->RiskCell { return RiskCell(risk); }
    );
    chitonCave[{0, 0}].value.MakeStartCell();

    // Djikstra's algorithm requires us to explore the next cheapest unvisited node, starting
    // from the starting node (which has a cost of 0 to reach itself).
    // With each step of the algorithm we take the next cheapest unvisited cell and find the
    // cost of visiting each node adjacent to that one.
    // Once we've routed to the end cell for the first time, we are guaranteed to have found
    // the cheapest route to that cell, since routing from unvisited cells in cost order means
    // that any subseqent routes to that cell will be more expensive.
    OrderedRiskCells orderedUnvisitedNodes{ CompareRisks };
    orderedUnvisitedNodes.insert(chitonCave[{0, 0}]);
    while (!ReadEndCell(chitonCave).HasBeenRoutedTo())
    {
        RouteFromNextCheapestCell(chitonCave, orderedUnvisitedNodes);
    }

    return ReadEndCell(chitonCave).LowestRiskToReach();
}

// As per Djikstra's algorithm, take the next cheapest un-routed-from cell and route
// from it. Routing from a cell requires us to look at all adjacent cells and potentially
// update them with a new cheapest cost to reach them. Given that we route from cells
// in order of cheapest route, the only way that the route via our current cell could
// be the new cheapest route to a given adjacent cell is if no other route has ever
// reached that cell before, which can save us some computation of looking at unecessary
// adjacent cells.
void Chiton::RouteFromNextCheapestCell(
    GridUtils::Grid<RiskCell> &chitonCave,
    OrderedRiskCells &orderedCells)
{
    auto routingFromCellItr = orderedCells.begin();
    GridUtils::Grid<RiskCell>::GridCell riskCell = *routingFromCellItr;
    orderedCells.erase(routingFromCellItr);

    assert(!chitonCave[riskCell.GetCoordinate()].value.hasBeenRoutedFrom);
    chitonCave[riskCell.GetCoordinate()].value.hasBeenRoutedFrom = true;

    // For each adjacent cell which has not previously been routed to, the cheapest
    // route to that cell is the one via the cell we're currently routing from, so
    // update accordingly.
    if (!riskCell.IsTopRow() && !riskCell.Up().value.HasBeenRoutedTo())
    { 
        UpdateCellWithCheapestRoute(orderedCells, riskCell.Up(), riskCell.value.LowestRiskToReach()); 
    }
    if (!riskCell.IsBottomRow() && !riskCell.Down().value.HasBeenRoutedTo())
    {
        UpdateCellWithCheapestRoute(orderedCells, riskCell.Down(), riskCell.value.LowestRiskToReach());
    }
    if (!riskCell.IsRightColumn() && !riskCell.Right().value.HasBeenRoutedTo())
    {
        UpdateCellWithCheapestRoute(orderedCells, riskCell.Right(), riskCell.value.LowestRiskToReach());
    }
    if (!riskCell.IsLeftColumn() && !riskCell.Left().value.HasBeenRoutedTo())
    {
        UpdateCellWithCheapestRoute(orderedCells, riskCell.Left(), riskCell.value.LowestRiskToReach());
    }
}

// Update a cell to have its new cheapest route (or 'risk') to be that with the given
// incoming risk. This is guaranteed to be the cheapest route we have ever found to
// this cell, since we will only ever attempt to route to each cell once, as a
// computational optimisation.
void Chiton::UpdateCellWithCheapestRoute(
    OrderedRiskCells &orderedCells,
    GridUtils::Grid<RiskCell>::GridCell& riskCell,
    ULLINT incomingRisk)
{
    GridUtils::Grid<RiskCell>::GridCell incomingCell = riskCell;

    riskCell.value.IsNewLowestRisk(incomingRisk);
    orderedCells.insert(riskCell);
}

// Sum up the incoming risk with the local risk of a cell, to give a total risk
// of reaching that cell with that incoming risk. This is the new lowest risk
// of reaching this cell (this is guaranteed by the rest of the code for this
// puzzle, so we save ourself some CPU cycles by not actually checking this.
void Chiton::RiskCell::IsNewLowestRisk(ULLINT incomingRisk)
{
        lowestRiskToReach = incomingRisk + localRisk;;
}

// Just a helper function to simplify syntax for reading the value of the end cell.
Chiton::RiskCell Chiton::ReadEndCell(GridUtils::Grid<RiskCell> const &riskGrid)
{
    GridUtils::Coordinate endCoord{ riskGrid.Width() - 1, riskGrid.Height() - 1 };
    return riskGrid[endCoord].value;
}
