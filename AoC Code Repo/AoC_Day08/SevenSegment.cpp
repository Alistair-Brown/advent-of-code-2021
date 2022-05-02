#include "SevenSegment.h"
#include <cassert>

// Construct a SevenSegDisplay which will be capable of working out which digits
// are represented by any combination of segments. To do this, it needs to be
// provided with the 10 possible segment combinations for this display. We apply
// a series of logic steps to work out which letter corresponds to which segment
// in the display, and hence which letters make up a given digit.
SevenSeg::SevenSegDisplay::SevenSegDisplay(
	std::array<std::string, SevenSeg::numLeftHandElements> unorderedDigitsAsSegments)
{
	std::vector<unsigned int> fiveLengthDigits, sixLengthDigits;
	unsigned int topMiddleSegment, middleMiddleSegment, bottomMiddleSegment;

	// First loop through the character making up each digit to pull out the obvious
	// matches and set others aside for further work.
	for (std::string singleDigit : unorderedDigitsAsSegments)
	{
		// Start off by summing up the segments that make up this digit, as
		// the bits corresponding to each character. This just makes all the
		// other logic steps much easier.
		unsigned int digitValueInBits{ 0 };
		for (char segment : singleDigit)
		{
			if (segment == 'a') { digitValueInBits += aInBits; }
			else if (segment == 'b') { digitValueInBits += bInBits; }
			else if (segment == 'c') { digitValueInBits += cInBits; }
			else if (segment == 'd') { digitValueInBits += dInBits; }
			else if (segment == 'e') { digitValueInBits += eInBits; }
			else if (segment == 'f') { digitValueInBits += fInBits; }
			else if (segment == 'g') { digitValueInBits += gInBits; }
		}

		// Some digits can be easily deduced e.g. only 1 is composed of just
		// 2 segments. So for certain lengths of string we can immediately save
		// off the bit sum in the mapping. But there are multiple digits that can
		// be made up of 5 or 6 segments, so put those to the side for further
		// logic later.
		switch (singleDigit.size())
		{
		case 2:
			digitToSegmentBitsMapping[1] = digitValueInBits;
			break;
		case 3:
			digitToSegmentBitsMapping[7] = digitValueInBits;
			break;
		case 4:
			digitToSegmentBitsMapping[4] = digitValueInBits;
			break;
		case 5:
			fiveLengthDigits.push_back(digitValueInBits);
			break;
		case 6:
			sixLengthDigits.push_back(digitValueInBits);
			break;
		case 7:
			digitToSegmentBitsMapping[8] = digitValueInBits;
			break;
		default:
			assert(false);
			break;
		}
	}

	// The first step in my logic for working out the remaining numbers requires us
	// to work out which characters represent the 3 central segements. We can work
	// this out as follows:
	//  - The top-middle is in 7 but not 1
	//  - The middle-middle is the only bit common to '4' and every 5-segment digit (2, 3, 5)
	//  - The bottom-middle is the only other segment to be shared between the 5-segment digits
	topMiddleSegment = digitToSegmentBitsMapping[7] - digitToSegmentBitsMapping[1];
	middleMiddleSegment = digitToSegmentBitsMapping[4] & fiveLengthDigits[0] & fiveLengthDigits[1] & fiveLengthDigits[2];
	bottomMiddleSegment = (fiveLengthDigits[0] & fiveLengthDigits[1] & fiveLengthDigits[2]) - topMiddleSegment - middleMiddleSegment;

	// We can now work out the 6-segment digits as follows:
	//  - '0' is just '8' without the middle-middle segment.
	//  - Other than '0', '9' is the only 6-segment digit to segment-wise
	//    AND with '1' to give '1'.
	//  - '6' is the only remaining 6-segment digit.
	for (int digit : sixLengthDigits)
	{
		if ((digitToSegmentBitsMapping[8] - digit) == middleMiddleSegment)
		{
			digitToSegmentBitsMapping[0] = digit;
		}
		else if ((digit & digitToSegmentBitsMapping[1]) == digitToSegmentBitsMapping[1])
		{
			digitToSegmentBitsMapping[9] = digit;
		}
		else
		{ 
			digitToSegmentBitsMapping[6] = digit;
		}
	}

	// We can now work out the 6-segment digits as follows:
	//  - '3' is the only 5-segment digit to segment-wise-AND with 1 to give 1
	//  - '5' is the only 5-segment digit to segment-wise-AND with 6 to give itself
	//  - 5 AND's with 6 to leave 5
	//  - '2' is the only remaining 6-segment digit.
	for (int digit : fiveLengthDigits)
	{
		if ((digit & digitToSegmentBitsMapping[1]) == digitToSegmentBitsMapping[1])
		{
			digitToSegmentBitsMapping[3] = digit;
		}
		else if ((digit & digitToSegmentBitsMapping[6]) == digit)
		{
			digitToSegmentBitsMapping[5] = digit;
		}
		else
		{
			digitToSegmentBitsMapping[2] = digit;
		}
	}
}

// Take a series of strings representing digits in their segment format
// (e.g. "ab" to represent the digit '1' if a and b are the right hand vertical
// segments of a 7-segment display), in the order of most to least significant
// digit. Based on the mapping between segments and digits this class did in
// its constructor, work out which number is being represented by these series
// of digits in segment format.
unsigned int SevenSeg::SevenSegDisplay::GetValueOfSegments(
	std::vector<std::string> digitsAsSegments)
{
	// The digits are given in order of significance from most to least, so we
	// start off multiplying the first deduced digit by the corresponding power of
	// 10 before adding it to the total, and reduced the power of 10 as we work
	// through the digits.
	unsigned int totalValue{ 0 };
	unsigned int powerOfTen{ digitsAsSegments.size() - 1 };
	
	for (std::string singleDigit : digitsAsSegments)
	{
		// Sum together the bits which form this digit. We'll be able to
		// compare this sum of segments to the digitToSegmentBitsMapping
		// to easily find out which digit is being represented.
		unsigned int digitValueInBits{ 0 };
		unsigned int trueDigitValue{ 0 };
		for (char segment : singleDigit)
		{
			if (segment == 'a') { digitValueInBits += aInBits; }
			else if (segment == 'b') { digitValueInBits += bInBits; }
			else if (segment == 'c') { digitValueInBits += cInBits; }
			else if (segment == 'd') { digitValueInBits += dInBits; }
			else if (segment == 'e') { digitValueInBits += eInBits; }
			else if (segment == 'f') { digitValueInBits += fInBits; }
			else if (segment == 'g') { digitValueInBits += gInBits; }
		}
		
		// Find the element of digitToSegmentBitsMapping which contains the value
		// we just summed up. This gives us the digit represented here.
		for (unsigned int ii = 0; ii < digitToSegmentBitsMapping.size(); ii++)
		{
			if (digitValueInBits == digitToSegmentBitsMapping[ii])
			{
				trueDigitValue = ii;
				break;
			}
		}

		// Digits are given most to least significant, so just a little
		// power of ten to apply before reducing the power for the next digit.
		totalValue += trueDigitValue * (unsigned int)pow(10, powerOfTen);
		powerOfTen--;
	}
	return totalValue;
}
