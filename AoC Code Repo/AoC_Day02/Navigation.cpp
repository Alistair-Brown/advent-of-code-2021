#include "Navigation.h"
#include <cassert>
#include <iostream>

// Convert a direction from string format (case-sensitive) into the equivalent
// Direction enum for easier use elsewhere in this namespace.
Navigation::Direction Navigation::GetDirectionFromString(std::string directionString)
{
	Direction directionToReturn;

	if (directionString == "forward") { directionToReturn = Direction::Forwards; }
	else if (directionString == "up") { directionToReturn = Direction::Up; }
	else if (directionString == "down") { directionToReturn = Direction::Down; }
	else { assert(false); }

	return directionToReturn;
}

// Apply a translation to a Position object, according to the direction and
// magnitude in a MovementCommand.
void Navigation::Position::ApplyMovement(MovementCommand movement)
{
	switch (movement.direction)	
	{
	case Direction::Up:
		verticalPosition += movement.magnitude;
		break;
	case Direction::Down:
		verticalPosition -= movement.magnitude;
		break;
	case Direction::Forwards:
		horizontalPosition += movement.magnitude;
		break;
	default:
		assert(false);
		break;
	}
}

// As per the instruction for day 2, movement commands now do the following:
//  - Up increases our 'aim' (according to a true axis, not an inverted one),
//    but has no impact on position
//  - Down decreases our 'aim'
//  - Forwards both increses the horizontal position, and increases the vertical
//    position by current aim multiplied by the movement magnitude.
void Navigation::Position::ApplyMovementWithAim(MovementCommand movement)
{
	switch (movement.direction)
	{
	case Direction::Up:
		aim += movement.magnitude;
		break;
	case Direction::Down:
		aim -= movement.magnitude;
		break;
	case Direction::Forwards:
		horizontalPosition += movement.magnitude;
		verticalPosition += aim * movement.magnitude;
		break;
	default:
		assert(false);
		break;
	}
}

int Navigation::Position::GetProductOfPositions() const
{
	return horizontalPosition * verticalPosition;
}

int Navigation::Position::GetProductOfPositionsInvertingDepth() const
{
	return horizontalPosition * depth();
}
