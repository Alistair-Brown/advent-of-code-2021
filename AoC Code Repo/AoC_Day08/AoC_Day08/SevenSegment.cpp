#include "SevenSegment.h"
#include <cassert>

SevenSeg::SevenSegDisplay::SevenSegDisplay(std::vector<std::string> unorderedDigitsAsSegments)
{
	std::vector<int> fiveLengthDigits, sixLengthDigits;
	int topMiddleSegment, middleMiddleSegment, bottomMiddleSegment;

	for (std::string singleDigit : unorderedDigitsAsSegments)
	{
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

		switch (singleDigit.size())
		{
		case 2:
			digitToBitMapping[1] = digitValueInBits;
			break;
		case 3:
			digitToBitMapping[7] = digitValueInBits;
			break;
		case 4:
			digitToBitMapping[4] = digitValueInBits;
			break;
		case 5:
			fiveLengthDigits.push_back(digitValueInBits);
			break;
		case 6:
			sixLengthDigits.push_back(digitValueInBits);
			break;
		case 7:
			digitToBitMapping[8] = digitValueInBits;
			break;
		default:
			assert(false);
			break;
		}
	}

	// Top middle is in 7 but not 1
	topMiddleSegment = digitToBitMapping[7] - digitToBitMapping[1];
	// middle middle is in 4 and every 5 segment digit (2, 3, 5)
	middleMiddleSegment = digitToBitMapping[4] & fiveLengthDigits[0] & fiveLengthDigits[1] & fiveLengthDigits[2];
	// bottom middle is the only other segment to be shared between the same 5 segment digits
	bottomMiddleSegment = (fiveLengthDigits[0] & fiveLengthDigits[1] & fiveLengthDigits[2]) - topMiddleSegment - middleMiddleSegment;

	for (int digit : sixLengthDigits)
	{
		// 0 is 8 minus the middle bit
		if ((digitToBitMapping[8] - digit) == middleMiddleSegment)
		{
			digitToBitMapping[0] = digit;
		}
		// 9 ANDs with 1 to give 1
		else if ((digit & digitToBitMapping[1]) == digitToBitMapping[1])
		{
			digitToBitMapping[9] = digit;
		}
		// The only other 6 digit is 6
		else
		{ 
			digitToBitMapping[6] = digit;
		}
	}

	for (int digit : fiveLengthDigits)
	{
		// 3 is the only 5 segment number to AND with 1 to give 1
		if ((digit & digitToBitMapping[1]) == digitToBitMapping[1])
		{
			digitToBitMapping[3] = digit;
		}
		// 5 AND's with 6 to leave 5
		else if ((digit & digitToBitMapping[6]) == digit)
		{
			digitToBitMapping[5] = digit;
		}
		// Leaving 2
		else
		{
			digitToBitMapping[2] = digit;
		}
	}

	// We've logic'd out the bits for each number, hooray!
}

unsigned int SevenSeg::SevenSegDisplay::GetValueOfSegments(std::vector<std::string> digitsAsSegments)
{
	unsigned int totalValue{ 0 };
	unsigned int powerOfTen{ digitsAsSegments.size() - 1 };
	
	for (std::string singleDigit : digitsAsSegments)
	{
		unsigned int digitValueInBits{ 0 };
		int trueDigitValue{ 0 };
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
		
		for (int ii = 0; ii < digitToBitMapping.size(); ii++)
		{
			if (digitValueInBits == digitToBitMapping[ii])
			{
				trueDigitValue = ii;
				break;
			}
		}

		totalValue += trueDigitValue * pow(10, powerOfTen);
		powerOfTen--;
	}
	return totalValue;
}
