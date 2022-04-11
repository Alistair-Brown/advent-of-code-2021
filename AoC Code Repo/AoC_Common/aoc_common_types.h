#pragma once
#include <utility>
#include <string>
#include <fstream>
// Defines a number of common types for general utility across the project (as opposed to
// types that are specific to individual puzzles, or to activities like parsing).

// To allow us to iterate over the solvers for each day's puzzle, we'll define a type
// that represents the role of a puzzle solver. We'll then be able to set up an array
// of these function pointers.
// Each day's puzzle will take a output filestream to that day's input, and return a pair
// of answers (one for each part of the puzzle).
using PuzzleAnswerPair = std::pair<std::string, std::string>;
using PuzzleFunc = PuzzleAnswerPair(*)(std::ofstream&);

// The unsigned long long int is pretty much the primary type of advent of code, so let's
// save ourselves some typing.
using ULLINT = unsigned long long int;