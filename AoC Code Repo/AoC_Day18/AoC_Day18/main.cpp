#include "Snailfish.h"
#include <iostream>

int main()
{
	std::cout << "Enter snailfish number" << std::endl;

	std::string nextNumAsString;
	std::getline(std::cin, nextNumAsString);

	std::vector<std::string> snailNumbersAsString;
	snailNumbersAsString.push_back(nextNumAsString);
	Snail::SnailfishNumber *currentSnailNum = new Snail::SnailfishNumber(Snail::SplitStringIntoSnailfishPair(nextNumAsString));
	Snail::SnailfishNumber *nextSnailNum;

	while (true)
	{
		std::getline(std::cin, nextNumAsString);
		if (nextNumAsString.size() == 0) { break; }
		snailNumbersAsString.push_back(nextNumAsString);
		nextSnailNum = new Snail::SnailfishNumber(Snail::SplitStringIntoSnailfishPair(nextNumAsString));
		currentSnailNum = Snail::AddSnailfishNumbers(currentSnailNum, nextSnailNum);
	}

	std::cout << currentSnailNum->Magnitude() << std::endl;

	Snail::SnailfishNumber *numberOne;
	Snail::SnailfishNumber *numberTwo;
	Snail::SnailfishNumber *sumOfJustTwo;
	int maxMagnitude = 0;
	for (int ii = 0; ii < snailNumbersAsString.size(); ii++)
	{
		for (int jj = ii + 1; jj < snailNumbersAsString.size(); jj++)
		{
			numberOne = new Snail::SnailfishNumber(Snail::SplitStringIntoSnailfishPair(snailNumbersAsString[ii]));
			numberTwo = new Snail::SnailfishNumber(Snail::SplitStringIntoSnailfishPair(snailNumbersAsString[jj]));
			sumOfJustTwo = Snail::AddSnailfishNumbers(numberOne, numberTwo);
			int magnitude = sumOfJustTwo->Magnitude();
			if (magnitude > maxMagnitude) { maxMagnitude = magnitude; }
			delete sumOfJustTwo;
		}
	}

	std::cout << "Max magnitude : " << maxMagnitude << std::endl;
}