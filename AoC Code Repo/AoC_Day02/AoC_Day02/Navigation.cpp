#include "Navigation.h"
#include <cassert>
#include <iostream>

Navigation::Direction Navigation::GetDirectionFromString(std::string directionString)
{
	Direction directionToReturn;

	if (directionString == "forward") { directionToReturn = Direction::Forwards; }
	else if (directionString == "backward") { directionToReturn = Direction::Backwards; }
	else if (directionString == "up") { directionToReturn = Direction::Up; }
	else if (directionString == "down") { directionToReturn = Direction::Down; }
	else { assert(false); }

	return directionToReturn;
}

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
	case Direction::Backwards:
		horizontalPosition -= movement.magnitude;
		break;
	default:
		assert(false);
		break;
	}
}

void Navigation::Position::ApplyMovementWithAim(MovementCommand movement)
{
	switch (movement.direction)
	{
	case Direction::Up:
		aim -= movement.magnitude;
		break;
	case Direction::Down:
		aim += movement.magnitude;
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

int Navigation::Position::GetProductOfPositions()
{
	return horizontalPosition * verticalPosition;
}

int Navigation::Position::GetProductOfPositionsInvertingDepth()
{
	std::cout << "Horizontal position : " << horizontalPosition << ", Vertical Position : " << verticalPosition << std::endl;
	return horizontalPosition * depth();
}
