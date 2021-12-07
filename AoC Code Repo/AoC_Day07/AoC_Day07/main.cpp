#include "Parsing.h"
#include "CrabSubmarines.h"
#include <iostream>

int main()
{
	std::cout << "Enter list of crabs" << std::endl;
	std::vector<int> crabPositionList = Parsing::ParseLineOfInts(Parsing::comma);

	CrabSubmarines::CrabArmada crabArmada;
	for (int crabPosition : crabPositionList)
	{
		crabArmada.AddCrabSumbarine(crabPosition);
	}

	std::cout << "Movement to converge is " << crabArmada.MinimalConvergence() << std::endl;
	std::cout << "Complex cost to converge is " << crabArmada.MoreComplicatedMinimumConvergence() << std::endl;
}