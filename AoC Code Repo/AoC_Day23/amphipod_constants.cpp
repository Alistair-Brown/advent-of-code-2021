#include "amphipod_constants.h"
#include <set>
#include <cassert>

// Casting directly from a char to an enum is risky, as your char may not be a valid enum
// value. This helper function provides a little bit of safety by checking we have a valid
// char before we perform the cast.
Amphipod::Species Amphipod::SpeciesFromChar(char speciesAsChar)
{
	const static std::set<char> validSpeciesChars{ 'A', 'B', 'C', 'D' };

	assert(validSpeciesChars.contains(speciesAsChar));

	return static_cast<Species>(speciesAsChar);
}

// Return the cost of moving an amphipod of the given species a single space.
unsigned int Amphipod::MoveCostFromSpecies(Species species)
{
	switch (species)
	{
	case Species::AMBER:
		return AMBER_MOVE_COST;
		break;
	case Species::BRONZE:
		return BRONZE_MOVE_COST;
		break;
	case Species::COPPER:
		return COPPER_MOVE_COST;
		break;
	case Species::DESERT:
		return DESERT_MOVE_COST;
		break;
	default:
		assert(false);
		return 0;
		break;
	}
}

// Find the number of corridor movement steps which represent 'useful' movement for a given
// species of amphipod making a given Burrow-to-Corridor move (i.e. how many steps along the
// corridor that amphipod is closer to its home burrow after the move). This number may be
// negative if the amphipod will end up further from its home burrow than it started.
int Amphipod::GetUsefulMovementFromMove(BurrowToCorridorMoves moveType, Species species)
{
	// Given that each array of useful movement per-species is ordered from Amber to Desert,
	// we can convert our input species to the relevant index to that array with a little
	// char arithmetic.
	int speciesIndex = static_cast<char>(species) - 'A';

	switch (moveType)
	{
	case (BurrowToCorridorMoves::AMBER_BURROW_TO_LEFT_CORRIDOR_ZERO):
		return AMBER_BURROW_TO_LEFT_CORRIDOR_ZERO_USEFUL_MOVEMENT[speciesIndex];
		break;
	case (BurrowToCorridorMoves::AMBER_BURROW_TO_LEFT_CORRIDOR_ONE):
		return AMBER_BURROW_TO_LEFT_CORRIDOR_ONE_USEFUL_MOVEMENT[speciesIndex];
		break;
	case (BurrowToCorridorMoves::AMBER_BURROW_TO_AMBER_BRONZE_CONNECTION):
		return AMBER_BURROW_TO_AMBER_BRONZE_CONN_USEFUL_MOVEMENT[speciesIndex];
		break;
	case (BurrowToCorridorMoves::AMBER_BURROW_TO_BRONZE_COPPER_CONNECTION):
		return AMBER_BURROW_TO_AMBER_BRONZE_CONN_USEFUL_MOVEMENT[speciesIndex];
		break;
	case (BurrowToCorridorMoves::AMBER_BURROW_TO_COPPER_DESERT_CONNECTION):
		return AMBER_BURROW_TO_COPPER_DESERT_CONN_USEFUL_MOVEMENT[speciesIndex];
		break;
	case (BurrowToCorridorMoves::AMBER_BURROW_TO_RIGHT_CORRIDOR_ZERO):
		return AMBER_BURROW_TO_RIGHT_CORRIDOR_ZERO_USEFUL_MOVEMENT[speciesIndex];
		break;
	case (BurrowToCorridorMoves::AMBER_BURROW_TO_RIGHT_CORRIDOR_ONE):
		return AMBER_BURROW_TO_RIGHT_CORRIDOR_ONE_USEFUL_MOVEMENT[speciesIndex];
		break;
	case (BurrowToCorridorMoves::BRONZE_BURROW_TO_LEFT_CORRIDOR_ZERO):
		return BRONZE_BURROW_TO_LEFT_CORRIDOR_ZERO_USEFUL_MOVEMENT[speciesIndex];
		break;
	case (BurrowToCorridorMoves::BRONZE_BURROW_TO_LEFT_CORRIDOR_ONE):
		return BRONZE_BURROW_TO_LEFT_CORRIDOR_ONE_USEFUL_MOVEMENT[speciesIndex];
		break;
	case (BurrowToCorridorMoves::BRONZE_BURROW_TO_AMBER_BRONZE_CONNECTION):
		return BRONZE_BURROW_TO_AMBER_BRONZE_CONN_USEFUL_MOVEMENT[speciesIndex];
		break;
	case (BurrowToCorridorMoves::BRONZE_BURROW_TO_BRONZE_COPPER_CONNECTION):
		return BRONZE_BURROW_TO_AMBER_BRONZE_CONN_USEFUL_MOVEMENT[speciesIndex];
		break;
	case (BurrowToCorridorMoves::BRONZE_BURROW_TO_COPPER_DESERT_CONNECTION):
		return BRONZE_BURROW_TO_COPPER_DESERT_CONN_USEFUL_MOVEMENT[speciesIndex];
		break;
	case (BurrowToCorridorMoves::BRONZE_BURROW_TO_RIGHT_CORRIDOR_ZERO):
		return BRONZE_BURROW_TO_RIGHT_CORRIDOR_ZERO_USEFUL_MOVEMENT[speciesIndex];
		break;
	case (BurrowToCorridorMoves::BRONZE_BURROW_TO_RIGHT_CORRIDOR_ONE):
		return BRONZE_BURROW_TO_RIGHT_CORRIDOR_ONE_USEFUL_MOVEMENT[speciesIndex];
		break;
	case (BurrowToCorridorMoves::COPPER_BURROW_TO_LEFT_CORRIDOR_ZERO):
		return COPPER_BURROW_TO_LEFT_CORRIDOR_ZERO_USEFUL_MOVEMENT[speciesIndex];
		break;
	case (BurrowToCorridorMoves::COPPER_BURROW_TO_LEFT_CORRIDOR_ONE):
		return COPPER_BURROW_TO_LEFT_CORRIDOR_ONE_USEFUL_MOVEMENT[speciesIndex];
		break;
	case (BurrowToCorridorMoves::COPPER_BURROW_TO_AMBER_BRONZE_CONNECTION):
		return COPPER_BURROW_TO_AMBER_BRONZE_CONN_USEFUL_MOVEMENT[speciesIndex];
		break;
	case (BurrowToCorridorMoves::COPPER_BURROW_TO_BRONZE_COPPER_CONNECTION):
		return COPPER_BURROW_TO_AMBER_BRONZE_CONN_USEFUL_MOVEMENT[speciesIndex];
		break;
	case (BurrowToCorridorMoves::COPPER_BURROW_TO_COPPER_DESERT_CONNECTION):
		return COPPER_BURROW_TO_COPPER_DESERT_CONN_USEFUL_MOVEMENT[speciesIndex];
		break;
	case (BurrowToCorridorMoves::COPPER_BURROW_TO_RIGHT_CORRIDOR_ZERO):
		return COPPER_BURROW_TO_RIGHT_CORRIDOR_ZERO_USEFUL_MOVEMENT[speciesIndex];
		break;
	case (BurrowToCorridorMoves::COPPER_BURROW_TO_RIGHT_CORRIDOR_ONE):
		return COPPER_BURROW_TO_RIGHT_CORRIDOR_ONE_USEFUL_MOVEMENT[speciesIndex];
		break;
	case (BurrowToCorridorMoves::DESERT_BURROW_TO_LEFT_CORRIDOR_ZERO):
		return DESERT_BURROW_TO_LEFT_CORRIDOR_ZERO_USEFUL_MOVEMENT[speciesIndex];
		break;
	case (BurrowToCorridorMoves::DESERT_BURROW_TO_LEFT_CORRIDOR_ONE):
		return DESERT_BURROW_TO_LEFT_CORRIDOR_ONE_USEFUL_MOVEMENT[speciesIndex];
		break;
	case (BurrowToCorridorMoves::DESERT_BURROW_TO_AMBER_BRONZE_CONNECTION):
		return DESERT_BURROW_TO_AMBER_BRONZE_CONN_USEFUL_MOVEMENT[speciesIndex];
		break;
	case (BurrowToCorridorMoves::DESERT_BURROW_TO_BRONZE_COPPER_CONNECTION):
		return DESERT_BURROW_TO_AMBER_BRONZE_CONN_USEFUL_MOVEMENT[speciesIndex];
		break;
	case (BurrowToCorridorMoves::DESERT_BURROW_TO_COPPER_DESERT_CONNECTION):
		return DESERT_BURROW_TO_COPPER_DESERT_CONN_USEFUL_MOVEMENT[speciesIndex];
		break;
	case (BurrowToCorridorMoves::DESERT_BURROW_TO_RIGHT_CORRIDOR_ZERO):
		return DESERT_BURROW_TO_RIGHT_CORRIDOR_ZERO_USEFUL_MOVEMENT[speciesIndex];
		break;
	case (BurrowToCorridorMoves::DESERT_BURROW_TO_RIGHT_CORRIDOR_ONE):
		return DESERT_BURROW_TO_RIGHT_CORRIDOR_ONE_USEFUL_MOVEMENT[speciesIndex];
		break;
	default:
		assert(false);
		return 0;
		break;
	}
}