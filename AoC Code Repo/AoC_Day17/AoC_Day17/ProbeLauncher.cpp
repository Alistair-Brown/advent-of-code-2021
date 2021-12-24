#include "ProbeLauncher.h"
#include <cassert>
#include <vector>
#include <set>

Launcher::ProbeLauncher::ProbeLauncher(std::pair<int, int> xRange, std::pair<int, int> yRange)
{
	targetRangeCoords = { xRange, yRange };

	// The probe will cross the origin line exactly on it's way back down. So assuming our target
	// position is below us, the max y speed is the one that *just* lands in the target zone
	// on the step following the one it hit the origin y with.
	// Also assuming the yRange is given with the lower coordinate first.
	yVelocityUpperBound = (-yRange.first) - 1;

	// x speed lower bound *just* reaches the target zone. At which point it will have travelled
	// a distance equal to the sum of an arithmetic sequence starting at 1 and incrementing by 1 each
	// step.
	// Number of steps can be got from this calculation too.
	// The initial x speed is equal to the number of steps the probe will take before stopping
	// x movement, and so the x speed lower bound I'm looking for is equal to the number
	// of steps in the minimum sequence size that reaches the target zone.
	int tempSum;
	for (int numSteps = 1;; numSteps++)
	{
		tempSum = numSteps * (2 + (numSteps - 1)) / 2;
		if (tempSum >= xRange.first)
		{
			// If this arithmetic sequence approach fails to actually overlap with the target range
			// (i.e. there is no sum of the sequence that is greater than min x but less than max x),
			// hit an assert because I need to rethink.
			assert(tempSum <= xRange.second);
			stepsRequiredForXLowerBound = numSteps;
			xVelocityLowerBound = numSteps;
			break;
		}
	}

	// The x speed upper bound is the one that will just not overshoot the target zone on the first step.
	// Similarly for the y lower bound.
	xVelocityUpperBound = xRange.second;
	yVelocityLowerBound = yRange.first;
}

int Launcher::ProbeLauncher::HighestYCoordinate()
{
	// We can use arithmethic sequences again here. Using the upper bound of
	// y speed we found in the constructor, the highest y height reached is
	// the sum of an arithmetic sequence that starts with 1 and adds 1 each time
	// (considering a sort of backwards view where we're accelerating from the high
	// point back to the origin).
	// In this sequence, the number of steps is equal to the starting speed, since the
	// speed only goes down by 1 each step.
	int highestY = (yVelocityUpperBound + 1) * yVelocityUpperBound / 2;

	// Let's do some checking of the very same assumption that our y loop
	// contains enough steps that the x will have got there in time.
	assert(((yVelocityUpperBound * 2) + 2) >= stepsRequiredForXLowerBound);

	return highestY;
}

int Launcher::ProbeLauncher::NumberOfPossibleVelocities()
{
	// To avoid doing too much unnecessary work, for each possible x and y component,
	// list the number of steps it might take to reach the target zone (might here refers
	// to the fact that there might be multiple numbers of steps it could take
	// for velocities that spend multiple steps in the targt zone). Then we can only
	// consider x and y values where these numbers of steps overlap as being velocities
	// that land in the target zone.
	// For x, a final step number of -1 indicates that the previous step number leaves
	// the probe 'stopped' in the target zone, and so higher y steps values are considered
	// overlaps too.
	std::vector<std::set<int>> numbersOfXSteps;
	std::vector<std::set<int>> numbersOfYSteps;

	for (int xVelocity = xVelocityLowerBound; xVelocity <= xVelocityUpperBound; xVelocity++)
	{
		std::set<int> possibleStepCounts;
		int stepsTaken{ 0 };
		int xPosition{ 0 };
		for (int currentSpeed = xVelocity; ; currentSpeed--)
		{
			if (currentSpeed == 0)
			{
				if (possibleStepCounts.size() > 0)
				{
					possibleStepCounts.insert(-1);
				}
				break;
			}

			stepsTaken++;
			xPosition += currentSpeed;
			if (xPosition > targetRangeCoords.first.second) 
			{ 
				break; 
			}
			else if (xPosition >= targetRangeCoords.first.first)
			{
				possibleStepCounts.insert(stepsTaken);
			}
		}
		if (possibleStepCounts.size() > 0)
		{
			numbersOfXSteps.push_back(possibleStepCounts);
		}
	}

	for (int yVelocity = yVelocityLowerBound; yVelocity <= xVelocityUpperBound; yVelocity++)
	{
		std::set<int> possibleStepCounts;
		int stepsTaken{ 0 };
		int yPosition{ 0 };
		for (int currentSpeed = yVelocity; ; currentSpeed--)
		{
			stepsTaken++;
			yPosition += currentSpeed;
			if (yPosition < targetRangeCoords.second.first)
			{
				break;
			}
			else if (yPosition <= targetRangeCoords.second.second)
			{
				possibleStepCounts.insert(stepsTaken);
			}
		}
		if (possibleStepCounts.size() > 0)
		{
			numbersOfYSteps.push_back(possibleStepCounts);
		}
	}

	int totalPossibleVelocities{ 0 };
	for (std::set<int> stepNumbersForGivenY : numbersOfYSteps)
	{
		for (std::set<int> stepNumbersForGivenX : numbersOfXSteps)
		{
			for (int specificNumberOfYSteps : stepNumbersForGivenY)
			{
				if (stepNumbersForGivenX.find(specificNumberOfYSteps) != stepNumbersForGivenX.end())
				{
					totalPossibleVelocities++;
					break;
				}
				else if (stepNumbersForGivenX.find(-1) != stepNumbersForGivenX.end())
				{
					if (specificNumberOfYSteps > *(stepNumbersForGivenX.rbegin()))
					{
						totalPossibleVelocities++;
						break;
					}
				}
			}
		}
	}

	return totalPossibleVelocities;
}


