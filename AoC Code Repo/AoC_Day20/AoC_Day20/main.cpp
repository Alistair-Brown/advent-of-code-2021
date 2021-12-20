#include "ImageEnhancement.h"
#include "Parsing.h"
#include <cassert>

int main()
{
	std::cout << "Enter input" << std::endl;

	Image::EnhancementAlgorithm enhancementAlgorithm{ Parsing::ParseLineFromConsole() };
	
	assert(Parsing::ParseLineFromConsole().size() == 0);

	std::deque<std::string> blurryImage{};
	std::string nextLineOfImage;
	while (true)
	{
		nextLineOfImage = Parsing::ParseLineFromConsole();
		if (nextLineOfImage.size() == 0) { break; }
		blurryImage.push_back(nextLineOfImage);
	}

	Image::Image imageBeingEnhanced{ blurryImage, enhancementAlgorithm };

	imageBeingEnhanced.UndergoRoundsOfEnhancement(2);

	std::cout << "After 2 rounds of enhancement, there are " << imageBeingEnhanced.NumberOfLightCells() << " light cells" << std::endl;

	imageBeingEnhanced.UndergoRoundsOfEnhancement(48);
	std::cout << "After 50 rounds of enhancement, there are " << imageBeingEnhanced.NumberOfLightCells() << " light cells" << std::endl;

}