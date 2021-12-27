#pragma once
#include <vector>
#include <utility>

namespace Reactor
{
	// A portion of the the reactor whose particular size is known and cared
	// about, that ignores any instructions relating to cubes outside of this size.
	class InitializationZone
	{
	private:
		std::vector<std::vector<std::vector<bool>>> onOrOffCubes;
		// Vectors can't have negative indices, so keep track of the midpoints, which
		// tell us the vector index of each axis that actually corresponds to 0 on that
		// axis, allowing us to get effective negative indices in the vector.
		int xMidpoint, yMidpoint, zMidpoint;
	public:
		InitializationZone() : InitializationZone(false, { -1, 1 }, { -1, 1 }, { -1, 1 }) {};
		InitializationZone(
			bool initialSetting,
			std::pair<int, int> xRange,
			std::pair<int, int> yRange,
			std::pair<int, int> zRange);

		void SetCubesInRange(
			bool newSetting,
			std::pair<int, int> xRange,
			std::pair<int, int> yRange,
			std::pair<int, int> zRange);
		int GetNumberOfOnCubes();
	};

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
	public:
		CompleteReactor(std::vector<std::pair<bool, InstructionVolume>> instructionsInOriginalOrder);
		unsigned long long int NumberOfOnCubes() { return numberOfOnCubes; }
	};
}