#pragma once
#include <string>

namespace Navigation {

	enum Direction { Up, Down, Forwards };
	Direction GetDirectionFromString(std::string directionString);

	// A command to move something according today's puzzle input scheme takes a direction and a magnitude.
	struct MovementCommand {
		public:
			Direction direction;
			int magnitude;
			
			MovementCommand(Direction direction, int magnitude) :
				direction(direction), magnitude(magnitude) {}
	};

	// Holds the position of an object which has both a horizontal and vertical component.
	// The user cannot read the individul position components, but can apply a movement input
	// to translate the position.
	// Can return the product of the two position components, either as a direct multiplication,
	// or by inverting the vertical position to account for a submarine position where negative
	// vertical position indicates positive depth.
	class Position {
	private:
		int horizontalPosition{ 0 };
		int verticalPosition{ 0 };
		int aim{ 0 };
		int depth() { return verticalPosition * -1; }
	public:
		void ApplyMovement(MovementCommand movement);
		void ApplyMovementWithAim(MovementCommand movement);
		int GetProductOfPositions();
		int GetProductOfPositionsInvertingDepth();
	};
}