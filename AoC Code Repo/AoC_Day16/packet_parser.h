#pragma once
#include <string>
#include <vector>
#include "aoc_common_types.h"
#include <memory>

namespace Packet
{
	// Abstract class representing those features common to all packet types, specifically:
	//  - A version number
	//  - A type ID
	//  - A value
	class Packet
	{
	protected:
		int version{};
		int typeID{};
	public:
		virtual int SumOfVersions() const = 0;
		virtual ULLINT Value() const = 0;
		int Version() const { return version; }
		int TypeID() const { return typeID; }

		virtual ~Packet() = default;
	};

	// A literal packet contains no futher nested packets, and just has its own
	// value. Therefore the sum of all versions of this packet and its children
	// is just this packet's version, and there's no complex computation
	// required to calculate a value based on child packets.
	class LiteralPacket : public Packet
	{
	private:
		ULLINT value;
	public:
		LiteralPacket(int versionIn, int typeIDIn, std::string &contents);
		~LiteralPacket() override = default;

		int SumOfVersions() const override { return version; }
		ULLINT Value() const override { return value; }
	};

	// An operator packet contains child packets, and has a value which is determined
	// by some operation performed on the values of its child packets. The exact operation
	// to be performed depends on the typeID of this operator packet.
	class OperatorPacket : public Packet
	{
	private:
		int lengthTypeID;
		unsigned int lengthDescriptor;
		std::vector<std::unique_ptr<Packet>> contents{};

		void ParseNextChildPacket(std::string &packets);
		void ParseContents(std::string &contents);

		ULLINT SumOperation() const;
		ULLINT ProductOperation() const;
		ULLINT MinimumOperation() const;
		ULLINT MaximumOperation() const;
		ULLINT GreaterThanOperation() const;
		ULLINT LessThanOperation() const;
		ULLINT EqualToOperation() const;

		static const int sumID{ 0 };
		static const int productID{ 1 };
		static const int minimumID{ 2 };
		static const int maximumID{ 3 };
		static const int literalID{ 4 };
		static const int greaterThanID{ 5 };
		static const int lessThanID{ 6 };
		static const int equalToOperation{ 7 };
	public:
		static OperatorPacket CreateOperatorPacket(std::string& completePacket);
		OperatorPacket(int versionIn, int typeIDIn, std::string& contents);
		~OperatorPacket() override = default;

		ULLINT Value() const override;
		int SumOfVersions() const override;
	};
}