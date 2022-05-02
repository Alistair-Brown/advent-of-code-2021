#pragma once
#include <array>
#include <vector>

// The cornerstone of my solution here is the representation of each segement as a bit.
// e.g. segement 'a' uses the 1-bit, segment 'b' uses the 2-bit etc. This allows me to
// do operations on collections of segments easily, for example if I know the the central
// segment of a display is 'e', I can just subtract the 5-th bit (16) from the sum of all
// bits a-f to get the bitwise representation of the '0' digit without having to keep
// track of the individual segments making that number up.
// It also saves me from worrying about variations in the order that segments are provided.
namespace SevenSeg
{
	const unsigned int aInBits = 1;
	const unsigned int bInBits = 2;
	const unsigned int cInBits = 4;
	const unsigned int dInBits = 8;
	const unsigned int eInBits = 16;
	const unsigned int fInBits = 32;
	const unsigned int gInBits = 64;

	// There are 10 possible digits, 0-9. There will be a corresponding
	// number of left hand elements in each line of input for us to
	// deduce the letters corresponding to each segement from, and the
	// 4 digits on the right hand side of each input line that represent
	// the output of a given display.
	constexpr unsigned int numberOfPossibleDigits = 10;
	constexpr unsigned int numLeftHandElements = 10;
	constexpr unsigned int numRightHandElements = 4;

	// A SevenSegDisplay is constructed using 10 strings representing each possible
	// digit (0-9) given as the segments making up that digit, e.g. "cfgeda", "afcbg"...
	// From this input, the SevenSegDisplay will work out which collection of
	// segments represent each digit. It can subsequently be provided with a vector
	// of further character collections representing the digits making up a number
	// (e.g. "cfgeda", "afcbg" for a 2-digit number), and return the value of that
	// number based on the deduction of character-to-segments that it has already
	// done.
	class SevenSegDisplay
	{
	private:
		// The digit-to-segments-mapping maps from a given displayed digit to
		// the integer that would be formed by the bit representation of the 
		// segments making up that digit. e.g. if the digit '1' used the segments
		// b (2nd bit, value 2) and d (4th bit, value 8) this array would map
		// the digit '1' to a value of 10.
		std::array<unsigned int, numberOfPossibleDigits> digitToSegmentBitsMapping;
	public:
		SevenSegDisplay(std::array<std::string, numLeftHandElements> unorderedDigitsAsSegments);
		unsigned int GetValueOfSegments(std::vector<std::string> digitsAsSegments);
	};
}