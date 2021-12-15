#include "Chiton.h"
#include <cassert>
#include <deque>

Chiton::ChitonCave::ChitonCave(std::vector<std::vector<int>> inputRiskGrid)
{
	riskGrid = inputRiskGrid;
	endPos = { inputRiskGrid[0].size() - 1, inputRiskGrid.size() - 1 };

	for (unsigned int yy = 0; yy < inputRiskGrid.size(); yy++)
	{
		minimumRiskToReachPosition.push_back(std::vector<RiskNode *>{});
		for (unsigned int xx = 0; xx < inputRiskGrid[0].size(); xx++)
		{
			minimumRiskToReachPosition[yy].push_back(new RiskNode({ yy, xx }, inputRiskGrid[yy][xx]));
		}
	}

	for (unsigned int yy = 0; yy < inputRiskGrid.size(); yy++)
	{
		for (unsigned int xx = 0; xx < inputRiskGrid[0].size(); xx++)
		{
			if (yy < inputRiskGrid.size() - 1)
			{
				minimumRiskToReachPosition[yy][xx]->AssignNextNode(minimumRiskToReachPosition[yy + 1][xx]);
			}
			if (xx < inputRiskGrid[0].size() - 1)
			{
				minimumRiskToReachPosition[yy][xx]->AssignNextNode(minimumRiskToReachPosition[yy][xx + 1]);
			}
			if (yy > 0)
			{
				minimumRiskToReachPosition[yy][xx]->AssignNextNode(minimumRiskToReachPosition[yy - 1][xx]);
			}
			if (xx > 0)
			{
				minimumRiskToReachPosition[yy][xx]->AssignNextNode(minimumRiskToReachPosition[yy][xx - 1]);
			}
		}
	}
	minimumRiskToReachPosition[0][0]->MakeStartingCell();
}

int Chiton::ChitonCave::MinimumRiskToEnd()
{
	FindMinimumRiskRoutes();
	// Don't get fooled by the fact that endPos is given as x,y coords, but
	// the 2D vector has y as the first element when indexing.
	return minimumRiskToReachPosition[endPos.second][endPos.first]->MinimumRiskToReach();
}

// General approach here is:
// - Start at the start position, with a total risk on this route so far of 0
// - Then look at each position around us, and if the total risk of the route after
//   moving there would be less than the current minimum we have stored to reach that
//   position, put a new potential route on the bottom of our queue of potential
//   routes that represents us moving there. Make the minimum to reach to that position
//   the total risk of that route.
// - Rinse and repeat until we're out of queue.
void Chiton::ChitonCave::FindMinimumRiskRoutes()
{
	// Our stack of potential routes contains a position (in form 'x,y', which is worth
	// remembering so you don't get tripped up indexing grid positions), and a total
	// risk we've encountered so far on that route.
	std::deque<RiskNode *> potentialRoutes{ minimumRiskToReachPosition[0][0] };

	while (potentialRoutes.size() > 0)
	{
		RiskNode *currentNode = potentialRoutes.back();
		potentialRoutes.pop_back();

		currentNode->hasBeenRoutedFrom = true;

		if (currentNode->UpNodePos().first < riskGrid.size())
		{
			RiskNode *upNode = minimumRiskToReachPosition[currentNode->UpNodePos().first][currentNode->UpNodePos().second];
			if (upNode->IsThisTheNewMinimum(currentNode->MinimumRiskToReach(), currentNode) &&
				!upNode->hasBeenRoutedFrom)
			{
				potentialRoutes.push_front(upNode);
			}
		}
		if (currentNode->RightNodePos().second < riskGrid[0].size())
		{
			RiskNode *rightNode = minimumRiskToReachPosition[currentNode->RightNodePos().first][currentNode->RightNodePos().second];
			if (rightNode->IsThisTheNewMinimum(currentNode->MinimumRiskToReach(), currentNode) &&
				!rightNode->hasBeenRoutedFrom)
			{
				potentialRoutes.push_front(rightNode);
			}
		}
		if (currentNode->DownNodePos().first >= 0)
		{
			RiskNode *downNode = minimumRiskToReachPosition[currentNode->DownNodePos().first][currentNode->DownNodePos().second];
			if (downNode->IsThisTheNewMinimum(currentNode->MinimumRiskToReach(), currentNode) &&
				!downNode->hasBeenRoutedFrom)
			{
				potentialRoutes.push_front(downNode);
			}
		}
		if (currentNode->LeftNodePos().second >= 0)
		{
			RiskNode *leftNode = minimumRiskToReachPosition[currentNode->LeftNodePos().first][currentNode->LeftNodePos().second];
			if (leftNode->IsThisTheNewMinimum(currentNode->MinimumRiskToReach(), currentNode) &&
				!leftNode->hasBeenRoutedFrom)
			{
				potentialRoutes.push_front(leftNode);
			}
		}
	}
}

void Chiton::RiskNode::AssignNextNode(RiskNode * node)
{
	nextNodes.push_back(node);
}

// A node has reported that the minimum route risk to it no longer runs
// through us, remove it from our list of nodes we have minimum risk routes
// leading to.
void Chiton::RiskNode::NoLongerNextNode(RiskNode * node)
{
	for (std::vector<RiskNode *>::iterator itr = nextNodes.begin();
		 itr != nextNodes.end();
		 itr++)
	{
		if (*itr == node)
		{
			nextNodes.erase(itr);
			break;
		}
	}
}

bool Chiton::RiskNode::IsThisTheNewMinimum(int inboundRisk, RiskNode * inboundNode)
{
	int newRisk = inboundRisk + localRisk;
	bool isNewMinimum{ false };

	if (this->gridLocation == std::pair<int,int>(2, 3))
	{
		int ii = 5;
		assert(ii == 5);
	}

	if (inboundRisk + localRisk < minimumRiskToReach)
	{
		minimumRiskToReach = newRisk;

		if (hasBeenRoutedFrom)
		{
			for (RiskNode *nextNode : nextNodes)
			{
				// Don't care about the return value, just need to make sure all 'next nodes'
				// update themselves according to this lower inbound risk.
				nextNode->IsThisTheNewMinimum(newRisk, this);
			}
		}
		isNewMinimum = true;
	}

	return isNewMinimum;
}
