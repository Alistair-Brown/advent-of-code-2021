#include "CrabSubmarines.h"
#include <iostream>
#include <cassert>
#include <deque>
#include "aoc_common_types.h"

// Given a required position of a crab submarine, resize the vector of possible
// submarine positions if it is not already large enough to accomodate this position.
// Note that we need to resize to 'requiredPosition + 1', since 0 is a possible
// position too.
void CrabSubmarines::CrabArmada::MaybeResizeArmada(unsigned int requiredPosition)
{
	if (requiredPosition >= crabsByPosition.size())
	{
		crabsByPosition.resize(requiredPosition + 1, 0);
	}
}

// Find the position that will cost the least to move all submarines to under the part one
// rules where the cost to move a single space is always 1. We can reason out that for any
// given potential convergence position, choosing the position to the right instead would
// cost all the crabs to the left of the current position 1 point of movement, but save
// all of the crabs to the right of that position 1 point of movement. And so the optimum
// position to move to under these rules is just the median position.
unsigned int CrabSubmarines::CrabArmada::PartOneOptimalConvergencePosition()
{
	unsigned int currentPosition{ 0 };
	int numberOfCrabs{ 0 };
	while (true)
	{
		numberOfCrabs += crabsByPosition[currentPosition];
		// The point at which we have more crabs on or to the left of this position is
		// that point at which we stop gaining anything by moving any further to the right.
		if ((float)numberOfCrabs >= (totalCrabSubmarines / 2.0))
		{
			break;
		}
		currentPosition++;
	}
	return currentPosition;
}

// Find the position that will cost the least to move all submarines to under the part two
// rules where the cost to move a given submarine a single space increases by 1 for each space
// already moved. 
// This function finds that optimum position by:
//  - For each possible position, find the cost of moving all submarines to the left of that
//    postion one step further to the right, and the cost of moving all submarines to the
//    right of that position one step further to the left.
//  - For each possible position, starting from the leftmost, compare the differential cost
//    of moving all submarines to the left one step further right to reach the next position, to
//    the differential cost of moving all submarines to the right one step further left in order to
//    reach this current position. The optimum position is the first position at which the
//    differential cost of moving the left hand submarines further right is greater than
//    the cost of bringing all of the righthand submarines one step further left.
unsigned int CrabSubmarines::CrabArmada::PartTwoOptimalConvergencePosition()
{
	std::vector<ULLINT> costOfNextMoveFromLeft;
	std::deque<ULLINT> costOfNextMoveFromRight;
	costOfNextMoveFromLeft.reserve(crabsByPosition.size());

	unsigned int optimumPosition{ 0 };
	unsigned long long int numberOfCrabsCollected{ 0 };
	unsigned long long costOfNextStep{ 0 };

	// When working out the differential cost to move each step further to the
	// left, we first update the total number of crabs 'collected' by our stepping through
	// positions to include the crabs at this position. Then the cost of the next step is
	// the cost of the previous step plus the total number of crabs collected. This approach
	// means that for the step immediately after a crab is 'collected' its contribution
	// to the cost of the next step will be 1. For the next step, it is counted again, and
	// so its total contribution is 2. For the next step, its total contribution becomes
	// 3, etc. according the the movement rules of this puzzle.
	for (unsigned int ii = 0; ii < crabsByPosition.size(); ii++)
	{
		numberOfCrabsCollected += crabsByPosition[ii];
		costOfNextStep += numberOfCrabsCollected;
		costOfNextMoveFromLeft.push_back(costOfNextStep);
	}

	// The process for calculating the differential cost of each move from the right
	// is the same as for the left, just working in the other direction along our
	// number line.
	costOfNextStep = 0;
	numberOfCrabsCollected = 0;
	for (int ii = crabsByPosition.size() - 1; ii >= 0; ii--)
	{
		numberOfCrabsCollected += crabsByPosition[ii];
		costOfNextStep += numberOfCrabsCollected;
		costOfNextMoveFromRight.push_front(costOfNextStep);
	}

	// The optimum position is the first from the left at which the cost of moving all
	// crabs to the left of that position one step further is greater than the cost of
	// bringing all crabs to the right of the position one additional step closer.
	for (unsigned int ii = 0; ii < crabsByPosition.size() - 1; ii++)
	{
		if (costOfNextMoveFromLeft[ii] > costOfNextMoveFromRight[ii + 1])
		{
			optimumPosition = ii;
			break;
		}
	}
	assert(optimumPosition > 0);
	return optimumPosition;
}

// Add a new crab submarine to a position within the armada, by incrementing
// the number of submarines at that position, and the total number in the armada.
// We may need to resize the armada if the required position doesn't currently fit
// into crabsByPosition.
void CrabSubmarines::CrabArmada::AddCrabSumbarine(unsigned int position)
{
	MaybeResizeArmada(position);
	crabsByPosition[position]++;
	totalCrabSubmarines++;
}

// In part one, the cost for each crab submarine to move to a position is simply the difference
// between its current position and the convergence position. 
// Find the minimum cost for all submarines to converge on a single point under these rules.
unsigned int CrabSubmarines::CrabArmada::PartOneMinimalConvergenceCost()
{
	// First find the optimal convergence position, which will require the least fuel
	// for all crabs to move there under the part one fuel consumption rules.
	int medianPosition = (int)PartOneOptimalConvergencePosition();

	// The total cost to converge is then just the sum of the differences between each
	// crab's position and the convergence position.
	unsigned int movementRequiredToConverge{ 0 };
	for (unsigned int ii = 0; ii < crabsByPosition.size(); ii++)
	{
		movementRequiredToConverge += crabsByPosition[ii] * abs((int)ii - medianPosition);
	}
	return movementRequiredToConverge;
}

// Part two has more complicated rules, where the cost of the next step taken by a
// given submarine grows linearly with the number of steps already taken. So the
// first step taken costs 1, the next step costs 2 etc.
// Find the minimum cost to converge all submarines to a single position under
// these rules.
unsigned int CrabSubmarines::CrabArmada::PartTwoMinimalConvergenceCost()
{
	// First find the optimal convergence position, which will require the least fuel
	// for all crabs to move there under the part two fuel consumption rules.
	int positionToMoveTo = (int)PartTwoOptimalConvergencePosition();

	// The cost to move to a given position under the part 2 rules is 1 + 2 + ... + n,
	// where n is the total number of steps we need to move. So our cost to move from
	// one position to another is the sum of an arithmetic sequence of n steps, where
	// the first number in the sequence is 1 and the nth is n.
	int movementRequiredToConverge{ 0 };
	for (unsigned int ii = 0; ii < crabsByPosition.size(); ii++)
	{
		unsigned int spacesToMove = abs((int)ii - positionToMoveTo);
		int movementCost = spacesToMove * (spacesToMove + 1) / 2;
		movementRequiredToConverge += movementCost * crabsByPosition[ii];
	}
	return movementRequiredToConverge;
}
