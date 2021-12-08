#pragma once
#include <array>
#include <vector>

namespace SevenSeg
{
	// To allow me to more easily compare numbers that may have their a-g chars in
	// different orders, each character will be assigned a bit, so I can just compare
	// ints.
	const unsigned int aInBits = 1;
	const unsigned int bInBits = 2;
	const unsigned int cInBits = 4;
	const unsigned int dInBits = 8;
	const unsigned int eInBits = 16;
	const unsigned int fInBits = 32;
	const unsigned int gInBits = 64;

	// The numbers 0-9 will be represented on the display
	const int numberOfPossibleDigits = 10;

	class SevenSegDisplay
	{
	private:
		std::array<int, numberOfPossibleDigits> digitToBitMapping;
	public:
		// This is the key function, that needs to take 10 strings, and work out which one is
		// which digit, storing the result in the member var digitToBitMapping;
		SevenSegDisplay(std::vector<std::string> unorderedDigitsAsSegments);
		unsigned int GetValueOfSegments(std::vector<std::string> digitsAsSegments);
	};
}