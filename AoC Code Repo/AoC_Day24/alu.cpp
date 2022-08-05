#include "alu.h"
#include <cassert>
#include <array>
#include <set>
#include <iostream>
#include "aoc_common_types.h"

bool ALU::ALUStage::PerformOperationByDesc(OperationDescriptor operation, LLINT& xVar, LLINT& yVar, LLINT& wVar, LLINT& zVar)
{
	return PerformOperation({
		operation.opId,
		GetIntRefFromChar(operation.varToEdit, xVar, yVar, wVar, zVar),
		GetIntFromChar(operation.editToApply, xVar, yVar, wVar, zVar)
	});
}

bool ALU::ALUStage::PerformOperation(Operation operation)
{
	if (operation.opId == addOp)
	{
		operation.varToEdit += operation.editFactor;
	}
	else if (operation.opId == mulOp)
	{
		operation.varToEdit *= operation.editFactor;
	}
	else if (operation.opId == divOp)
	{
		if (operation.editFactor == 0) { return false; }
		else { operation.varToEdit /= operation.editFactor; }
	}
	else if (operation.opId == modOp)
	{
		if ((operation.editFactor <= 0) || (operation.varToEdit < 0)) { return false; }
		else { operation.varToEdit %= operation.editFactor; }
	}
	else if (operation.opId == eqlOp)
	{
		operation.varToEdit = (operation.varToEdit == operation.editFactor) ? 1 : 0;
	}
	else
	{
		assert(false);
	}
	return true;
}

LLINT& ALU::ALUStage::GetIntRefFromChar(char charIn, LLINT& x, LLINT& y, LLINT& w, LLINT& z)
{
	switch (charIn)
	{
	case 'x':
			return x;
			break;
	case 'y':
		return y;
		break;
	case 'w':
		return w;
		break;
	case 'z':
		return z;
		break;
	default:
		assert(false);
		return x;
		break;
	}
}

LLINT ALU::ALUStage::GetIntFromChar(std::string charIn, LLINT x, LLINT y, LLINT w, LLINT z)
{
	if (charIn == "x") { return x; }
	if (charIn == "y") { return y; }
	if (charIn == "w") { return w; }
	if (charIn == "z") { return z; }
	else
	{
		int returnVal = std::atoi(charIn.c_str());
		return returnVal;
	}
}

bool ALU::ALUStage::RunStage(LLINT& xVar, LLINT& yVar, LLINT& wVar, LLINT& zVar)
{
	for (OperationDescriptor operationDesc : operations)
	{
		if (!PerformOperationByDesc(operationDesc, xVar, yVar, wVar, zVar))
		{
			return false;
		}
	}

	return true;
}

bool ALU::ArithmeticLogicUnit::TryNextLevelDepth(unsigned int depth, std::vector<std::set<LLINT>>& possibleZValues, LLINT currentZ)
{
	LLINT xx = 0;
	LLINT yy = 0;

	if (possibleZValues[depth].contains(currentZ)) { return false; }
	
	for (LLINT ww = 9; ww > 0; ww--)
	{
		LLINT forwardZ = currentZ;

		// Actually we need to move the z checking higher up, like here or somewhere

		if (aluStages[depth].RunStage(xx, yy, ww, forwardZ))
		{
			if (depth == 13) 
			{
				if (forwardZ == 0)
				{
					highestPossibleInput.push_front(ww);
					return true;
				}
				else
				{
					return false;
				}
			}
			else
			{
				if (TryNextLevelDepth(depth + 1, possibleZValues, forwardZ))
				{
					// Also success, means we hit the bottom and are working back
					highestPossibleInput.push_front(ww);
					return true;
				}
				else
				{
					possibleZValues[depth].insert(currentZ);
				}
			}
		}
	}

	return false;
}

ALU::ArithmeticLogicUnit::ArithmeticLogicUnit(std::vector<ALUStage> stages)
{
	aluStages = stages;

	LLINT myx{ 0 };
	LLINT myy{ 0 };
	LLINT myw{ 9 };
	LLINT myz{ 0 };

	stages[0].RunStage(myx, myy, myw, myz);
	myw = 9;
	stages[1].RunStage(myx, myy, myw, myz);
	myw = 2;
	stages[2].RunStage(myx, myy, myw, myz);
	myw = 9;
	stages[3].RunStage(myx, myy, myw, myz);
	myw = 9;
	stages[4].RunStage(myx, myy, myw, myz);
	myw = 5;
	stages[5].RunStage(myx, myy, myw, myz);
	myw = 1;
	stages[6].RunStage(myx, myy, myw, myz);
	myw = 3;
	stages[7].RunStage(myx, myy, myw, myz);
	myw = 8;
	stages[8].RunStage(myx, myy, myw, myz);
	myw = 9;
	stages[9].RunStage(myx, myy, myw, myz);
	myw = 9;
	stages[10].RunStage(myx, myy, myw, myz);
	myw = 9;
	stages[11].RunStage(myx, myy, myw, myz);
	myw = 7;
	stages[12].RunStage(myx, myy, myw, myz);
	myw = 1;
	stages[13].RunStage(myx, myy, myw, myz);

	std::vector<std::set<LLINT>> possibleZValues(14);

	TryNextLevelDepth(0, possibleZValues, 0);
}
