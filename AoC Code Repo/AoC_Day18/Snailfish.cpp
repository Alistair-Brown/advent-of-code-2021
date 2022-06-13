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

std::unique_ptr<Snail::SnailfishNumber> Snail::AddSnailfishNumbers(
	std::unique_ptr<Snail::SnailfishNumber> numberOne,
	std::unique_ptr<Snail::SnailfishNumber> numberTwo)
{
	std::unique_ptr<SnailfishNumber> newSnailfishNumber = 
		std::make_unique<SnailfishNumber>( 
			std::pair<std::unique_ptr<SnailfishNumber>, std::unique_ptr<SnailfishNumber>>{
		std::move(numberOne), std::move(numberTwo) });

	while (true)
	{
		std::unique_ptr<Number> dummyNumPtr{};
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
		numberPair.first = std::make_unique<RegularNumber>(Parsing::ConvertStringToInt(numberPairIn.first));
	}
	else
	{
		numberPair.first = std::make_unique<SnailfishNumber>(SplitStringIntoSnailfishPair(numberPairIn.first));
	}

	if (numberPairIn.second.size() == 1)
	{
		numberPair.second = std::make_unique<RegularNumber>(Parsing::ConvertStringToInt(numberPairIn.second));
	}
	else
	{
		numberPair.second = std::make_unique<SnailfishNumber>(SplitStringIntoSnailfishPair(numberPairIn.second));
	}
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
			numberPair.first = std::make_unique<RegularNumber>(0);
		}
		numberPair.second->IncreaseLeftValue(pairToExplode.second);
		exploded = true;
		pairToExplode.second = 0;
	}
	else if (numberPair.second->MaybeExplode(nestDepth + 1, pairToExplode))
	{
		if (nestDepth == 4)
		{
			numberPair.second = std::make_unique<RegularNumber>(0);
		}
		numberPair.first->IncreaseRightValue(pairToExplode.first);
		exploded = true;
		pairToExplode.first = 0;
	}
	return exploded;
}

bool Snail::SnailfishNumber::MaybeSplit(std::unique_ptr<Number>& newNumber)
{
	bool split{ false };

	// A 'true' to maybe split will have a pointer to the new snailNumber if
	// the split was in an immediate child, or nullptr if the split occured lower
	// down the tree and we're now just backing up.
	if (numberPair.first->MaybeSplit(newNumber))
	{
		split = true;
		if (newNumber)
		{
			numberPair.first = std::move(newNumber);
		}
	}
	else if (numberPair.second->MaybeSplit(newNumber))
	{
		split = true;
		if (newNumber)
		{
			numberPair.second = std::move(newNumber);
		}
	}

	return split;
}

bool Snail::RegularNumber::MaybeExplode(int depth, std::pair<int, int>& pairToExplode)
{
	// Regular numbers don't explode
	return false;
}

bool Snail::RegularNumber::MaybeSplit(std::unique_ptr<Number> &newNumber)
{
	assert(!newNumber);
	bool split{ false };
	if (value > 9)
	{
		split = true;
		int oddComponent = value % 2;
		std::unique_ptr<Number> leftNumber = std::make_unique<RegularNumber>(value / 2);
		std::unique_ptr<Number> rightNumber = std::make_unique<RegularNumber>((value / 2) + oddComponent);
		newNumber = std::make_unique<SnailfishNumber>(
			std::pair<std::unique_ptr<Number>, std::unique_ptr<Number>>(
				std::move(leftNumber), std::move(rightNumber)));
	}
	return split;
}
