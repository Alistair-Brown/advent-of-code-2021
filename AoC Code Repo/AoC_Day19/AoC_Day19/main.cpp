#include "BeaconScanner.h"
#include "Parsing.h"
#include <iostream>
#include <regex>

int main()
{
	std::vector<Beacon::Scanner> normalizedScanners{};
	std::vector<Beacon::Scanner> unnormalizedScanners{};
	std::regex headerPattern{ "--- scanner [0-9]+ ---" };
	std::string inputLine;
	
	std::cout << "Enter input" << std::endl;
	
	while (true)
	{
		std::getline(std::cin, inputLine);
		if (!std::regex_match(inputLine, headerPattern)) { break; }
		std::vector<std::vector<int>> beaconPositions = Parsing::ParseMultipleLinesOfInts(Parsing::comma);

		if (normalizedScanners.size() == 0)
		{
			normalizedScanners.push_back(Beacon::Scanner(beaconPositions));
			normalizedScanners[0].RotationallyRelativizeBeaconsInRange(0);
			normalizedScanners[0].FullyRelativizeBeaconsInRange();
			normalizedScanners[0].AddBeaconsToTotal(normalizedScanners[0].FullyNormalizedBeaconsInRange());
		}
		else
		{
			unnormalizedScanners.push_back(Beacon::Scanner(beaconPositions));
		}
	}

	int normalizedIterator = 0;
	Beacon::Position relativePos;
	while (unnormalizedScanners.size() > 0)
	{
		std::vector<int> unnormalizedElementsToErase;
		for (int ii = 0; ii < unnormalizedScanners.size(); ii++)
		{
			int rotationIndex = normalizedScanners[normalizedIterator].CheckForOverlap(unnormalizedScanners[ii], relativePos);
			if (rotationIndex >= 0)
			{
				Beacon::Scanner overlappingScanner = unnormalizedScanners[ii];
				overlappingScanner.SetRelativeScannerPosition(
					relativePos + normalizedScanners[normalizedIterator].GetRelativeScannerPosition());
				overlappingScanner.RotationallyRelativizeBeaconsInRange(rotationIndex);
				overlappingScanner.FullyRelativizeBeaconsInRange();
				normalizedScanners.push_back(overlappingScanner);
				normalizedScanners[0].AddBeaconsToTotal(overlappingScanner.FullyNormalizedBeaconsInRange());
				unnormalizedElementsToErase.push_back(ii);
			}
		}
		// Remove any unnormalized scanners we've now been able to match, remembering that
		// elements are being deleted as we go when selecting the index to delete.
		for (int ii = 0; ii < unnormalizedElementsToErase.size(); ii++)
		{
			unnormalizedScanners.erase(unnormalizedScanners.begin() + (unnormalizedElementsToErase[ii] - ii));
		}
		normalizedIterator++;
	}

	std::cout << "Total number of beacons is " << normalizedScanners[0].NumberOfTotalBeacons() << std::endl;
}