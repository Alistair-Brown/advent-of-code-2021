#include "Beacon.h"
#include <cmath>
#include <iostream>

// Scanner's are initialised with a list of beacon positions relative to their own position
// and orientation, with which they:
//  - Store those relative positions as a vector
//  - Create, for each beacon, and each possible rotation that could be applied to the scanner,
//    a set of the offsets from that beacon to every other beacon
//  - Create a set of all of the distinct offsets between beacons (i.e. this list would include
//    the offset from beacon 1 to 2, but not from 2 to 1.
Beacons::Scanner::Scanner(unsigned int scannerIndexIn, std::vector<std::vector<int>> const &beaconsIn) :
	scannerIndex {scannerIndexIn}
{
	for (std::vector<int> const& beacon : beaconsIn)
	{
		relativeBeaconPositions.emplace_back(beacon);
	}

	// To save worrying about when to push new sets into the vectors during the loops below,
	// just initialise the allBeaconOffsets vector with the correct size now.
	std::vector<std::set<Vector3>> emptyRotationVector(NUM_POSSIBLE_ROTATIONS);
	allBeaconOffsets = std::vector<std::vector<std::set<Vector3>>>(relativeBeaconPositions.size(), emptyRotationVector);

	// Loop through every beacon and every possible rotation we could apply to the scanner, finding
	// the offsets to every other beacon in each case. While doing this, also create a separate set
	// containing just one instance of each unique offset between 2 beacons, without applying any
	// rotation to the scanner's space.
	for (unsigned int beaconOne = 0; beaconOne < relativeBeaconPositions.size() - 1; beaconOne++)
	{
		for (unsigned int rotationIndex = 0; rotationIndex < NUM_POSSIBLE_ROTATIONS; rotationIndex++)
		{
			for (unsigned int beaconTwo = 0; beaconTwo < relativeBeaconPositions.size(); beaconTwo++)
			{
				if (beaconOne == beaconTwo) { continue; }

				allBeaconOffsets[beaconOne][rotationIndex].insert(
					(relativeBeaconPositions[beaconTwo] - relativeBeaconPositions[beaconOne]).Rotate(rotationIndex));

				if ((rotationIndex == 0) && (beaconTwo > beaconOne))
				{
					beaconOffsetsWithoutRepeats.emplace(
						relativeBeaconPositions[beaconOne],
						relativeBeaconPositions[beaconTwo],
						beaconOne,
						beaconTwo);
				}
			}
		}
	}
}

// Check to see whether there is an overlap between this scanner and another. To overlap,
// scanners are required to have 12 beacons in common. If overlap exists, save a pointer
// to that other scanner, along with the rotation and translation to apply to map between
// their views of space.
// Returns true if the beacons overlap, false otherwise.
bool Beacons::Scanner::CheckForOverlap(std::shared_ptr<Scanner> otherScanner)
{
	// If two scanners have 12 beacons in common, they will also have at least 66 instances of
	// two beacons in one scanner's space having the same offset (probably after some rotation)
	// between them as two beacons in the other scanner's space. As a quick check for whether it's
	// worth continuing checking for overlap, we can therefore check to see if there are 66 cases
	// where two beacons have the same *distance* between them in each scanner's space. If they do,
	// we probably have an overlap, and can put together details of the overlap using the pairs of
	// beacon offsets which likely contain the same beacons picked up by different scanners.
	std::vector<std::pair<BeaconOffset, BeaconOffset>> likelyPairs =
		BeaconOffset::OffsetsWithCommonDistance(beaconOffsetsWithoutRepeats, otherScanner->beaconOffsetsWithoutRepeats);
	if (likelyPairs.size() >= REQUIRED_BEACON_OFFSETS_IN_COMMON)
	{
		// We iterate over all of the pairs of beacon offsets just in case we have a false match,
		// but it only takes one pair to potentially prove that the beacons overlap.
		//  - Take a pair of beacon offsets which we suspect represent the same offset between two
		//    beacons but with different reference positions and rotations.
		//  - Check whether there is any rotation that could be applied to scanner 2 such that one
		//    of the beacons in that scanner's reference frame has 11 offsets to other beacons that
		//    are identical to offsets in this scanner's reference frame.
		//  - If it does, we have an overlap, and have found the rotation and translation to apply
		//    to a position in scanner two's reference space to translate it to scanner one.
		// Consider first the case where beacon one from scanner two is the same as beacon one from
		// scanner one, and then if that fails, try assuming that beacon *two* from scanner two is
		// beacon one from scanner one.
		for (auto const & likelyPair : likelyPairs)
		{
			BeaconOffset thisScannerPair = likelyPair.first;
			BeaconOffset otherScannerPair = likelyPair.second;

			std::optional<unsigned int> requiredRotation = RotationIndexToGiveOverlap(
				thisScannerPair.beaconIndices.first, otherScannerPair.beaconIndices.first, otherScanner);
			if (requiredRotation.has_value())
			{
				overlappingScanners.emplace_back(
					std::weak_ptr<Scanner>{otherScanner},
					RotationAndTranslation{ thisScannerPair.beaconPositions.first - otherScannerPair.beaconPositions.first.Rotate(requiredRotation.value()), requiredRotation.value()});
				return true;
			}

			requiredRotation = RotationIndexToGiveOverlap(
				thisScannerPair.beaconIndices.first, otherScannerPair.beaconIndices.second, otherScanner);
			if (requiredRotation.has_value())
			{
				overlappingScanners.emplace_back(
					std::weak_ptr<Scanner>{otherScanner},
					RotationAndTranslation{ thisScannerPair.beaconPositions.first - otherScannerPair.beaconPositions.second.Rotate(requiredRotation.value()), requiredRotation.value() });
				return true;
			}
		}
	}

	return false;
}

// Given two beacons which we suspect may be the same beacon relative to different scanners,
// determine if they truly are the same beacon. If they are, return the index of the rotation to
// apply to the second of those scanners to align it with the first.
std::optional<unsigned int> Beacons::Scanner::RotationIndexToGiveOverlap(
	unsigned int beaconIndexFromThisScanner,
	unsigned int beaconIndexFromOtherScanner,
	std::shared_ptr<Scanner> otherScanner)
{
	// For any given rotation, a scanner can holds a set of every other beacon's location relative
	// to any beacon. Leaving scanner one stationary, look for a rotation that can be applied to
	// scanner two to give 11 cases of the exact same offset appearing in both sets of beacon
	// offsets. If those 11 cases exist, then we have proven that the two scanners share 12 beacons
	// in common, and found the rotation index we need to apply to scanner two to align it with
	// scanner one rotationally.
	// The computational cost of looking for identical offsets (which are vectors) in two sets
	// is reduced by the fact that each set is ordered by the value of the vector.
	for (unsigned int rotationIndex = 0; rotationIndex < NUM_POSSIBLE_ROTATIONS; rotationIndex++)
	{
		unsigned int matches{ 0 };
		auto thisItr = allBeaconOffsets[beaconIndexFromThisScanner][0].begin();
		auto otherItr = otherScanner->allBeaconOffsets[beaconIndexFromOtherScanner][rotationIndex].begin();
		while (
			(thisItr != allBeaconOffsets[beaconIndexFromThisScanner][0].end()) &&
			(otherItr != otherScanner->allBeaconOffsets[beaconIndexFromOtherScanner][rotationIndex].end()))
		{
			if (*thisItr == *otherItr)
			{
				matches++;
				thisItr++;
			}
			else if (*thisItr < *otherItr) { thisItr++;	}
			else { otherItr++; }
		}
		if (matches >= REQUIRED_BEACONS_IN_COMMON - 1)
		{
			return rotationIndex;
		}
	}
	return std::optional<unsigned int>();
}

// To find the number of beacons which this scanner is now aware of the existence of, we simply
// need to put together a set of all of those beacons and get its size.
unsigned int Beacons::Scanner::NumberOfBeacons()
{
	std::set<Vector3> allBeacons = AllKnownRelativeBeacons();
	return allBeacons.size();
}

// The Manhattan Distance is the sum of the absolute difference between each component of 2
// vectors, i.e. |x1 - x2| + |y1 - y2| + |z1 - z2|. This function returns the maximum Manhattan
// Distance between any two scanners that this scanner is aware of (awareness can come either
// from overlapping with that scanner ourself, or by having one of the scanners we do overlap
// with overlap with another scanner in turn).
unsigned int Beacons::Scanner::MaxScannerManhattanDistance()
{
	std::set<Beacons::Vector3> scannerPositionsRelativeToSelf = AllKnownRelativeScanners();
	unsigned int maxManhattanDistance{ 0 };
	for (auto itrOne = scannerPositionsRelativeToSelf.begin(); itrOne != std::prev(scannerPositionsRelativeToSelf.end(), 1); itrOne++)
	{
		for (auto itrTwo = std::next(itrOne, 1); itrTwo != scannerPositionsRelativeToSelf.end(); itrTwo++)
		{
			unsigned int manhattanDistance = itrOne->ManhattanDistance(*itrTwo);
			if (manhattanDistance > maxManhattanDistance) { maxManhattanDistance = manhattanDistance; }
		}
	}
	return maxManhattanDistance;
}

// Return a set of Vectors representing the positions of all known beacons, relative to
// this scanner's position and rotation. We do this by asking each of the scanners this one
// overlaps with for their own set of known beacons (i.e. this function is recursive), and
// then apply our known rotation and translation transform to those positions to bring them
// relative to this scanner.
std::set<Beacons::Vector3> Beacons::Scanner::AllKnownRelativeBeacons()
{
	std::set<Vector3> relativeBeacons{};
	for (Vector3 const& position : relativeBeaconPositions) { relativeBeacons.insert(position); }

	for (ScannerOverlap scannerOverlap : overlappingScanners)
	{
		auto lock = scannerOverlap.otherScanner.lock();
		assert(lock);
		std::set<Vector3> beaconsRelativeToOtherScanner = lock->AllKnownRelativeBeacons();
		for (Vector3 const& beacon : beaconsRelativeToOtherScanner)
		{
			relativeBeacons.insert(
				beacon.Rotate(scannerOverlap.relativePositionAndRotation.rotationIndex) +
				scannerOverlap.relativePositionAndRotation.translation
			);
		}
	}

	return relativeBeacons;
};

// Return a set of Vectors representing the positions of all known scanners, relative to
// this scanner's position and rotation (we include this scanner in this set, with a position
// of 0,0,0). We do this by asking each of the scanners this one overlaps with for their
// own set of known scanners (i.e. this function is recursive), and then apply our known
// rotation and translation transform to those positions to bring them relative to this
// scanner.
std::set<Beacons::Vector3> Beacons::Scanner::AllKnownRelativeScanners()
{
	std::set<Vector3> relativeScanners{};
	relativeScanners.emplace(0, 0, 0);

	for (ScannerOverlap scannerOverlap : overlappingScanners)
	{
		auto lock = scannerOverlap.otherScanner.lock();
		assert(lock);
		std::set<Vector3> scannersRelativeToOtherScanner = lock->AllKnownRelativeScanners();
		for (Vector3 const& scanner : scannersRelativeToOtherScanner)
		{
			relativeScanners.insert(
				scanner.Rotate(scannerOverlap.relativePositionAndRotation.rotationIndex) +
				scannerOverlap.relativePositionAndRotation.translation
			);
		}
	}

	return relativeScanners;
}

// The offset between two beacon positions is just, by vector arithmetic, the difference between
// their positions. This offset also pre-calculates the square of the distance between them (we
// use the square to avoid dealing with rounding errors from taking the route after using
// pythagoras).
Beacons::BeaconOffset::BeaconOffset(
	Vector3 beaconOnePosition,
	Vector3 beaconTwoPosition,
	unsigned int beaconOneIndex,
	unsigned int beaconTwoIndex)
{
	offset = beaconOnePosition - beaconTwoPosition;
	distanceSquared = (offset.xx * offset.xx) + (offset.yy * offset.yy) + (offset.zz * offset.zz);
	beaconIndices = { beaconOneIndex, beaconTwoIndex };
	beaconPositions = { beaconOnePosition, beaconTwoPosition };
}

// Given two sets of BeaconOffsets, find out how many instance there are of the same offset distance
// (well actually the square of the distance, which is equivalent for our purposes) appearing in both
// sets. This is kept computationally cheap by the fact that sets of BeaconOffsets are ordered by
// the square of the offset distance.
std::vector<std::pair<Beacons::BeaconOffset, Beacons::BeaconOffset>> Beacons::BeaconOffset::OffsetsWithCommonDistance(
	std::set<BeaconOffset> offsetSetOne,
	std::set<BeaconOffset> offsetSetTwo)
{
	std::vector<std::pair<Beacons::BeaconOffset, Beacons::BeaconOffset>> offsetsWithCommonDistance{};

	auto itrOne = offsetSetOne.begin();
	auto itrTwo = offsetSetTwo.begin();
	while ((itrOne != offsetSetOne.end()) && (itrTwo != offsetSetTwo.end()))
	{
		if (itrOne->distanceSquared == itrTwo->distanceSquared)
		{
			offsetsWithCommonDistance.push_back(std::pair<BeaconOffset, BeaconOffset>{*itrOne, * itrTwo});
			itrOne++;
		}
		else if (itrOne->distanceSquared < itrTwo->distanceSquared) { itrOne++; }
		else { itrTwo++; }
	}

	return offsetsWithCommonDistance;
}

// To simplify the handling of rotation in this puzzle, each of the 24 cardinal orientations
// are assigned a 'rotation index'. This function applies the rotation defined by a given
// rotation index to a vector in 3D space.
Beacons::Vector3 Beacons::Vector3::Rotate(unsigned int rotationIndex) const noexcept
{
	assert(rotationIndex < NUM_POSSIBLE_ROTATIONS);

	// To visualise the possible rotations, remember that there are 6 different directions
	// the scanner could think was the x axis. And for each of these directions, there
	// are 4 possible rotations for the y and z axes. Then imagine what the position of
	// the coordinate (1,1,1) would be in each case, and that should help you to define the
	// position rotation required.
	// I'm visualing a case where x goes to the right, y goes up, and z goes into the page.
	switch (rotationIndex)
	{
	case 0: // x pointing in x +ve. No y-z rotation
		return { xx, yy, zz };
		break;
	case 1: // x pointing in x +ve. y-z rotated 90 degrees
		return { xx, -zz, yy };
		break;
	case 2: // x pointing in x +ve. y-z rotated 180 degrees
		return { xx, -yy, -zz };
		break;
	case 3: // x pointing in x +ve. y-z rotated 270 degrees
		return { xx, zz, -yy };
		break;
	case 4: // x pointing in x -ve after rotating about z. No y-z rotation relative to that
		return { -xx, -yy, zz };
		break;
	case 5: // x pointing in x -ve after rotating about z. y-z rotated 90 degrees
		return { -xx, zz, yy };
		break;
	case 6: // x pointing in x -ve after rotating about z. y-z rotated 180 degrees
		return { -xx, yy, -zz };
		break;
	case 7: // x pointing in x -ve after rotating about z. y-z rotated 270 degrees
		return { -xx, -zz, -yy };
		break;
	case 8: // x pointing in y +ve after rotating about z. No y-z rotation relative to that
		return { yy, -xx, zz };
		break;
	case 9: // x pointing in y +ve after rotating about z. y-z rotated 90 degrees
		return { yy, -zz, -xx };
		break;
	case 10: // x pointing in y +ve after rotating about z. y-z rotated 180 degrees
		return { yy, xx, -zz };
		break;
	case 11: // x pointing in y +ve after rotating about z. y-z rotated 270 degrees
		return { yy, zz, xx };
		break;
	case 12: // x pointing in y -ve after rotating about z. No y-z rotation relative to that
		return { -yy, xx, zz };
		break;
	case 13: // x pointing in y -ve after rotating about z. y-z rotated 90 degrees
		return { -yy, -zz, xx };
		break;
	case 14: // x pointing in y -ve after rotating about z. y-z rotated 180 degrees
		return { -yy, -xx, -zz };
		break;
	case 15: // x pointing in y -ve after rotating about z. y-z rotated 270 degrees
		return { -yy, zz, -xx };
		break;
	case 16: // x pointing in z +ve after rotating about y. No y-z rotation relative to that
		return { zz, yy, -xx };
		break;
	case 17: // x pointing in z +ve after rotating about y. y-z rotated 90 degrees
		return { zz, xx, yy };
		break;
	case 18: // x pointing in z +ve after rotating about y. y-z rotated 180 degrees
		return { zz, -yy, xx };
		break;
	case 19: // x pointing in z +ve after rotating about y. y-z rotated 270 degrees
		return { zz, -xx, -yy };
		break;
	case 20: // x pointing in z -ve after rotating about y. No y-z rotation relative to that
		return { -zz, yy, xx };
		break;
	case 21: // x pointing in z -ve after rotating about y. y-z rotated 90 degrees
		return { -zz, -xx, yy };
		break;
	case 22: // x pointing in z -ve after rotating about y. y-z rotated 180 degrees
		return { -zz, -yy, -xx };
		break;
	case 23: // x pointing in z -ve after rotating about y. y-z rotated 270 degrees
		return { -zz, xx, -yy };
		break;
	default:
		assert(false);
		return {};
		break;
	}
}
