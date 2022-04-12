#pragma once
#include <fstream>
#include <filesystem>
#include <string_view>

namespace InputRetrieval
{
	// Puzzle inputs should be stored within the Puzzle_Inputs directory, with
	// a consistent filename in which the day number is appended with a leading
	// zero where it would otherwise be a single digit, giving a full path like:
	// "..\Puzzle_Inputs\Puzzle_Input_Day_04.txt".
	constexpr const char relativePathToInputDir[] = R"(..\Puzzle_Inputs\)";
	constexpr const char puzzleInputFileStem[] = R"(Puzzle_Input_Day_)";
	constexpr const char puzzleInputFileExtension[] = R"(.txt)";

	// Returns an input file stream to the file containing the puzzle input for
	// a given day. It is the responsibility of the caller to close the filestream
	// when finished with it.
	std::ifstream GetOutputStreamForPuzzleInput(unsigned int day);
}