#include <iostream>
#include <string>
#include "Parsing.h"
#include "Binary.h"
#include <vector>

int main()
{
	std::vector<Binary::BinaryNumber> inputLines;
	std::string singleLine;

	std::cout << "Enter all input followed by end" << std::endl;

	// Get the first line to set the line length, then loop through the remaining lines
	// creating BinaryNumbers as we go to allow us to inspect the bits later.
	std::cin >> singleLine;
	int lineLength = singleLine.length();
	while (true)
	{		
		if (singleLine == "end") { break; }
		inputLines.push_back(Binary::BinaryNumber{ singleLine });
		std::cin >> singleLine;
	}

	// The gamma value is found from the most common setting of each bit, while
	// the epsilon value is the least.
	Binary::BinaryNumber gammaValue = Binary::BinaryNumber::MostCommonBitSettings(
		inputLines,
		1,
		lineLength
	);
	Binary::BinaryNumber epsilonValue = Binary::BinaryNumber::LeastCommonBitSettings(
		inputLines,
		0,
		lineLength
	);
	int gammeEpsilonProduct = gammaValue.DecimalValue() * epsilonValue.DecimalValue();
	std::cout << "Product of gamma and epsilon is " << gammeEpsilonProduct << std::endl;

	Binary::BinaryNumber oxygenValue = Binary::BinaryNumber::FindSingleNumber(
		Binary::BinaryNumber::MostCommonBitSettings,
		inputLines,
		1,
		lineLength);
	Binary::BinaryNumber carbonValue = Binary::BinaryNumber::FindSingleNumber(
		Binary::BinaryNumber::LeastCommonBitSettings,
		inputLines,
		0,
		lineLength);
	std::cout << "Product of oxygen and carbon is " << (oxygenValue.DecimalValue() * carbonValue.DecimalValue()) << std::endl;
}

