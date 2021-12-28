#pragma once
#include <string>
#include <vector>

namespace alu
{
	enum OperationType {Input, Value, Add, Multiply, Divide, Modulo, Equality};

	struct Instruction
	{
		OperationType type;
		std::string inputOne;
		std::string inputTwo;

		Instruction() : Instruction(OperationType::Input, "", "") {};
		Instruction(OperationType type, std::string inputOne, std::string inputTwo) :
			type{ type }, inputOne{ inputOne }, inputTwo{ inputTwo } {};
	};

	Instruction CreateInstructionFromStringLine(std::vector<std::string>);

	class Operation
	{
	public:
		virtual long long int GetValue() = 0;
		virtual std::string PrintAsString() = 0;
	};

	class InputOperation : Operation
	{
	private:
		char inputDigit; // 'a' for most significant, 'b' for next etc
	public:
		InputOperation(char inputDigit) : inputDigit{ inputDigit } {};

		long long int GetValue() { return 0; }
		std::string PrintAsString() { return std::string(1, inputDigit); }
	};

	class ValueOperation : Operation
	{
	private:
		long long int value;
	public:
		ValueOperation(long long int value) : value{ value } {};

		long long int GetValue() { return 0; }
		std::string PrintAsString() { return std::to_string(value); }
	};

	class AddOperation : Operation
	{
	private:
		Operation *valueOne;
		Operation *valueTwo;
	public:
		AddOperation(Operation *valueOne, Operation *valueTwo) : valueOne{ valueOne }, valueTwo{ valueTwo } {};

		long long int GetValue() { return 0; }
		std::string PrintAsString() { 
			return "( " + valueOne->PrintAsString() + " + " + valueTwo->PrintAsString() + " )"; }
	};

	class MultiplyOperation : Operation
	{
	private:
		Operation *valueOne;
		Operation *valueTwo;
	public:
		MultiplyOperation(Operation *valueOne, Operation *valueTwo) : valueOne{ valueOne }, valueTwo{ valueTwo } {};

		long long int GetValue() { return 0; }
		std::string PrintAsString() {
			return "( " + valueOne->PrintAsString() + " * " + valueTwo->PrintAsString() + " )";
		}
	};

	class DivideOperation : Operation
	{
	private:
		Operation *valueOne;
		Operation *valueTwo;
	public:
		DivideOperation(Operation *valueOne, Operation *valueTwo) : valueOne{ valueOne }, valueTwo{ valueTwo } {};

		long long int GetValue() { return 0; }
		std::string PrintAsString() {
			return "( " + valueOne->PrintAsString() + " / " + valueTwo->PrintAsString() + " )";
		}
	};

	class ModuloOperation : Operation
	{
	private:
		Operation *valueOne;
		Operation *valueTwo;
	public:
		ModuloOperation(Operation *valueOne, Operation *valueTwo) : valueOne{ valueOne }, valueTwo{ valueTwo } {};

		long long int GetValue() { return 0; }
		std::string PrintAsString() {
			return "( " + valueOne->PrintAsString() + " % " + valueTwo->PrintAsString() + " )";
		}
	};

	class EqualityOperation : Operation
	{
	private:
		Operation *valueOne;
		Operation *valueTwo;
	public:
		EqualityOperation(Operation *valueOne, Operation *valueTwo) : valueOne{ valueOne }, valueTwo{ valueTwo } {};

		long long int GetValue() { return 0; }
		std::string PrintAsString() {
			return "( " + valueOne->PrintAsString() + " == " + valueTwo->PrintAsString() + " )";
		}
	};

	class ALU
	{
	private:
		// TODO: I currently leak lots of memory with these pointers. I need to find a way
		// of freeing unreferenced operations, while being sure they really are unreferenced
		// i.e. I can't just free the xValue when reassigning xValue, because I might have used
		// it as the input for one of the other operations.
		Operation *xValue;
		Operation *yValue;
		Operation *wValue;
		Operation *zValue;
		Operation *invalid{ nullptr };
		char nextInputDigit{ 'a' };

		void ApplyInstruction(Instruction instruction);
		Operation *&GetVariableReferenceFromString(std::string variableName);
		char GetNextInputDigit();
	public:
		ALU(std::vector<Instruction> instructions);
		std::string GetVariableExpressionAsString(char variable);
	};
}