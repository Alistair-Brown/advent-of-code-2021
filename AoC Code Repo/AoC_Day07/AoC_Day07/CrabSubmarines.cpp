#include "CrabSubmarines.h"
#include <iostream>
#include <cassert>
#include <deque>

void CrabSubmarines::CrabArmada::MaybeResizeArmada(int newMaxPosition)
{
	if (newMaxPosition >= crabsByPosition.size())
	{
		// Need to resize than one bigger than the position, because 0 is a position too.
		crabsByPosition.resize(newMaxPosition + 1, 0);
	}
}

int CrabSubmarines::CrabArmada::ArmadaMedianPosition()
{
	int currentPosition{ 0 };
	int numberOfCrabs{ 0 };
	while (true)
	{
		numberOfCrabs += crabsByPosition[currentPosition];
		if ((float)numberOfCrabs >= (totalCrabSubmarines / 2.0))
		{
			break;
		}
		currentPosition++;
	}
	std::cout << "Median position is " << currentPosition << std::endl;
	return currentPosition;
}

int CrabSubmarines::CrabArmada::ComplexArmadaMedianPosition()
{
	std::vector<unsigned long long int> costOfNextMoveFromLeft;
	std::deque<unsigned long long int> costOfNextMoveFromRight;
	int position{ 0 };

	unsigned long long int numberOfCrabsCollected{ 0 };
	unsigned long long effectiveCrabs{ 0 };
	costOfNextMoveFromLeft.reserve(crabsByPosition.size());
	for (unsigned int ii = 0; ii < crabsByPosition.size(); ii++)
	{
		numberOfCrabsCollected += crabsByPosition[ii];
		effectiveCrabs += numberOfCrabsCollected;
		costOfNextMoveFromLeft.push_back(effectiveCrabs);
	}

	effectiveCrabs = 0;
	numberOfCrabsCollected = 0;
	for (int ii = crabsByPosition.size() - 1; ii >= 0; ii--)
	{
		numberOfCrabsCollected += crabsByPosition[ii];
		effectiveCrabs += numberOfCrabsCollected;
		costOfNextMoveFromRight.push_front(effectiveCrabs);
	}

	for (unsigned int ii = 0; ii < crabsByPosition.size() - 1; ii++)
	{
		if (costOfNextMoveFromLeft[ii] > costOfNextMoveFromRight[ii + 1])
		{
			position = ii;
			break;
		}
	}
	assert(position > 0);
	return position;
}

int CrabSubmarines::CrabArmada::ComplexCostToMoveArmadaToPosition(int positionToMoveTo)
{
	int movementRequiredToConverge{ 0 };
	for (int ii = 0; ii < crabsByPosition.size(); ii++)
	{
		int spacesToMove = abs(ii - positionToMoveTo);
		// Sum of an arithmetic sequence
		int movementCost = spacesToMove * (spacesToMove + 1) / 2;
		movementRequiredToConverge += movementCost * crabsByPosition[ii];
	}
	return movementRequiredToConverge;
}

void CrabSubmarines::CrabArmada::AddCrabSumbarine(int position)
{
	MaybeResizeArmada(position);
	crabsByPosition[position]++;
	totalCrabSubmarines++;
}

int CrabSubmarines::CrabArmada::MinimalConvergence()
{
	int medianPosition = ArmadaMedianPosition();
	int movementRequiredToConverge{ 0 };
	for (int ii = 0; ii < crabsByPosition.size(); ii++)
	{
		movementRequiredToConverge += crabsByPosition[ii] * abs(ii - medianPosition);
	}
	return movementRequiredToConverge;
}

int CrabSubmarines::CrabArmada::MoreComplicatedMinimumConvergence()
{
	//int minimumConvergence{ 0 };
	//int position{ 0 };
	//for (int ii = 0; ii < crabsByPosition.size(); ii++)
	//{
	//	int convergence = ComplexCostToMoveArmadaToPosition(ii);
	//	if ((minimumConvergence == 0) || (convergence < minimumConvergence))
	//	{
	//		position = ii;
	//		minimumConvergence = convergence;
	//	}
	//}
	//std::cout << "Optimum complex position is " << position << std::endl;
	//std::cout << "My wrong position is " << ComplexArmadaMedianPosition() << std::endl;
	//return minimumConvergence;
	return ComplexCostToMoveArmadaToPosition(ComplexArmadaMedianPosition());
}
