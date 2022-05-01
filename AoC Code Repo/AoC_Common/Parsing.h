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
	void SeekNextLine(std::ifstream &inputFile);

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

	// Return a value of any type (with heavy restrictions, read on) with a value
	// set from a string (e.g. for a string type, the value is just the same
	// as the input, but for an int type we get the value using stoi).
	// 
	// We need to be quite careful with what types we will attempt to deduce a
	// value for from a string, so this template is currently explicitly specialized
	// for only int and string as types for which it makes sense to assign to value
	// to from a string and for which we've written code to do so correctly.
	template <typename T>
	T SetValueFromString(const std::string &stringIn)
	{
		static_assert(false);
	}
	template <>
	inline std::string SetValueFromString(const std::string &stringIn)
	{
		return std::string{ stringIn };
	}
	template <>
	inline int SetValueFromString(const std::string &stringIn)
	{
		// If the input string is one which could be validly converted into an int,
		// stoi will use it in its entirety, which we can verify by checking the value
		// of pos once stoi is finished parsing the string into an int.
		unsigned int pos;
		int returnValue = std::stoi(stringIn, &pos);
		assert(pos == stringIn.size());

		return std::stoi(stringIn);
	}

	// This overload of AssignMatchesToElements handles the final case of this
	// variadic template, with a single input string and single variable reference
	// left to assign a value to.
	template <typename T>
	void AssignStringsToVars(std::deque<std::string> inputStrings,
		T &finalVar)
	{
		// All overloads of this function must be called with an equal number of input
		// strings and variable references to parse into. Since the other overload recurses
		// into this one, we can enforce that here by making sure we only have one
		// input string remaining to assign as the value of this final variable.
		assert(inputStrings.size() == 1);

		finalVar = SetValueFromString<T>(inputStrings[0]);
	}

	// Given an arbitrary number of strings (as a vector), and an equal number of variable
	// references, assign the value of each variable according to the corresponding string.
	// Currently the only types that this function will support assigning values to
	// are int and string (most types can't have their value deduced from a string,
	// so we need to be careful to only perform this operation on types for which
	// it makes sense).
	template <typename T, typename... Args>
	void AssignStringsToVars(std::deque<std::string> inputStrings,
		T &firstVariable,
		Args & ... remainingVariables)
	{
		firstVariable = SetValueFromString<T>(inputStrings[0]);
		inputStrings.pop_front();
		AssignStringsToVars(inputStrings, remainingVariables...);
	}

	// TODO: While this function is a pretty flexible way of handling arbitrary
	// parsing requirements, the use of a regex is pretty slow, and so it ends
	// up significantly impacting the overall computation time for a given puzzle.
	// (order of a couple of seconds). If I want my output to be really lightning fast
	// for all puzzles, I'll need to switch this out for something less regex-y.
	//
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
		AssignStringsToVars(matchesAsStrings, elemsToParseInto...);
	}

	// Parses a line of the input file consisting of values separated by a known delimiter,
	// into a vector of the desired type (only int and string currently supported by this template).
	// The line to be parsed will be taken from the current position of the get pointer,
	// and the get pointer will be left pointing at the subsequent line.
	template <typename T>
	std::vector<T> ParseLineOnDelimiters(std::ifstream &inputFile, std::string delimiter)
	{
		static_assert(ValidParsingType<T>());

		std::vector<T> parsedValues{};
		T singleValue;

		std::string inputLine;
		std::getline(inputFile, inputLine);

		// An empty input line will result in an empty vector being returned.
		if (inputLine.length() == 0)
		{
			return parsedValues;
		}

		// Remove any leading whitespace
		while (inputLine.substr(0, 1) == space)
		{
			inputLine.erase(0, 1);
		}

		// Now work along the line parsing value based on where the next delimiter is, erasing parsed
		// values and delimiters as we go.
		while (true)
		{
			singleValue = SetValueFromString<T>(inputLine.substr(0, inputLine.find(delimiter)));
			parsedValues.push_back(singleValue);

			// Jump forward past the delimiter to get the next value, also removing any extra
			// whitespace following the delimiter.
			inputLine.erase(0, inputLine.find(delimiter) + delimiter.size());
			while (inputLine.substr(0, 1) == space)
			{
				inputLine.erase(0, 1);
			}

			// If this is the last value, we won't have a final delimiter after it. So if there
			// are no delimiters remaining we just parse out the rest of the line.
			if (inputLine.find(delimiter) == std::string::npos)
			{
				singleValue = SetValueFromString<T>(inputLine);
				parsedValues.push_back(singleValue);
				break;
			}
		}

		return parsedValues;
	}

	// Parse multiple lines of an input file where each line consists of a series
	// of values separated by a known delimiter. Values will be parsed into a 2D vector
	// where each input line forms a single vector of values of the desired type (only int
	// and string currently supported by this template), within the 'outer' vector.
	// Parsing will continue until we reach a blank line or the end of the file.
	// Parsing will begin from the current position of the get pointer,
	// and the get pointer will be left pointing at either the end of the file, or the
	// line after the blank line in the case that a blank line is encountered.
	template <typename T>
	std::vector<std::vector<T>> ParseMultipleLinesWithDelimiters(
		std::ifstream &inputFile,
		std::string delimiter)
	{
		std::vector<std::vector<T>> parsedLines{};

		while (!inputFile.eof())
		{
			std::vector<T> singleLine = ParseLineOnDelimiters<T>(inputFile, delimiter);
			if (singleLine.size() > 0)
			{
				parsedLines.push_back(singleLine);
			}
			else
			{
				break;
			}
		}
		return parsedLines;
	}
}
