#pragma once
#include <utility>
#include <string>
#include "Parsing.h"
#include <memory>

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
		virtual bool MaybeSplit(std::unique_ptr<Number> &newNumber) = 0;

		virtual ~Number() = default;
	};
	
	class RegularNumber : public Number
	{
	private:
		int value;
	public:
		RegularNumber(int value) : value{ value } {};

		int Magnitude() { return value; }
		void IncreaseLeftValue(int increase) {value += increase; }
		void IncreaseRightValue(int increase) {value += increase; }
		std::string ValueAsString() { return std::to_string(value); }

		bool MaybeExplode(int depth, std::pair<int, int> &pairToExplode);
		bool MaybeSplit(std::unique_ptr<Number> &newNumber);

		~RegularNumber() override = default;
	};

	class SnailfishNumber : public Number
	{
	private:
		std::pair<std::unique_ptr<Number>, std::unique_ptr<Number>> numberPair;
	public:
		SnailfishNumber() : numberPair {nullptr, nullptr} {};
		SnailfishNumber(std::pair<std::unique_ptr<Number>, std::unique_ptr<Number>> numberPairIn) : numberPair{ std::move(numberPairIn) } {};
		SnailfishNumber(std::pair<std::string, std::string> numberPairIn);
		
		int Magnitude();
		std::string ValueAsString();
		void IncreaseLeftValue(int increase) {numberPair.first->IncreaseLeftValue(increase); }
		void IncreaseRightValue(int increase) { numberPair.second->IncreaseRightValue(increase); }

		bool MaybeExplode(int depth, std::pair<int, int> &pairToExplode);
		bool MaybeSplit(std::unique_ptr<Number> &newNumber);

		~SnailfishNumber() override = default;
	};

	std::pair<std::string, std::string> SplitStringIntoSnailfishPair(std::string completeSnailfishNumber);
	std::unique_ptr<SnailfishNumber> AddSnailfishNumbers(
		std::unique_ptr<SnailfishNumber> numberOne,
		std::unique_ptr<SnailfishNumber> numberTwo);
}