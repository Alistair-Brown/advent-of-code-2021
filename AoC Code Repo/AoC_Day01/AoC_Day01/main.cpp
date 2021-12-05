#include <iostream>

int main()
{
	int totalGreaterThans{ 0 };
	int previousNumber;
	int currentNumber;

	std::cout << "Enter a number on each line" << std::endl;

	std::cin >> previousNumber;

	while (true)
	{
		std::cin >> currentNumber;

		if (currentNumber == 0)
		{
			break;
		}

		if (currentNumber > previousNumber)
		{
			totalGreaterThans++;
		}
		previousNumber = currentNumber;
	}

	std::cout << "Total is " << totalGreaterThans << std::endl;

	// Now part 2

	int numberOne;
	int numberTwo;
	int numberThree;
	int temp;
	totalGreaterThans = 0;

	std::cout << "Enter the data again" << std::endl;

	std::cin >> numberOne;
	std::cin >> numberTwo;
	std::cin >> numberThree;
	previousNumber = numberOne + numberTwo + numberThree;

	while (true)
	{
		std::cin >> temp;

		if (temp == 0)
		{
			break;
		}

		currentNumber = numberTwo + numberThree + temp;

		if (currentNumber > previousNumber)
		{
			totalGreaterThans++;
		}
		previousNumber = currentNumber;
		numberOne = numberTwo;
		numberTwo = numberThree;
		numberThree = temp;
	}

	std::cout << "Total is " << totalGreaterThans << std::endl;

	return 0;
}