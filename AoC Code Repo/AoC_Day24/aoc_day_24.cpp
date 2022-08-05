#include "puzzle_solvers.h"
#include "Parsing.h"
#include "alu.h"
#include <regex>

// Solution operates on the principle that we don't really have 10^14 options to consider
// we actually just have 14 points at which a new input is inserted, at each of which x, y, w and z
// have 10000 possible combinations. So I really have 140000 cases to consider. This then gets a
// lot less when you inspect the input and realise that at the point of each input, the only
// one of those 4 which will not be overwritten before it's next used is z. So really we just have
// to start with finding the possible values of w and z after each input which give a z of 0.
// Then for the step before that we find the possible combinations of w and z which give valid
// values of w and z as found for the previous step. And so on back to the start. Then at the
// start we take either the highest or lowest z as required, then the highest or lowest value
// of w that pair with a valid z for the next round, and so on, to give our 14 digit number.


// New proposal, there are more than 1-9 values z can take at each step, it can be as large as
// it likes, but I should be able to find a range of z values that each input w allows,
// for each stage, starting with the final one. Plus
// a modulo with 26 that it must satisfy. And if I do that for each input at each step, I can
// go back down my original plan. I'll then be able to say does input w 9 give a z that fits the criteria
// for entering stage 2, and then with z and various other w's (starting large), can we get a z that
// fits the input criteria for stage 3, etc.
PuzzleAnswerPair PuzzleSolvers::AocDayTwentyFourSolver(std::ifstream& puzzleInputFile)
{
	// Takes too long to run at the moment, drop out
	return PuzzleAnswerPair{ "Not implemented", "Not implemented" };

	assert(Parsing::ReadWholeLineFromInputFile(puzzleInputFile) == "inp w");
	std::vector<std::string> inputLines = Parsing::SeparateRemainingInputIntoLines(puzzleInputFile);

	std::vector<ALU::ALUStage> aluStages;

	std::vector<ALU::ALUStage::OperationDescriptor> operations;
	std::string opId;
	char paramOne;
	std::string paramTwo;
	std::regex matchString{ "^([a-z]+) ([a-z]) ([-]{0,1}[a-z0-9]+)$" };
	for (std::string inputLine : inputLines)
	{
		if (inputLine == "inp w")
		{
			aluStages.emplace_back(operations);
			operations.clear();
		}
		else
		{
			Parsing::ParseStringIntoElements(inputLine, matchString, opId, paramOne, paramTwo);
			operations.emplace_back(opId, paramOne, paramTwo);
		}
	}
	aluStages.emplace_back(operations); // Final stage

	ALU::ArithmeticLogicUnit myALU{ aluStages };

	return PuzzleAnswerPair{ "Not implemented", "Not implemented" };
}
