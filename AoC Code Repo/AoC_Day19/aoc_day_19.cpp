#include "Parsing.h"
#include <regex>
#include <cassert>
#include "puzzle_solvers.h"
#include "Beacon.h"
#include <vector>

// For day 19, we have a number of scanners and beacons floating in the ocean. Each scanner
// can detect the beacons within 1000 metres of themselves in each direction, but do not know
// their own location or orientation. We have as our input the positions of the beacons each
// scanner can detect, relative to that scanner's position and orientation. If 2 scanners can be
// shown to both be detecting 12 common beacons, that is considered enough proof that those 12 are
// indeed the same beacons being detected by each scanner, and we can combine the reports from
// each of those scanners to put together a larger portion of the total 3D space. By finding
// enough of these overlaps we can put together the whole 3D space these scanners and beacons
// exist within.
// For part one we must then report the total number of *beacons* in existence.
// For part two we must find the greatest 'Manhattan distance' between any two *scanners*.
PuzzleAnswerPair PuzzleSolvers::AocDayNineteenSolver(std::ifstream& puzzleInputFile)
{
	// The input is given as a scanner number, followed by a number of lines giving the
	// coordinates of beacons that scanner can detect, in the form "x,y,z". There is a
	// blank line separating each scanner's portion of the input.
	// From this we put together our list of scanners.
	std::vector<std::shared_ptr<Beacons::Scanner>> scanners;
	std::regex scannerHeaderRegex{ "--- scanner [0-9]+ ---" };
	unsigned int scannerIndex{ 0 };
	while (!puzzleInputFile.eof())
	{
		std::string scannerHeader{};
		std::getline(puzzleInputFile, scannerHeader);
		assert(std::regex_match(scannerHeader, scannerHeaderRegex));

		std::vector<std::vector<int>> beaconPositions = Parsing::ParseMultipleLinesWithDelimiters<int>(
			puzzleInputFile, Parsing::comma);
		scanners.push_back(std::make_shared<Beacons::Scanner>(scannerIndex, beaconPositions));
		scannerIndex++;
	}

	// We want to find the overlapping scanners without wasting work, but without missing
	// scanners either. We do this by constructing a 'tree' of scanner overlaps:
	//  - We first look for any scanners that overlap with scanner 0
	//  - Each of those scanners are now consider routed *to*, and are added to the
	//    list of scanners to route *from*.
	//  - We then repeat this process with each of the scanners we now have set up to
	//    route from, but not checking for overlaps with any scanners that have already
	//    been routed to by a previous successful overlap.
	//  - If we do this until all scanners have been routed to (scanner 0 is considered
	//    routed to by default, being our starting point), we will have a tree containing
	//    every scanner exactly once, with nodes joined by known overlaps between scanners.
	std::vector<bool> scannersRoutedTo(scanners.size(), false);
	scannersRoutedTo[0] = true;

	std::list<unsigned int> nextScannersToRouteFrom;
	nextScannersToRouteFrom.push_back(0);	

	unsigned int numScannersRoutedTo = 1;
	while (numScannersRoutedTo < scanners.size())
	{
		unsigned int scannerToRouteFrom = nextScannersToRouteFrom.front();
		nextScannersToRouteFrom.pop_front();

		for (unsigned int otherScanner = 0; otherScanner < scanners.size(); otherScanner++)
		{
			if (scannersRoutedTo[otherScanner]) { continue; }

			if (scanners[scannerToRouteFrom]->CheckForOverlap(scanners[otherScanner]))
			{
				scannersRoutedTo[otherScanner] = true;
				numScannersRoutedTo++;
				nextScannersToRouteFrom.push_back(otherScanner);
			}
		}
	}

	// From it's position at the root of the tree, scanner 0 can discover the location,
	// relative to itself, of all other scanners and beacons. We can use this to ask
	// it the total number of beacons, and the maximum 'manhattan distance' between
	// any two scanners.
	unsigned int numberOfScanners = scanners[0]->NumberOfBeacons();
	unsigned int maxManhattanDistance = scanners[0]->MaxScannerManhattanDistance();

	return PuzzleAnswerPair{ std::to_string(numberOfScanners), std::to_string(maxManhattanDistance)};
}