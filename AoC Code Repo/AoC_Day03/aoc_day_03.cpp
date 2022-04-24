#include <iostream>
#include <string>
#include "Parsing.h"
#include "Binary.h"
#include <vector>
#include "puzzle_solvers.h"
#include "Parsing.h"

// Day three involves working with binary numbers, specifically finding new binary numbers
// based on the most or least common settings of given bits within an initial list of binary
// numbers.
PuzzleAnswerPair PuzzleSolvers::AocDayThreeSolver(std::ifstream &puzzleInputFile)
{
	std::vector<std::string> rawInputLines = Parsing::SeparateInputIntoLines(puzzleInputFile);
	unsigned int binaryNumberLength = rawInputLines[0].size();
	std::vector<Binary::BinaryNumber> binaryInputLines{};
	for (std::string singleLine : rawInputLines)
	{
		binaryInputLines.emplace_back(singleLine);
	}

	// The gamma value is found from the most common setting of each bit, while
	// the epsilon value is the least common setting of each bit.
	// The product of the gamma and epsilon values is the solution to part one.
	Binary::BinaryNumber gammaValue = Binary::BinaryNumber::MostCommonBitSettings(
		binaryInputLines,
		true,
		binaryNumberLength
	);
	Binary::BinaryNumber epsilonValue = Binary::BinaryNumber::LeastCommonBitSettings(
		binaryInputLines,
		false,
		binaryNumberLength
	);
	int gammeEpsilonProduct = gammaValue.DecimalValue() * epsilonValue.DecimalValue();

	// The oxygen value is found by iteratively reducing the list of binary numbers to
	// only contain those for which the setting of a bit at a given position is equal
	// the most common setting of the bit at that position for all numbers remaining in
	// the list, until a single number remains. The carbon value is found similarly,
	// but by iteratively filtering based on the least common setting of bits at given
	// positions.
	// The product of the oxygen and carbon values is the answer to part two.
	Binary::BinaryNumber oxygenValue = Binary::BinaryNumber::IterativeFilterToSingleNumber(
		Binary::BinaryNumber::MostCommonBitSettings,
		binaryInputLines,
		true,
		binaryNumberLength);
	Binary::BinaryNumber carbonValue = Binary::BinaryNumber::IterativeFilterToSingleNumber(
		Binary::BinaryNumber::LeastCommonBitSettings,
		binaryInputLines,
		false,
		binaryNumberLength);
	int oxygenCarbonProduct = oxygenValue.DecimalValue() * carbonValue.DecimalValue();

	return PuzzleAnswerPair{ std::to_string(gammeEpsilonProduct), std::to_string(oxygenCarbonProduct) };
}

