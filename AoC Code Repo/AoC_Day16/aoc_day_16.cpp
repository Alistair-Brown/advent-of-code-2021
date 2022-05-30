#include "Parsing.h"
#include "packet_parser.h"
#include "puzzle_solvers.h"

PuzzleAnswerPair PuzzleSolvers::AocDaySixteenSolver(std::ifstream& puzzleInputFile)
{
	std::string hexString = Parsing::ReadSingleStringFromInputFile(puzzleInputFile);
	std::string binaryString = Parsing::ConvertHexStringToBinary(hexString);

	Packet::OperatorPacket outerPacket{ Packet::OperatorPacket(binaryString) };

	unsigned long sumOfVersion = outerPacket.SumOfVersions();
	ULLINT packetValue = outerPacket.Value();
	return PuzzleAnswerPair{ std::to_string(sumOfVersion), std::to_string(packetValue) };
}