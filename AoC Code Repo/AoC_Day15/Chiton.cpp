#include "chiton.h"
#include <cassert>
#include <time.h>
#include <chrono>

ULLINT Chiton::LowestRiskPath(std::vector<std::vector<int>> const &riskMap)
{
    auto t1 = std::chrono::high_resolution_clock::now();
    GridUtils::Grid<RiskCell> chitonCave =
        GridUtils::Grid<RiskCell>::GridFactory<int>(
            riskMap,
            [](int risk)->RiskCell { return RiskCell(risk); }
    );
    MarkEndCell(chitonCave);
    chitonCave[{0, 0}].value.MakeStartCell();


    std::set < GridUtils::Grid<RiskCell>::GridCell,
        bool(*)(GridUtils::Grid<RiskCell>::GridCell, GridUtils::Grid<RiskCell>::GridCell) > riskToReachPosition(CompareRisks);
    riskToReachPosition.insert(chitonCave[{0, 0}]);

    auto t2 = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> msSpent1{ t2 - t1 };

    auto t3 = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> msSpent3 {0};
    std::chrono::duration<double, std::milli> msSpent4{ 0 };
    std::chrono::duration<double, std::milli> msSpent5{ 0 };
    ULLINT routingSteps{ 0 };
    bool done{ false };
    while (!done)
    {
        auto t7 = std::chrono::high_resolution_clock::now();
        routingSteps++;
        GridUtils::Grid<RiskCell>::GridCell routingFromCell = *riskToReachPosition.begin();
        riskToReachPosition.erase(routingFromCell);
        auto t8 = std::chrono::high_resolution_clock::now();
        msSpent4 += t8 - t7;

        auto t5 = std::chrono::high_resolution_clock::now();
        AttemptRouteToAdjacentCells(chitonCave, riskToReachPosition, routingFromCell);
        auto t6 = std::chrono::high_resolution_clock::now();
        msSpent3 += t6 - t5;

        auto t9 = std::chrono::high_resolution_clock::now();
        done = ReadEndCell(chitonCave).HasBeenRoutedTo();
        auto t10 = std::chrono::high_resolution_clock::now();
        msSpent5 += t10 - t9;
    }
    auto t4 = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> msSpent2{ t4 - t3 };

    return ReadEndCell(chitonCave).CurrentLowestRiskToReach();
}

void Chiton::AttemptRouteToAdjacentCells(
    GridUtils::Grid<RiskCell> &chitonCave,
    std::set < GridUtils::Grid<RiskCell>::GridCell,
        bool(*)(GridUtils::Grid<RiskCell>::GridCell, GridUtils::Grid<RiskCell>::GridCell) > &orderedCells,
    GridUtils::Grid<RiskCell>::GridCell &riskCell)
{
    assert(!chitonCave[riskCell.GetCoordinate()].value.hasBeenRoutedFrom);
    chitonCave[riskCell.GetCoordinate()].value.hasBeenRoutedFrom = true;
    auto t1 = std::chrono::high_resolution_clock::now();
    static std::chrono::duration<double, std::milli> msSpent{ 0 };
    if (!riskCell.IsTopRow() && !riskCell.Up().value.hasBeenRoutedFrom)
    { 
        MaybeRouteToThisCell(orderedCells, riskCell.Up(), riskCell.value.CurrentLowestRiskToReach()); 
    }
    if (!riskCell.IsBottomRow() && !riskCell.Down().value.hasBeenRoutedFrom)
    {
        MaybeRouteToThisCell(orderedCells, riskCell.Down(), riskCell.value.CurrentLowestRiskToReach());
    }
    if (!riskCell.IsRightColumn() && !riskCell.Right().value.hasBeenRoutedFrom)
    {
        MaybeRouteToThisCell(orderedCells, riskCell.Right(), riskCell.value.CurrentLowestRiskToReach());
    }
    if (!riskCell.IsLeftColumn() && !riskCell.Left().value.hasBeenRoutedFrom)
    {
        MaybeRouteToThisCell(orderedCells, riskCell.Left(), riskCell.value.CurrentLowestRiskToReach());
    }
    auto t2 = std::chrono::high_resolution_clock::now();
    msSpent += t2 - t1;
}

void Chiton::MaybeRouteToThisCell(
    std::set < GridUtils::Grid<RiskCell>::GridCell,
        bool(*)(GridUtils::Grid<RiskCell>::GridCell, GridUtils::Grid<RiskCell>::GridCell) > &orderedCells,
    GridUtils::Grid<RiskCell>::GridCell& riskCell,
    ULLINT incomingRisk)
{
    static ULLINT timesCalled{ 0 };
    static std::chrono::duration<double, std::milli> msSpent{ 0 };
    timesCalled++;
    auto t1 = std::chrono::high_resolution_clock::now();
    GridUtils::Grid<RiskCell>::GridCell incomingCell = riskCell;
    if (riskCell.value.IsNewLowestRisk(incomingRisk))
    {
        if (orderedCells.contains(incomingCell))
        {
            orderedCells.erase(incomingCell);
        }
        orderedCells.insert(riskCell);
    }
    auto t2 = std::chrono::high_resolution_clock::now();
    msSpent += t2 - t1;
}

void Chiton::MarkEndCell(GridUtils::Grid<RiskCell> riskGrid)
{
    GridUtils::Coordinate endCoord{ riskGrid.Width() - 1, riskGrid.Height() - 1 };
    riskGrid[endCoord].value.MakeEndCell();
}

Chiton::RiskCell Chiton::ReadEndCell(GridUtils::Grid<RiskCell> &riskGrid)
{
    auto t1 = std::chrono::high_resolution_clock::now();
    static std::chrono::duration<double, std::milli> msSpent{ 0 };

    GridUtils::Coordinate endCoord{ riskGrid.Width() - 1, riskGrid.Height() - 1 };
    return riskGrid[endCoord].value;

    auto t2 = std::chrono::high_resolution_clock::now();
    msSpent += t2 - t1;
}

bool Chiton::RiskCell::IsNewLowestRisk(ULLINT incomingRisk)
{
    ULLINT newRisk = incomingRisk + localRisk;
    if (newRisk < lowestRiskToReach)
    {
        lowestRiskToReach = newRisk;
        timesRoutedTo++;

        return true;
    }
    else
    {
        return false;
    }
}
