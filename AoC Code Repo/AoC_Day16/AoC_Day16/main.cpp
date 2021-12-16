#include "Parsing.h"
#include "PacketParser.h"

int main()
{
	std::cout << "Enter hex string" << std::endl;

	std::string hexString = Parsing::ParseStringFromConsoleIgnoringLineBreaks();
	std::string binaryString = Parsing::ConvertHexStringToBinary(hexString);

	std::cout << "Output binary is:\n" << binaryString << std::endl;

	Packet::OperatorPacket outerPacket{ Packet::OperatorPacket(binaryString) };

	std::cout << "Sum of versions is " << outerPacket.SumOfVersions() << std::endl;
	std::cout << "Value of packet is " << outerPacket.Value() << std::endl;
}