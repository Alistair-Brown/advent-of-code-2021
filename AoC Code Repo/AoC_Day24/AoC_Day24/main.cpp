#include "alu.h"
#include <iostream>
#include "Parsing.h"

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

	alu::ALU arithmeticLogicUnit{ instructions };
	std::cout << "x: " << arithmeticLogicUnit.GetVariableExpressionAsString('x') << std::endl;
	std::cout << "y: " << arithmeticLogicUnit.GetVariableExpressionAsString('y') << std::endl;
	std::cout << "w: " << arithmeticLogicUnit.GetVariableExpressionAsString('w') << std::endl;
	std::cout << "z: " << arithmeticLogicUnit.GetVariableExpressionAsString('z') << std::endl;
}