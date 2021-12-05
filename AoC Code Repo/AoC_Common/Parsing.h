#pragma once
#include <iostream>
#include <string>
#include <vector>

namespace Parsing
{
	const std::string comma = ",";
	const std::string space = " ";

	std::string ParseStringFromConsole();
	std::vector<int> ParseLineOfInts(std::string delimiter);
	int ParseIntFromConsole();
	int ConvertStringToInt(std::string stringToConvert);
}
