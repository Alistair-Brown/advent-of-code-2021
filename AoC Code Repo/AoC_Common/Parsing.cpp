#include "Parsing.h"

std::string Parsing::ParseStringFromConsole() 
{
	std::string parsedString;
	std::cin >> parsedString;
	return parsedString;
}

std::string Parsing::ParseLineFromConsole()
{
	std::string parsedLine;
	std::getline(std::cin, parsedLine);
	return parsedLine;
}

std::string Parsing::ParseStringFromConsoleIgnoringLineBreaks()
{
	std::string completeString;
	std::string nextString;

	while (true)
	{
		std::getline(std::cin, nextString);
		if (nextString.size() == 0) { break; }
		else { completeString += nextString; }
	}
	return completeString;
}

int Parsing::ParseIntFromConsole()
{
	int parsedInt;
	std::cin >> parsedInt;
	return parsedInt;
}

int Parsing::ConvertStringToInt(std::string stringToConvert)
{
	return std::strtol(stringToConvert.c_str(), nullptr, 10);
}

std::vector<int> Parsing::ParseLineOfInts(std::string delimiter)
{
	std::vector<int> parsedInts;
	int currentInt;
	std::string lineFromConsole;
	std::getline(std::cin, lineFromConsole);

	if (lineFromConsole.length() == 0)
	{
		return parsedInts;
	}

	// Remove any leading whitespace
	while (lineFromConsole.substr(0, 1) == space)
	{
		lineFromConsole.erase(0, 1);
	}

	// Now work along the line parsing ints based on where the next delimiter is, erasing parsed
	// ints and delimites as we go.
	while (true)
	{
		currentInt = strtol(lineFromConsole.substr(0, lineFromConsole.find(delimiter)).c_str(), nullptr, 10);
		parsedInts.push_back(currentInt);

		// Jump forward past the delimiter to get the next int
		lineFromConsole.erase(0, lineFromConsole.find(delimiter) + 1);
		
		// Remove any extra whitespace between the delimiter and our next int.
		while (lineFromConsole.substr(0, 1) == space)
		{
			lineFromConsole.erase(0, 1);
		}

		// If this is the last int, we won't have a final delimiter after it, so just
		// parse it out and stop reading the line.
		if (lineFromConsole.find(delimiter) == std::string::npos)
		{
			currentInt = strtol(lineFromConsole.c_str(), nullptr, 10);
			parsedInts.push_back(currentInt);
			break;
		}
	}

	return parsedInts;
}

std::vector<std::vector<int>> Parsing::ParseMultipleLinesOfInts(std::string delimiter)
{
	std::vector<std::vector<int>> parsedLines;

	while (true)
	{
		std::vector<int> singleLine = ParseLineOfInts(delimiter);
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

// Grid returns has the top left coordinate as its origin, with a negative y axis.
// I've not yet come across a puzzle where we can't just use the returned grid as if
// it has an origin at the bottom left, but that technically means we're inverting
// the input grid
std::vector<std::vector<int>> Parsing::ParseWhitespacelessGridOfDigits()
{
	std::string currentLine;
	std::vector<std::vector<int>> gridOfInts{};
	while (true)
	{
		std::getline(std::cin, currentLine);
		if (currentLine.size() == 0) { break; }

		std::vector<int> gridLine{};

		for (char digit : currentLine)
		{
			gridLine.push_back(digit - '0');
		}
		gridOfInts.push_back(gridLine);
	}
	return gridOfInts;
}

std::vector<std::string> Parsing::ParseSpaceSeparatedString()
{
	std::string inputLine;
	std::string nextString;
	std::vector<std::string> parsedStrings;
	std::getline(std::cin, inputLine);

	for (char character : inputLine)
	{
		if (character == ' ')
		{
			if (nextString.size() > 0)
			{
				parsedStrings.push_back(nextString);
			}
			nextString.clear();
		}
		else
		{
			nextString += character;
		}
	}
	if (nextString.size() > 0)
	{
		parsedStrings.push_back(nextString);
	}

	return parsedStrings;
}

std::vector<std::vector<std::string>> Parsing::ParseGroupsOfString(
	std::vector<int> groupSizes,
	std::string intraGroupDelimiter,
	std::string interGroupDelimiter)
{
	std::string matchString{ "" };

	// Create the match string based on the format of groups and delimiter we expect
	bool firstGroup = true;
	for (int groupSize : groupSizes)
	{
		if (!firstGroup) { matchString.append(interGroupDelimiter); }

		for (int ii = 0; ii < groupSize; ii++)
		{
			if (ii != 0) { matchString.append(intraGroupDelimiter); }
			matchString.append("([0-9a-zA-Z]+)");
		}
		firstGroup = false;
	}

	std::string inputLine;
	std::getline(std::cin, inputLine);
	std::regex regexMatchString{ matchString };
	std::smatch matches;

	std::vector<std::vector<std::string>> parsedStrings;
	if (std::regex_search(inputLine, matches, regexMatchString))
	{
		int nextCaptureGroupToRead = 1;
		for (int groupSize : groupSizes)
		{
			std::vector<std::string> singleParsedGroup;
			for (int ii = 0; ii < groupSize; ii++)
			{
				singleParsedGroup.push_back(matches[nextCaptureGroupToRead]);
				nextCaptureGroupToRead++;
			}
			parsedStrings.push_back(singleParsedGroup);
		}
	}

	return parsedStrings;
}

//
//
// Functions below here read input files rather than command line.
//
//

// Stream the next portion of the input file into a string, and return that string.
// Makes no assumptions about the position of the read pointer, and will leave the
// read pointer at the end of the read string.
std::string Parsing::ReadSingleStringFromInputFile(std::ifstream& inputFile)
{
	std::string stringFromFile{};
	inputFile >> stringFromFile;
	return stringFromFile;
}

// Parse an input file stream into a vector of strings, one per line.
// This function will assume the get pointer for the input file stream
// is already in the correct position, and will not close the file once
// done parsing, that's all up to the caller.
std::vector<std::string> Parsing::SeparateRemainingInputIntoLines(std::ifstream & inputFile)
{
	std::vector<std::string> parsedLines{};
	unsigned int linesParsed = 0;

	while (!inputFile.eof())
	{
		parsedLines.push_back("");
		std::getline(inputFile, parsedLines[linesParsed++]);
	}

	return parsedLines;
}

// Moves the get pointer for an input file stream to the start of the next line.
// Primary use case is to skip blank lines in input files.
void Parsing::SeekNextLine(std::ifstream & inputFile)
{
	std::string dummyString;
	std::getline(inputFile, dummyString);
}

// Parse an input file stream consisting of a grid of digits with no whitespace
// other than the newline between each line of the grid. Return a 2D vector representing
// this grid, where the 0th row of that 2D vector is the first line from the file.
// This function will assume the get pointer for the input file stream
// is already in the correct position, and will not close the file once
// done parsing, that's all up to the caller.
std::vector<std::vector<int>> Parsing::ParseWhitespacelessGridOfDigits(std::ifstream & inputFile)
{
	std::string currentLine;
	std::vector<std::vector<int>> gridOfInts{};
	while (!inputFile.eof())
	{
		inputFile >> currentLine;

		std::vector<int> gridLine{};
		for (char digit : currentLine)
		{
			gridLine.push_back(digit - '0');
		}
		gridOfInts.push_back(gridLine);
	}
	return gridOfInts;
}

// Given string representing a hex number, return a string representing the equivalent
// binary. e.g. "3A2" would return "001110100010".
std::string Parsing::ConvertHexStringToBinary(std::string hexString)
{
	std::string binaryString{};

	for (char character : hexString)
	{
		switch (character)
		{
		case '0':
			binaryString += binaryZero;
			break;
		case '1':
			binaryString += binaryOne;
			break;
		case '2':
			binaryString += binaryTwo;
			break;
		case '3':
			binaryString += binaryThree;
			break;
		case '4':
			binaryString += binaryFour;
			break;
		case '5':
			binaryString += binaryFive;
			break;
		case '6':
			binaryString += binarySix;
			break;
		case '7':
			binaryString += binarySeven;
			break;
		case '8':
			binaryString += binaryEight;
			break;
		case '9':
			binaryString += binaryNine;
			break;
		case 'A':
		case 'a':
			binaryString += binaryA;
			break;
		case 'B':
		case 'b':
			binaryString += binaryB;
			break;
		case 'C':
		case 'c':
			binaryString += binaryC;
			break;
		case 'D':
		case 'd':
			binaryString += binaryD;
			break;
		case 'E':
		case 'e':
			binaryString += binaryE;
			break;
		case 'F':
		case 'f':
			binaryString += binaryF;
			break;
		default:
			assert(false);
			break;
		}
	}

	return binaryString;
}

ULLINT Parsing::ConvertBinaryToInt(std::string binaryString)
{
	ULLINT currentBitValue = pow(2, binaryString.size() - 1);
	ULLINT totalValue{ 0 };

	for (char bit : binaryString)
	{
		if (bit == '1') { totalValue += currentBitValue; }
		currentBitValue /= 2;
	}
	return totalValue;
}
