#include "Reactor.h"
#include <cassert>

// Size here is 
Reactor::InitializationZone::InitializationZone(
	bool initialSetting,
	std::pair<int, int> xRange,
	std::pair<int, int> yRange,
	std::pair<int, int> zRange)
{
	// Midpoint tracking will be screwed if we don't have a range going into the negative.
	assert((xRange.first < 0) && (yRange.first < 0) && (zRange.first < 0));

	// Set up vector of bools for the 1D z-direction, duplicate that into a 2D plane in the y-direction,
	// then duplicate that in the z direction to create the whole cube.
	int zMidpointTracker{ 0 };
	std::vector<bool> zAxisOnly;
	for (int zz = zRange.first; zz <= zRange.second; zz++)
	{
		zAxisOnly.push_back(initialSetting);
		if (zz == 0) { zMidpoint = zMidpointTracker; }
		zMidpointTracker++;
	}
	int yMidpointTracker{ 0 };
	std::vector<std::vector<bool>> yzPlane;
	for (int yy = yRange.first; yy <= yRange.second; yy++)
	{
		yzPlane.push_back(zAxisOnly);
		if (yy == 0) { yMidpoint = yMidpointTracker; }
		yMidpointTracker++;
	}
	int xMidpointTracker{ 0 };
	for (int xx = xRange.first; xx <= xRange.second; xx++)
	{
		onOrOffCubes.push_back(yzPlane);
		if (xx == 0) { xMidpoint = xMidpointTracker; }
		xMidpointTracker++;
	}
}

void Reactor::InitializationZone::SetCubesInRange(
	bool newSetting,
	std::pair<int, int> xRange,
	std::pair<int, int> yRange,
	std::pair<int, int> zRange)
{
	// When accessing 0-indexed vectors, use our knowledge of what the true
	// 0th position of the vector is to offset our indices.
	// Careful to only access cells within the range of the initialisation zone.
	for (int xx = xRange.first + xMidpoint; xx <= xRange.second + xMidpoint; xx++)
	{
		if (xx < 0) { continue; }
		else if (xx >= (int)onOrOffCubes.size()) { break; }
		for (int yy = yRange.first + yMidpoint; yy <= yRange.second + yMidpoint; yy++)
		{
			if (yy < 0) { continue; }
			else if (yy >= (int)onOrOffCubes[0].size()) { break; }
			for (int zz = zRange.first + zMidpoint; zz <= zRange.second + zMidpoint; zz++)
			{
				if (zz < 0) { continue; }
				else if (zz >= (int)onOrOffCubes[0][0].size()) { break; }

				onOrOffCubes[xx][yy][zz] = newSetting;
			}
		}
	}
}

int Reactor::InitializationZone::GetNumberOfOnCubes()
{
	int numberOfOnCubes{ 0 };
	for (std::vector<std::vector<bool>> plane : onOrOffCubes)
	{
		for (std::vector<bool> axis : plane)
		{
			for (bool cube : axis)
			{
				if (cube) { numberOfOnCubes++; }
			}
		}
	}
	return numberOfOnCubes;
}

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

Reactor::CompleteReactor::CompleteReactor(std::vector<std::pair<bool, InstructionVolume>> instructionsInOriginalOrder)
{
	std::vector<InstructionVolume> subsequentInstructions;
	numberOfOnCubes = 0;

	for (std::vector<std::pair<bool, InstructionVolume>>::iterator instructionVolumeItr = instructionsInOriginalOrder.end(); ; )
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

		if (instructionVolumeItr == instructionsInOriginalOrder.begin()) { break; }
	}
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
