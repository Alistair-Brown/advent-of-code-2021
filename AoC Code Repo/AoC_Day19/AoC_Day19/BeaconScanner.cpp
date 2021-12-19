#include "BeaconScanner.h"
#include <cassert>

// There are 24 possible rotations of a position. Depending on the index passed
// in, this function returns one of them, where the 0th index represents no rotation.
// Using an index rather than any way of actually representing the rotation because
// all the caller should care about is getting each possible rotation consistently,
// don't want them to have to start worrying about how to define each rotation.
Beacon::Position Beacon::Position::GetRotatedPosition(int rotationIndex)
{
	assert(rotationIndex < PossibleRotations);

	Position positionToReturn{};

	// To visualise the possible rotations, remember that there are 6 different directions
	// the scanner could think was the x axis. And for each of these directions, there
	// are 4 possible rotations for the y and z axes. Then imagine what the position of
	// the coordinate (1,1,1) would be in each case, and that should help you to define the
	// position rotation required.
	// I'm visualing a case where x goes to the right, y goes up, and z goes into the page.
	switch (rotationIndex)
	{
	case 0: // x pointing in x +ve. No y-z rotation
		return Position(position);
		break;
	case 1: // x pointing in x +ve. y-z rotated 90 degrees
		return Position({ position.xPos, -position.zPos, position.yPos });
		break;
	case 2: // x pointing in x +ve. y-z rotated 180 degrees
		return Position({ position.xPos, -position.yPos, -position.zPos });
		break;
	case 3: // x pointing in x +ve. y-z rotated 270 degrees
		return Position({ position.xPos, position.zPos, -position.yPos });
		break;
	case 4: // x pointing in x -ve after rotating about z. No y-z rotation relative to that
		return Position({ -position.xPos, -position.yPos, position.zPos });
		break;
	case 5: // x pointing in x -ve after rotating about z. y-z rotated 90 degrees
		return Position({ -position.xPos, position.zPos, position.yPos });
		break;
	case 6: // x pointing in x -ve after rotating about z. y-z rotated 180 degrees
		return Position({ -position.xPos, position.yPos, -position.zPos });
		break;
	case 7: // x pointing in x -ve after rotating about z. y-z rotated 270 degrees
		return Position({ -position.xPos, -position.zPos, -position.yPos });
		break;
	case 8: // x pointing in y +ve after rotating about z. No y-z rotation relative to that
		return Position({ position.yPos, -position.xPos, position.zPos });
		break;
	case 9: // x pointing in y +ve after rotating about z.
		return Position({ position.yPos, -position.zPos, -position.xPos });
		break;
	case 10: // x pointing in y +ve after rotating about z.
		return Position({ position.yPos, position.xPos, -position.zPos });
		break;
	case 11: // x pointing in y +ve after rotating about z. 
		return Position({ position.yPos, position.zPos, position.xPos });
		break;
	case 12: // x pointing in y -ve after rotating about z. No y-z rotation relative to that
		return Position({ -position.yPos, position.xPos, position.zPos });
		break;
	case 13: // x pointing in y -ve after rotating about z.
		return Position({ -position.yPos, -position.zPos, position.xPos });
		break;
	case 14: // x pointing in y -ve after rotating about z.
		return Position({ -position.yPos, -position.xPos, -position.zPos });
		break;
	case 15: // x pointing in y -ve after rotating about z.
		return Position({ -position.yPos, position.zPos, -position.xPos });
		break;
	case 16: // x pointing in z +ve after rotating about y. No y-z rotation relative to that
		return Position({ position.zPos, position.yPos, -position.xPos });
		break;
	case 17: // x pointing in z +ve after rotating about y. No y-z rotation relative to that
		return Position({ position.zPos, position.xPos, position.yPos });
		break;
	case 18: // x pointing in z +ve after rotating about y. No y-z rotation relative to that
		return Position({ position.zPos, -position.yPos, position.xPos });
		break;
	case 19: // x pointing in z +ve after rotating about y. No y-z rotation relative to that
		return Position({ position.zPos, -position.xPos, -position.yPos });
		break;
	case 20: // x pointing in z -ve after rotating about y. No y-z rotation relative to that
		return Position({ -position.zPos, position.yPos, position.xPos });
		break;
	case 21: // x pointing in z -ve after rotating about y. No y-z rotation relative to that
		return Position({ -position.zPos, -position.xPos, position.yPos });
		break;
	case 22: // x pointing in z -ve after rotating about y. No y-z rotation relative to that
		return Position({ -position.zPos, -position.yPos, -position.xPos });
		break;
	case 23: // x pointing in z -ve after rotating about y. No y-z rotation relative to that
		return Position({ -position.zPos, position.xPos, -position.yPos });
		break;
	default:
		assert(false);
		break;
	}
}

Beacon::Scanner::Scanner(std::vector<std::vector<int>> inputBeacons)
{
	for (std::vector<int> beaconPosition : inputBeacons)
	{
		beaconsInRange.push_back(Position( Vector3(beaconPosition[0], beaconPosition[1], beaconPosition[2])));
	}

	for (int ii = 0; ii < beaconsInRange.size(); ii++)
	{
		std::set<Position> relativePositions{};
		for (int jj = 0; jj < beaconsInRange.size(); jj++)
		{
			if (ii == jj) { continue; }
			relativePositions.insert(beaconsInRange[jj] - beaconsInRange[ii]);
		}
		beaconOffsets.push_back(relativePositions);
	}
}

// If the beacons between each scanner overlap, return an int representing the
// rotation required to apply to the other scanner to bring it rotationally in line
// with this one.
// Returns -1 if there is not an overlap.
int Beacon::Scanner::CheckForOverlap(Scanner otherScanner, Beacon::Position &relativePosition)
{
	std::vector<std::set<Position>> thisBeaconOffsets = beaconOffsets;
	std::vector<std::set<Position>> otherBeaconOffsets = otherScanner.beaconOffsets;
	relativePosition = Vector3({ 0,0,0 });
	Position localMatchedBeaconPos;
	Position remoteMatchedBeaconPos;

	int requiredRotation = -1;

	for (int rotationIndex = 0; rotationIndex < PossibleRotations; rotationIndex++)
	{
		// Create a new vector of sets which is rotated for the current offset
		std::vector<std::set<Position>> rotatedBeaconOffsets;
		for (std::set<Position> positionSet : otherBeaconOffsets)
		{
			std::set<Position> rotatedPositions;
			for (Position position : positionSet)
			{
				rotatedPositions.insert(position.GetRotatedPosition(rotationIndex));
			}
			rotatedBeaconOffsets.push_back(rotatedPositions);
		}

		// Get rotated beacon positions to help me find the scanner offsets.
		std::vector<Position> rotatedBeaconsInRange;
		for (Position beacon : otherScanner.beaconsInRange)
		{
			rotatedBeaconsInRange.push_back(beacon.GetRotatedPosition(rotationIndex));
		}

		int remoteBeaconIndex = 0;
		for (std::set<Position> singleBeaconOffsets : rotatedBeaconOffsets)
		{
			int localBeaconIndex = 0;
			for (std::set<Position> offsetsToCompare : rotationalBeaconOffsets)
			{
				int numberOfHits{ 0 };
				for (Position position : singleBeaconOffsets)
				{
					if (offsetsToCompare.find(position) != offsetsToCompare.end())
					{
						numberOfHits++;
						if (numberOfHits == (BeaconsRequiredForOverlap - 1))
						{
							localMatchedBeaconPos = rotationallyRelativeBeaconsInRange[localBeaconIndex];
							remoteMatchedBeaconPos = rotatedBeaconsInRange[remoteBeaconIndex];
						}
					}
				}
				// For 12 beacon overlap, each beacon should have 11 beacon offsets in common
				// with the other scanner's view.
				if (numberOfHits >= (BeaconsRequiredForOverlap - 1))
				{
					requiredRotation = rotationIndex;
					relativePosition = localMatchedBeaconPos - remoteMatchedBeaconPos;
					goto BREAK_LOOP;
				}
				localBeaconIndex++;
			}
			remoteBeaconIndex++;
		}
	}

BREAK_LOOP:
	return requiredRotation;
}

void Beacon::Scanner::RotationallyRelativizeBeaconsInRange(int rotationIndex)
{
	for (Position beaconInRange : beaconsInRange)
	{
		rotationallyRelativeBeaconsInRange.push_back(beaconInRange.GetRotatedPosition(rotationIndex));
	}
	for (std::set<Position> singleBeaconOffset : beaconOffsets)
	{
		std::set<Position> rotatedOffsets;
		for (Position offset : singleBeaconOffset)
		{
			rotatedOffsets.insert(offset.GetRotatedPosition(rotationIndex));
		}
		rotationalBeaconOffsets.push_back(rotatedOffsets);
	}
}

void Beacon::Scanner::FullyRelativizeBeaconsInRange()
{
	for (Position rotatedBeacon : rotationallyRelativeBeaconsInRange)
	{
		fullyRelativeBeaconsInRange.push_back(rotatedBeacon + relativeScannerPosition);
	}
}

void Beacon::Scanner::AddBeaconsToTotal(std::vector<Position> newBeacons)
{
	for (Position position : newBeacons)
	{
		if (rotationallyNormalizedAllBeacons.find(position) == rotationallyNormalizedAllBeacons.end())
		{
			rotationallyNormalizedAllBeacons.insert(position);
		}
	}
}
