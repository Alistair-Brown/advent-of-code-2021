#pragma once
#include <set>
#include <vector>
#include <deque>
#include <stack>
#include <array>
#include <iostream>
#include "amphipod_constants.h"

namespace Amphipod
{
	// A WarrenState contains a possible arrangement of the amphipods in a warren, and the total
	// energy cost that was used to reach that arrangement.
	// A WarrenState is capable of finding all further states that can be reached by moving a
	// single amphipod to a legal position.
	class WarrenState
	{
	private:

		// A burrow can be visualised as a stack of Amphipods, with the top of the stack being the
		// nearest to the opening of the burrow. Burrows are filled from bottom to top, so there
		// may be empty spaces between the top of this stack and the top of the burrow, which aren't
		// represented here.
		// Every burrow has a 'home species', which are those that are trying to reach it to solve
		// the puzzle.
		struct Burrow 
		{
			Species homeSpecies;
			bool containsOnlyHomeSpecies;
			std::stack<Species> burrowContents;

			Burrow(Species species, std::vector<char> contents);
			Burrow() : Burrow(Species::AMBER, {}) {};

			Species TopOfBurrow() const { return burrowContents.top(); }
			Species &TopOfBurrow() { return burrowContents.top(); }
			unsigned int AmphipodsInBurrow() const { return burrowContents.size(); }
			void RecheckForOnlyHomeSpecies();

			bool operator !=(Burrow const& otherBurrow) const noexcept
			{
				return !((homeSpecies == otherBurrow.homeSpecies) &&
					(burrowContents == otherBurrow.burrowContents));
			}
			bool operator <(Burrow const& otherBurrow) const noexcept
			{
				if (homeSpecies != otherBurrow.homeSpecies) { return homeSpecies < otherBurrow.homeSpecies; }
				else { return burrowContents < otherBurrow.burrowContents; }
			}
		};

		// A Warren consists of 4 burrows (each of which contains a number of Amphipods between
		// 0 and the depth of the burrow) and a number of corridor spaces which can hold amphipods
		// (the corridor spaces directly outside each burrow cannot have Amphipods stopped in
		// them so will always be empty for a given state and don't need to be tracked here).
		Burrow amberBurrow;
		Burrow bronzeBurrow;
		Burrow copperBurrow;
		Burrow desertBurrow;
		std::array<char, 2> leftCorridor{ EMPTY_SPACE, EMPTY_SPACE };
		std::array<char, 2> rightCorridor{ EMPTY_SPACE, EMPTY_SPACE };
		char amberBronzeConnection{ EMPTY_SPACE };
		char bronzeCopperConnection{ EMPTY_SPACE };
		char copperDesertConnection{ EMPTY_SPACE };

		// Defines the maximum number of amphipods that can fit in a single burrow.
		unsigned int burrowDepth;

		// The goal of this puzzle is to find the cheapest way to correctly rearrange the
		// amphipods, so we must track the cost spent to reach a given state. When it comes to
		// sorting WarrenStates for Dijkstra's algorithm though, cost so far is an inefficient
		// method for selecting the best state to try next, it is also useful to know the lower
		// bound of the remaining cost that the final state could be reached in. We don't need to
		// track the actual lower bound though, just the change relative to the starting position.
		int costSoFar { 0 };
		int minimumRemainingCostRelativeToStart{ 0 };

		// A couple of helper structs to wrap up the main parameters that define whether
		// a move (to a burrow or corridor) can take place, and how that move will affect both
		// the cost so far of the state and the lower bound of the remaining cost required to
		// reach the end state.
		struct BurrowMove
		{
			Burrow& targetBurrow;
			std::vector<char> blockingSpaces;
			unsigned int corridorMovementSteps;
		};
		struct CorridorMove
		{
			char& targetCorridor;
			std::vector<char> blockingSpaces;
			unsigned int corridorMovementSteps;
			BurrowToCorridorMoves movementDescriptor;
		};

		// Query whether there is a clear path to either a burrow or corridor space. A corridor
		// move just requires that there are no other amphipods in the way, whereas a burrow
		// will also only accept the right type of amphipod.
		bool ClearPathToCorridor(std::vector<char> blockingSpaces) const;
		bool ClearPathToBurrow(
			Species speciesMoving,
			std::vector<char> blockingSpaces,
			Burrow const& targetBurrow
		) const;

		// From a given source burrow or corridor space, check a number of possible moves
		// to other burrows or corridor spaces to see if any of them are possible, in which
		// case the resulting state is appended to the provided vector.
		// When making burrow-to-burrow moves, we also return a bool for whether a move was
		// possible, since this can save us needing to also look for possible corridor moves.
		bool MaybeMakeBurrowToBurrowMoves(
			Burrow& sourceBurrow,
			std::vector<BurrowMove> potentialMoves,
			std::vector<WarrenState>& statesToAppendTo
		);
		void MaybeMakeCorridorToBurrowMoves(
			char& sourceCorridor,
			std::vector<BurrowMove> potentialMoves,
			std::vector<WarrenState>& statesToAppendTo
		);
		void MaybeMakeBurrowToCorridorMoves(
			Burrow& sourceBurrow,
			std::vector<CorridorMove> potentialMoves,
			std::vector<WarrenState>& statesToAppendTo
		);

	public:
		WarrenState(
			unsigned int burrowDepthIn,
			std::vector<char> amberBurrowIn,
			std::vector<char> bronzeBurrowIn,
			std::vector<char> copperBurrowIn,
			std::vector<char> desertBurrowIn
		);

		// In order for us to carry out pathfinding between the different states, a WarrenState
		// must be able to look for all other states that can be reached by a single amphipod move.
		std::vector<WarrenState> GetNextReachableWarrenStates();

		bool IsFinalState() const;
		void PrintSelfToScreen() const;
		int CostSoFar() const { return costSoFar; };

		// In order for us to pathfind with Dijkstra's algorithm, we must be able to sort
		// WarrenStates by their cost, so that when we take the cheapest state from the set
		// and find it is the FinalState, we can guarantee that we have indeed found the optimal
		// solution. Sorting by costSoFar alone though is very inefficient, as perfectly optimal
		// moves of expensive Desert amphipods will take a long time for us to get to them in
		// the set.
		// So instead we (accurately) sort cheapness by the sum of the cost so far and the
		// lower bound of remaining movement cost to reach the solution. Hence while an expensive
		// move of a Desert amphipod to its home burrow will cause a big increase in costSoFar,
		// it will also cause an equivalent reduction in the remaining lower bound, and so will
		// still be prioritised in the sort.
		// As a tie break, we revert to looking at the cost so far, since we can theorise that
		// the optimal solution will begin with the movement of the cheaper amphipods out of the
		// way of the more expensive ones.
		// Note that we don't actually calculate the lower bound of remaining cost at any point, we
		// just track the change relative to a starting value of 0, which is equivalent, and simpler.
		// The comparisons of non-cost-related elements aren't actually important for the ordering,
		// they just ensure states with the same costs can both appear in the set.
		bool operator <(WarrenState const& otherWarren) const noexcept
		{
			return (
				(costSoFar + minimumRemainingCostRelativeToStart) !=
					(otherWarren.costSoFar + otherWarren.minimumRemainingCostRelativeToStart) ?
				(costSoFar + minimumRemainingCostRelativeToStart) <
					(otherWarren.costSoFar + otherWarren.minimumRemainingCostRelativeToStart) :
				costSoFar != otherWarren.costSoFar ? costSoFar < otherWarren.costSoFar :
				amberBurrow != otherWarren.amberBurrow ? amberBurrow < otherWarren.amberBurrow :
				bronzeBurrow != otherWarren.bronzeBurrow ? bronzeBurrow < otherWarren.bronzeBurrow :
				copperBurrow != otherWarren.copperBurrow ? copperBurrow < otherWarren.copperBurrow :
				desertBurrow != otherWarren.desertBurrow ? desertBurrow < otherWarren.desertBurrow :
				leftCorridor != otherWarren.leftCorridor ? leftCorridor < otherWarren.leftCorridor :
				rightCorridor != otherWarren.rightCorridor ? rightCorridor < otherWarren.rightCorridor :
				amberBronzeConnection != otherWarren.amberBronzeConnection ? amberBronzeConnection < otherWarren.amberBronzeConnection :
				bronzeCopperConnection != otherWarren.bronzeCopperConnection ? bronzeCopperConnection < otherWarren.bronzeCopperConnection :
				copperDesertConnection != otherWarren.copperDesertConnection ? copperDesertConnection < otherWarren.copperDesertConnection :
				false);
		};
	};

	// The WarrenSolver is initialised with the initial state of the warren, which can be
	// fully defined by knowing how deep the burrows are and the contents of each burrow.
	// It can be queried to find out the cheapest energy cost with which the amphipods
	// can be rearranged to put every species in the correct burrow.
	class WarrenSolver
	{
	private:
		// While calculating the cheapest solve, the solver builds up a set of possible 'states'
		// (arrangements of amphipods), which it works through using Dijkstra's algorithm.
		// Once the problem has been solved once, we save off the result to be returned without
		// recalculation on any future queries.
		std::set<WarrenState> warrenStates{};
		unsigned int cheapestSolve{ 0 };
	public:
		WarrenSolver(
			unsigned int burrowDepthIn,
			std::vector<char> amberBurrowIn,
			std::vector<char> bronzeBurrowIn,
			std::vector<char> copperBurrowIn,
			std::vector<char> desertBurrowIn);

		unsigned int CheapestSolve();
	};
}