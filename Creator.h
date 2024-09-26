#pragma once
#include "Matrix.h"

class Creator
{
public:
	Creator(const Matrix& PM, const std::vector<FieldExtra>& rowC, const std::vector<FieldExtra>& colC);
	double solution();

private:
	bool isTaskClosed() const;

	double partFindWeight(size_t) const;
	std::pair<size_t, size_t> partFindPotential(PotentialMatrix&) const;
	void partChangeWeightMatrix(const std::pair<size_t, size_t>&, const std::pair<size_t, size_t>&);

	const Matrix& priceMatrix;
	WeightMatrix* weigM;
};