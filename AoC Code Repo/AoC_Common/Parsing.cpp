#include "Parsing.h"

// Stream the next portion of the input file into a string, and return that string.
// Makes no assumptions about the position of the read pointer, and will leave the
// read pointer at the end of the read string.
std::string Parsing::ReadSingleStringFromInputFile(std::ifstream& inputFile)
{
	std::string stringFromFile{};
	inputFile >> stringFromFile;
	return stringFromFile;
}

// Stream the next line of the input file into a string, and return that string.
// Makes no assumptions about the position of the read pointer, and will leave the
// read pointer at the start of the next line.
std::string Parsing::ReadWholeLineFromInputFile(std::ifstream& inputFile)
{
	std::string stringFromFile{};
	std::getline(inputFile, stringFromFile);
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
	ULLINT currentBitValue = 1;
	for (unsigned int ii = 1; ii < binaryString.size(); ii++) { currentBitValue *= 2; }

	ULLINT totalValue{ 0 };

	for (char bit : binaryString)
	{
		if (bit == '1') { totalValue += currentBitValue; }
		currentBitValue /= 2;
	}
	return totalValue;
}

int Parsing::ConvertStringToInt(std::string stringToConvert)
{
	return std::strtol(stringToConvert.c_str(), nullptr, 10);
}
