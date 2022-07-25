#pragma once

namespace Amphipod
{
	// The amphipods in a burrow can be from one of 4 species, which is most readably implemented
	// as an enum. We will use simple chars to represent the spaces outside the burrows though,
	// so use a char as the backing type for the enum, and also define a separate char to represent
	// an empty space.
	enum class Species : char
	{
		AMBER = 'A',
		BRONZE = 'B',
		COPPER = 'C',
		DESERT = 'D'
	};
	constexpr char EMPTY_SPACE = '.';
	Species SpeciesFromChar(char speciesAsChar);

	// Each species of amphipod has a different energy cost required for moving them a
	// single space.
	constexpr unsigned int AMBER_MOVE_COST = 1;
	constexpr unsigned int BRONZE_MOVE_COST = 10;
	constexpr unsigned int COPPER_MOVE_COST = 100;
	constexpr unsigned int DESERT_MOVE_COST = 1000;
	unsigned int MoveCostFromSpecies(Species species);

	// Enumerates every possible move from one of the burrows to a space in the corridor. Required
	// when working out the change in lower bound of solution 'cheapness' resulting from a given
	// burrow-to-corridor move.
	// Amphipods are not allowed to stop in the corridor spaces directly outside of burrows, so the
	// only corridor spaces we need to consider are the two in each of the left and right ends of
	// the corridor (where the inner of each of those is the 0th position for that corridor end,
	// and the outer is the 1st), and the connecting spaces between each of the burrows (e.g. the
	// 'Amber-Bronze connection is the corridor between the Amber and Bronze burrows).
	enum class BurrowToCorridorMoves
	{
		AMBER_BURROW_TO_LEFT_CORRIDOR_ZERO,
		AMBER_BURROW_TO_LEFT_CORRIDOR_ONE,
		AMBER_BURROW_TO_AMBER_BRONZE_CONNECTION,
		AMBER_BURROW_TO_BRONZE_COPPER_CONNECTION,
		AMBER_BURROW_TO_COPPER_DESERT_CONNECTION,
		AMBER_BURROW_TO_RIGHT_CORRIDOR_ZERO,
		AMBER_BURROW_TO_RIGHT_CORRIDOR_ONE,
		BRONZE_BURROW_TO_LEFT_CORRIDOR_ZERO,
		BRONZE_BURROW_TO_LEFT_CORRIDOR_ONE,
		BRONZE_BURROW_TO_AMBER_BRONZE_CONNECTION,
		BRONZE_BURROW_TO_BRONZE_COPPER_CONNECTION,
		BRONZE_BURROW_TO_COPPER_DESERT_CONNECTION,
		BRONZE_BURROW_TO_RIGHT_CORRIDOR_ZERO,
		BRONZE_BURROW_TO_RIGHT_CORRIDOR_ONE,
		COPPER_BURROW_TO_LEFT_CORRIDOR_ZERO,
		COPPER_BURROW_TO_LEFT_CORRIDOR_ONE,
		COPPER_BURROW_TO_AMBER_BRONZE_CONNECTION,
		COPPER_BURROW_TO_BRONZE_COPPER_CONNECTION,
		COPPER_BURROW_TO_COPPER_DESERT_CONNECTION,
		COPPER_BURROW_TO_RIGHT_CORRIDOR_ZERO,
		COPPER_BURROW_TO_RIGHT_CORRIDOR_ONE,
		DESERT_BURROW_TO_LEFT_CORRIDOR_ZERO,
		DESERT_BURROW_TO_LEFT_CORRIDOR_ONE,
		DESERT_BURROW_TO_AMBER_BRONZE_CONNECTION,
		DESERT_BURROW_TO_BRONZE_COPPER_CONNECTION,
		DESERT_BURROW_TO_COPPER_DESERT_CONNECTION,
		DESERT_BURROW_TO_RIGHT_CORRIDOR_ZERO,
		DESERT_BURROW_TO_RIGHT_CORRIDOR_ONE
	};

	// Any move from a burrow to a corridor will have some amount of 'useful' corridor movement,
	// which may be positive or negative depending on whether it moves the amphipod close to its
	// home burrow, or further from it. Movement within the burrow itself is not considered
	// in this number (the rules the puzzle imposes on legal moves mean that movement
	// out of a burrow will always be 'useful', and so ends up being uninteresting for the
	// purposes we use these arrays for).
	// These constant arrays provide this 'useful' movement for each species of amphipod for each 
	// possible move. For example, a move from the Amber burrow to the Amber-Bronze connection
	// (AMBER_BURROW_TO_AMBER_BRONZE_CONN_USEFUL_MOVEMENT) is 2 useful steps of corridor for a Bronze,
	// Copper or Desert amphipod, since it moves them 2 steps along the corridor closer to their
	// target burrows. But for an Amber amphipod these are steps away from its target burrow, and
	// the amount of useful movement is negative, -2. These values have been worked out by hand
	// for each possible burrow-to-corridor move.
	constexpr int AMBER_BURROW_TO_LEFT_CORRIDOR_ZERO_USEFUL_MOVEMENT[4] = { -2, 0, 0, 0 };
	constexpr int AMBER_BURROW_TO_LEFT_CORRIDOR_ONE_USEFUL_MOVEMENT[4] = { -3, -1, -1, -1 };
	constexpr int AMBER_BURROW_TO_AMBER_BRONZE_CONN_USEFUL_MOVEMENT[4] = { -2, 2, 2, 2 };
	constexpr int AMBER_BURROW_TO_BRONZE_COPPER_CONN_USEFUL_MOVEMENT[4] = { -4, 2, 4, 4 };
	constexpr int AMBER_BURROW_TO_COPPER_DESERT_CONN_USEFUL_MOVEMENT[4] = { -6, 0, 4, 6 };
	constexpr int AMBER_BURROW_TO_RIGHT_CORRIDOR_ZERO_USEFUL_MOVEMENT[4] = { -8, -2, 2, 6 };
	constexpr int AMBER_BURROW_TO_RIGHT_CORRIDOR_ONE_USEFUL_MOVEMENT[4] = { -9, -3, 1, 5 };
	constexpr int BRONZE_BURROW_TO_LEFT_CORRIDOR_ZERO_USEFUL_MOVEMENT[4] = { 2, -4, -2, -2 };
	constexpr int BRONZE_BURROW_TO_LEFT_CORRIDOR_ONE_USEFUL_MOVEMENT[4] = { 1, -5, -3, -3 };
	constexpr int BRONZE_BURROW_TO_AMBER_BRONZE_CONN_USEFUL_MOVEMENT[4] = { 2, -2, 0, 0 };
	constexpr int BRONZE_BURROW_TO_BRONZE_COPPER_CONN_USEFUL_MOVEMENT[4] = { 0, -2, 2, 2 };
	constexpr int BRONZE_BURROW_TO_COPPER_DESERT_CONN_USEFUL_MOVEMENT[4] = { -2, -4, 2, 4 };
	constexpr int BRONZE_BURROW_TO_RIGHT_CORRIDOR_ZERO_USEFUL_MOVEMENT[4] = { -4, -6, 0, 4 };
	constexpr int BRONZE_BURROW_TO_RIGHT_CORRIDOR_ONE_USEFUL_MOVEMENT[4] = { -5, -7, -1, 3 };
	constexpr int COPPER_BURROW_TO_LEFT_CORRIDOR_ZERO_USEFUL_MOVEMENT[4] = { 4, 0, -6, -4 };
	constexpr int COPPER_BURROW_TO_LEFT_CORRIDOR_ONE_USEFUL_MOVEMENT[4] = { 3, -1, -7, -5 };
	constexpr int COPPER_BURROW_TO_AMBER_BRONZE_CONN_USEFUL_MOVEMENT[4] = { 4, 2, -4, -2 };
	constexpr int COPPER_BURROW_TO_BRONZE_COPPER_CONN_USEFUL_MOVEMENT[4] = { 2, 2, -2, 0 };
	constexpr int COPPER_BURROW_TO_COPPER_DESERT_CONN_USEFUL_MOVEMENT[4] = { 0, 0, -2, 2 };
	constexpr int COPPER_BURROW_TO_RIGHT_CORRIDOR_ZERO_USEFUL_MOVEMENT[4] = { -2, -2, -4, 2 };
	constexpr int COPPER_BURROW_TO_RIGHT_CORRIDOR_ONE_USEFUL_MOVEMENT[4] = { -3, -3, -5, 1 };
	constexpr int DESERT_BURROW_TO_LEFT_CORRIDOR_ZERO_USEFUL_MOVEMENT[4] = { 6, 2, -2, -8 };
	constexpr int DESERT_BURROW_TO_LEFT_CORRIDOR_ONE_USEFUL_MOVEMENT[4] = { 5, 1, -3, -9 };
	constexpr int DESERT_BURROW_TO_AMBER_BRONZE_CONN_USEFUL_MOVEMENT[4] = { 6, 4, 0, -6 };
	constexpr int DESERT_BURROW_TO_BRONZE_COPPER_CONN_USEFUL_MOVEMENT[4] = { 4, 4, 2, -4 };
	constexpr int DESERT_BURROW_TO_COPPER_DESERT_CONN_USEFUL_MOVEMENT[4] = { 2, 2, 2, -2 };
	constexpr int DESERT_BURROW_TO_RIGHT_CORRIDOR_ZERO_USEFUL_MOVEMENT[4] = { 0, 0, 0, -2 };
	constexpr int DESERT_BURROW_TO_RIGHT_CORRIDOR_ONE_USEFUL_MOVEMENT[4] = { -1, -1, -1, -3 };

	int GetUsefulMovementFromMove(BurrowToCorridorMoves moveType, Species species);
}