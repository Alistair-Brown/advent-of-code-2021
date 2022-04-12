#include "input_retrieval.h"
#include <assert.h>
#include <iostream>

// Returns an input file stream to the file containing the puzzle input for
// a given day. It is the responsibility of the caller to close the filestream
// when finished with it.
std::ifstream InputRetrieval::GetOutputStreamForPuzzleInput(unsigned int day)
{
	// All possible day numbers for advent code are either 1 or 2 digits e.g. 6 or 21.
	// Single digit day numbers are all prepended with a leading 0 to keep them ordered
	// nicely in the folder, so account for that here.
	std::string dayString = std::to_string(day);
	assert(dayString.length() <= 2);
	if (dayString.length() == 1) { dayString.insert(0, 1, '0'); }

	// Now that we've constructed the day portion of the filename, we can put the
	// full path to the input together.
	std::string fullRelativePathToInput{
		std::string{relativePathToInputDir} +
		std::string{puzzleInputFileStem} +
		dayString +
		puzzleInputFileExtension
	};
	std::ifstream puzzleInputFile{ fullRelativePathToInput };
	assert(puzzleInputFile);

	return puzzleInputFile;
}
