#include "amphipod_burrows.h"
#include <cassert>

// Initialisation of the WarrenSolver just requires us to create our initial WarrenState for the
// starting arrangement of amphipods, and insert it into the set of possible states ready to
// begin pathfinding from.
Amphipod::WarrenSolver::WarrenSolver(
	unsigned int burrowDepthIn,
	std::vector<char> amberBurrowIn,
	std::vector<char> bronzeBurrowIn,
	std::vector<char> copperBurrowIn,
	std::vector<char> desertBurrowIn)
{
	WarrenState initialState{
		burrowDepthIn,
		amberBurrowIn,
		bronzeBurrowIn,
		copperBurrowIn,
		desertBurrowIn
	};

	warrenStates.insert(initialState);
}

// Find the cheapest energy cost with which the amphipods could be rearranged from their initial
// configuration to the desired final state. This is done by considering 'states' which the
// warren could find itself in, where each possible move leaves the warren in a new state. We
// can then implement a pathfinding algorithm between these states to find the cheapest route
// to the desired final state.
unsigned int Amphipod::WarrenSolver::CheapestSolve()
{
	// cheapestSolve is initialised to 0, so if it's currently non-zero, we've already run this
	// operation, and can just return that cached result.
	if (cheapestSolve > 0) { return cheapestSolve; }

	// Implement Dijkstra's algorithm, checking the cheapest of our current known states to
	// see if it represents the desired final configuration. If it doesn't, find all further
	// states which could be reached by making a single move from that state, and add them
	// to the ordered set of states to try. Once we reach a state that has the desired final
	// configuration, we are guaranteed that it is the cheapest way of reaching that state,
	// and we have our answer.
	while (true)
	{
		auto nextCheapestStateItr = warrenStates.begin();
		auto nextCheapestState = *nextCheapestStateItr;
		warrenStates.erase(nextCheapestStateItr);

		if (nextCheapestState.IsFinalState())
		{
			cheapestSolve = nextCheapestState.CostSoFar();
			break;
		}

		std::vector<WarrenState> nextStates = nextCheapestState.GetNextReachableWarrenStates();
		for (WarrenState warrentState : nextStates)
		{
			warrenStates.insert(warrentState);
		}

		// If we've run out of states to pathfind from, then we have a bug (or the puzzle input
		// provided was incorrect and insoluble).
		if (warrenStates.size() == 0) { assert(false); }
	}

	return cheapestSolve;
}

// Constructor for WarrenState, primarily just requires us to pass the correct vector
// representation of a burrow to the relevant Burrow struct.
Amphipod::WarrenState::WarrenState(
	unsigned int burrowDepthIn,
	std::vector<char> amberBurrowIn,
	std::vector<char> bronzeBurrowIn,
	std::vector<char> copperBurrowIn,
	std::vector<char> desertBurrowIn)
{
	burrowDepth = burrowDepthIn;
	assert(amberBurrowIn.size() == burrowDepth);
	assert(bronzeBurrowIn.size() == burrowDepth);
	assert(copperBurrowIn.size() == burrowDepth);
	assert(desertBurrowIn.size() == burrowDepth);

	amberBurrow = Burrow(Species::AMBER, amberBurrowIn);
	bronzeBurrow = Burrow(Species::BRONZE, bronzeBurrowIn);
	copperBurrow = Burrow(Species::COPPER, copperBurrowIn);
	desertBurrow = Burrow(Species::DESERT, desertBurrowIn);
}

// The vector format of a burrow starts from the top of the burrow. So to create the equivalent
// stack of amphipods we must work backwards through this vector. Once complete, do our first
// check for whether this burrow is ready to receive further amphipods.
Amphipod::WarrenState::Burrow::Burrow(Species species, std::vector<char> contents)
{
	homeSpecies = species;

	for (auto reverseItr = contents.rbegin(); reverseItr != contents.rend(); reverseItr++)
	{
		burrowContents.push(SpeciesFromChar(* reverseItr));
	}

	containsOnlyHomeSpecies = false;
	RecheckForOnlyHomeSpecies();
}

// A burrow can only accept new amphipods entering it if it currently contains only its home
// species (or is entirely empty). This function requires checking of the contents of the burrow
// without editing the stack, so a temporary copy is taken. This function would be one to
// investigate to speed up the run time.
void Amphipod::WarrenState::Burrow::RecheckForOnlyHomeSpecies()
{
	if (containsOnlyHomeSpecies) { return; }

	containsOnlyHomeSpecies = true;
	std::stack<Species> tempContents = burrowContents;
	while (tempContents.size() > 0)
	{
		if (tempContents.top() != homeSpecies)
		{
			containsOnlyHomeSpecies = false;
			break;
		}
		tempContents.pop();
	}
}

// Find all states (arrangements of amphipods in the warren) that can be reached from this state by
// making a single valid amphipod move. We progress through this function as follows:
//  - First look for all new states that could result from moving an amphipod from one burrow to
//    another. It is invalid to move an amphipod out of a burrow which already contains only
//    its home species (this can never be efficient, and is against the rules of the puzzle).
//  - For any burrows which we could not move an amphipod *from*, check to see if there are any
//    corridor spaces that we could move that amphipod into instead, and create new states for each.
//  - For each corridor space which is not empty, check to see whether we can move the amphipod
//    in that space into a burrow, and create a new state for this move if so.
std::vector<Amphipod::WarrenState> Amphipod::WarrenState::GetNextReachableWarrenStates()
{
	std::vector<Amphipod::WarrenState> reachableStates{};

	if (!amberBurrow.containsOnlyHomeSpecies)
	{
		bool foundBurrow = MaybeMakeBurrowToBurrowMoves(
			amberBurrow,
			{ { bronzeBurrow, { amberBronzeConnection }, 3 },
			  { copperBurrow, { amberBronzeConnection, bronzeCopperConnection }, 5 },
			  { desertBurrow, { amberBronzeConnection, bronzeCopperConnection, copperDesertConnection }, 7 } },
			reachableStates
		);

		if (!foundBurrow)
		{
			MaybeMakeBurrowToCorridorMoves(
				amberBurrow,
				{ { leftCorridor[0], { leftCorridor[0] }, 2, BurrowToCorridorMoves::AMBER_BURROW_TO_LEFT_CORRIDOR_ZERO },
				  { leftCorridor[1], { leftCorridor[0], leftCorridor[1] }, 3, BurrowToCorridorMoves::AMBER_BURROW_TO_LEFT_CORRIDOR_ONE},
				  { amberBronzeConnection, { amberBronzeConnection }, 2, BurrowToCorridorMoves::AMBER_BURROW_TO_AMBER_BRONZE_CONNECTION },
				  { bronzeCopperConnection, { amberBronzeConnection,bronzeCopperConnection }, 4, BurrowToCorridorMoves::AMBER_BURROW_TO_BRONZE_COPPER_CONNECTION },
				  { copperDesertConnection, { amberBronzeConnection, bronzeCopperConnection, copperDesertConnection }, 6, BurrowToCorridorMoves::AMBER_BURROW_TO_COPPER_DESERT_CONNECTION },
				  { rightCorridor[0], {amberBronzeConnection, bronzeCopperConnection, copperDesertConnection, rightCorridor[0] }, 8, BurrowToCorridorMoves::AMBER_BURROW_TO_RIGHT_CORRIDOR_ZERO},
				  { rightCorridor[1], {amberBronzeConnection, bronzeCopperConnection, copperDesertConnection, rightCorridor[0], rightCorridor[1] }, 9, BurrowToCorridorMoves::AMBER_BURROW_TO_RIGHT_CORRIDOR_ZERO}, },
				reachableStates
			);
		}
	}

	if (!bronzeBurrow.containsOnlyHomeSpecies)
	{
		bool foundBurrow = MaybeMakeBurrowToBurrowMoves(
			bronzeBurrow,
			{ { amberBurrow, { amberBronzeConnection }, 3 },
			{ copperBurrow, { bronzeCopperConnection }, 3 },
			{ desertBurrow, { bronzeCopperConnection, copperDesertConnection }, 5 } },
			reachableStates
		);

		if (!foundBurrow)
		{
			MaybeMakeBurrowToCorridorMoves(
				bronzeBurrow,
				{ { leftCorridor[0], { amberBronzeConnection, leftCorridor[0] }, 4, BurrowToCorridorMoves::BRONZE_BURROW_TO_LEFT_CORRIDOR_ZERO },
				  { leftCorridor[1], { amberBronzeConnection, leftCorridor[0], leftCorridor[1] }, 5, BurrowToCorridorMoves::BRONZE_BURROW_TO_LEFT_CORRIDOR_ONE},
				  { amberBronzeConnection, { amberBronzeConnection }, 2, BurrowToCorridorMoves::BRONZE_BURROW_TO_AMBER_BRONZE_CONNECTION },
				  { bronzeCopperConnection, { bronzeCopperConnection }, 2, BurrowToCorridorMoves::BRONZE_BURROW_TO_BRONZE_COPPER_CONNECTION },
				  { copperDesertConnection, { bronzeCopperConnection, copperDesertConnection }, 4, BurrowToCorridorMoves::BRONZE_BURROW_TO_COPPER_DESERT_CONNECTION },
				  { rightCorridor[0], { bronzeCopperConnection, copperDesertConnection, rightCorridor[0] }, 6, BurrowToCorridorMoves::BRONZE_BURROW_TO_RIGHT_CORRIDOR_ZERO},
				  { rightCorridor[1], { bronzeCopperConnection, copperDesertConnection, rightCorridor[0], rightCorridor[1] }, 7, BurrowToCorridorMoves::BRONZE_BURROW_TO_RIGHT_CORRIDOR_ZERO}, },
				reachableStates
			);
		}
	}

	if (!copperBurrow.containsOnlyHomeSpecies)
	{
		bool foundBurrow = MaybeMakeBurrowToBurrowMoves(
			copperBurrow,
			{ { amberBurrow, { amberBronzeConnection, bronzeCopperConnection }, 5 },
			{ bronzeBurrow, { bronzeCopperConnection }, 3 },
			{ desertBurrow, { copperDesertConnection }, 3 } },
			reachableStates
		);

		if (!foundBurrow)
		{
			MaybeMakeBurrowToCorridorMoves(
				copperBurrow,
				{ { leftCorridor[0], { bronzeCopperConnection, amberBronzeConnection, leftCorridor[0] }, 6, BurrowToCorridorMoves::COPPER_BURROW_TO_LEFT_CORRIDOR_ZERO },
				  { leftCorridor[1], { bronzeCopperConnection, amberBronzeConnection, leftCorridor[0], leftCorridor[1] }, 7, BurrowToCorridorMoves::COPPER_BURROW_TO_LEFT_CORRIDOR_ONE},
				  { amberBronzeConnection, { bronzeCopperConnection, amberBronzeConnection }, 4, BurrowToCorridorMoves::COPPER_BURROW_TO_AMBER_BRONZE_CONNECTION },
				  { bronzeCopperConnection, { bronzeCopperConnection }, 2, BurrowToCorridorMoves::COPPER_BURROW_TO_BRONZE_COPPER_CONNECTION },
				  { copperDesertConnection, { copperDesertConnection }, 2, BurrowToCorridorMoves::COPPER_BURROW_TO_COPPER_DESERT_CONNECTION },
				  { rightCorridor[0], { copperDesertConnection, rightCorridor[0] }, 4, BurrowToCorridorMoves::COPPER_BURROW_TO_RIGHT_CORRIDOR_ZERO},
				  { rightCorridor[1], { copperDesertConnection, rightCorridor[0], rightCorridor[1] }, 5, BurrowToCorridorMoves::COPPER_BURROW_TO_RIGHT_CORRIDOR_ZERO}, },
				reachableStates
			);
		}
	}

	if (!desertBurrow.containsOnlyHomeSpecies)
	{
		bool foundBurrow = MaybeMakeBurrowToBurrowMoves(
			desertBurrow,
			{ { amberBurrow, { amberBronzeConnection, bronzeCopperConnection, copperDesertConnection }, 3 },
			{ bronzeBurrow, { bronzeCopperConnection, copperDesertConnection }, 5 },
			{ copperBurrow, { copperDesertConnection }, 3 } },
			reachableStates
		);

		MaybeMakeBurrowToCorridorMoves(
			desertBurrow,
			{ { leftCorridor[0], { copperDesertConnection, bronzeCopperConnection, amberBronzeConnection, leftCorridor[0] }, 8, BurrowToCorridorMoves::DESERT_BURROW_TO_LEFT_CORRIDOR_ZERO },
			  { leftCorridor[1], { copperDesertConnection, bronzeCopperConnection, amberBronzeConnection, leftCorridor[0], leftCorridor[1] }, 9, BurrowToCorridorMoves::DESERT_BURROW_TO_LEFT_CORRIDOR_ONE},
			  { amberBronzeConnection, { copperDesertConnection, bronzeCopperConnection, amberBronzeConnection }, 6, BurrowToCorridorMoves::DESERT_BURROW_TO_AMBER_BRONZE_CONNECTION },
			  { bronzeCopperConnection, { copperDesertConnection, bronzeCopperConnection }, 4, BurrowToCorridorMoves::DESERT_BURROW_TO_BRONZE_COPPER_CONNECTION },
			  { copperDesertConnection, { copperDesertConnection }, 2, BurrowToCorridorMoves::DESERT_BURROW_TO_COPPER_DESERT_CONNECTION },
			  { rightCorridor[0], { rightCorridor[0] }, 2, BurrowToCorridorMoves::DESERT_BURROW_TO_RIGHT_CORRIDOR_ZERO},
			  { rightCorridor[1], { rightCorridor[0], rightCorridor[1] }, 3, BurrowToCorridorMoves::DESERT_BURROW_TO_RIGHT_CORRIDOR_ZERO}, },
			reachableStates
		);
	}

	if (leftCorridor[1] != EMPTY_SPACE)
	{
		MaybeMakeCorridorToBurrowMoves(
			leftCorridor[1],
			{ { amberBurrow, { leftCorridor[0] }, 2 },
			  { bronzeBurrow, { leftCorridor[0], amberBronzeConnection }, 4 },
			  { copperBurrow, { leftCorridor[0], amberBronzeConnection, bronzeCopperConnection }, 6 },
			  { desertBurrow, { leftCorridor[0], amberBronzeConnection, bronzeCopperConnection, copperDesertConnection }, 8 } },
			reachableStates);
	}

	if (leftCorridor[0] != EMPTY_SPACE)
	{
		MaybeMakeCorridorToBurrowMoves(
			leftCorridor[0],
			{ { amberBurrow, { }, 1 },
			  { bronzeBurrow, { amberBronzeConnection }, 3 },
			  { copperBurrow, { amberBronzeConnection, bronzeCopperConnection }, 5 },
			  { desertBurrow, {amberBronzeConnection, bronzeCopperConnection, copperDesertConnection }, 7 } },
			reachableStates);
	}

	if (amberBronzeConnection != EMPTY_SPACE)
	{
		MaybeMakeCorridorToBurrowMoves(
			amberBronzeConnection,
			{ { amberBurrow, { }, 1 },
			  { bronzeBurrow, { }, 1 },
			  { copperBurrow, { bronzeCopperConnection }, 3 },
			  { desertBurrow, { bronzeCopperConnection, copperDesertConnection }, 5 } },
			reachableStates);
	}

	if (bronzeCopperConnection != EMPTY_SPACE)
	{
		MaybeMakeCorridorToBurrowMoves(
			bronzeCopperConnection,
			{ { amberBurrow, { amberBronzeConnection }, 3 },
			  { bronzeBurrow, { }, 1 },
			  { copperBurrow, { }, 1 },
			  { desertBurrow, { copperDesertConnection }, 3 } },
			reachableStates);
	}

	if (copperDesertConnection != EMPTY_SPACE)
	{
		MaybeMakeCorridorToBurrowMoves(
			copperDesertConnection,
			{ { amberBurrow, { amberBronzeConnection, bronzeCopperConnection }, 5 },
			  { bronzeBurrow, { bronzeCopperConnection }, 3 },
			  { copperBurrow, { }, 1 },
			  { desertBurrow, { }, 1 } },
			reachableStates);
	}

	if (rightCorridor[0] != EMPTY_SPACE)
	{
		MaybeMakeCorridorToBurrowMoves(
			rightCorridor[0],
			{ { amberBurrow, { amberBronzeConnection, bronzeCopperConnection, copperDesertConnection }, 7 },
			  { bronzeBurrow, { bronzeCopperConnection, copperDesertConnection }, 5 },
			  { copperBurrow, { copperDesertConnection }, 3 },
			  { desertBurrow, { }, 1 } },
			reachableStates);
	}

	if (rightCorridor[1] != EMPTY_SPACE)
	{
		MaybeMakeCorridorToBurrowMoves(
			rightCorridor[1],
			{ { amberBurrow, { amberBronzeConnection, bronzeCopperConnection, copperDesertConnection, rightCorridor[0 ]}, 8},
			  { bronzeBurrow, { bronzeCopperConnection, copperDesertConnection, rightCorridor[0] }, 6},
			  { copperBurrow, { copperDesertConnection, rightCorridor[0] }, 4},
			  { desertBurrow, { rightCorridor[0] }, 2}},
			reachableStates);
	}

	return reachableStates;
};

// Given a source burrow and number of potential moves to other burrows, see if any of those moves are
// possible, and append the resulting state to the provided vector of states if so.
// If a burrow move was possible, return true (indicating that we shouldn't also attempt moves to a
// corridor from that position, as those cannot be more optimal), otherwise return false.
bool Amphipod::WarrenState::MaybeMakeBurrowToBurrowMoves(
	Burrow& sourceBurrow,
	std::vector<BurrowMove> potentialMoves,
	std::vector<WarrenState>& statesToAppendTo)
{
	bool successfulBurrowMove{ false };
	Species speciesMoving = sourceBurrow.TopOfBurrow();

	for (BurrowMove burrowMove : potentialMoves)
	{
		if (ClearPathToBurrow(
			sourceBurrow.TopOfBurrow(),
			burrowMove.blockingSpaces,
		    burrowMove.targetBurrow))
		{
			// The total movement cost of this burrow move is the sum of the steps taken in
			// the burrows and corrdior, multiplied by the species movement cost.
			unsigned int burrowSteps = (burrowDepth - sourceBurrow.AmphipodsInBurrow()) +
				(burrowDepth - burrowMove.targetBurrow.AmphipodsInBurrow());
			int totalMoveCost = MoveCostFromSpecies(speciesMoving) *
				(burrowSteps + burrowMove.corridorMovementSteps);

			// We minimise the number of copies required by moving the amphipod around on our source
			// state first and then copying this WarrenState into the list (we will revert this
			// change shortly).
			sourceBurrow.burrowContents.pop();
			burrowMove.targetBurrow.burrowContents.push(speciesMoving);
			sourceBurrow.RecheckForOnlyHomeSpecies();

			// Generate the new state and make the relevant updates to its cost fields (we can do this
			// on the new state directly, since we don't require references to particular burrows like
			// we do for amphipod movement). Movement into a target burrow is entirely 'useful' movement,
			// so the reduction in minimum remaining cost is exactly equal to the movement cost.
			auto &newState = statesToAppendTo.emplace_back(*this);
			newState.costSoFar += totalMoveCost;
			newState.minimumRemainingCostRelativeToStart -= totalMoveCost;

			// Now that we've copied ourself into the new state, revert the burrow changes we made
			// earlier to leave this particular state exactly as it started.
			burrowMove.targetBurrow.burrowContents.pop();
			sourceBurrow.burrowContents.push(speciesMoving);
			sourceBurrow.containsOnlyHomeSpecies = false;

			// A given amphipod only has one home burrow, so if we've made one successful burrow move,
			// there can't possibly be others, and we can return.
			successfulBurrowMove = true;
			break;
		}
	}

	return successfulBurrowMove;
}

// With a given source burrow and a number of possible moves to corridor spaces, create new states
// for any of those moves which are actually possible.
void Amphipod::WarrenState::MaybeMakeBurrowToCorridorMoves(
	Burrow& sourceBurrow,
	std::vector<CorridorMove> potentialMoves,
	std::vector<WarrenState>& statesToAppendTo)
{
	Species speciesMoving = sourceBurrow.TopOfBurrow();

	for (CorridorMove corridorMove : potentialMoves)
	{
		if (ClearPathToCorridor(corridorMove.blockingSpaces))
		{			
			// We minimise the number of copies required by moving the amphipod around on our source
			// state first and later copying this WarrenState into the list (we will revert this
			// change shortly).			
			sourceBurrow.burrowContents.pop();			
			corridorMove.targetCorridor = static_cast<char>(speciesMoving);
			sourceBurrow.RecheckForOnlyHomeSpecies();

			// The cost of the move is simply the number of steps multiplied by the cost of moving
			// this species. The change in minimum remaining cost is more complex, requiring
			// us to find out how many of the corridor steps were useful for moving this species
			// in this way (this may be negative if moving away from that species' burrow), and
			// then working out whether the burrow steps were useful (if this species is moving
			// *out* of its burrow, these are not useful steps).
			unsigned int burrowSteps = burrowDepth - sourceBurrow.AmphipodsInBurrow();			
			int usefulCorridorMovementSteps = GetUsefulMovementFromMove(corridorMove.movementDescriptor, speciesMoving);
			int usefulBurrowSteps = (speciesMoving != sourceBurrow.homeSpecies) ? 
				burrowSteps : (burrowSteps * -1);
			int movementCost = MoveCostFromSpecies(speciesMoving) * (burrowSteps + corridorMove.corridorMovementSteps);
			int minRemCostDelta = MoveCostFromSpecies(speciesMoving) * (usefulBurrowSteps + usefulCorridorMovementSteps);

			// Generate the new state and make the relevant updates to its cost fields (we can do this
			// on the new state directly, since we don't require references to particular burrows like
			// we do for amphipod movement).
			auto &newState = statesToAppendTo.emplace_back(*this);
			newState.costSoFar += movementCost;
			newState.minimumRemainingCostRelativeToStart -= minRemCostDelta;

			// Now that we've copied ourself into the new state, revert the burrow changes we made
			// earlier to leave this particular state exactly as it started.
			sourceBurrow.burrowContents.push(speciesMoving);
			sourceBurrow.containsOnlyHomeSpecies = false;
			corridorMove.targetCorridor = EMPTY_SPACE;
		}
	}
}

// For a given source corridor position, check a number of possible moves to burrows and if
// one is valid (an amphipod only has one home burrow, so only one can be valid), add the new
// state for that move to the provided vector.
void Amphipod::WarrenState::MaybeMakeCorridorToBurrowMoves(
	char& sourceCorridor,
	std::vector<BurrowMove> potentialMoves,
	std::vector<WarrenState>& statesToAppendTo)
{
	Species speciesMoving = SpeciesFromChar(sourceCorridor);

	for (BurrowMove burrowMove : potentialMoves)
	{
		if (ClearPathToBurrow(speciesMoving, burrowMove.blockingSpaces, burrowMove.targetBurrow))
		{
			// We minimise the number of copies required by moving the amphipod around on our source
			// state first and later copying this WarrenState into the list (we will revert this
			// change shortly).		
			sourceCorridor = EMPTY_SPACE;
			burrowMove.targetBurrow.burrowContents.push(speciesMoving);

			// The total movement cost of this burrow move is the sum of the steps taken in
			// the burrows and corrdior, multiplied by the species movement cost.
			unsigned int burrowSteps = burrowDepth - burrowMove.targetBurrow.AmphipodsInBurrow();
			int movementCost = MoveCostFromSpecies(speciesMoving) * (burrowSteps + burrowMove.corridorMovementSteps);

			// Generate the new state and make the relevant updates to its cost fields (we can do this
			// on the new state directly, since we don't require references to particular burrows like
			// we do for amphipod movement). Movement into a target burrow is entirely 'useful' movement,
			// so the reduction in minimum remaining cost is exactly equal to the movement cost.
			auto &newState = statesToAppendTo.emplace_back(*this);
			newState.costSoFar += movementCost;
			newState.minimumRemainingCostRelativeToStart -= movementCost;

			// Now that we've copied ourself into the new state, revert the burrow changes we made
			// earlier to leave this particular state exactly as it started.
			sourceCorridor = static_cast<char>(speciesMoving);
			burrowMove.targetBurrow.burrowContents.pop();

			// A given amphipod only has one home burrow, so if we've made one successful burrow move,
			// there can't possibly be others, and we can return.
			break;
		}
	}
}

// We have a clear path to a corridor space if all of the spaces blocking the path to it
// are empty (including that corridor space itself, which should be included in the vector
// of blockingSpaces).
bool Amphipod::WarrenState::ClearPathToCorridor(std::vector<char> blockingSpaces) const
{
	for (char& space : blockingSpaces)
	{
		if (space != EMPTY_SPACE) { return false; }
	}
	return true;
}

// For there to be a clear path to a burrow we require both that the spaces in the way are
// clear. But for a move into a burrow to be legal, the following must also be true:
//  - The burrow contains only species of its own target type (or none at all)
//  - The species being moved into the burrow is also of the target type
bool Amphipod::WarrenState::ClearPathToBurrow(
	Species speciesMoving,
	std::vector<char> blockingSpaces,
	Burrow const& targetBurrow) const
{
	if ((!targetBurrow.containsOnlyHomeSpecies) ||
		(targetBurrow.homeSpecies != speciesMoving) ||
		(targetBurrow.AmphipodsInBurrow() == burrowDepth)) 
	{
		return false;
	}
	for (char& space : blockingSpaces)
	{
		if (space != EMPTY_SPACE) { return false; }
	}
	return true;
}

// A WarrenState is the final desired state if each burrow is full and contains only the species
// that that burrow is supposed to hold (their 'home' species).
bool Amphipod::WarrenState::IsFinalState() const
{
	return (
		amberBurrow.containsOnlyHomeSpecies &&
		(amberBurrow.AmphipodsInBurrow() == burrowDepth) &&
		bronzeBurrow.containsOnlyHomeSpecies &&
		(bronzeBurrow.AmphipodsInBurrow() == burrowDepth) &&
		copperBurrow.containsOnlyHomeSpecies &&
		(copperBurrow.AmphipodsInBurrow() == burrowDepth) &&
		desertBurrow.containsOnlyHomeSpecies &&
		(desertBurrow.AmphipodsInBurrow() == burrowDepth));
}

// For debug purposes, print the arrangement of amphipods represented by this state to the console.
// We don't want to actually edit the state as we do this, so we have to take a copy of each
// burrow contents in order to look through the stacks without editing the original.
void Amphipod::WarrenState::PrintSelfToScreen() const
{
	std::stack<Species> editableAmberBurrow = amberBurrow.burrowContents;
	std::stack<Species> editableBronzeBurrow = bronzeBurrow.burrowContents;
	std::stack<Species> editableCopperBurrow = copperBurrow.burrowContents;
	std::stack<Species> editableDesertBurrow = desertBurrow.burrowContents;

	std::cout << "Cost so far: " << costSoFar << std::endl;
	std::cout << "#############\n";
	std::cout << "#" << leftCorridor[1] << leftCorridor[0] <<
		EMPTY_SPACE << amberBronzeConnection <<
		EMPTY_SPACE << bronzeCopperConnection <<
		EMPTY_SPACE << copperDesertConnection << EMPTY_SPACE <<
		rightCorridor[0] << rightCorridor[1] << "#\n";
	std::cout << "###" << (editableAmberBurrow.size() == burrowDepth ? static_cast<char>(editableAmberBurrow.top()) : EMPTY_SPACE) <<
		"#" << (editableBronzeBurrow.size() == burrowDepth ? static_cast<char>(editableBronzeBurrow.top()) : EMPTY_SPACE) <<
		"#" << (editableCopperBurrow.size() == burrowDepth ? static_cast<char>(editableCopperBurrow.top()) : EMPTY_SPACE) <<
		"#" << (editableDesertBurrow.size() == burrowDepth ? static_cast<char>(editableDesertBurrow.top()) : EMPTY_SPACE) << "###\n";

	for (unsigned int remainingRows = burrowDepth - 1; remainingRows > 0; remainingRows--)
	{
		if (editableAmberBurrow.size() > remainingRows) { editableAmberBurrow.pop(); }
		if (editableBronzeBurrow.size() > remainingRows) { editableBronzeBurrow.pop(); }
		if (editableCopperBurrow.size() > remainingRows) { editableCopperBurrow.pop(); }
		if (editableDesertBurrow.size() > remainingRows) { editableDesertBurrow.pop(); }

		std::cout << "  #" << (editableAmberBurrow.size() == remainingRows ? static_cast<char>(editableAmberBurrow.top()) : EMPTY_SPACE) <<
			"#" << (editableBronzeBurrow.size() == remainingRows ? static_cast<char>(editableBronzeBurrow.top()) : EMPTY_SPACE) <<
			"#" << (editableCopperBurrow.size() == remainingRows ? static_cast<char>(editableCopperBurrow.top()) : EMPTY_SPACE) <<
			"#" << (editableDesertBurrow.size() == remainingRows ? static_cast<char>(editableDesertBurrow.top()) : EMPTY_SPACE) << "#  \n";
	}
	std::cout << "  #########  \n" << std::endl;
}
