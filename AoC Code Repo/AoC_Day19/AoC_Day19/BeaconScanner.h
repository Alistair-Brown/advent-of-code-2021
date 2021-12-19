#pragma once
#include <vector>
#include <array>
#include <set>

namespace Beacon
{
	const int BeaconsRequiredForOverlap{ 12 };
	const int PossibleRotations{ 24 };

	struct Vector3
	{
		int xPos{ 0 };
		int yPos{ 0 };
		int zPos{ 0 };
		Vector3() {};
		Vector3(int xPos, int yPos, int zPos) : xPos{ xPos }, yPos{ yPos }, zPos{ zPos } {};

		Vector3 operator *(Vector3 otherVector) {
			return Vector3(xPos * otherVector.xPos,
						   yPos * otherVector.yPos,
						   zPos * otherVector.zPos);
		}
		bool operator <(const Vector3 &otherVector) const {
			return ((xPos < otherVector.xPos) ||
					((xPos == otherVector.xPos) && (yPos < otherVector.yPos)) ||
					((xPos == otherVector.xPos) && (yPos == otherVector.yPos) && (zPos < otherVector.zPos)));
		}
		Vector3 operator -(Vector3 otherVector)
		{
			return Vector3(xPos - otherVector.xPos,
						   yPos - otherVector.yPos,
						   zPos - otherVector.zPos);
		}
		Vector3 operator +(Vector3 otherVector)
		{
			return Vector3(xPos + otherVector.xPos,
				yPos + otherVector.yPos,
				zPos + otherVector.zPos);
		}
	};

	class Position
	{
	private:
		Vector3 position{};
	public:
		Position() {};
		Position(Vector3 position) : position{ position } {};
		Position GetRotatedPosition(int rotationIndex);

		Position operator *(Position otherVector) {
			return Position(position * otherVector.position);
		}
		bool operator <(const Position &otherVector) const {
			return (position < otherVector.position);
		}
		Position operator -(Position otherPosition) {
			return Position(position - otherPosition.position);
		}
		Position operator +(Position otherPosition) {
			return Position(position + otherPosition.position);
		}
	};

	class Scanner
	{
	private:
		std::vector<Position> beaconsInRange{};
		Position relativeScannerPosition{ {0,0,0} };
		std::vector<Position> rotationallyRelativeBeaconsInRange{};
		std::vector<Position> fullyRelativeBeaconsInRange{};
		std::set<Position> rotationallyNormalizedAllBeacons{};
		// To determine overlaps between scanners, we need to know the positions of beacons
		// relative to each other, since that's scanner agnostic.
		std::vector<std::set<Position>> beaconOffsets{};
		std::vector<std::set<Position>> rotationalBeaconOffsets{};
	public:
		Scanner(std::vector<std::vector<int>> inputBeacons);
		int CheckForOverlap(Scanner otherScanner, Position &relativePosition);
		void RotationallyRelativizeBeaconsInRange(int rotationIndex);
		void FullyRelativizeBeaconsInRange();
		void SetRelativeScannerPosition(Position relativePosition) { relativeScannerPosition = relativePosition; }
		Position GetRelativeScannerPosition() { return relativeScannerPosition; }
		void AddBeaconsToTotal(std::vector<Position> newBeacons);
		std::vector<Position> FullyNormalizedBeaconsInRange() { return fullyRelativeBeaconsInRange; }
		int NumberOfTotalBeacons() {return rotationallyNormalizedAllBeacons.size(); }
	};
}