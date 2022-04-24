#pragma once
#include <string>
#include <vector>

namespace Binary
{
	// Hold a numerical value and allow operations to be performed on/with it based
	// on the bits that make up that number in binary.
	class BinaryNumber
	{
	private:
		int decimalValue;
		static int IntegerValueOfBit(int bit);
	public:
		BinaryNumber(int decimalValue) : decimalValue(decimalValue) {};
		BinaryNumber(std::string binaryNumberAsString);
		bool IsBitSet(int bit);
		void InvertBits(int numBits);
		int DecimalValue() { return decimalValue; }

		static BinaryNumber MostCommonBitSettings(std::vector<BinaryNumber> numbersToAverageOver, int tieBreakValue, int numBits = 16);
		static BinaryNumber LeastCommonBitSettings(std::vector<BinaryNumber> numbersToAverageOver, int tieBreakValue, int numBits = 16);
		static std::vector<BinaryNumber> FilterOnBitSetting(std::vector<BinaryNumber> inputList, bool bitIsSet, int bitPosition);
		// Defines a function capable of returning a single BinaryNumber by applying some rule to a list of
		// BinaryNumbers or width numBits, using the tieBreakSetting to decide the bit in the case of any
		// ties in the rule.
		typedef BinaryNumber(*bitMaskCreator)(std::vector<BinaryNumber> inputList, int tieBreakSetting, int numBits);
		static BinaryNumber FindSingleNumber(
			bitMaskCreator bitMask,
			std::vector<Binary::BinaryNumber> inputLines,
			int tieBreak,
			int lineLength);
	};


}