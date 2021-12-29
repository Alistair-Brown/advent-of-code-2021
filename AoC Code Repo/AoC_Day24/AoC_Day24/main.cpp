#include "alu.h"
#include <iostream>
#include "Parsing.h"

// My code for today is truly horrible, so warrants a comment just to explain what I was thinking.
// This started as an attempt for me to parse the instructions into a more human readable form, by
// getting rid of superfluous things like dividing by 1. As part of that, I often needed to know
// what possible ranges an operation's output could be in.
// Note that most operations just return a flat 1 as their output, which is never read, because
// it was the bounds of their value that mattered more. I realised that z (the number we need to
// be 0) was never negative by the end, so any set of inputs that give a minimum z of 0 were at least
// possible valid. So my approach became to define just the most significant digits, finding the highest/lowest
// value of those that gave a minimum z of 0, and then keep adding lesser and lesser significant digits
// until we'd found the full set that gave a 0 z.
// This got a bit ugly in that my ranges consider an input to be 1-9, but don't stop one operation using the
// bottom of that range from meaning another operation could take an higher portion. And so a bunch of significant
// figures that supposedly can give a 0 z might not actually give it for any actual lesser significant figures.
// So there's quite a lot of backtracking to reduce/increase the value of higher significant figures as
// needed when things don't pan out. And this does eventually give the right answer quicker than a full on
// iterate-every-possibility, but still takes ages.

// UPDATE: If I come back to this, I noticed that the way the operations
// act on the digits appears to effectively deal with them in groups (i.e. some combination of a,b,c is required
// that gives a certain value when compared to d, after operations have been applied. A skim of the reddit
// suggests this is similar thinking to what other people who solved this have done.
// In more detail, I've just spotted that whenever we read in our next input, we read that into w, having stored
// some existing value in z, and don't do anything else with x or y before updating them.
// So it's the value of z before each new input that matters. If I can find the points where z can only take the
// value 1 or 0 (i.e. an equality operation has just happened), and find the highest or lowest combination
// of preceding numbers that provides each possibility, I can effectively slice off part of the full equation.
// But that needs to be done another day, with an almost entirely new program that I approach with a
// coherent plan, can't let myself sink any more time into this one.
int main()
{
	std::cout << "Enter program" << std::endl;

	std::vector<std::string> parsedInputLine;
	std::vector<alu::Instruction> instructions;
	while (true)
	{
		parsedInputLine = Parsing::ParseSpaceSeparatedString();
		if (parsedInputLine.size() == 0) { break; }

		instructions.push_back(alu::CreateInstructionFromStringLine(parsedInputLine));
	}

	//alu::ALU arithmeticLogicUnit{ instructions, "" };
	//std::cout << "x: " << arithmeticLogicUnit.GetVariableExpressionAsString('x') << std::endl;
	//std::cout << "y: " << arithmeticLogicUnit.GetVariableExpressionAsString('y') << std::endl;
	//std::cout << "w: " << arithmeticLogicUnit.GetVariableExpressionAsString('w') << std::endl;
	//std::cout << "z: " << arithmeticLogicUnit.GetVariableExpressionAsString('z') << std::endl;
	//std::cout << "z range is " << arithmeticLogicUnit.PossibleZRange().first << ", " << arithmeticLogicUnit.PossibleZRange().second << std::endl;

	std::string knownDigits{ "" };
	for (int ii = 1; ii < 14; ii++)
	{
		for (char attemptedChar = '1'; attemptedChar <= '9'; attemptedChar++)
		{
			alu::ALU arithmeticLogicUnit{ instructions, knownDigits + attemptedChar };
			//std::cout << "Minimum z is" << arithmeticLogicUnit.PossibleZRange().first << std::endl;
			if (arithmeticLogicUnit.PossibleZRange().first <= 0)
			{
				knownDigits += attemptedChar;
				break;
			}
		}

		// If we failed to find a valid digit, try increasing our
		// current last char
		if (knownDigits.size() == ii)
		{
			char lastChar = knownDigits[knownDigits.size() - 1];
			knownDigits = knownDigits.substr(0, knownDigits.size() - 1);
			knownDigits += (char)(lastChar + 1);
			ii--;

			// Just manually stepping further back if we need to is getting very yucky, but
			// let's see if we can pull this out.
			if (lastChar == '9')
			{
				knownDigits = knownDigits.substr(0, knownDigits.size() - 1);
				lastChar = knownDigits[knownDigits.size() - 1];
				knownDigits = knownDigits.substr(0, knownDigits.size() - 1);
				knownDigits += (char)(lastChar + 1);
				ii--;

				if (lastChar == '9')
				{
					knownDigits = knownDigits.substr(0, knownDigits.size() - 1);
					lastChar = knownDigits[knownDigits.size() - 1];
					knownDigits = knownDigits.substr(0, knownDigits.size() - 1);
					knownDigits += (char)(lastChar + 1);
					ii--;

					if (lastChar == '9')
					{
						knownDigits = knownDigits.substr(0, knownDigits.size() - 1);
						lastChar = knownDigits[knownDigits.size() - 1];
						knownDigits = knownDigits.substr(0, knownDigits.size() - 1);
						knownDigits += (char)(lastChar + 1);
						ii--;
					}
				}
			}
		}
	}

	std::cout << "Smalled accepted number is " << knownDigits << std::endl;

	knownDigits = "";
	for (int ii = 0; ii < 14; ii++)
	{
		for (char attemptedChar = '9'; attemptedChar > '0'; attemptedChar--)
		{
			alu::ALU arithmeticLogicUnit{ instructions, knownDigits + attemptedChar };
			//std::cout << "Minimum z is" << arithmeticLogicUnit.PossibleZRange().first << std::endl;
			if (arithmeticLogicUnit.PossibleZRange().first == 0)
			{
				knownDigits += attemptedChar;
				break;
			}
		}

		// If we failed to find a valid digit, try decreasing our
		// current last char
		if (knownDigits.size() == ii)
		{
			char lastChar = knownDigits[knownDigits.size() - 1];
			knownDigits = knownDigits.substr(0, knownDigits.size() - 1);
			knownDigits += (char)(lastChar - 1);
			ii--;

			// Just manually stepping further back if we need to is getting very yucky, but
			// let's see if we can pull this out.
			if (lastChar == '1')
			{
				knownDigits = knownDigits.substr(0, knownDigits.size() - 1);
				lastChar = knownDigits[knownDigits.size() - 1];
				knownDigits = knownDigits.substr(0, knownDigits.size() - 1);
				knownDigits += (char)(lastChar - 1);
				ii--;

				if (lastChar == '1')
				{
					knownDigits = knownDigits.substr(0, knownDigits.size() - 1);
					char anotherLastChar = knownDigits[knownDigits.size() - 1];
					knownDigits = knownDigits.substr(0, knownDigits.size() - 1);
					knownDigits += (char)(anotherLastChar - 1);
					ii--;

					if (lastChar == '1')
					{
						knownDigits = knownDigits.substr(0, knownDigits.size() - 1);
						char anotherLastChar = knownDigits[knownDigits.size() - 1];
						knownDigits = knownDigits.substr(0, knownDigits.size() - 1);
						knownDigits += (char)(anotherLastChar - 1);
						ii--;
					}
				}
			}
		}
	}

	std::cout << "Largest accepted number is " << knownDigits << std::endl;

}