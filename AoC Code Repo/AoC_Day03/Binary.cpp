#include "Binary.h"
#include <math.h>
#include <cassert>

// To perform a bitwise-AND with an unsigned int, we need to access the decimal
// value inside a BinaryNumber object.
unsigned int Binary::BinaryNumber::operator&(unsigned int otherNumber)
{
	return (this->decimalValue & otherNumber);
}

// The equivalent decimal value of a bit at a given position (from the least significant)
// can of course be found from the corresponding power of 2, taking care to account
// for the fact that the cast from double to int will round down.
unsigned int Binary::BinaryNumber::DecimalValueOfBitPosition(unsigned int bitPosition)
{
	return static_cast<unsigned int>(pow(2, bitPosition) + 0.5);
}

// Constructor to create a binary number from the equivalent string (e.g. "011010").
Binary::BinaryNumber::BinaryNumber(std::string binaryNumberAsString)
{
	// Binary numbers of greater than 16 bits can't be guaranteed to fit in an int
	unsigned int stringLength = binaryNumberAsString.length();
	assert(stringLength <= maxNumberOfBits);

	// Work from the rightmost character of the string as the 0th bit, moving
	// left to extract the value of each bit up to the highest value.
	decimalValue = 0;
	unsigned int bitValue{ 1 };
	for (int ii = (stringLength - 1); ii >= 0; ii--)
	{
		if (binaryNumberAsString[ii] == '1')
		{
			decimalValue += bitValue;
		}
		else
		{
			// Just a safety check that the string really does represent a binary
			// number, any character which is not '1' must be '0'.
			assert(binaryNumberAsString[ii] == '0');
		}
		bitValue <<= 1;
	}
}

// For a selection of BinaryNumbers of length numBis, return a single BinaryNumber that represents
// the most common setting of each bit. setOnTie determines whether a given bit will be
// set or not in the case that exactly half of the BinaryNumber's in the collection
// have that bit set.
Binary::BinaryNumber Binary::BinaryNumber::MostCommonBitSettings(
	std::vector<BinaryNumber> binaryNumbers,
	bool setOnTie,
	unsigned int numBits)
{
	assert(numBits <= maxNumberOfBits);

	// Build up the new BinaryNumber starting from the least significant bit and
	// bit-shifting left until we've covered the required number of bits. If a given
	// bit is more commonly set than not, we add its equivalent decimal value to a
	// an integer tracking the decimal value of the BinaryNumber formed from the
	// most common setting of each bit.
	unsigned int decimalValueOfCommonBits = 0;
	unsigned int mostSignificantBitValue = DecimalValueOfBitPosition(numBits);
	unsigned int currentBitInDecimal{ 1 };
	while (currentBitInDecimal <= mostSignificantBitValue)
	{
		// For each BinaryNumber in the list, the bitTracker is either incremented or
		// decremented depending on whether the current bit being considered is set.
		// We can then tell whether a given bit was more commonly set than not by
		// whether the final value of bitTracker is +ve or -ve (remebering to break
		// ties correctly).
		int bitTracker = 0;
		for (BinaryNumber currentNumber : binaryNumbers)
		{
			bitTracker += (currentNumber & currentBitInDecimal) ? 1 : -1;
		}
		if (bitTracker == 0) { bitTracker = setOnTie ? 1 : -1; }

		if (bitTracker > 0)
		{
			decimalValueOfCommonBits += currentBitInDecimal;
		}
		currentBitInDecimal <<= 1;
	}

	// We've deduced the decimal value of the BinaryNumber formed from the most
	// common setting of each bit, which we can convert directly into a BinaryNumber
	// using the right constructor.
	return BinaryNumber(decimalValueOfCommonBits);
}

// To get the BinaryNumber formed from the least common bit of each BinaryNumber in
// a collection, we can simply invert each bit in the BinaryNumber formed from the
// most common setting of each bit.
Binary::BinaryNumber Binary::BinaryNumber::LeastCommonBitSettings(
	std::vector<BinaryNumber> numbersToAverageOver,
	bool setOnTie,
	unsigned int numBits)
{
	// When getting the most common setting of each bit, we need to pass the inverse of our
	// desired tie break setting, since we will invert every bit in the result.
	BinaryNumber bitResult = MostCommonBitSettings(numbersToAverageOver, !setOnTie, numBits);
	bitResult.InvertBits(numBits);
	return bitResult;
}

// Filter a list of BinaryNumbers down to a single number by iteratively:
//  - Using the bitMaskCreator to create a bit mask based only on the numbers
//    not already filtered out of the list by previous iterations.
//  - Applying the bit mask to create a new list of BinaryNumber's containing
//    only those numbers which had the same setting as the bitMask for a certain
//    bit.
//  - On the first iteration of the filter, this 'certain bit' will be the most
//    significant bit, on the next iteration it will be the next most significant,
//    etc.
//  - Once there is only a single BinaryNumber remaining (the puzzle input guarantees
//    this will be the case eventually), return that as the result.
Binary::BinaryNumber Binary::BinaryNumber::IterativeFilterToSingleNumber(
	Binary::BinaryNumber::BitMaskCreator bitMaskCreator,
	std::vector<Binary::BinaryNumber> inputLines,
	bool setOnTie,
	unsigned int binaryNumberLength)
{
	// For the first iteration, the bit we care about is the most significant, for
	// the length of binary number we are dealing with.
	unsigned int currentIterationSignificantBit = DecimalValueOfBitPosition(binaryNumberLength - 1);

	// For as long as we have more than one binary number left in the list:
	// - Construct this iteration's bitmask from the numbers remaining in the list, and determine
	//   whether the bit at the position we are currently interested in is set in that mask.
	// - Filter the list down to only contain the BinaryNumber's with that bit set or not, as
	//   required.
	// - Bitshift our 'bit of interest' to right to get the next most significant bit position
	//   for our next iteration.
	do
	{
		bool currentBitSet = bitMaskCreator(inputLines, setOnTie, binaryNumberLength) & currentIterationSignificantBit;
		inputLines = Binary::BinaryNumber::FilterOnBitMask(inputLines, currentBitSet, currentIterationSignificantBit);
		currentIterationSignificantBit >>= 1;
	} while (inputLines.size() > 1);
	return inputLines.front();
}

// Return a new vector of BinaryNumbers filtered from the original vector to contain either:
//  - In the filter-in case, only those numbers which contain the bitMask
//  - In the filter-out case, only those numbers which do not contain the bitMask
std::vector<Binary::BinaryNumber> Binary::BinaryNumber::FilterOnBitMask(
	std::vector<BinaryNumber> inputList,
	bool filterIn,
	unsigned int bitMask)
{
	std::vector<BinaryNumber> filteredList;
	bool matchesMask;
	for (BinaryNumber currentNumber : inputList)
	{
		matchesMask = (currentNumber & bitMask) == bitMask;
		if (filterIn ? matchesMask : !matchesMask)
		{
			filteredList.push_back(currentNumber);
		}
	}
	return filteredList;
}

// Invert the first numBits bits of a BinaryNumber, starting from the least significant bit.
void Binary::BinaryNumber::InvertBits(unsigned int numBits)
{
	unsigned int bitMask{ 1 };
	for (unsigned int bitPosition = 0; bitPosition < numBits; bitPosition++)
	{
		decimalValue ^= bitMask;
		bitMask <<= 1;
	}
}
