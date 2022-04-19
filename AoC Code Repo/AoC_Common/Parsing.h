#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <array>
#include <type_traits>
#include <fstream>
#include <string_view>
#include <sstream>
#include <cassert>
#include <regex>
#include <deque>

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

	std::vector<std::string> SeparateInputIntoLines(std::ifstream &inputFile);

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
	template <typename T>
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

	// This overload of AssignMatchesToElements handles the final case of this
	// variadic template, with a single input string and single variable reference
	// left to stream it into.
	template <typename T>
	void StreamStringsToVars(std::deque<std::string> inputStrings,
		T &finalVar)
	{
		static_assert(ValidParsingType<T>());

		// All overloads of this function must be called with an equal number of input
		// strings and variable references to parse into. Since the other overload recurses
		// into this one, we can enforce that here by making sure we only have one
		// input string remaining to stream into this final variable.
		assert(inputStrings.size() == 1);

		std::istringstream streamToInsert{ inputStrings[0] };
		while (!streamToInsert.eof())
		{
			streamToInsert >> finalVar;

			// If we're trying to extract an integer value, it shouldn't take multiple
			// extraction operations to retrieve the value from the stringstream.
			if (std::is_integral<T>::value) { assert(streamToInsert.eof()); }
		}
	}

	// Given an arbitrary number of strings (as a vector), and an equal number of variable
	// references, stream the value of each string into the corresponding variable.
	// Currently the only types that this function will support streaming into
	// are int and string.
	template <typename T, typename... Args>
	void StreamStringsToVars(std::deque<std::string> inputStrings,
		T &firstVariable,
		Args & ... remainingVariables)
	{
		static_assert(ValidParsingType<T>());

		std::istringstream streamToInsert{ inputStrings[0] };
		while (!streamToInsert.eof())
		{
			streamToInsert >> firstVariable;

			// If we're trying to extract an integer value, it shouldn't take multiple
			// extraction operations to retrieve the value from the stringstream.
			if (std::is_integral<T>::value) { assert(streamToInsert.eof()); }
		}

		// To continue streaming the remaining strings into the remaining variables,
		// we just remove the front element from the string deque, and recurse with
		// the remaining variables.
		inputStrings.pop_front();
		StreamStringsToVars(inputStrings, remainingVariables...);
	}

	// Given a string and a regex pattern of matches, extract the match groups
	// into an arbitrary number of different variables, provided to this function
	// by reference. Currently the only supported types that we will parse the
	// matches into are int and string.
	// The number of match groups in the regular expression must be equal to the
	// number of arguments passed for parsing into, and the regular expression must
	// match the input string.
	template <typename... Args>
	void ParseStringIntoElements(
		std::string lineToParse,
		std::regex regexMatchString,
		Args & ... elemsToParseInto)
	{
		std::smatch matches;

		std::regex_search(lineToParse, matches, regexMatchString);
		assert((matches.size() - 1) == sizeof...(elemsToParseInto));

		// Note that the first match in an smatch is the complete matched string,
		// which we're not interested in, we just want the captured groups.
		std::deque<std::string> matchesAsStrings{};
		for (unsigned int ii = 1; ii < matches.size(); ii++)
		{
			matchesAsStrings.push_back(matches[ii]);
		}

		// With the input string correctly split into the desired portions,
		// assign those values to our variable references.
		StreamStringsToVars(matchesAsStrings, elemsToParseInto...);
	}
}
