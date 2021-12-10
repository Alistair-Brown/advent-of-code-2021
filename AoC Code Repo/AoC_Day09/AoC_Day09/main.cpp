#include "Parsing.h"
#include "Heightmap.h"

int main()
{
	std::cout << "Enter your grid" << std::endl;
	std::vector<std::vector<int>> gridOfDigits = Parsing::ParseWhitespacelessGridOfDigits();

	HeightMap::HeightMap heightMap{ gridOfDigits };
	std::cout << "Sum of risk levels is " << heightMap.SumRiskLevels() << std::endl;
	std::cout << "Product of basins is " << heightMap.ProductOfLargestBasins() << std::endl;
}