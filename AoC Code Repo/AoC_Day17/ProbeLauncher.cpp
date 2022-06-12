#include "ProbeLauncher.h"
#include <cassert>

// A ProbeLauncher is initialised with an area through which we desire to launch the probe.
// This area is given as a min & max of both x & y values, to form a square.
// Using this area, the lower and upper bounds of initial probe velocties which might pass
// through this area are calculated.
Launcher::ProbeLauncher::ProbeLauncher(std::pair<int, int> xRange, std::pair<int, int> yRange)
{
	targetRangeCoords = { xRange, yRange };

	// Lower bound of y-velocity is simply that which means the probe will not overshoot the
	// minimum y position on the very first step (bearing in mind that all target y-positions
	// are guaranteed to be negative).
	yVelocityLowerBound = yRange.first;

	// The way that y-velocity decreases by 1 each step means that, for any positive initial
	// y-velocity, the probe will cross the y=0 with the opposite velocity to that which it was
	// initially fired with. So the max y velocity is the one that *just* lands in the target zone
	// on the step following the one it re-hit y=0 with.
	yVelocityUpperBound = (-yRange.first) - 1;

	// The x-velocity lower bound will *just* reach the target zone. At that point it will have travelled
	// a distance equal to the sum of an arithmetic sequence starting at 1 and incrementing by 1 each
	// step.
	// That lower bound x-velocity is equal to the number of steps the probe will take before the
	// x-velocity reaches 0, and so the x speed lower bound I'm looking for is equal to the number
	// of steps in the minimum sequence size that reaches the target zone.
	int tempSum;
	for (int numSteps = 1;; numSteps++)
	{
		tempSum = numSteps * (2 + (numSteps - 1)) / 2;
		if (tempSum >= xRange.first)
		{
			// The sum of steps must cause us to actually finish *within* the target zone, i.e.
			// less that the maximum-x as well as greater than the minimum-x.
			assert(tempSum <= xRange.second);
			xVelocityLowerBound = numSteps;
			break;
		}
	}

	// The x speed upper bound is that which will *just* not overshoot the target zone on the first step.
	xVelocityUpperBound = xRange.second;
	
}

// We can use an arithmethic sequence to find the highest y coordinate a probe
// could be fired to and still pass through the target zone on the way back down.
// Using the known y-velocity upper bound, the highest y height reached is
// the sum of an arithmetic sequence that starts with 1 and adds 1 each time
// (considering a sort of backwards view where we're accelerating from the high
// point back to the origin).
// In this sequence, the number of steps is equal to the starting speed, since the
// speed only goes down by 1 each step.
int Launcher::ProbeLauncher::HighestYCoordinate()
{
	return (yVelocityUpperBound + 1) * yVelocityUpperBound / 2;
}

// To calculate the number of possible starting velocities that will reach the target zone,
// we first find the number of steps that it would take for each of our possible x and y velocity
// components to reach the target zone along their potential axes. Having done that, we can see
// how many overlaps exist between those two lists (i.e. if a given x-velocity will spend steps 8 & 9
// in the target zone, then for each possible y-velocity that will be in the target zone after
// that many steps, we have a different unique starting velocity that finishes in the target
// zone).
unsigned int Launcher::ProbeLauncher::NumberOfPossibleVelocities()
{
	// First find the number of steps it would take each of our potential x velocities
	// to reach the target zone (some will not reach the zone). It's possible that a
	// given starting velocity will spend multiple steps in the target zone, so we have
	// a set of potential step numbers for each starting velocity.
	// The drag formula means that the x-velocity may reach zero in the target zone, in
	// which case the set will contain a -1 to indicate that any numbers of y-steps greater
	// than the other number(s) in that x-set should also be considered overlaps.
	std::vector<std::set<int>> numbersOfXSteps;
	for (int xVelocity = xVelocityLowerBound; xVelocity <= xVelocityUpperBound; xVelocity++)
	{
		std::set<int> possibleStepCounts = XStepsToReachTarget(xVelocity);
		if (possibleStepCounts.size() > 0)
		{
			numbersOfXSteps.push_back(possibleStepCounts);
		}
	}

	// Repeat for each of our starting y-velocities. As with x, a probe may spend multiple
	// steps in the target y-region, but unlike x it can never remain stopped forever, so
	// no '-1' values to worry about.
	std::vector<std::set<int>> numbersOfYSteps;
	for (int yVelocity = yVelocityLowerBound; yVelocity <= xVelocityUpperBound; yVelocity++)
	{
		std::set<int> possibleStepCounts = YStepsToReachTarget(yVelocity);
		if (possibleStepCounts.size() > 0)
		{
			numbersOfYSteps.push_back(possibleStepCounts);
		}
	}

	// Find the number of overlaps between these two sets, each of which represents a distinct
	// starting velocity which will spend at least one step in the target zone.
	unsigned int totalPossibleVelocities = NumberOfVelocitiesWithOverlappingStepsToTarget(
		numbersOfXSteps, numbersOfYSteps
	);

	return totalPossibleVelocities;
}

// For a given starting x-velocity, return a set of the step numbers a probe would spend
// within the target region of x-values. A given probe might not spend any steps in the
// target zone, or might spend multiple. Some x-velocities will result in the probe coming
// to a stop in the target x-region, which will be indicated by the set containing a '-1'.
std::set<int> Launcher::ProbeLauncher::XStepsToReachTarget(int initialXVelocity)
{
	std::set<int> possibleStepCounts;
	int stepsTaken{ 0 };
	int currentXPosition{ 0 };
	for (int currentSpeed = initialXVelocity; ; currentSpeed--)
	{
		// With a speed of 0, the probe has come to rest. Given that the previous
		// step didn't break the loop, we have not exceeded the target range. If there
		// are any entries in the possibleStepCounts, we have come to a rest within
		// the target range, which we will indicate by inserting a -1 into the set.
		if (currentSpeed == 0)
		{
			if (possibleStepCounts.size() > 0)
			{
				possibleStepCounts.insert(-1);
			}
			break;
		}

		stepsTaken++;
		currentXPosition += currentSpeed;

		// If our x-position exceeds the maximum of the range, there are no more steps to
		// consider. Otherwise, if our x-position exceeds the minimum of the range, this
		// is a step count that lands us in the target zone.
		if (currentXPosition > targetRangeCoords.first.second)
		{
			break;
		}
		else if (currentXPosition >= targetRangeCoords.first.first)
		{
			possibleStepCounts.insert(stepsTaken);
		}
	}

	return possibleStepCounts;
}

// For a given starting y-velocity, return a set of the step numbers a probe would spend
// within the target region of y-values. A given probe might not spend any steps in the
// target zone, or might spend multiple.
std::set<int> Launcher::ProbeLauncher::YStepsToReachTarget(int initialYVelocity)
{
	std::set<int> possibleStepCounts;
	int stepsTaken{ 0 };
	int yPosition{ 0 };
	for (int currentSpeed = initialYVelocity; ; currentSpeed--)
	{
		stepsTaken++;
		yPosition += currentSpeed;

		// If our y-position is less than the minimum of the range, we are below the minimum
		// y-positon and there are no more steps to consider. Otherwise, if our y-position
		// is less than or equal to the maximum of the range, this is a step count that lands
		// us in the target zone.
		if (yPosition < targetRangeCoords.second.first)
		{
			break;
		}
		else if (yPosition <= targetRangeCoords.second.second)
		{
			possibleStepCounts.insert(stepsTaken);
		}
	}

	return possibleStepCounts;
}

// Takes as arguments a set of step numbers that a probe would be in the target zone after taking,
// for a number of distinct x and y velocity components. Each case where a given step count appears
// in an element in both the x and y vector represents a different unique starting velocity that
// would place a probe in the target region.
unsigned int Launcher::ProbeLauncher::NumberOfVelocitiesWithOverlappingStepsToTarget(
	std::vector<std::set<int>> xStepsToReachTarget,
	std::vector<std::set<int>> yStepsToReachTarget
)
{
	unsigned int numberOfOverlappingVelocities{};
	for (std::set<int> stepNumbersForGivenY : yStepsToReachTarget)
	{
		for (std::set<int> stepNumbersForGivenX : xStepsToReachTarget)
		{
			for (int specificNumberOfYSteps : stepNumbersForGivenY)
			{
				// If this number of steps for a y-velocity to reach the target region is
				// also a number of steps that this x-velocity would take to reach the target
				// region then we have a hit. Break out so we don't end up double counting
				// this unique velocity.
				if (stepNumbersForGivenX.contains(specificNumberOfYSteps))
				{
					numberOfOverlappingVelocities++;
					break;
				}
				// If this is an x-velocity that came to rest in the target region, then we
				// have a match only if this number of y steps is at least as large as the
				// smallest number of steps that that x-velocity takes to reach the target
				// zone.
				else if (stepNumbersForGivenX.contains(-1))
				{
					if (specificNumberOfYSteps >= *(stepNumbersForGivenX.rbegin()))
					{
						numberOfOverlappingVelocities++;
						break;
					}
				}
			}
		}
	}

	return numberOfOverlappingVelocities;
}
