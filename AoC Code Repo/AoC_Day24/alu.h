#pragma once
#include <vector>
#include <string>
#include <deque>
#include <set>
#include "aoc_common_types.h"

namespace ALU
{
	const std::string inputOp{ "inp" };
	const std::string addOp{ "add" };
	const std::string mulOp{ "mul" };
	const std::string divOp{ "div" };
	const std::string modOp{ "mod" };
	const std::string eqlOp{ "eql" };

	// Represents single stage
	class ALUStage
	{
	public: struct OperationDescriptor;
	private:
		struct Operation
		{
			std::string opId;
			LLINT& varToEdit;
			LLINT editFactor;
		};

		std::vector<OperationDescriptor> operations;
		bool PerformOperationByDesc(
			OperationDescriptor operation,
			LLINT& xVar,
			LLINT& yVar,
			LLINT& wVar,
			LLINT& zVar); // Returns false if can't be done
		bool PerformOperation(Operation operation);

		LLINT& GetIntRefFromChar(char charIn, LLINT& x, LLINT& y, LLINT& w, LLINT& z);
		LLINT GetIntFromChar(std::string charIn, LLINT x, LLINT y, LLINT w, LLINT z);

	public:
		struct OperationDescriptor
		{
			std::string opId;
			char varToEdit;
			std::string editToApply;
		};

		ALUStage(std::vector<OperationDescriptor> operationDescriptors) : operations{ operationDescriptors } {};

		// runs the stage with a given set of vars, and returns them as references
		// returns true if the stage ran successfully, false otherwise
		bool RunStage(LLINT& xVar, LLINT& yVar, LLINT& wVar, LLINT& zVar);
	};

	class ArithmeticLogicUnit
	{
	private:
		std::deque<int> highestPossibleInput;
		std::deque<int> lowestPossibleInput;

		std::vector<ALUStage> aluStages;

		bool TryNextLevelDepth(unsigned int depth, std::vector<std::set<LLINT>>& possibleZValues, LLINT currentZ);

	public:
		ArithmeticLogicUnit(std::vector<ALUStage> stages);

		std::deque<int> HighestPossibleInput() { return highestPossibleInput; };
		std::deque<int> LighestPossibleInput() { return lowestPossibleInput; };
	};
}