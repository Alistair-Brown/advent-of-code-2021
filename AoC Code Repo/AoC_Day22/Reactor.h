#pragma once
#include <vector>
#include <utility>
#include "aoc_common_types.h"

namespace Reactor
{
	// Each instruction for turning a region of the reactor on or off has a cuboid
	// of space which it acts on. Such a cuboid can be described as a range of values
	// in each of the 3 axes.
	struct InstructionVolume
	{
		std::pair<long long int, long long int> xRange;
		std::pair<long long int, long long int> yRange;
		std::pair<long long int, long long int> zRange;
		InstructionVolume(
			std::pair<long long int, long long int> xRange,
			std::pair<long long int, long long int> yRange,
			std::pair<long long int, long long int> zRange
		) :
			xRange{ xRange }, yRange{ yRange }, zRange{ zRange } {};

		bool ConvertToOverlappingVolume(InstructionVolume &otherVolume);
		ULLINT NumberOfNonOverlappingCubes(std::vector<InstructionVolume> potentialOverlappingVolumes);
	};

	// A Reactor is initialised with a set of instructions, and possibly some limits affecting which
	// of those instructions should actually take effect. The construction of the Reactor from
	// these instructions will determine the numberOfOnCubes, which can be publically queried.
	class Reactor
	{
	private:
		ULLINT numberOfOnCubes;
		void ApplyInstructions(std::vector<std::pair<bool, InstructionVolume>> instructionsInOrder);
	public:
		Reactor(std::vector<std::pair<bool, InstructionVolume>> instructionsInOriginalOrder);
		Reactor(
			std::vector<std::pair<bool, InstructionVolume>> instructionsInOriginalOrder,
			std::pair<int, int> xLimits,
			std::pair<int, int> yLimits,
			std::pair<int, int> zLimits);
		ULLINT NumberOfOnCubes() { return numberOfOnCubes; }
	};
}