#include "Snailfish.h"
#include <cassert>

// The result of adding 2 SnailfishNumbers together is a new SnailfishNumber where the
// left-hand portion of that number is the left hand operand of the addition, and the right
// hand-hand portion is that right hand operand of the addition.
// The rules of the puzzle state that the number must then be 'reduced'. Reduction means that
// we keep checking for any explosions (SnailfishNumber dissipating its values in each direction)
// and Splits (RegularNumber breaking down into a SnailfishNumber) until there are none left to
// occur.
std::unique_ptr<Snail::SnailfishNumber> Snail::AddSnailfishNumbers(
	std::unique_ptr<Snail::SnailfishNumber> numberOne,
	std::unique_ptr<Snail::SnailfishNumber> numberTwo)
{
	std::unique_ptr<SnailfishNumber> newSnailfishNumber =
		std::make_unique<SnailfishNumber>(
			std::pair<std::unique_ptr<SnailfishNumber>, std::unique_ptr<SnailfishNumber>>{
		std::move(numberOne), std::move(numberTwo) });

	// Having created our new number, 'reduce' it by checking for any explosions and
	// splits which must now occur. The occurence of either is always followed by another
	// check for an explosion prior to the next check for a split.
	while (true)
	{
		// The MaybeExplode and MaybeSplit operations both take a reference to some output
		// parameter that will be populated for use of other locations within the recursive
		// stack of calls we're about to kick off. We're not interested in those values at
		// this top level, but must still initialise them for use in the function.
		std::unique_ptr<Number> emptyNumPtr{};
		std::pair<int, int> emptyNumPair{ 0,0 };
		if (newSnailfishNumber->MaybeExplode(1, emptyNumPair)) { continue; }
		else if (newSnailfishNumber->MaybeSplit(emptyNumPtr)) { continue; }
		else { break; }
	}

	return newSnailfishNumber;
}

// Splitting occurs when a RegularNumber has a value of more than 9. The RegularNumber will
// be replaced with a SnailfishNumber where with a left-value equal to the existing RegularNumber
// halved and rounded down, and the right-value equal to the existing RegularNumber halved
// and rounded up.
// This function returns a boolean for whether splitting occured. If splitting does occur,
// the unique_ptr reference will have been assigned the newly created SnailfishNumber to point
// at.
bool Snail::RegularNumber::MaybeSplit(std::unique_ptr<Number>& newNumber)
{
	// The ouput parameter newNumber only exists to be assigned to in the case that splitting
	// occurs, it should currently be empty.
	assert(!newNumber);

	bool split{ false };
	if (value > 9)
	{
		split = true;
		int oddComponent = value % 2;
		int leftValue = value / 2;
		int rightValue = (value / 2) + oddComponent;
		newNumber = std::make_unique<SnailfishNumber>(
			std::pair<std::unique_ptr<Number>, std::unique_ptr<Number>>(
				std::move(std::make_unique<RegularNumber>(leftValue)),
				std::move(std::make_unique<RegularNumber>(rightValue))));
	}
	return split;
}

// If a Snailfish number is nested withing 4 other SnailfishNumbers it will explode.
// To explode a SnailfishNumber you add its left hand value to the nearest RegularNumber to
// the left of it (as it would appear by eye if you wrote the top-level SnailfishNumber out), add
// the right-hand value to the nearest Regular Number to the right of it, and then replace the
// SnailfishNumber with a RegularNumber of 0.
// This function acts by each SnailfishNumber recursively calling it on its left and right values,
// increasing the nestDepth with each call. Either we will reach a RegularNumber before exceeding
// the maximum nest depth, and return false all the way up the recursion (indicating no explosion
// occured), or we will end up calling this function with a nestDepth of MaxNestDepth (5).
// If this function is called with the max nestDepth, the output parameter explodedValues
// will be populated with the constituent values of the exploded values. It is then up to the
// immediate parent of that SnailfishNumber to resolve the impact of this explosion before returning
// true further up the recursion stack.
bool Snail::SnailfishNumber::MaybeExplode(int nestDepth, std::pair<int, int>& explodedValues)
{
	bool exploded_rc = false;

	// If we've reached the max depth, set the explodedValues to contain our constituent
	// values and set the return code to true to indicate that an explosion occured. 
	if (nestDepth == MaxNestDepth)
	{
		explodedValues = { numberPair.first->Magnitude(), numberPair.second->Magnitude() };
		exploded_rc = true;
	}
	else
	{
		// Not max depth yet, recurse down through children, starting with the left. If an explosion
		// occurs down one of these legs of recursion then we must:
		//  - If we are the immediate parent of the exploded element, replace it with a regular number
		//    with value 0.
		//  - Play our part in incrementing the nearest regular number in each direction of the
		//    exploded number. Because we only have access to our children numbers, not parent, we
		//    can only force an increment of our other child number, selecting the correct member from
		//    the exploded number, and then setting that member to 0 to prevent higher recursions from
		//    using it further. The remaining element of explodedValues is left alone for a higher nesting
		//    to act on.
		if (numberPair.first->MaybeExplode(nestDepth + 1, explodedValues))
		{
			exploded_rc = true;

			if (nestDepth == (MaxNestDepth - 1))
			{
				numberPair.first = std::make_unique<RegularNumber>(0);
			}
			
			if (explodedValues.second > 0)
			{
				numberPair.second->IncreaseLeftmostValue(explodedValues.second);
				explodedValues.second = 0;
			}
		}
		else if (numberPair.second->MaybeExplode(nestDepth + 1, explodedValues))
		{
			exploded_rc = true;

			if (nestDepth == (MaxNestDepth - 1))
			{
				numberPair.second = std::make_unique<RegularNumber>(0);
			}

			if (explodedValues.first > 0)
			{
				numberPair.first->IncreaseRightmostValue(explodedValues.first);
				explodedValues.first = 0;
			}
		}
	}
	return exploded_rc;
}

// Splitting is when a RegularNumber of value greater than 9 splits into a SnailfishNumber.
// A SnailfishNumber itself will never split, but still needs to check its children to
// propogate this MaybeSplit check through the tree.
// If a split has occured, then newNumber may contain the newly created SnailfishNumber pointer.
// Only the immediate parent needs to place this new number into one side of its pair. The
// move operation of the unique_ptr will leave it null'd, preventing any SnailfishNumbers higher
// up the recursion stack from also trying to save it.
bool Snail::SnailfishNumber::MaybeSplit(std::unique_ptr<Number>& newNumber)
{
	bool split{ false };

	// In the case of a split, the newNumber will only be a valid pointer if we are the
	// immediate parent of the new number, in which case we slot it into our numberPair,
	// with a moveOperation that leaves it empty for the next caller up the recursion stack.
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

// Create a SnailfishNumber based on the string representation of their left and right
// components. Those components might just be a single digit int like "4", or they
// might be a SnailfishNumber themselves, like "[[4,3],7]".
// In the case of one side of the pair having length 1, it must be an int, and we
// can simply create a RegularNumber. Otherwise we have another SnailfishNumber on
// our hands and must split it up further to create a SnailfishNumber pointer.
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

// Split up the string representation of a SnailfishNumber into a pair of strings representing
// the left and right hand components of that SnailfishNumber. e.g. "[[4,1],8]" would become
// "[4,1]" and "8".
std::pair<std::string, std::string> Snail::SplitStringIntoSnailfishPair(std::string const &snailfishNumberString)
{
	std::string leftPortion;
	std::string rightPortion;
	int bracketDepth{ 0 };

	// Before we remove the leading '[', assert that the string has the expected surrounding
	// square brackets.
	assert((snailfishNumberString[0] == '[') &&
		(snailfishNumberString[snailfishNumberString.size() - 1] == ']'));
	std::string_view numberStringView{ snailfishNumberString };
	numberStringView.remove_prefix(1);

	// Now look for that middle comma that separates the two halves of the SnailfishNumber.
	// We do this by keeping track of how many nestings deep into square brackets we are.
	// If we reach a ',' while not within any surrounding square brackets, we have found
	// the separator between the two elements. Before we reach this point, each character
	// encountered is appended to the left portion of this SnailfishNumber, and after we
	// reach the separator we begin appending to the right portion of the number instead.
	bool firstHalf{ true };
	for (char character : numberStringView)
	{
		if (firstHalf)
		{
			if ((character == ',') && (bracketDepth == 0))
			{
				firstHalf = false;
				continue;
			}

			if (character == '[')
			{
				bracketDepth++;
			}
			else if (character == ']')
			{
				bracketDepth--;
				assert(bracketDepth >= 0);
			}

			leftPortion += character;
		}
		else
		{
			if ((character == ']') && (bracketDepth == 0))
			{
				break;
			}

			if (character == '[')
			{
				bracketDepth++;
			}
			else if (character == ']')
			{
				bracketDepth--;
				assert(bracketDepth >= 0);
			}

			rightPortion += character;
		}
	}

	return { leftPortion, rightPortion };
}

// The magnitude of a SnailfishNumber is 3 times the left hand element, plus 2 times the
// right hand element. Magnitude calculations are recursive, we'll call the Magnitude
// function on our children to find their own magnitudes first.
int Snail::SnailfishNumber::Magnitude()
{
	return ((3 * numberPair.first->Magnitude()) + (2 * numberPair.second->Magnitude()));
}

// This function exists purely for debugging purposes, and produces the human readable version
// of a SnailfishNumber, recursing down into the numberPair to build up the string.
std::string Snail::SnailfishNumber::ValueAsString()
{
	std::string valueAsString{ "[" };
	valueAsString += numberPair.first->ValueAsString();
	valueAsString += ",";
	valueAsString += numberPair.second->ValueAsString();
	valueAsString += "]";
	return valueAsString;
}
