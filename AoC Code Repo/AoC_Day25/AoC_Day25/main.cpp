#include "Cucumber.h"
#include "Parsing.h"

int main()
{
	std::cout << "Enter initial sea cucumber grid" << std::endl;

	std::vector<std::string> inputGrid;
	std::string inputLine;
	while (true)
	{
		inputLine = Parsing::ParseLineFromConsole();
		if (inputLine.size() == 0) { break; }
		inputGrid.push_back(inputLine);
	}

	Cucumber::Trench cucumberTrench{ inputGrid };

	unsigned int stepsToStop = cucumberTrench.MoveUntilAllBlocked();

	std::cout << "It takes " << stepsToStop << " steps before the cucumbers stop moving" << std::endl;
}