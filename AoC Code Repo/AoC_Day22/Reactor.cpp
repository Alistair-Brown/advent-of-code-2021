#include "Reactor.h"
#include <cassert>
#include <algorithm>
#include <iterator>

unsigned long long int Reactor::CompleteReactor::NumberOfNonOverlappingCubes(InstructionVolume mainVolume, std::vector<InstructionVolume> potentialOverlappingVolumes)
{
	unsigned long long int numberOfNonOverlappingCubes =
		(mainVolume.xRange.second - mainVolume.xRange.first + 1) *
		(mainVolume.yRange.second - mainVolume.yRange.first + 1) *
		(mainVolume.zRange.second - mainVolume.zRange.first + 1);

	std::vector<InstructionVolume> actualOverlappingVolumes;
	for (InstructionVolume potentialOverlappingVolume : potentialOverlappingVolumes)
	{
		if (mainVolume.ConvertToOverlappingVolume(potentialOverlappingVolume))
		{
			actualOverlappingVolumes.push_back(potentialOverlappingVolume);
		}
	}

	// Subtract all cubes which overlap with this volume from the total number of
	// non overlapp cubes. For each overlapping volume, only subtract the cubes which
	// don't overlap with other volumes we haven't yet checked, to avoid double counting.
	while (actualOverlappingVolumes.size() > 0)
	{
		InstructionVolume overlappingVolume = actualOverlappingVolumes.back();
		actualOverlappingVolumes.pop_back();

		numberOfNonOverlappingCubes -= NumberOfNonOverlappingCubes(overlappingVolume, actualOverlappingVolumes);
	}

	return numberOfNonOverlappingCubes;
}

void Reactor::CompleteReactor::ApplyInstructions(std::vector<std::pair<bool, InstructionVolume>> instructionsInOrder)
{
	std::vector<InstructionVolume> subsequentInstructions;
	for (std::vector<std::pair<bool, InstructionVolume>>::iterator instructionVolumeItr = instructionsInOrder.end(); ; )
	{
		instructionVolumeItr--;

		// If this instruction turns cubes on, increment the number of 'on' cubes by the volume of this
		// instrcution that doesn't overlap with subsequent instructions (since those subsequent instructions
		// will override this one).
		if (instructionVolumeItr->first)
		{
			numberOfOnCubes += NumberOfNonOverlappingCubes(instructionVolumeItr->second, subsequentInstructions);
		}

		// We're working backwards through the instructions, so with each new instruction we reach, the
		// ones we've already dealt with are technically the subsequent instructions.
		subsequentInstructions.push_back(instructionVolumeItr->second);

		if (instructionVolumeItr == instructionsInOrder.begin()) { break; }
	}
}

Reactor::CompleteReactor::CompleteReactor(std::vector<std::pair<bool, InstructionVolume>> instructionsInOriginalOrder)
{
	numberOfOnCubes = 0;
	ApplyInstructions(instructionsInOriginalOrder);
}

Reactor::CompleteReactor::CompleteReactor(
	std::vector<std::pair<bool, InstructionVolume>> instructionsInOriginalOrder,
	std::pair<int, int> xLimits,
	std::pair<int, int> yLimits,
	std::pair<int, int> zLimits
)
{
	numberOfOnCubes = 0;
	std::vector<std::pair<bool, InstructionVolume>> filteredInstructions;
	
	auto instructionInRange = [xLimits, yLimits, zLimits](std::pair<bool, InstructionVolume> instructionPair) noexcept -> bool
	{
		return (
			instructionPair.second.xRange.first >= xLimits.first &&
			instructionPair.second.xRange.second <= xLimits.second &&
			instructionPair.second.yRange.first >= yLimits.first &&
			instructionPair.second.yRange.second <= yLimits.second &&
			instructionPair.second.zRange.first >= zLimits.first &&
			instructionPair.second.zRange.second <= zLimits.second
			);
	};

	std::copy_if(
		instructionsInOriginalOrder.begin(),
		instructionsInOriginalOrder.end(),
		std::back_inserter(filteredInstructions),
		instructionInRange
	);

	ApplyInstructions(filteredInstructions);
}

// Converts a volume into just the portion of it that overlaps with 'this' volume.
// Returns true if there is some overlap, false otherwise.
bool Reactor::InstructionVolume::ConvertToOverlappingVolume(InstructionVolume & volumeToConvert)
{
	bool overlap{ false };
	bool xOverlap =
		!((volumeToConvert.xRange.second < xRange.first) || (volumeToConvert.xRange.first > xRange.second));
	bool yOverlap =
		!((volumeToConvert.yRange.second < yRange.first) || (volumeToConvert.yRange.first > yRange.second));
	bool zOverlap =
		!((volumeToConvert.zRange.second < zRange.first) || (volumeToConvert.zRange.first > zRange.second));

	if (xOverlap && yOverlap && zOverlap)
	{
		overlap = true;

		if (volumeToConvert.xRange.first < xRange.first) { volumeToConvert.xRange.first = xRange.first; }
		if (volumeToConvert.xRange.second > xRange.second) { volumeToConvert.xRange.second = xRange.second; }
		if (volumeToConvert.yRange.first < yRange.first) { volumeToConvert.yRange.first = yRange.first; }
		if (volumeToConvert.yRange.second > yRange.second) { volumeToConvert.yRange.second = yRange.second; }
		if (volumeToConvert.zRange.first < zRange.first) { volumeToConvert.zRange.first = zRange.first; }
		if (volumeToConvert.zRange.second > zRange.second) { volumeToConvert.zRange.second = zRange.second; }
	}

	return overlap;
}
