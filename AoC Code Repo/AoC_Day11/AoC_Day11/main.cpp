#include "Parsing.h"
#include "Dumbo_Octopus.h"

int main()
{
	std::cout << "Enter the octopus grid" << std::endl;
	
	Dumbo::OctopusGrid octupusEnergyGrid{ Parsing::ParseWhitespacelessGridOfDigits() };
	octupusEnergyGrid.AdvanceSteps(100);

	std::cout << "Total flashes was " << octupusEnergyGrid.NumberOfFlashesOccured() << std::endl;
	std::cout << "First simultaneous flash at " << octupusEnergyGrid.AdvanceUntilFirstSimultaneousFlash() << std::endl;
}