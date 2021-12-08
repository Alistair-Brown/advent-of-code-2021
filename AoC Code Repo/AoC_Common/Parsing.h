#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <array>

namespace Parsing
{
	const std::string comma = ",";
	const std::string space = " ";

	std::string ParseStringFromConsole();
	std::vector<int> ParseLineOfInts(std::string delimiter);
	std::vector<std::vector<std::string>> ParseGroupsOfString(
		std::vector<int> groupSizes,
		std::string intraGroupDelimiter,
		std::string interGroupDelimiter);
	int ParseIntFromConsole();
	int ConvertStringToInt(std::string stringToConvert);
}
