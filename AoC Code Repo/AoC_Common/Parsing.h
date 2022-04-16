#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <array>
#include <type_traits>
#include <fstream>

namespace Parsing
{
	std::string ParseStringFromConsole();
	std::string ParseLineFromConsole();
	std::string ParseStringFromConsoleIgnoringLineBreaks();
	std::vector<int> ParseLineOfInts(std::string delimiter);
	std::vector<std::vector<int>> ParseMultipleLinesOfInts(std::string delimiter);
	std::vector<std::vector<int>> ParseWhitespacelessGridOfDigits();
	std::vector<std::string> ParseSpaceSeparatedString();
	std::vector<std::vector<std::string>> ParseGroupsOfString(
		std::vector<int> groupSizes,
		std::string intraGroupDelimiter,
		std::string interGroupDelimiter);
	int ParseIntFromConsole();
	int ConvertStringToInt(std::string stringToConvert);
	std::string ConvertHexStringToBinary(std::string hexString);
	unsigned long long int ConvertBinaryToInt(std::string binaryString);

	const std::string comma = ",";
	const std::string space = " ";

	const std::string binaryZero{ "0000" };
	const std::string binaryOne{ "0001" };
	const std::string binaryTwo{ "0010" };
	const std::string binaryThree{ "0011" };
	const std::string binaryFour{ "0100" };
	const std::string binaryFive{ "0101" };
	const std::string binarySix{ "0110" };
	const std::string binarySeven{ "0111" };
	const std::string binaryEight{ "1000" };
	const std::string binaryNine{ "1001" };
	const std::string binaryA{ "1010" };
	const std::string binaryB{ "1011" };
	const std::string binaryC{ "1100" };
	const std::string binaryD{ "1101" };
	const std::string binaryE{ "1110" };
	const std::string binaryF{ "1111" };

	// Most of the parsing functions in this header only expect to parse input files
	// into ints and strings. This function allows that to be statically asserted.
	template <typename T>
	constexpr bool ValidParsingType()
	{
		bool isValid = (
			std::is_same<int, T>::value ||
			std::is_same<std::string, T>::value);
		return isValid;
	}

	// Parses an input file of values separated by whitespace into a vector of
	// the desired type (only int and string currently supported by this template).
	// This function will begin parsing from the current location of the get pointer,
	// and will not reset the get pointer when parsing is complete.
	template<typename T>
	std::vector<T> SeparateOnWhitespace(std::ifstream &inputFile)
	{
		static_assert(ValidParsingType<T>());

		std::vector<T> parsedValues{};
		T singleValue;
		while (!inputFile.eof())
		{
			inputFile >> singleValue;
			parsedValues.push_back(singleValue);
		}

		return parsedValues;
	}
}
