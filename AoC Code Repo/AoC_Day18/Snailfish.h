#pragma once
#include <utility>
#include <string>
#include "Parsing.h"
#include <memory>

namespace Snail
{
	constexpr unsigned int MaxNestDepth = 5;

	// We're going to want to do a lot of operations treating either element of a snailfish
	// number as an abstract 'number', rather than having to know whether it is a snailfish
	// number or a regular one. So we want an abstract class for that.
	class Number 
	{
	public:
		virtual int Magnitude() = 0;
		virtual std::string ValueAsString() = 0;
		virtual void IncreaseLeftmostValue(int increase) = 0;
		virtual void IncreaseRightmostValue(int increase) = 0;
		virtual bool MaybeExplode(int depth, std::pair<int, int> &pairToExplode) = 0;
		virtual bool MaybeSplit(std::unique_ptr<Number> &newNumber) = 0;

		virtual ~Number() = default;
	};
	
	// Most of the member functions of a regular number drop out as trivially simple, since
	// it just has a value, and cannot be 'exploded', since this is a reduction step that
	// only acts on snailfish numbers. The only non-trivial member function of RegularNumber
	// is MaybeSplit, which will cause it to split into a SnailfishNumber if it's magnitude
	// exceeds 9.
	class RegularNumber : public Number
	{
	private:
		int value;
	public:
		RegularNumber(int value) : value{ value } {};

		int Magnitude() { return value; }
		void IncreaseLeftmostValue(int increase) {value += increase; }
		void IncreaseRightmostValue(int increase) {value += increase; }
		std::string ValueAsString() { return std::to_string(value); }

		bool MaybeExplode(int depth, std::pair<int, int>& pairToExplode) { return false; }
		bool MaybeSplit(std::unique_ptr<Number> &newNumber);
	};

	// The key feature of the SnailfishNumber is that it has made up of a pair of other
	// numbers, which are pointed to as abstract Numbers, since they could be RegularNumber's
	// or SnailfishNumbers themselves.
	class SnailfishNumber : public Number
	{
	private:
		std::pair<std::unique_ptr<Number>, std::unique_ptr<Number>> numberPair;
	public:
		SnailfishNumber(std::pair<std::unique_ptr<Number>, std::unique_ptr<Number>> numberPairIn) :
			numberPair{ std::move(numberPairIn) } {};
		SnailfishNumber(std::pair<std::string, std::string> numberPairIn);
		SnailfishNumber(SnailfishNumber&&) = default;

		int Magnitude();
		std::string ValueAsString();

		SnailfishNumber& operator +=(SnailfishNumber&&);

		// A key part of 'splitting' a RegularNumber is that there will be a corresponding
		// value increase in the nearest RegularNumbers to that one (nearest in the sense
		// of how near they would be if the SnailfishNumber was read by eye).
		// These functions play a crucial part in this, by allowing us to recurse
		// all the way down to the far left or right of a given SnailfishNumber.
		void IncreaseLeftmostValue(int increase) {numberPair.first->IncreaseLeftmostValue(increase); }
		void IncreaseRightmostValue(int increase) { numberPair.second->IncreaseRightmostValue(increase); }

		bool MaybeExplode(int depth, std::pair<int, int> &pairToExplode);
		bool MaybeSplit(std::unique_ptr<Number> &newNumber);
	};

	// Given a string representing the full snailfish number, split the string into the left and
	// right elements of that snailfish number, e.g. "[4,[9,1]]" becomes "4" and "[9,1]".
	std::pair<std::string, std::string> SplitStringIntoSnailfishPair(std::string const &completeSnailfishNumber);

	// Add two SnailfishNumbers together, and perform as many explode and split reduction
	// steps as required to leave a valid SnailfishNumber. This is intentionally not an operator
	// overload on SnailfishNumber, since it is specifically for adding two *complete* SnailfishNumbers
	// we have pointers to, rather than arbitratrily adding SnailfishNumbers which might
	// be nested within others. This is because a SnailfishNumber does not know how deeply
	// it is nested, and this is a key element of the 'explode' step in reduction.
	std::unique_ptr<SnailfishNumber> AddSnailfishNumbers(
		std::unique_ptr<SnailfishNumber> numberOne,
		std::unique_ptr<SnailfishNumber> numberTwo);
}