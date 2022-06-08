#include "Parsing.h"
#include "packet_parser.h"
#include "puzzle_solvers.h"

// Day 16 asks us to parse out a packet of a fiddly format defined by the puzzle rules
// (Packet here in the sense of 'IP packet' or 'UDP packet', except this is a funky
// type of packet defined for this puzzle of course). Packets can be literal values,
// or 'operator' packets which contain further nested packets. So the puzzle input
// is a single operator packet, which contains futher packets nested within it (some
// of which have further nested packets).
// Part one asks to evaluate the version (every packet in the format defined by this
// puzzle uses the first 3 bits of its header to define its 'version') of every packet
// contained within the outer packet, taking the versions of all of the nested packets
// into account. Part 2 asks to evaluate the value of the outer operator packet, using
// various rules for calculating the value of the different packet 'types'.
PuzzleAnswerPair PuzzleSolvers::AocDaySixteenSolver(std::ifstream& puzzleInputFile)
{
	// Input is given as a hex string, but the packet is defined in terms of irregular
	// chunks of binary rather than nice bytes, so the first task is to parse the hex out
	// into binary.
	std::string hexString = Parsing::ReadSingleStringFromInputFile(puzzleInputFile);
	std::string binaryString = Parsing::ConvertHexStringToBinary(hexString);

	// Then we can parse this out into the complete packet, and retrieve the sum of versions
	// and the packet's value.
	Packet::OperatorPacket outerPacket = Packet::OperatorPacket::CreateOperatorPacket(binaryString);
	unsigned int sumOfVersion = outerPacket.SumOfVersions();
	ULLINT packetValue = outerPacket.Value();

	return PuzzleAnswerPair{ std::to_string(sumOfVersion), std::to_string(packetValue) };
}