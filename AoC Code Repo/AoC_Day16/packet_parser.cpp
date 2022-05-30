#include "packet_parser.h"
#include "Parsing.h"
#include <cassert>
#include <iostream>

void Packet::OperatorPacket::ParseNextChildPacket(std::string & packets)
{
	int childVersion = (int)Parsing::ConvertBinaryToInt(packets.substr(0, 3));
	packets.erase(0, 3);

	int childTypeID = (int)Parsing::ConvertBinaryToInt(packets.substr(0, 3));
	packets.erase(0, 3);
	
	Packet *childPacket;

	if (childTypeID == literalID)
	{
		childPacket = (Packet *) new LiteralPacket(childVersion, childTypeID, packets);
	}
	else
	{
		childPacket = (Packet *) new OperatorPacket(childVersion, childTypeID, packets);
	}

	contents.push_back(childPacket);
}

void Packet::OperatorPacket::ParsePacket(int versionIn, int typeIDIn, std::string & contents)
{
	version = versionIn;
	typeID = typeIDIn;

	lengthTypeID = (int)Parsing::ConvertBinaryToInt(contents.substr(0, 1));
	contents.erase(0, 1);

	if (lengthTypeID == 0)
	{
		length = (int)Parsing::ConvertBinaryToInt(contents.substr(0, 15));
		contents.erase(0, 15);
	}
	else
	{
		assert(lengthTypeID == 1);
		length = (int)Parsing::ConvertBinaryToInt(contents.substr(0, 11));
		contents.erase(0, 11);
	}

	ParseContents(contents);
}

void Packet::OperatorPacket::ParseContents(std::string & contents)
{
	if (lengthTypeID == 0)
	{
		unsigned long long int initialSize = contents.size();
		while (contents.size() > (initialSize - length))
		{
			ParseNextChildPacket(contents);
		}
	}
	else
	{
		for (int ii = 0; ii < length; ii++)
		{
			ParseNextChildPacket(contents);
		}
	}
}

unsigned long long int Packet::OperatorPacket::SumOperation()
{
	unsigned long long int value{ 0 };
	for (Packet * packet : contents)
	{
		value += packet->Value();
	}
	return value;
}

unsigned long long int Packet::OperatorPacket::ProductOperation()
{
	unsigned long long int value{ 1 };
	for (Packet * packet : contents)
	{
		value *= packet->Value();
	}
	return value;
}

unsigned long long int Packet::OperatorPacket::MinimumOperation()
{
	unsigned long long int value{ UINT64_MAX };
	for (Packet * packet : contents)
	{
		unsigned long long int packetValue = packet->Value();
		if (packetValue < value) { value = packetValue; }
	}
	return value;
}

unsigned long long int Packet::OperatorPacket::MaximumOperation()
{
	unsigned long long int value{ 0 };
	for (Packet * packet : contents)
	{
		unsigned long long int packetValue = packet->Value();
		if (packetValue > value) { value = packetValue; }
	}
	return value;
}

unsigned long long int Packet::OperatorPacket::GreaterThanOperation()
{
	assert(contents.size() == 2);
	unsigned long long int packetValue1 = contents[0]->Value();
	unsigned long long int packetValue2 = contents[1]->Value();
	unsigned long long int rc = packetValue1 > packetValue2 ? 1 : 0;
	return rc;
}

unsigned long long int Packet::OperatorPacket::LessThanOperation()
{
	assert(contents.size() == 2);
	unsigned long long int packetValue1 = contents[0]->Value();
	unsigned long long int packetValue2 = contents[1]->Value();
	unsigned long long int rc = packetValue1 < packetValue2 ? 1 : 0;
	return rc;
}

unsigned long long int Packet::OperatorPacket::EqualToOperation()
{
	assert(contents.size() == 2);
	unsigned long long int packetValue1 = contents[0]->Value();
	unsigned long long int packetValue2 = contents[1]->Value();
	unsigned long long int rc = packetValue1 == packetValue2 ? 1 : 0;
	return rc;
}

Packet::OperatorPacket::OperatorPacket(std::string completePacket)
{
	int versionIn = (int)Parsing::ConvertBinaryToInt(completePacket.substr(0, 3));
	completePacket.erase(0, 3);

	int typeIDIn = (int)Parsing::ConvertBinaryToInt(completePacket.substr(0, 3));
	completePacket.erase(0, 3);
	// We shouldn't have called the operator packet constructor on a literal packet
	assert(typeIDIn != literalID);

	ParsePacket(versionIn, typeIDIn, completePacket);
}

Packet::OperatorPacket::~OperatorPacket()
{
	for (Packet * packet : contents)
	{
		delete packet;
	}
}

unsigned long long int Packet::OperatorPacket::Value()
{
	unsigned long long int value;

	switch (typeID)
	{
	case sumID:
		value = SumOperation();
		break;
	case productID:
		value = ProductOperation();
		break;
	case minimumID:
		value = MinimumOperation();
		break;
	case maximumID:
		value = MaximumOperation();
		break;
	case greaterThanID:
		value = GreaterThanOperation();
		break;
	case lessThanID:
		value = LessThanOperation();
		break;
	case equalToOperation:
		value = EqualToOperation();
		break;
	default:
		assert(false);
		break;
	}
	return value;
}

int Packet::OperatorPacket::SumOfVersions()
{
	int sumOfVersions = version;
	for (Packet *packet : contents)
	{
		sumOfVersions += packet->SumOfVersions();
	}
	return sumOfVersions;
}

Packet::LiteralPacket::LiteralPacket(int versionIn, int typeIDIn, std::string & contents)
{
	version = versionIn;
	typeID = typeIDIn;

	std::string valueAsBinary;

	bool reachedEnd{ false };
	while (!reachedEnd)
	{
		if (Parsing::ConvertBinaryToInt(contents.substr(0, 1)) == 0) { reachedEnd = true; }
		contents.erase(0, 1);

		valueAsBinary += contents.substr(0, 4);
		contents.erase(0, 4);
	}

	value = Parsing::ConvertBinaryToInt(valueAsBinary);
}
