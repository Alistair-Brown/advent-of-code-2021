#include "Navigation.h"
#include "Parsing.h"
#include <iostream>
#include <string>
#include "puzzle_solvers.h"

// Day 2 tracks the position of a submarine as a series of movement commands are applied to
// it.
// The simple submarine of part 1 applies movement commands in one way, while the part 2
// submarine adopts a more complex approach involving 'aim'.
PuzzleAnswerPair PuzzleSolvers::AocDayTwoSolver(std::ifstream &puzzleInputFile)
{
	Navigation::Position simpleSubmarinePosition;
	Navigation::Position moreComplexSubmarinePosition;

	std::vector<std::string> commandList = Parsing::SeparateRemainingInputIntoLines(puzzleInputFile);

	// For each command in the list, parse it into the constituent direction and magnitude
	// components, using these to create a MovementCommand we can apply to each of our submarines.
	for (std::string singleDirection : commandList)
	{
		std::string directionStr{};
		int magnitude{};

		Parsing::ParseStringIntoElements(singleDirection, std::regex{ "([a-z]+) ([0-9]+)" }, directionStr, magnitude);
		Navigation::MovementCommand command { Navigation::GetDirectionFromString(directionStr), magnitude };

		simpleSubmarinePosition.ApplyMovement(command);
		moreComplexSubmarinePosition.ApplyMovementWithAim(command);
	}

	return PuzzleAnswerPair{
		std::to_string(simpleSubmarinePosition.GetProductOfPositionsInvertingDepth()) ,
		std::to_string(moreComplexSubmarinePosition.GetProductOfPositionsInvertingDepth()) };
}