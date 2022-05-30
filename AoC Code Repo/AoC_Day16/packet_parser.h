#pragma once
#include <string>
#include <vector>

namespace Packet
{
	class Packet
	{
	protected:
		int version;
		int typeID;
	public:
		virtual int SumOfVersions() = 0;
		virtual unsigned long long int Value() = 0;
		int Version() { return version; }
		int TypeID() { return typeID; }
	};

	class LiteralPacket : Packet
	{
	private:
		unsigned long long int value;
	public:
		LiteralPacket(int versionIn, int typeIDIn, std::string &contents);
		int SumOfVersions() { return version; }
		unsigned long long int Value() { return value; }
	};

	class OperatorPacket : Packet
	{
	private:
		int lengthTypeID;
		int length;
		std::vector<Packet *> contents{};

		void ParseNextChildPacket(std::string &packets);
		void ParsePacket(int versionIn, int typeIDIn, std::string &contents);
		void ParseContents(std::string &contents);

		unsigned long long int SumOperation();
		unsigned long long int ProductOperation();
		unsigned long long int MinimumOperation();
		unsigned long long int MaximumOperation();
		unsigned long long int GreaterThanOperation();
		unsigned long long int LessThanOperation();
		unsigned long long int EqualToOperation();

		static const int sumID{ 0 };
		static const int productID{ 1 };
		static const int minimumID{ 2 };
		static const int maximumID{ 3 };
		static const int literalID{ 4 };
		static const int greaterThanID{ 5 };
		static const int lessThanID{ 6 };
		static const int equalToOperation{ 7 };
	public:
		OperatorPacket(std::string completePacket);
		OperatorPacket(int versionIn, int typeIDIn, std::string &contents) { ParsePacket(versionIn, typeIDIn, contents); }
		~OperatorPacket();
		unsigned long long int Value();
		int SumOfVersions();
	};
}