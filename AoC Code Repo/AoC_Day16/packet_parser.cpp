#include "packet_parser.h"
#include "Parsing.h"
#include <cassert>
#include <iostream>

// Given the string representing an entire operator packet (not having parsed out the version
// and typeID), return that OperatorPacket.
Packet::OperatorPacket Packet::OperatorPacket::CreateOperatorPacket(std::string &completePacket)
{
	int versionIn = static_cast<int>(Parsing::ConvertBinaryToInt(completePacket.substr(0, 3)));
	completePacket.erase(0, 3);

	int typeIDIn = static_cast<int>(Parsing::ConvertBinaryToInt(completePacket.substr(0, 3)));
	completePacket.erase(0, 3);
	assert(typeIDIn != literalID);

	return OperatorPacket(versionIn, typeIDIn, completePacket);
}

// Construct an OperatorPacket object given a version, typeID, and the packet contents as
// a string of binary.
Packet::OperatorPacket::OperatorPacket(int versionIn, int typeIDIn, std::string& contents)
{
	version = versionIn;
	typeID = typeIDIn;

	// The contents of an operator packet opens with a length header, which provides
	// the length of the remaining contents in one of 2 ways. The first bit of the
	// length header is the length type ID, with the remaining bits of the length header
	// forming the length descriptor. If the length type ID is 1, the length descriptor
	// is 11 bits long, otherwise the length type ID is 0, and the length descriptor is
	// 15 bits long. The length descriptor is just stored raw here, it will be parsed
	// properly as part of parsing the packet contents.
	lengthTypeID = static_cast<int>(Parsing::ConvertBinaryToInt(contents.substr(0, 1)));
	contents.erase(0, 1);
	if (lengthTypeID == 1)
	{
		lengthDescriptor = static_cast<unsigned int>(Parsing::ConvertBinaryToInt(contents.substr(0, 11)));
		contents.erase(0, 11);
	}
	else
	{
		assert(lengthTypeID == 0);
		lengthDescriptor = static_cast<unsigned int>(Parsing::ConvertBinaryToInt(contents.substr(0, 15)));
		contents.erase(0, 15);
	}

	ParseContents(contents);
}

// Parse out the contents of this OperatorPacket into the respective child packets.
// We use the length descriptor to work out how long this OperatorPacket is and
// only parse out as much of the contents string as is required to form that contents.
void Packet::OperatorPacket::ParseContents(std::string& contents)
{
	// If the length type ID is 0, the length descriptor represents the length of this
	// packet's contents in bits. So keep parsing child packets off the front of the contents
	// string until we've read the right number of bits.
	if (lengthTypeID == 0)
	{
		ULLINT initialSize = contents.size();
		while (contents.size() > (initialSize - lengthDescriptor))
		{
			ParseNextChildPacket(contents);
		}
		assert(contents.size() == initialSize - lengthDescriptor);
	}
	// Otherwise the type ID is 0, and the length descriptor represents the number of
	// child packets within this operator pacekt, so just parse out that many child
	// packets.
	else
	{
		for (unsigned int ii = 0; ii < lengthDescriptor; ii++)
		{
			ParseNextChildPacket(contents);
		}
	}
}

// Parse out the next child packet of this Operator packet from the contents string.
// This will remove the portion of the contents string representing this child packet
// (including any of its own children), so it is ready to have the next packet parsed
// from it.
// We first strip out the 3 bits each for the version and typeID of this child packet.
// Depending on the typeID, that child may be either a Literal or OperatorPacket, and
// we should create it as such.
void Packet::OperatorPacket::ParseNextChildPacket(std::string & packets)
{
	int childVersion = (int)Parsing::ConvertBinaryToInt(packets.substr(0, 3));
	packets.erase(0, 3);

	int childTypeID = (int)Parsing::ConvertBinaryToInt(packets.substr(0, 3));
	packets.erase(0, 3);

	if (childTypeID == literalID)
	{
		contents.emplace_back(std::make_unique<LiteralPacket>(childVersion, childTypeID, packets));
	}
	else
	{
		contents.emplace_back(std::make_unique<OperatorPacket>(childVersion, childTypeID, packets));
	}
}

// The method for getting the Value of an operator packet depends on what type of operation
// this packet represents. I could have used further subclassing and factories to avoid this
// switch, but it's a single switch statement, let's not over-engineer to too far an extreme.
ULLINT Packet::OperatorPacket::Value() const
{
	ULLINT value;

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

// To find the sum of all the version numbers for packets contained within an
// operator packet, including all nested packets, we simply request the SumOfVersions
// for every packet contained directly within this one (i.e. not the packets
// nested within those). The literal packets will return just their own version,
// while other operator packets will similarly get the SumOfVersions for each of
// their packets, allowing us to recurse through any level of nesting.
int Packet::OperatorPacket::SumOfVersions() const
{
	int sumOfVersions = version;
	for (std::unique_ptr<Packet> const &packet : contents)
	{
		sumOfVersions += packet->SumOfVersions();
	}
	return sumOfVersions;
}

// 'Literal' packets contain no inner packets, their value can be deduced through a
// simple parsing of the binary string making up their contents.
// This constructor produces such a LiteralPacket, parsing that value from the
// provided 'contents' string. It is possible that this contents string has further
// packets after the literal contents, but this constructor only reads as much as
// is required to get the literal value. The binary making up the literal will be stripped
// from the contents as it is parsed, so that contents is left ready to have further
// packets parsed from it.
Packet::LiteralPacket::LiteralPacket(int versionIn, int typeIDIn, std::string & contents)
{
	version = versionIn;
	typeID = typeIDIn;

	// Parse contents as per puzzle rules. The binary number representing the value
	// is broken up into 4 bit chunks. Each of these chunks is prefixed with a '1',
	// except for the final one, which is prefixed with '0'. We use this knowledge
	// to extract the binary number from the front of the contents string until we
	// reach the final chunk, indicated by its 0 prefix. Contents is stripped as
	// we go.
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

// Return the sum of the values of this packet's contents.
ULLINT Packet::OperatorPacket::SumOperation() const
{
	ULLINT value{ 0 };
	for (std::unique_ptr<Packet> const& packet : contents)
	{
		value += packet->Value();
	}
	return value;
}

// Return the product of the values of this packet's contents.
ULLINT Packet::OperatorPacket::ProductOperation() const
{
	ULLINT value{ 1 };
	for (std::unique_ptr<Packet> const& packet : contents)
	{
		value *= packet->Value();
	}
	return value;
}

// Return the lowest of the respective values of this packet's contents.
ULLINT Packet::OperatorPacket::MinimumOperation() const
{
	ULLINT value{ UINT64_MAX };
	for (std::unique_ptr<Packet> const& packet : contents)
	{
		ULLINT packetValue = packet->Value();
		if (packetValue < value) { value = packetValue; }
	}
	return value;
}

// Return the greatest of the respective values of this packet's contents.
ULLINT Packet::OperatorPacket::MaximumOperation() const
{
	ULLINT value{ 0 };
	for (std::unique_ptr<Packet> const& packet : contents)
	{
		ULLINT packetValue = packet->Value();
		if (packetValue > value) { value = packetValue; }
	}
	return value;
}

// Return 1 if the value of the first packet contained within this one is greater than
// the value of the second, 0 otherwise. Requires that this packet contains only 2
// inner packets.
ULLINT Packet::OperatorPacket::GreaterThanOperation() const
{
	assert(contents.size() == 2);
	ULLINT packetValue1 = contents[0]->Value();
	ULLINT packetValue2 = contents[1]->Value();
	ULLINT rc = packetValue1 > packetValue2 ? 1 : 0;
	return rc;
}

// Return 1 if the value of the first packet contained within this one is less than
// the value of the second, 0 otherwise. Requires that this packet contains only 2
// inner packets.
ULLINT Packet::OperatorPacket::LessThanOperation() const
{
	assert(contents.size() == 2);
	ULLINT packetValue1 = contents[0]->Value();
	ULLINT packetValue2 = contents[1]->Value();
	ULLINT rc = packetValue1 < packetValue2 ? 1 : 0;
	return rc;
}

// Return 1 if the value of the first packet contained within this one is equal to
// the value of the second, 0 otherwise. Requires that this packet contains only 2
// inner packets.
ULLINT Packet::OperatorPacket::EqualToOperation() const
{
	assert(contents.size() == 2);
	ULLINT packetValue1 = contents[0]->Value();
	ULLINT packetValue2 = contents[1]->Value();
	ULLINT rc = packetValue1 == packetValue2 ? 1 : 0;
	return rc;
}