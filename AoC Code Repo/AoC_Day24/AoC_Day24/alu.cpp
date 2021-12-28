#include "alu.h"
#include <cassert>
#include "Parsing.h"

void alu::ALU::ApplyInstruction(Instruction instruction)
{
	Operation *&namedVariable = GetVariableReferenceFromString(instruction.inputOne);
	switch (instruction.type)
	{
	case OperationType::Input:		
		namedVariable = (Operation *)new InputOperation(GetNextInputDigit());
		break;
	case OperationType::Value:
		namedVariable = (Operation *)new ValueOperation(Parsing::ConvertStringToInt(instruction.inputTwo));
		break;
	case OperationType::Add:
	case OperationType::Multiply:
	case OperationType::Divide:
	case OperationType::Modulo:
	case OperationType::Equality:
		Operation *secondInput;
		if (GetVariableReferenceFromString(instruction.inputTwo) == nullptr)
		{
			secondInput = (Operation *)new ValueOperation(Parsing::ConvertStringToInt(instruction.inputTwo));
		}
		else
		{
			secondInput = GetVariableReferenceFromString(instruction.inputTwo);
		}
		switch (instruction.type)
		{
		case OperationType::Add:
			namedVariable = (Operation *)new AddOperation(namedVariable, secondInput);
			break;
		case OperationType::Multiply:
			namedVariable = (Operation *)new MultiplyOperation(namedVariable, secondInput);
			break;
		case OperationType::Divide:
			namedVariable = (Operation *)new DivideOperation(namedVariable, secondInput);
			break;
		case OperationType::Modulo:
			namedVariable = (Operation *)new ModuloOperation(namedVariable, secondInput);
			break;
		case OperationType::Equality:
			namedVariable = (Operation *)new EqualityOperation(namedVariable, secondInput);
			break;
		default:
			assert(false);
			break;
		}
		break;
	default:
		assert(false);
		break;
	}
}

alu::Operation *& alu::ALU::GetVariableReferenceFromString(std::string variableName)
{
	if (variableName == "x") { return xValue; }
	else if (variableName == "y") { return yValue; }
	else if (variableName == "w") { return wValue; }
	else if (variableName == "z") { return zValue; }
	else { return invalid; }
}

char alu::ALU::GetNextInputDigit()
{
	char digitToReturn = nextInputDigit;
	nextInputDigit++;
	return digitToReturn;
}

alu::ALU::ALU(std::vector<Instruction> instructions)
{
	xValue = (Operation *)new ValueOperation(0);
	yValue = (Operation *)new ValueOperation(0);
	wValue = (Operation *)new ValueOperation(0);
	zValue = (Operation *)new ValueOperation(0);

	for (Instruction instruction : instructions)
	{
		ApplyInstruction(instruction);
	}
}

std::string alu::ALU::GetVariableExpressionAsString(char variable)
{
	Operation *namedVariable = GetVariableReferenceFromString(std::string(1, variable));
	assert(namedVariable != nullptr);
	return namedVariable->PrintAsString();
}

alu::Instruction alu::CreateInstructionFromStringLine(std::vector<std::string> parsedStringLine)
{
	Instruction createdInstruction;
	if (parsedStringLine[0] == "inp")
	{
		createdInstruction = Instruction(OperationType::Input, parsedStringLine[1], "");
	}
	else if (parsedStringLine[0] == "add")
	{
		createdInstruction = Instruction(OperationType::Add, parsedStringLine[1], parsedStringLine[2]);
	}
	else if (parsedStringLine[0] == "mul")
	{
		createdInstruction = Instruction(OperationType::Multiply, parsedStringLine[1], parsedStringLine[2]);
	}
	else if (parsedStringLine[0] == "div")
	{
		createdInstruction = Instruction(OperationType::Divide, parsedStringLine[1], parsedStringLine[2]);
	}
	else if (parsedStringLine[0] == "mod")
	{
		createdInstruction = Instruction(OperationType::Modulo, parsedStringLine[1], parsedStringLine[2]);
	}
	else if (parsedStringLine[0] == "eql")
	{
		createdInstruction = Instruction(OperationType::Equality, parsedStringLine[1], parsedStringLine[2]);
	}
	return createdInstruction;
}
