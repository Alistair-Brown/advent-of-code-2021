#pragma once
#include <vector>

namespace CrabSubmarines
{
	class CrabArmada
	{
	private:
		std::vector<int> crabsByPosition;
		int totalCrabSubmarines{ 0 };
		void MaybeResizeArmada(int newMaxPosition);
		int ArmadaMedianPosition();
		int ComplexArmadaMedianPosition();
		int ComplexCostToMoveArmadaToPosition(int positionToMoveTo);
	public:
		void AddCrabSumbarine(int position);
		int MinimalConvergence();
		int MoreComplicatedMinimumConvergence();
	};
}