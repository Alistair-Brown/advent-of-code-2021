#pragma once
#include <string>
#include <vector>

namespace Binary
{
	// Hold an unsigned integer value and allows operations to be performed on/with it based
	// on the binary representation of that number.
	class BinaryNumber
	{
	private:
		unsigned int decimalValue;

		unsigned int operator &(unsigned int otherNumber);
		static inline unsigned int maxNumberOfBits{ 16 };
		static unsigned int DecimalValueOfBitPosition(unsigned int bitPosition);
	public:
		BinaryNumber(unsigned int decimalValue) : decimalValue(decimalValue) {};
		BinaryNumber(std::string binaryNumberAsString);

		void InvertBits(unsigned int numBits);
		int DecimalValue() { return decimalValue; }

		// The bitMaskCreator function type represents a function capable of returning a single
		// BinaryNumber based on an inputList of existing BinaryNumbers. setOnTieBreak
		// will determine the value of any bits in this BinaryNumber which cannot be determined
		// by whatever rule the bitMaskCreator is applying to deduce the new number.
		typedef BinaryNumber(*BitMaskCreator)(std::vector<BinaryNumber> inputList, bool setOnTie, unsigned int numBits);

		static BinaryNumber MostCommonBitSettings(std::vector<BinaryNumber> numbersToAverageOver, bool setOnTie, unsigned int numBits);
		static BinaryNumber LeastCommonBitSettings(std::vector<BinaryNumber> numbersToAverageOver, bool setOnTie, unsigned int numBits);
		static std::vector<BinaryNumber> FilterOnBitMask(std::vector<BinaryNumber> inputList, bool bitIsSet, unsigned int bitMask);

		static BinaryNumber IterativeFilterToSingleNumber(
			BitMaskCreator bitMask,
			std::vector<Binary::BinaryNumber> inputLines,
			bool setOnTie,
			unsigned int lineLength);
	};
}