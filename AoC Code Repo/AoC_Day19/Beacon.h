#pragma once
#include <vector>
#include <cassert>
#include <utility>
#include <set>
#include <memory>
#include <list>
#include <map>
#include <optional>
#include <cmath>

namespace Beacons
{
	// Two scanners must have 12 beacons in common to have sufficient overlap. We can reason
	// that within these 12 common beacons, there are 66 distinct routes, or 'offsets' from
	// one beacon to another (found using the sum of an arithmetic sequence below).
	constexpr unsigned int REQUIRED_BEACONS_IN_COMMON = 12;
	constexpr unsigned int REQUIRED_BEACON_OFFSETS_IN_COMMON =
		((REQUIRED_BEACONS_IN_COMMON - 1) * ((REQUIRED_BEACONS_IN_COMMON - 1) + 1)) / 2;
	constexpr unsigned int NUM_POSSIBLE_ROTATIONS = 24;

	// There's a lot of wrangling with 3-dimensional vectors in this puzzle. This struct
	// provides a 3D-vector to make that easier, by:
	//  - Providing a number of arithimetic and comparison operators
	//  - Allowing you to find the result of rotating a vector using one of the 24 possible
	//    rotations, which are each assigned a unique index in my solution
	//  - Finding the Manhattan Distance between two vectors (this is the sum of the absolute
	//    difference between each component of the vector)
	struct Vector3
	{
		int xx;
		int yy;
		int zz;
		Vector3() : Vector3{ 0, 0, 0 } {};
		Vector3(int xx, int yy, int zz) : xx{ xx }, yy{ yy }, zz{ zz } {};
		Vector3(std::vector<int> const& position) :
			xx{ position[0] }, yy{ position[1] }, zz{ position[2] } { assert(position.size() == 3); }

		Vector3 Rotate(unsigned int rotationIndex) const noexcept;

		unsigned int ManhattanDistance(Vector3 const& otherVec) const
		{
			int xComponent = xx > otherVec.xx ? xx - otherVec.xx : otherVec.xx - xx;
			int yComponent = yy > otherVec.yy ? yy - otherVec.yy : otherVec.yy - yy;
			int zComponent = zz > otherVec.zz ? zz - otherVec.zz : otherVec.zz - zz;
			assert(xComponent >= 0);
			assert(yComponent >= 0);
			assert(zComponent >= 0);
			return xComponent + yComponent + zComponent;
		}

		Vector3 operator -(Vector3 const& otherVec) const noexcept
		{
			return { (xx - otherVec.xx), (yy - otherVec.yy), (zz - otherVec.zz) };
		}
		Vector3 operator +(Vector3 const& otherVec) const noexcept
		{
			return { (xx + otherVec.xx), (yy + otherVec.yy), (zz + otherVec.zz) };
		}
		bool operator <(Vector3 const& otherVec) const noexcept
		{
			return (
				xx != otherVec.xx ? xx < otherVec.xx :
				yy != otherVec.yy ? yy < otherVec.yy :
				zz != otherVec.zz ? zz < otherVec.zz : false
				);
		};
		bool operator ==(Vector3 const& otherVec) const noexcept
		{
			return (
				(xx == otherVec.xx) &&
				(yy == otherVec.yy) &&
				(zz == otherVec.zz)
				);
		};
		bool operator !=(Vector3 const& otherVec) const noexcept { return !(*this == otherVec); }
	};

	// Often in this puzzle we must first rotate and then translate a vector in order to
	// transform some location into it's equivalent relative to a different scanner. This
	// struct bundles those two operations together for convenience.
	struct RotationAndTranslation
	{
		Vector3 translation;
		unsigned int rotationIndex;
	};

	// This represents the offset between 2 beacons detected by a given scanner. As well
	// as the 3D-vector between the beacons positions, it also holds the square
	// of the distance from one beacon to the other, which is the primary element by which
	// these offsets are sorted in containers.
	// Since this is specifically the offset between two beacons detected by a single scanner,
	// this struct also holds the positions of each beacon relative to that scanner, and the
	// indices of each beacon in that scanner's list of beacons. 
	struct BeaconOffset
	{
		Vector3 offset;
		unsigned int distanceSquared;		
		std::pair<unsigned int, unsigned int> beaconIndices;
		std::pair<Vector3, Vector3> beaconPositions;

		BeaconOffset(
			Vector3 beaconOnePosition,
			Vector3 beaconTwoPosition,
			unsigned int beaconOneIndex,
			unsigned int beaconTwoIndex);

		bool operator<(BeaconOffset const& otherOffset) const noexcept
		{
			return (
				distanceSquared != otherOffset.distanceSquared ?
					distanceSquared < otherOffset.distanceSquared :
				offset != otherOffset.offset ?
					offset < otherOffset.offset :
				beaconIndices.first != otherOffset.beaconIndices.first ?
					beaconIndices.first < otherOffset.beaconIndices.first :
				beaconIndices.second < otherOffset.beaconIndices.second
				);
		};

		// Given two sets of BeaconOffsets, find how many of the offsets within one
		// set share a square distance within an offset in the other.
		// Returns a vector of these offsets which share a common distance.
		static std::vector<std::pair<BeaconOffset, BeaconOffset>> OffsetsWithCommonDistance(
			std::set<BeaconOffset> offsetSetOne,
			std::set<BeaconOffset> offsetSetTwo
		);
	};

	// A scanner is initialised with a list of beacons which are within its detection
	// range and which it knows the positions of relative to its own position and orientation.
	// A scanner can check whether it has 12 beacons in common with any other scanner and
	// hence has an overlapping detection range, keeping track of any scanners it find it
	// overlaps with in this way.
	class Scanner
	{
	private:
		// For debugging purposes, this scanner remembers the order that it came in the puzzle
		// input.
		unsigned int scannerIndex{};

		// A scanner holds the following informatin about the beacons in its range:
		//  - The position of each of those beacons relative to this scanner.
		//  - For each beacon, the relative position of each other beacon. A separate
		//    set of overlaps is held for each possible rotation that could be applied
		//    to 3D space.
		//  - Every unique offset between any 2 beacons (i.e. this set will not contain
		//    both the offset of beacon 1 to beacon 2 and that of beacon 2 to beacon 1).
		std::vector<Vector3> relativeBeaconPositions;
		std::vector<std::vector<std::set<Vector3>>> allBeaconOffsets;
		std::set<BeaconOffset> beaconOffsetsWithoutRepeats;

		// sturct holding a pointer to a scanner known to overlap with this one, and the rotation
		// and translation to apply to any location in that scanner's space to find the equivalent
		// position relative to this scanner.
		// A scanner holds a list of these for the scanners it has found that it overlaps with.
		struct ScannerOverlap
		{
			std::weak_ptr<Scanner> otherScanner;
			RotationAndTranslation relativePositionAndRotation;
		};
		std::list<ScannerOverlap> overlappingScanners{};
		
		// Check whether there is any rotation that could be applied to another scanner that
		// would cause a given beacon in that scanner's range to have enough identical offsets
		// to a beacon in *this* scanner's range to prove that they are in fact the same
		// overlapping beacon.
		// Returns the rotation index if such a rotation exists.
		std::optional<unsigned int> RotationIndexToGiveOverlap(
			unsigned int beaconIndexFromThisScanner,
			unsigned int beaconIndexFromOtherScanner,
			std::shared_ptr<Scanner> otherScanner);

		// Given the overlaps with other scanners that this scanner knows of, and the scanners
		// that *those* scanners overlap with in turn (i.e. these functions are recursive) return
		// a set holding the position of each known scanner/beacon relative to this scanner.
		std::set<Vector3> AllKnownRelativeBeacons();
		std::set<Vector3> AllKnownRelativeScanners();

	public:
		Scanner(unsigned int scannerIndexIn, std::vector<std::vector<int>> const &beaconsIn);

		bool CheckForOverlap(std::shared_ptr<Scanner> otherScanner);

		unsigned int NumberOfBeacons();
		unsigned int MaxScannerManhattanDistance();
	};
}