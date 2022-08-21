#include "alu.h"
#include <cassert>
#include <array>
#include <set>
#include <iostream>
#include "aoc_common_types.h"

// The valid inputs for each stage of the ALU can be found by working backwards through the stages.
// We know that any valid inputs to the final stage are those that leave the z register with the
// value 0. Having found the valid inputs to that final stage, we can than find all possible inputs
// to the previous stage that have these as their output (the output to one stage being the input
// to the next). And so on until we have all valid model numbers, a model number being the 14
// digits representing the input fed into the w register for each of the 14 stages.
ALU::ArithmeticLogicUnit::ArithmeticLogicUnit(std::vector<ALUStage> stagesIn)
{
	stages = stagesIn;
	stages.rbegin()->CalculateAllowableInputs({ {0, {}} });
	for (int ii = 12; ii >= 0; ii--)
	{
		stages[ii].CalculateAllowableInputs(stages[ii + 1].AllowableInputs());
	}
}

// All registers in the ALU start with the value 0. So we take the valid input for
// the first stage that has a z input of 0, and query the 14 maximum value w inputs
// that result in a valid model number.
std::string ALU::ArithmeticLogicUnit::LargestModelNumber() const
{
	assert(stages[0].AllowableInputs().contains(0));

	std::string modelNumber{};
	for (int digit : stages[0].AllowableInputs().at(0).maximumWInputs)
	{
		char intAsChar = '0' + digit;
		modelNumber.append(std::string{ intAsChar });
	}
	return modelNumber;
}

// All registers in the ALU start with the value 0. So we take the valid input for
// the first stage that has a z input of 0, and query the 14 minimum value w inputs
// that result in a valid model number.
std::string ALU::ArithmeticLogicUnit::SmallestModelNumber() const
{
	assert(stages[0].AllowableInputs().contains(0));

	std::string modelNumber{};	
	for (int digit : stages[0].AllowableInputs().at(0).minimumWInputs)
	{
		char intAsChar = '0' + digit;
		modelNumber.append(std::string{ intAsChar });
	}
	return modelNumber;
}

// This functions relies heavily on having some algebra fun with the operations in a single stage. I won't
// try and replicate this algebra in the comments, but the essence of this function is:
//  - Rely on the fact that there are always a series of operations performed on the x register that leave
//    it with the value 0 or 1 about halfway through the stage
//  - Use some handworked algebra to check that the xValueAfterModuloOps is consistent with the required
//    output z value and input w. If not, then this function call doesn't represent a reachable state and
//    can be ignored (so return an empty vector of allowable z values).
//  - For a required output z value, given input w value, and value of x after the module operations (0 or 1),
//    use algebra to find the value of initial z for the stage divided by the stage's zDivValue (one of the
//    variables unique to each stage).
//  - Having found the result of integer division of input z by zDivValue, we have a number of possible
//    input z values. Of those, the ones that are actual valid input values are those that are consistent
//    with the value of xAfterModuloOps (more handworked algebra) and greater than or equal to 0 (for
//    validity with one of the module operations in the stage).
std::vector<LLINT> ALU::ALUStage::InputZToGiveRequiredOutput(LLINT requiredOutputZ, int inputW, int xAterModuloOps)
{
	std::vector<LLINT> allowableZ{};
	if (((requiredOutputZ - ((inputW + yAddValue) * xAterModuloOps)) % ((25 * xAterModuloOps) + 1)) != 0) { return allowableZ; }

	LLINT inputZOverZDiv =
		(requiredOutputZ - ((inputW + yAddValue) * xAterModuloOps)) / ((25 * xAterModuloOps) + 1);	

	if (inputZOverZDiv >= 0)
	{
		for (LLINT possibleZ = inputZOverZDiv * zDivValue; possibleZ < (inputZOverZDiv * zDivValue) + zDivValue; possibleZ++)
		{
			if (possibleZ < 0) { continue; }

			int actualXValueAfterModuloFun = ((possibleZ % 26) + xAddValue) == inputW ? 0 : 1;

			if (actualXValueAfterModuloFun == xAterModuloOps)
			{
				allowableZ.push_back(possibleZ);
			}
		}
	}

	return allowableZ;
}

// Given a set of allowable outputs to a stage, find the equivalent map of allowable inputs. The final z
// value of a stage is a function of only the input w and z values (from manual examination of input).
// The allowable outputs are provided as a map keyed by valid values to leave in the z register at the
// end of this stage, with the value of each element then providing lists of w inputs to provide to
// subsequent stages to leave the z register with a final value of 0.
// The allowable inputs therefore take the form of an equivalent map, where the keys are now the valid
// values of the z register at the input to this stage, and the lists of w inputs have a single w input
// prepended to the existing lists representing the w input for this stage for that particular z value.
void ALU::ALUStage::CalculateAllowableInputs(std::map<LLINT, RemainingWInputs> const& allowableOutputs)
{
	if (stageCalculationsRun) { return; }

	// Some handworked algebra has shown that we can find the valid z input given a desired z output,
	// known w input, and value of an 'x modulo coefficient' (the value of the x register after a series
	// of operations culminating in a comparison with 0 that leave it with the value 0 or 1). So a series
	// of for loops iterating over each of those factors gives the full set of valid z inputs for this
	// stage, along with the accompanying w inputs.
	for (auto &allowableOutput : allowableOutputs)
	{
		for (int wIn = 1; wIn <= 9; wIn++)
		{
			for (int xModuloCoefficient = 0; xModuloCoefficient <= 1; xModuloCoefficient++)
			{
				std::vector<LLINT> inputZs = InputZToGiveRequiredOutput(allowableOutput.first, wIn, xModuloCoefficient);

				// If we have found valid input z values (the value of the z register on entry to this
				// stage), add them to our map of allowable inputs. The new lists of w inputs are simply
				// the w inputs for the subsequent stages, with whatever w input we are currently checking
				// prepended to that list.
				for (LLINT &possibleZ : inputZs)
				{
					std::list newMinimumWInputs = allowableOutput.second.minimumWInputs;
					std::list newMaximumWInputs = allowableOutput.second.maximumWInputs;
					newMinimumWInputs.push_front(wIn);
					newMaximumWInputs.push_front(wIn);

					// If we already have an entry in our map for this input z value, only the
					// maximum w inputs list needs to be updated for that entry, since we work
					// through the w inputs in increasing value in this function, so we must
					// have already found a smaller valid w input for this z input.
					if (allowableInputs.contains(possibleZ))
					{
						allowableInputs[possibleZ].maximumWInputs = newMaximumWInputs;
					}
					else
					{
						allowableInputs[possibleZ] = {newMinimumWInputs, newMaximumWInputs};
					}
				}
			}
		}
	}

	stageCalculationsRun = true;
}
