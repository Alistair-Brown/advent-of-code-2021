#pragma once
#include <utility>
#include <string>
#include "Parsing.h"

namespace Snail
{
	class Number 
	{
	public:
		virtual int Magnitude() = 0;
		virtual std::string ValueAsString() = 0;
		virtual void IncreaseLeftValue(int increase) = 0;
		virtual void IncreaseRightValue(int increase) = 0;
		virtual bool MaybeExplode(int depth, std::pair<int, int> &pairToExplode) = 0;
		virtual bool MaybeSplit(Number *& newNumber) = 0;
	};
	
	class RegularNumber : Number
	{
	private:
		int value;
	public:
		RegularNumber(int value) : value{ value } {};
		~RegularNumber();
		int Magnitude() { return value; }
		void IncreaseLeftValue(int increase) {value += increase; }
		void IncreaseRightValue(int increase) {value += increase; }
		std::string ValueAsString() { return std::to_string(value); }

		bool MaybeExplode(int depth, std::pair<int, int> &pairToExplode);
		bool MaybeSplit(Number *& newNumber);
	};

	class SnailfishNumber : Number
	{
	private:
		std::pair<Number *, Number *> numberPair;
	public:
		SnailfishNumber() : numberPair {nullptr, nullptr} {};
		SnailfishNumber(std::pair<Number *, Number *> numberPairIn) : numberPair{ numberPairIn } {};
		SnailfishNumber(std::pair<std::string, std::string> numberPairIn);
		~SnailfishNumber();
		int Magnitude();
		std::string ValueAsString();
		void IncreaseLeftValue(int increase) {numberPair.first->IncreaseLeftValue(increase); }
		void IncreaseRightValue(int increase) { numberPair.second->IncreaseRightValue(increase); }

		bool MaybeExplode(int depth, std::pair<int, int> &pairToExplode);
		bool MaybeSplit(Number *& newNumber);
	};

	std::pair<std::string, std::string> SplitStringIntoSnailfishPair(std::string completeSnailfishNumber);
	SnailfishNumber *AddSnailfishNumbers(SnailfishNumber *numberOne, SnailfishNumber *numberTwo);
}