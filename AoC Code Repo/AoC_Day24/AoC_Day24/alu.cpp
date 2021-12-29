#include "alu.h"
#include <cassert>
#include "Parsing.h"

void alu::ALU::NewlyAllocatedOperation(Operation *newOperation)
{
	allocatedOperations.push_back(newOperation);
}

void alu::ALU::ApplyInstruction(Instruction instruction)
{
	Operation *&namedVariable = GetVariableReferenceFromString(instruction.inputOne);

	// Change the value of the named variable according to the instruction type we're
	// applying. Where we can instantly resolve the effect of this instruction (like adding
	// zero having no effect, or multiplying by zero guaranteeing a zero result), do so
	// to reduce the final expression complexity.
	bool namedVariableIsZero = namedVariable->HasConstantValue() && (namedVariable->GetValue() == 0);
	bool namedVariableIsOne = namedVariable->HasConstantValue() && (namedVariable->GetValue() == 1);
	bool secondInputIsZero;
	bool secondInputIsOne;
	bool namedVariableGreaterThanNine = namedVariable->HasConstantValue() && (namedVariable->GetValue() > 9);
	bool secondInputGreaterThanNine;

	assert(namedVariable->HasKnownMinMax());

	char nextDigit;
	switch (instruction.type)
	{
	case OperationType::Input:
		nextDigit = GetNextInputDigit();
		if ((nextDigit <= '9') && (nextDigit >= '1'))
		{
			namedVariable = (Operation *)new ValueOperation(nextDigit - '0');
			NewlyAllocatedOperation(namedVariable);
		}
		else
		{
			namedVariable = (Operation *)new InputOperation(nextDigit);
			NewlyAllocatedOperation(namedVariable);
		}
		break;
	case OperationType::Value:
		namedVariable = (Operation *)new ValueOperation(Parsing::ConvertStringToInt(instruction.inputTwo));
		NewlyAllocatedOperation(namedVariable);
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
			NewlyAllocatedOperation(secondInput);
		}
		else
		{
			secondInput = GetVariableReferenceFromString(instruction.inputTwo);
		}
		assert(secondInput->HasKnownMinMax());
		secondInputIsZero = secondInput->HasConstantValue() && (secondInput->GetValue() == 0);
		secondInputIsOne = secondInput->HasConstantValue() && (secondInput->GetValue() == 1);
		switch (instruction.type)
		{
		case OperationType::Add:
			if (namedVariableIsZero)
			{
				namedVariable = secondInput;
			}
			else if (!secondInputIsZero)
			{
				if (namedVariable->HasConstantValue() && secondInput->HasConstantValue())
				{
					namedVariable = (Operation *)new ValueOperation(namedVariable->GetValue() + secondInput->GetValue());
					NewlyAllocatedOperation(namedVariable);
				}
				else
				{
					bool newKnownMinMax{ false };
					long long int newMin;
					long long int newMax;
					if (namedVariable->HasKnownMinMax() && secondInput->HasKnownMinMax())
					{
						newKnownMinMax = true;
						newMin = namedVariable->MinPossibleValue() + secondInput->MinPossibleValue();
						newMax = namedVariable->MaxPossibleValue() + secondInput->MaxPossibleValue();
					}

					namedVariable = (Operation *)new AddOperation(namedVariable, secondInput);
					NewlyAllocatedOperation(namedVariable);

					if (newKnownMinMax)
					{
						namedVariable->SetMinMaxRange(newMin, newMax);
					}
				}
			}
			break;
		case OperationType::Multiply:
			if (secondInputIsZero)
			{
				namedVariable = (Operation *)new ValueOperation(0);
				NewlyAllocatedOperation(namedVariable);
			}
			else if (namedVariableIsOne)
			{
				namedVariable = secondInput;
			}
			else if (!namedVariableIsZero && !secondInputIsOne)
			{
				if (namedVariable->HasConstantValue() && secondInput->HasConstantValue())
				{
					namedVariable = (Operation *)new ValueOperation(namedVariable->GetValue() * secondInput->GetValue());
					NewlyAllocatedOperation(namedVariable);
				}
				else
				{
					bool newKnownMinMax{ false };
					long long int newMin;
					long long int newMax;
					if (namedVariable->HasKnownMinMax() && secondInput->HasKnownMinMax())
					{
						newKnownMinMax = true;
						newMin = namedVariable->MinPossibleValue() * secondInput->MinPossibleValue();
						newMax = namedVariable->MaxPossibleValue() * secondInput->MaxPossibleValue();
					}
					assert(newMax >= 0);
					namedVariable = (Operation *)new MultiplyOperation(namedVariable, secondInput);
					NewlyAllocatedOperation(namedVariable);

					if (newKnownMinMax)
					{
						namedVariable->SetMinMaxRange(newMin, newMax);
					}
				}
			}
			break;
		case OperationType::Divide:
			if (namedVariable->HasKnownMinMax() && secondInput->HasKnownMinMax() &&
				(namedVariable->MaxPossibleValue() < secondInput->MinPossibleValue()))
			{
				// I haven't taken care with negative numbers here, so assert if a range has
				// gone negative so I can come and fix that.
				assert((namedVariable->MaxPossibleValue() >= 0) && (namedVariable->MinPossibleValue() >= 0));
				namedVariable = (Operation *)new ValueOperation(0);
				NewlyAllocatedOperation(namedVariable);
			}
			else if (!namedVariableIsZero && !secondInputIsOne)
			{
				if (namedVariable->HasConstantValue() && secondInput->HasConstantValue())
				{
					namedVariable = (Operation *)new ValueOperation(namedVariable->GetValue() / secondInput->GetValue());
					NewlyAllocatedOperation(namedVariable);
				}
				else
				{
					bool newKnownMinMax{ false };
					long long int newMin;
					long long int newMax;
					if (namedVariable->HasKnownMinMax() && secondInput->HasKnownMinMax())
					{
						// I haven't taken care with negative numbers here, so assert if a range has
						// gone negative so I can come and fix that.
						assert((namedVariable->MinPossibleValue() >= 0) && (namedVariable->MinPossibleValue() >= 0));

						newKnownMinMax = true;
						newMin = namedVariable->MinPossibleValue() / secondInput->MaxPossibleValue();
						newMax = namedVariable->MaxPossibleValue() / secondInput->MinPossibleValue();
					}

					namedVariable = (Operation *)new DivideOperation(namedVariable, secondInput);
					NewlyAllocatedOperation(namedVariable);

					if (newKnownMinMax)
					{
						namedVariable->SetMinMaxRange(newMin, newMax);
					}
				}
			}
			break;
		case OperationType::Modulo:
			if (!namedVariableIsZero)
			{
				if (secondInputIsOne)
				{
					namedVariable = (Operation *)new ValueOperation(0);
					NewlyAllocatedOperation(namedVariable);
				}
				else if (!(namedVariable->HasKnownMinMax() && secondInput->HasKnownMinMax() &&
					(namedVariable->MaxPossibleValue() < secondInput->MinPossibleValue())))
				{
					if (namedVariable->HasConstantValue() && secondInput->HasConstantValue())
					{
						namedVariable = (Operation *)new ValueOperation(namedVariable->GetValue() % secondInput->GetValue());
						NewlyAllocatedOperation(namedVariable);
					}
					else
					{
						bool newKnownMinMax{ false };
						long long int newMin;
						long long int newMax;
						if (secondInput->HasKnownMinMax())
						{
							newKnownMinMax = true;
							newMin = 0;
							newMax = secondInput->MaxPossibleValue() - 1;
						}

						namedVariable = (Operation *)new ModuloOperation(namedVariable, secondInput);
						NewlyAllocatedOperation(namedVariable);

						if (newKnownMinMax)
						{
							namedVariable->SetMinMaxRange(newMin, newMax);
						}
					}
				}
			}
			break;
		case OperationType::Equality:
			secondInputGreaterThanNine = secondInput->HasConstantValue() && (secondInput->GetValue() > 9);
			if ((namedVariableGreaterThanNine && !secondInput->CanBeGreaterThanNine()) ||
				(secondInputGreaterThanNine && !namedVariable->CanBeGreaterThanNine()))
			{
				namedVariable = (Operation *)new ValueOperation(0);
				NewlyAllocatedOperation(namedVariable);
			}
			else if (namedVariable->HasConstantValue() && secondInput->HasConstantValue())
			{
				namedVariable = (Operation *)new ValueOperation(
					(namedVariable->GetValue() == secondInput->GetValue()) ? 1 : 0);
				NewlyAllocatedOperation(namedVariable);
			}
			else if (namedVariable->HasKnownMinMax() && secondInput->HasKnownMinMax() &&
				((namedVariable->MaxPossibleValue() < secondInput->MinPossibleValue()) ||
				(namedVariable->MinPossibleValue() > secondInput->MaxPossibleValue())))
			{
				namedVariable = (Operation *)new ValueOperation(0);
				NewlyAllocatedOperation(namedVariable);
			}
			else
			{
				namedVariable = (Operation *)new EqualityOperation(namedVariable, secondInput);
				NewlyAllocatedOperation(namedVariable);
			}
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
	char digitToReturn;
	if (nextDigitOfString < knownDigitString.size())
	{
		digitToReturn = knownDigitString[nextDigitOfString];
		nextDigitOfString++;
	}
	else
	{
		digitToReturn = nextInputDigit;
		nextInputDigit++;
	}
	return digitToReturn;
}

alu::ALU::ALU(std::vector<Instruction> instructions, std::string knownDigits)
{
	knownDigitString = knownDigits;
	xValue = (Operation *)new ValueOperation(0);
	yValue = (Operation *)new ValueOperation(0);
	wValue = (Operation *)new ValueOperation(0);
	zValue = (Operation *)new ValueOperation(0);
	NewlyAllocatedOperation(xValue);
	NewlyAllocatedOperation(yValue);
	NewlyAllocatedOperation(wValue);
	NewlyAllocatedOperation(zValue);

	for (Instruction instruction : instructions)
	{
		ApplyInstruction(instruction);
	}
}

alu::ALU::~ALU()
{
	for (Operation *operation : allocatedOperations)
	{
		delete operation;
	}
}

std::string alu::ALU::GetVariableExpressionAsString(char variable)
{
	Operation *namedVariable = GetVariableReferenceFromString(std::string(1, variable));
	assert(namedVariable != nullptr);
	return namedVariable->PrintAsString();
}

std::pair<long long int, long long int> alu::ALU::PossibleZRange()
{
	assert(zValue->HasKnownMinMax());
	return { zValue->MinPossibleValue(), zValue->MaxPossibleValue() };
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

void alu::Operation::SetMinMaxRange(long long int min, long long int max)
{
	knownMinMax = true;
	minPossibleValue = min;
	maxPossibleValue = max;
}

alu::ValueOperation::ValueOperation(long long int val)
{
	value = val;
	knownMinMax = true;
	minPossibleValue = val;
	maxPossibleValue = val;
}
