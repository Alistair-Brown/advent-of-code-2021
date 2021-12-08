#include "Parsing.h"
#include <regex>
#include <cassert>

std::string Parsing::ParseStringFromConsole() 
{
	std::string parsedString;
	std::cin >> parsedString;
	return parsedString;
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

std::vector<std::vector<std::string>> Parsing::ParseGroupsOfString(std::vector<int> groupSizes, std::string intraGroupDelimiter, std::string interGroupDelimiter)
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
