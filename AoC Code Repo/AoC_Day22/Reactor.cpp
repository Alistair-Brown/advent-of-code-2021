#include "Reactor.h"
#include <cassert>
#include <algorithm>
#include <iterator>

// Converts another InstructionVolume into just the portion of it that overlaps with this volume.
// Returns true if there is some overlap, false otherwise.
bool Reactor::InstructionVolume::ConvertToOverlappingVolume(InstructionVolume& otherVolume)
{
	bool overlap{ false };
	bool xOverlap =
		!((otherVolume.xRange.second < xRange.first) || (otherVolume.xRange.first > xRange.second));
	bool yOverlap =
		!((otherVolume.yRange.second < yRange.first) || (otherVolume.yRange.first > yRange.second));
	bool zOverlap =
		!((otherVolume.zRange.second < zRange.first) || (otherVolume.zRange.first > zRange.second));

	if (xOverlap && yOverlap && zOverlap)
	{
		overlap = true;

		if (otherVolume.xRange.first < xRange.first) { otherVolume.xRange.first = xRange.first; }
		if (otherVolume.xRange.second > xRange.second) { otherVolume.xRange.second = xRange.second; }
		if (otherVolume.yRange.first < yRange.first) { otherVolume.yRange.first = yRange.first; }
		if (otherVolume.yRange.second > yRange.second) { otherVolume.yRange.second = yRange.second; }
		if (otherVolume.zRange.first < zRange.first) { otherVolume.zRange.first = zRange.first; }
		if (otherVolume.zRange.second > zRange.second) { otherVolume.zRange.second = zRange.second; }
	}

	return overlap;
}

// Find the number of unit cubes in this InstructionVolume which do not overlap with any of the other
// potentialOverlappingVolumes.
ULLINT Reactor::InstructionVolume::NumberOfNonOverlappingCubes(std::vector<InstructionVolume> potentialOverlappingVolumes)
{
	ULLINT numberOfOverlappingCubes = 0;
	ULLINT totalVolumeOfThisInstruction =
		(xRange.second - xRange.first + 1) *
		(yRange.second - yRange.first + 1) *
		(zRange.second - zRange.first + 1);

	// First convert each of the potentialOverlappingVolumes to be just the region of themselves
	// that overlaps with this volume, if there is any overlap at all.
	std::vector<InstructionVolume> actualOverlappingVolumes;
	for (InstructionVolume potentialOverlappingVolume : potentialOverlappingVolumes)
	{
		if (ConvertToOverlappingVolume(potentialOverlappingVolume))
		{
			actualOverlappingVolumes.push_back(potentialOverlappingVolume);
		}
	}

	// For each of the volumes which overlap this one, find the number of cubes which
	// don't overlap with any of the *other* overlapping volumes we haven't yet considered.
	// Keeping track of this as a running total will give us the total number of cubes
	// which overlap with our original volume, while avoiding double counting cubes which
	// appear in multiple of the overlap volumes.
	while (actualOverlappingVolumes.size() > 0)
	{
		InstructionVolume overlappingVolume = actualOverlappingVolumes.back();
		actualOverlappingVolumes.pop_back();

		numberOfOverlappingCubes += overlappingVolume.NumberOfNonOverlappingCubes(actualOverlappingVolumes);
	}

	return totalVolumeOfThisInstruction - numberOfOverlappingCubes;
}

// This Reactor constructor will apply all of the inputted instructions in order, leaving a
// certain number of cubes (integer coordinates in 3D space) on as a result.
Reactor::Reactor::Reactor(std::vector<std::pair<bool, InstructionVolume>> instructionsInOriginalOrder)
{
	numberOfOnCubes = 0;
	ApplyInstructions(instructionsInOriginalOrder);
}

// This Reactor constructor will only apply those instructions that act exclusively within the
// specified limits. e.g. with an x limit of (-50, 50), an instruction with an xRange of (-50, 6)
// would be applied, but one with an xRange of (-34, 51) would not be.
Reactor::Reactor::Reactor(
	std::vector<std::pair<bool, InstructionVolume>> instructionsInOriginalOrder,
	std::pair<int, int> xLimits,
	std::pair<int, int> yLimits,
	std::pair<int, int> zLimits
)
{
	numberOfOnCubes = 0;
	std::vector<std::pair<bool, InstructionVolume>> filteredInstructions;

	auto instructionInRange = [xLimits, yLimits, zLimits]
		(std::pair<bool, InstructionVolume> instructionPair) noexcept -> bool
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

// This is the clever bit. Actually applying all of the instructions, in order, to toggle some 3D array
// of bools on and off would take a prohibitively long time (the puzzle input makes sure of that). So
// instead we realise that for any given coordinate, it is only the final instruction concerning that
// coordinate that ultimately determines its setting. Therefore we can instead look at the instructions
// in *reverse* order, and, when working out how many extra cubes will be turned on as a result of a
// given 'on' instruction, only consider the region of each instruction that does not overlap with any
// 'subsequent' instruction from the original order. 'off' instructions have no impact on the
// number of eventual 'on' cubes other than contributing to the region that will be ignored in any
// 'previous' (in the original order) 'on' instructions.
// This way we don't have to consider an actual 3D array at all, we just increment the total number
// of 'on' cubes based on the portion of each 'on' instruction that doesn't overlap with subsequent
// instructions.
void Reactor::Reactor::ApplyInstructions(std::vector<std::pair<bool, InstructionVolume>> instructionsInOrder)
{
	std::vector<InstructionVolume> subsequentInstructions;
	for (std::vector<std::pair<bool, InstructionVolume>>::iterator instructionVolumeItr = instructionsInOrder.end() - 1; ;
		instructionVolumeItr--)
	{
		// If this instruction turns cubes on, increment the number of 'on' cubes by the volume of this
		// instrcution that doesn't overlap with 'subsequent' instructions (since those subsequent instructions
		// will override this one).
		if (instructionVolumeItr->first)
		{
			numberOfOnCubes	+= instructionVolumeItr->second.NumberOfNonOverlappingCubes(subsequentInstructions);
		}

		// We're working backwards through the instructions, so with each new instruction we reach, the
		// ones we've already dealt with are technically the subsequent instructions.
		subsequentInstructions.push_back(instructionVolumeItr->second);

		if (instructionVolumeItr == instructionsInOrder.begin()) { break; }
	}
}
