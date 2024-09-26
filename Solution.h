#pragma once
#include "Matrix.h"

class Solution
{
public:
	enum class StrOrCol { str, col };
	using coord = std::pair<size_t, size_t>;
	
	static void setFirstWeight(WeightMatrix&, const Matrix&);
	static double getFullPrice(const WeightMatrix&, const Matrix&);
	static void findPotentialMarks(const Matrix&, const WeightMatrix&, PotentialMatrix&);
	static void findAllPotential(PotentialMatrix&);
	static coord findWorstPotential(const PotentialMatrix&, const Matrix&);
	static coord findCycle(const WeightMatrix&, const coord&);
	static void changeWeightMatrix(WeightMatrix&, const coord&, const coord&);

	// more useful functions
	static void printAllBadPotential(const PotentialMatrix&, const Matrix&);
	static void printAllBadPotential(const PotentialMatrix&, const Matrix&, size_t, size_t);
	static void printFoundCycle(const WeightMatrix&, const coord&, const coord&);

private:
	static void recursiveFind(const Matrix&, const WeightMatrix&, PotentialMatrix&, StrOrCol, size_t);
};