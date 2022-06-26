#pragma once
#include <vector>
#include <utility>

namespace Reactor
{
	struct InstructionVolume
	{
		std::pair<long long int, long long int> xRange;
		std::pair<long long int, long long int> yRange;
		std::pair<long long int, long long int> zRange;
		InstructionVolume(
			std::pair<long long int, long long int> xRange,
			std::pair<long long int, long long int> yRange,
			std::pair<long long int, long long int> zRange) :
			xRange{ xRange }, yRange{ yRange }, zRange{ zRange } {};
		bool ConvertToOverlappingVolume(InstructionVolume &volumeToConvert);
	};

	class CompleteReactor
	{
	private:
		unsigned long long int numberOfOnCubes;
		unsigned long long int NumberOfNonOverlappingCubes(
			InstructionVolume mainVolume,
			std::vector<InstructionVolume> potentialOverlappingVolumes);

		void ApplyInstructions(std::vector<std::pair<bool, InstructionVolume>> instructionsInOrder);
	public:
		CompleteReactor(std::vector<std::pair<bool, InstructionVolume>> instructionsInOriginalOrder);
		CompleteReactor(
			std::vector<std::pair<bool, InstructionVolume>> instructionsInOriginalOrder,
			std::pair<int, int> xLimits,
			std::pair<int, int> yLimits,
			std::pair<int, int> zLimits);
		unsigned long long int NumberOfOnCubes() { return numberOfOnCubes; }
	};
}