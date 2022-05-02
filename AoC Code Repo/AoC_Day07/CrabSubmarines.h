#pragma once
#include <vector>

namespace CrabSubmarines
{
	// A CrabArmada is a collection of crab submarines, each of which has a position
	// along what is effectively a number line.
	// We can query the armada for the total fuel cost of converging all submarines
	// on a single point, where the armada will select the point to converge on as
	// being that with the lowest associated total fuel cost. There are two different
	// rules of fuel consumption that can be used when finding this minimal convergence.
	class CrabArmada
	{
	private:
		// Positions of crab submarines are held as a vector of possible positions, each
		// element of which holds the actual number of crab submarines at that position.
		// If a new crab being added to the armada exists at a position greater than the
		// current position vector, the armada can be resized to accomodate it.
		std::vector<unsigned int> crabsByPosition;
		unsigned int totalCrabSubmarines{ 0 };
		void MaybeResizeArmada(unsigned int requiredPosition);

		unsigned int PartOneOptimalConvergencePosition() const;
		unsigned int PartTwoOptimalConvergencePosition() const;
	public:
		void AddCrabSumbarine(unsigned int position);
		unsigned int PartOneMinimalConvergenceCost() const;
		unsigned int PartTwoMinimalConvergenceCost() const;
	};
}