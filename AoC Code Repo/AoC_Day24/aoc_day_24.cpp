#include "puzzle_solvers.h"
#include "Parsing.h"
#include "alu.h"
#include <regex>

// For day 24 we must model the submarine's Arithmetic Logic Unit (ALU). Our puzzle input
// is a series of operations which will be performed by this ALU to manipulate its 4 registers,
// named w, x, y and z. One of these potential operations involves accepting user input, and there are 14
// such inputs requested by the series of operations in our puzzle. Each input is a digit between 1-9,
// such that our input in order could be read like a 14 digit number. We must find the largest and
// smallest 14 digit numbers that could be used as input to leave the z register with a final value
// of 0. Solving this in a timely fashion requires us to do some manual inspection of this input
// which is covered in more details in comments closer to the relevant code.
PuzzleAnswerPair PuzzleSolvers::AocDayTwentyFourSolver(std::ifstream& puzzleInputFile)
{
	// The operations performed by this ALU can be broken up into 14 stages, where each stage is the
	// set of operations performed between each request for input (which is displayed as 'inp w' in
	// the list of operations, instructing the ALU to read user input and store it in the w register).
	std::vector<ALU::ALUStage> aluStages;
	assert(Parsing::ReadWholeLineFromInputFile(puzzleInputFile) == "inp w");
	std::vector<std::string> inputLines = Parsing::SeparateRemainingInputIntoLines(puzzleInputFile);
	
	// It turns out that each 'stage' of operations is almost identical, with the only differences
	// being in 3 particular places: a division of the value in the z register by a certain amount,
	// an addition of a certain amount to the x register, and an addition of a certain amount to
	// the y register. So we can just parse out those particular variable values and initialise each
	// ALU stage accordingly.
	int zDivValue;
	int xAddValue;
	int yAddValue;
	std::regex matchString{ ALU::regexMatchString };
	std::string singleStageAsString{};
	for (std::string inputLine : inputLines)
	{
		if (inputLine == "inp w")
		{
			Parsing::ParseStringIntoElements(singleStageAsString, matchString, zDivValue, xAddValue, yAddValue);
			aluStages.emplace_back(zDivValue, xAddValue, yAddValue);
			singleStageAsString.clear();
		}
		else
		{
			singleStageAsString.append(inputLine + "\n");
		}
	}
	Parsing::ParseStringIntoElements(singleStageAsString, matchString, zDivValue, xAddValue, yAddValue);
	aluStages.emplace_back(zDivValue, xAddValue, yAddValue);
	singleStageAsString.clear();

	// Our full ALU is made up of the individual stages in order, and can be queried to return the largest
	// and smallest valid model numbers (where a model number is the 14 digit number representing the 14
	// inputs, and a valid model number is one where the z register is left with the value 0 after all
	// operations have been performed).
	ALU::ArithmeticLogicUnit arithmeticLogicUnit{ aluStages };
	return PuzzleAnswerPair{ arithmeticLogicUnit.LargestModelNumber(), arithmeticLogicUnit.SmallestModelNumber() };
}
