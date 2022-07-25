#include "puzzle_solvers.h"
#include "amphipod_burrows.h"
#include "Parsing.h"
#include <regex>

// Day 23 asks us to find the most efficient way to reorganise some amphipods within their burrows.
// Our puzzle input shows the initial state of the warren, with Amber (A), Bronze (B), Copper (C)
// and Desert (D) amphipods occupying each of the 4 burrows within the warren, connected by a long
// corridor (I'm using slightly different terminology than the original puzzle, which I think
// is easier to understand), like so:
// 
// #############
// #...........#
// ###D#C#A#B###
//   #D#C#B#A#
//   #########
//
// We must reorganise the amphipods so that all the Amber amphipods end up in the far left burrow,
// Bronze in the next, then Copper, and finally Desert in the far right burrow. Each type of amphipod
// has a different energy cost associated with moving it. Ambers cost 1 per space moved, Bronzes cost 10,
// Coppers cost 100 and Deserts cost 1000. Amphipods cannot move through each other. So more efficient
// solutions will be those that slide the cheaper amphipods around such that we minimise the number
// of spaces moved by the more expensive amphipods.
//
// For part 1 we must find the cheapest cost that the amphipods can be rearranged for. For part 2, we
// must again find the cheapest cost, but for a warren which has the following lines inserted in
// the middle of the burrows:
//
//   #D#C#B#A#
//   #D#B#A#C#
// 
// So the complete part 2 warren for the original input shown above would be:
//
// #############
// #...........#
// ###D#C#A#B###
//   #D#C#B#A#
//   #D#B#A#C#
//   #D#C#B#A#
//   #########
PuzzleAnswerPair PuzzleSolvers::AocDayTwentyThreeSolver(std::ifstream& puzzleInputFile)
{
	// Parse out the couple of lines of input which are actually interesting, checking that the
	// surrounding lines match what we'd expect for this puzzle's input too, just for good measure.
	std::vector<std::string> inputLines = Parsing::SeparateRemainingInputIntoLines(puzzleInputFile);
	assert(inputLines.size() == 5);
	assert(inputLines[0] == "#############");
	assert(inputLines[1] == "#...........#");
	assert(inputLines[4] == "  #########");
	std::vector<std::string> relevantLines = { inputLines[2], inputLines[3] };
	
	// We can now use a regex to identify the characters in each of the 2 spots in each
	// burrow.
	std::regex lineOne{ "###([A-D])#([A-D])#([A-D])#([A-D])###" };
	std::regex lineTwo{ "#([A-D])#([A-D])#([A-D])#([A-D])#" };
	std::pair<char, char> amberChars{};
	std::pair<char, char> bronzeChars{};
	std::pair<char, char> copperChars{};
	std::pair<char, char> desertChars{};
	Parsing::ParseStringIntoElements(
		relevantLines[0],
		lineOne,
		amberChars.first,
		bronzeChars.first,
		copperChars.first,
		desertChars.first);
	Parsing::ParseStringIntoElements(
		relevantLines[1],
		lineTwo,
		amberChars.second,
		bronzeChars.second,
		copperChars.second,
		desertChars.second);

	// The amphipods in each burrow can be represented as a vector of characters, working from the
	// top to bottom of each burrow. We need two sets of burrows, one for the shallower part one
	// warren, and one for the deeper burrows in part two.
	std::vector<char> amberBurrow{};
	std::vector<char> bronzeBurrow{};
	std::vector<char> copperBurrow{};
	std::vector<char> desertBurrow{};
	std::vector<char> deeperAmberBurrow{};
	std::vector<char> deeperBronzeBurrow{};
	std::vector<char> deeperCopperBurrow{};
	std::vector<char> deeperDesertBurrow{};

	// The part one burrows just contain the amphipods shown on the puzzle input.
	amberBurrow.push_back(amberChars.first);	
	bronzeBurrow.push_back(bronzeChars.first);
	copperBurrow.push_back(copperChars.first);
	desertBurrow.push_back(desertChars.first);
	amberBurrow.push_back(amberChars.second);
	bronzeBurrow.push_back(bronzeChars.second);
	copperBurrow.push_back(copperChars.second);
	desertBurrow.push_back(desertChars.second);

	// The deeper burrows of part 2 are constructed as if the puzzle input had contained the
	// following lines between the top and bottom of each burrow:
	// #D#C#B#A#
	// #D#B#A#C#
	deeperAmberBurrow.push_back(amberChars.first);
	deeperBronzeBurrow.push_back(bronzeChars.first);
	deeperCopperBurrow.push_back(copperChars.first);
	deeperDesertBurrow.push_back(desertChars.first);
	deeperAmberBurrow.push_back('D');
	deeperBronzeBurrow.push_back('C');
	deeperCopperBurrow.push_back('B');
	deeperDesertBurrow.push_back('A');
	deeperAmberBurrow.push_back('D');
	deeperBronzeBurrow.push_back('B');
	deeperCopperBurrow.push_back('A');
	deeperDesertBurrow.push_back('C');
	deeperAmberBurrow.push_back(amberChars.second);
	deeperBronzeBurrow.push_back(bronzeChars.second);
	deeperCopperBurrow.push_back(copperChars.second);
	deeperDesertBurrow.push_back(desertChars.second);

	// We can now construct the warrens for both puzzle parts and ask them for their respective
	// cheapest solves.
	Amphipod::WarrenSolver warrenSolver{ 2, amberBurrow, bronzeBurrow, copperBurrow, desertBurrow };
	Amphipod::WarrenSolver deeperWarrenSolver{ 4, deeperAmberBurrow, deeperBronzeBurrow, deeperCopperBurrow, deeperDesertBurrow };
	unsigned int cheapestSolvePartOne = warrenSolver.CheapestSolve();
	unsigned int cheapestSolvePartTwo = deeperWarrenSolver.CheapestSolve();

	return PuzzleAnswerPair{ std::to_string(cheapestSolvePartOne), std::to_string(cheapestSolvePartTwo)};
}