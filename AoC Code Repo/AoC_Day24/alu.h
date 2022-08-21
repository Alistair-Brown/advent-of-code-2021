#pragma once
#include <vector>
#include <string>
#include <deque>
#include <set>
#include <list>
#include <optional>
#include <map>
#include "aoc_common_types.h"

namespace ALU
{
	// The set of instructions to be performed by each 'stage' of the ALU (where a stage
	// is the execution carried out between requests for input) is identical for every
	// stage, apart from a few particular values. This regex represents those identical
	// instructions with capture groups for the variable values.
	const std::string regexMatchString{
		"^mul x 0\n"
		"add x z\n"
		"mod x 26\n"
		"div z ([-]{0,1}[0-9]+)\n"
		"add x ([-]{0,1}[0-9]+)\n"
		"eql x w\n"
		"eql x 0\n"
		"mul y 0\n"
		"add y 25\n"
		"mul y x\n"
		"add y 1\n"
		"mul z y\n"
		"mul y 0\n"
		"add y w\n"
		"add y ([-]{0,1}[0-9]+)\n"
		"mul y x\n"
		"add z y\n$"
	};

	// Represents a valid set of inputs to insert into the w register for the remaining
	// stages at any given point in the ALU. i.e. if these represented valid inputs
	// from the 12th stage, each list would be 3 ints log, representing the inputs prior to
	// the 12th, 13th and 14th stage. Since we are only interested in the minimum and
	// maximum inputs for this puzzle, that is all we hold here.
	struct RemainingWInputs
	{
		std::list<int> minimumWInputs;
		std::list<int> maximumWInputs;
	};

	// Inspection of our puzzle input revealed that each series of operations between requests
	// for input (a 'stage') is identical apart from 3 variables. So with knowledge of the identical
	// operations encoded into this class, the only instance variables needed to represent a
	// particular stage are those variables, being a particular division to apply to the z register
	// and particular additions to apply to the x and y registers.
	class ALUStage
	{
	private:
		int zDivValue;
		int xAddValue;
		int yAddValue;		

		// For each possible value the z register could hold prior to this stage that leaves
		// a possibility of the z register holding 0 after all stages are run, provides the required
		// inputs to feed into w for this stage and each remaining to ensure that this z value of 0
		// is reached. Both the maximum and minimum values of possible w inputs are provided for
		// each initial value of the z register.
		std::map<LLINT, RemainingWInputs> allowableInputs{};
		bool stageCalculationsRun{ false };

		std::vector<LLINT> InputZToGiveRequiredOutput(LLINT requiredOutputZ, int inputW, int xAfterModuloOps);
	public:
		ALUStage(int zDivValue, int xAddValue, int yAddValue) :
			zDivValue{ zDivValue }, xAddValue{ xAddValue }, yAddValue{ yAddValue } {};

		// It can be shown by examining the puzzle input that the only things which impact
		// the final value of the z register for a given stage are the initial value of
		// the z register and the input that is read into the w register prior to that
		// stage. So we can work backwards through each stage, finding the allowable inputs
		// for a given stage (both the value to feed into w and the required current value
		// of z) based on the allowable outputs (possibly final values of the z register).
		void CalculateAllowableInputs(std::map<LLINT, RemainingWInputs> const& allowableOutputs);
		std::map<LLINT, RemainingWInputs> const &AllowableInputs() const { return allowableInputs; };
	};

	// The ArithmeticLogicUnit consists of a number of stages representing the operations
	// performed between each request for input. Finding allowable inputs for each of these
	// stages during construction of the ALU allows us to query the largest and smallest
	// model numbers (14 digit number made of the inputs to each of the 14 stages) that
	// leave the z register with a value of 0 and are hence valid.
	class ArithmeticLogicUnit
	{
	private:
		std::vector<ALUStage> stages{};
	public:
		ArithmeticLogicUnit(std::vector<ALUStage> stagesIn);

		std::string LargestModelNumber() const;
		std::string SmallestModelNumber() const;
	};
}