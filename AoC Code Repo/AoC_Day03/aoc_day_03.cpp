#include <iostream>
#include <string>
#include "Parsing.h"
#include "Binary.h"
#include <vector>
#include "puzzle_solvers.h"
#include "Parsing.h"

PuzzleAnswerPair PuzzleSolvers::AocDayThreeSolver(std::ifstream &puzzleInputFile)
{
	std::vector<std::string> rawInputLines = Parsing::SeparateInputIntoLines(puzzleInputFile);
	unsigned int lineLength = rawInputLines[0].size();
	std::vector<Binary::BinaryNumber> binaryInputLines{};
	for (std::string singleLine : rawInputLines)
	{
		binaryInputLines.emplace_back(singleLine);
	}

	// The gamma value is found from the most common setting of each bit, while
	// the epsilon value is the least.
	Binary::BinaryNumber gammaValue = Binary::BinaryNumber::MostCommonBitSettings(
		binaryInputLines,
		1,
		lineLength
	);
	Binary::BinaryNumber epsilonValue = Binary::BinaryNumber::LeastCommonBitSettings(
		binaryInputLines,
		0,
		lineLength
	);
	int gammeEpsilonProduct = gammaValue.DecimalValue() * epsilonValue.DecimalValue();

	Binary::BinaryNumber oxygenValue = Binary::BinaryNumber::FindSingleNumber(
		Binary::BinaryNumber::MostCommonBitSettings,
		binaryInputLines,
		1,
		lineLength);
	Binary::BinaryNumber carbonValue = Binary::BinaryNumber::FindSingleNumber(
		Binary::BinaryNumber::LeastCommonBitSettings,
		binaryInputLines,
		0,
		lineLength);
	int oxygenCarbonProduct = oxygenValue.DecimalValue() * carbonValue.DecimalValue();

	return PuzzleAnswerPair{ std::to_string(gammeEpsilonProduct), std::to_string(oxygenCarbonProduct) };
}

