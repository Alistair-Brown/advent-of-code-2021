#include "Binary.h"
#include <math.h>
#include <cassert>

int Binary::BinaryNumber::IntegerValueOfBit(int bit)
{
	return pow(2, bit);
}

// For a number of BinaryNumbers, return a single BinaryNumber that
// represents the most common setting of each bit.
Binary::BinaryNumber Binary::BinaryNumber::MostCommonBitSettings(
	std::vector<BinaryNumber> numbersToAverageOver,
	int tieBreakValue,
	int numBits)
{
	assert(numBits <= 16);
	assert((tieBreakValue == 1) || (tieBreakValue == 0));

	int decimalValueOfCommonBits = 0;
	for (int bitPosition = 0; bitPosition < numBits; bitPosition++)
	{
		// We increment the bit tracker for a 1, decrement it for a 0, can tell the
		// most common setting by whether this is +ve or -ve by the end of the list.
		int bitTracker = 0;
		for (BinaryNumber currentNumber : numbersToAverageOver)
		{
			bitTracker += (currentNumber.IsBitSet(bitPosition)) ? 1 : -1;
		}

		if (bitTracker == 0) { bitTracker = tieBreakValue; }
		if (bitTracker > 0)
		{
			decimalValueOfCommonBits += IntegerValueOfBit(bitPosition);
		}
	}

	return BinaryNumber(decimalValueOfCommonBits);
}

// The nuber representing the least common setting of each bit is just the bit-inverted most
// common setting.
Binary::BinaryNumber Binary::BinaryNumber::LeastCommonBitSettings(
	std::vector<BinaryNumber> numbersToAverageOver,
	int tieBreakValue,
	int numBits)
{
	// When getting the most common setting, we need to pass the inverse of our
	// desired tie break value, to account for the fact we're going to invert the result.
	tieBreakValue = (tieBreakValue == 1) ? 0 : 1;
	BinaryNumber bitResult = MostCommonBitSettings(numbersToAverageOver, tieBreakValue, numBits);
	bitResult.InvertBits(numBits);
	return bitResult;
}

// I should really go back and rename this function.
// What it does is filter a list down to a single number by iteratively finding a
// single number to filter on based on the current state of the list, and creating a
// new list containing only those numbers which had a particular bit set in that 'filter
// number'. The particular bit we're filtering on for each iteration starts as the leftmost
// and moves one step to the right each iteration.
Binary::BinaryNumber Binary::BinaryNumber::FindSingleNumber(
	Binary::BinaryNumber::bitMaskCreator bitMask,
	std::vector<Binary::BinaryNumber> inputLines,
	int tieBreak,
	int lineLength)
{
	std::vector<Binary::BinaryNumber> filteredList = inputLines;
	int currenBitPosition = lineLength - 1;
	bool currentBitSet = bitMask(filteredList, tieBreak, lineLength).IsBitSet(currenBitPosition);
	do
	{
		filteredList = Binary::BinaryNumber::FilterOnBitSetting(filteredList, currentBitSet, currenBitPosition);
		currenBitPosition--;
		currentBitSet = bitMask(filteredList, tieBreak, lineLength).IsBitSet(currenBitPosition);
	} while (filteredList.size() > 1);

	return filteredList.front();
}

// Return a new vector of BinaryNumbers containing only those numbers which have the bit
// at the specified position either set or unset, as specified.
std::vector<Binary::BinaryNumber> Binary::BinaryNumber::FilterOnBitSetting(
	std::vector<BinaryNumber> inputList,
	bool bitIsSet,
	int bitPosition)
{
	std::vector<BinaryNumber> filteredList;
	for (BinaryNumber currentNumber : inputList)
	{
		if (bitIsSet == currentNumber.IsBitSet(bitPosition))
		{
			filteredList.push_back(currentNumber);
		}
	}
	return filteredList;
}

bool Binary::BinaryNumber::IsBitSet(int bit)
{
	int tempInt = IntegerValueOfBit(bit);
	bool bitIsSet = (decimalValue & tempInt) > 0;
	return bitIsSet;
}

Binary::BinaryNumber::BinaryNumber(std::string binaryNumberAsString)
{
	// Binary numbers of greater than 16 bits can't be guaranteed to fit in an int
	int stringLength = binaryNumberAsString.length();
	if (stringLength > 16) { assert(false); }

	// Work from the rightmost character of the string as the 0th bit, moving
	// left to extract the value of each bit up to the highest value.
	decimalValue = 0;
	int bitPosition = 0;
	for (int ii = (stringLength - 1); ii >= 0; ii--)
	{
		if (binaryNumberAsString[ii] == '1')
		{
			decimalValue += IntegerValueOfBit(bitPosition);
		}
		else
		{
			// Our 'binary' string needs to actually be a binary number
			assert(binaryNumberAsString[ii] == '0');
		}
		bitPosition++;
	}
}

// For a number made of numBits in binary, invert every bit of the binary
// number.
void Binary::BinaryNumber::InvertBits(int numBits)
{
	for (int bitPosition = 0; bitPosition < numBits; bitPosition++)
	{
		if (IsBitSet(bitPosition))
		{
			decimalValue -= IntegerValueOfBit(bitPosition);
		}
		else
		{
			decimalValue += IntegerValueOfBit(bitPosition);
		}
	}
}
