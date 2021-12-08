#include "Parsing.h"
#include "SevenSegment.h"

int main()
{
	std::cout << "Enter your input" << std::endl;

	int totalHits{ 0 };
	int totalRightHandValue{ 0 };
	while (true)
	{
		std::vector<std::vector<std::string>> parsedStrings =
			Parsing::ParseGroupsOfString(std::vector<int> {10, 4}, " ", " \\| ");
		if (parsedStrings.size() != 0) 
		{
			for (std::string element : parsedStrings[1])
			{
				if ((element.size() == 2) ||
					(element.size() == 3) ||
					(element.size() == 4) ||
					(element.size() == 7))
				{
					totalHits++;
				}
			}

			SevenSeg::SevenSegDisplay currentDisplay{ parsedStrings[0] };
			int incrementBy = currentDisplay.GetValueOfSegments(parsedStrings[1]);
			std::cout << "Inrementing total by " << incrementBy << std::endl;
			totalRightHandValue += incrementBy;
		}
		else
		{
			break;
		}
	}

	std::cout << "Total hits of 1,4,7,8 was " << totalHits << std::endl;

	std::cout << "Total RH value was " << totalRightHandValue << std::endl;
}