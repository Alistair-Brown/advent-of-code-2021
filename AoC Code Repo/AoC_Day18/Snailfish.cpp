#include "Snailfish.h"
#include <cassert>

std::pair<std::string, std::string> Snail::SplitStringIntoSnailfishPair(std::string completeSnailfishNumber)
{
	std::string leftPortion;
	std::string rightPortion;
	int depth{ 0 };

	assert((completeSnailfishNumber[0] == '[') &&
		(completeSnailfishNumber[completeSnailfishNumber.size() - 1] == ']'));

	// Remove leading '['
	completeSnailfishNumber.erase(0, 1);

	// Find middle comma to split the number around
	bool firstHalf{ true };
	for (char character : completeSnailfishNumber)
	{
		if (firstHalf)
		{
			if ((character == ',') && (depth == 0))
			{
				firstHalf = false;
				continue;
			}

			if (character == '[')
			{
				depth++;
			}
			else if (character == ']')
			{
				depth--;
				assert(depth >= 0);
			}

			leftPortion += character;
		}
		else
		{
			if ((character == ']') && (depth == 0))
			{
				break;
			}

			if (character == '[')
			{
				depth++;
			}
			else if (character == ']')
			{
				depth--;
				assert(depth >= 0);
			}

			rightPortion += character;
		}
	}

	return { leftPortion, rightPortion };
}

Snail::SnailfishNumber *Snail::AddSnailfishNumbers(SnailfishNumber *numberOne, SnailfishNumber *numberTwo)
{
	Snail::SnailfishNumber *newSnailfishNumber = new SnailfishNumber({ (Number *)numberOne, (Number *)numberTwo });

	while (true)
	{
		Number *dummyNumPtr = nullptr;
		std::pair<int, int> dummyPair{ 0,0 };
		if (newSnailfishNumber->MaybeExplode(1, dummyPair)) { continue; }
		else if (newSnailfishNumber->MaybeSplit(dummyNumPtr)) { continue; }
		else { break; }
	}
	return newSnailfishNumber;
}

Snail::SnailfishNumber::SnailfishNumber(std::pair<std::string, std::string> numberPairIn)
{
	if (numberPairIn.first.size() == 1)
	{
		numberPair.first = (Number *) new RegularNumber(Parsing::ConvertStringToInt(numberPairIn.first));
	}
	else
	{
		numberPair.first = (Number *) new SnailfishNumber(SplitStringIntoSnailfishPair(numberPairIn.first));
	}

	if (numberPairIn.second.size() == 1)
	{
		numberPair.second = (Number *) new RegularNumber(Parsing::ConvertStringToInt(numberPairIn.second));
	}
	else
	{
		numberPair.second = (Number *) new SnailfishNumber(SplitStringIntoSnailfishPair(numberPairIn.second));
	}
}

Snail::SnailfishNumber::~SnailfishNumber()
{
	delete numberPair.first;
	delete numberPair.second;
}

int Snail::SnailfishNumber::Magnitude()
{
	return ((3 * numberPair.first->Magnitude()) + (2 * numberPair.second->Magnitude()));
}

std::string Snail::SnailfishNumber::ValueAsString()
{
	std::string valueAsString{ "[" };
	valueAsString += numberPair.first->ValueAsString();
	valueAsString += ",";
	valueAsString += numberPair.second->ValueAsString();
	valueAsString += "]";
	return valueAsString;
}

bool Snail::SnailfishNumber::MaybeExplode(int nestDepth, std::pair<int, int>& pairToExplode)
{
	bool exploded{ false };
	if (nestDepth == 5)
	{
		// The numberPair will consist of regular numbers or it would have already
		// exploded.
		pairToExplode = { numberPair.first->Magnitude(), numberPair.second->Magnitude() };
		exploded = true;
	}
	else if (numberPair.first->MaybeExplode(nestDepth + 1, pairToExplode))
	{
		if (nestDepth == 4)
		{
			delete numberPair.first;
			numberPair.first = (Number *) new RegularNumber(0);
		}
		numberPair.second->IncreaseLeftValue(pairToExplode.second);
		exploded = true;
		pairToExplode.second = 0;
	}
	else if (numberPair.second->MaybeExplode(nestDepth + 1, pairToExplode))
	{
		if (nestDepth == 4)
		{
			delete numberPair.second;
			numberPair.second = (Number *) new RegularNumber(0);
		}
		numberPair.first->IncreaseRightValue(pairToExplode.first);
		exploded = true;
		pairToExplode.first = 0;
	}
	return exploded;
}

bool Snail::SnailfishNumber::MaybeSplit(Number *& newNumber)
{
	bool split{ false };

	// A 'true' to maybe split will have a pointer to the new snailNumber if
	// the split was in an immediate child, or nullptr if the split occured lower
	// down the tree and we're now just backing up.
	if (numberPair.first->MaybeSplit(newNumber))
	{
		split = true;
		if (newNumber != nullptr)
		{
			delete numberPair.first;
			numberPair.first = newNumber;
			newNumber = nullptr;
		}
	}
	else if (numberPair.second->MaybeSplit(newNumber))
	{
		split = true;
		if (newNumber != nullptr)
		{
			delete numberPair.second;
			numberPair.second = newNumber;
			newNumber = nullptr;
		}
	}

	return split;
}

Snail::RegularNumber::~RegularNumber()
{
	std::cout << "Hit regular destructor" << std::endl;
}

bool Snail::RegularNumber::MaybeExplode(int depth, std::pair<int, int>& pairToExplode)
{
	// Regular numbers don't explode
	return false;
}

bool Snail::RegularNumber::MaybeSplit(Number *& newNumber)
{
	assert(newNumber == nullptr);
	bool split{ false };
	if (value > 9)
	{
		split = true;
		int oddComponent = value % 2;
		Number *leftNumber = (Number *)new RegularNumber(value / 2);
		Number *rightNumber = (Number *)new RegularNumber((value / 2) + oddComponent);
		newNumber = (Number *)new SnailfishNumber({ leftNumber, rightNumber });
	}
	return split;
}
