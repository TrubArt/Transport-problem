#pragma once
#ifndef Matrixxx
#define Matrixxx
#include "Field.h"
#include <vector>
#include <initializer_list>

// класс-контейнер матрица
class Matrix
{
	class Proxy;
public:
	Matrix(size_t row, size_t col);
	Matrix(std::initializer_list<std::initializer_list<double>> list);
	virtual ~Matrix() = default;

	size_t getRowSize() const;
	size_t getColSize() const;
	std::vector<FieldExtra>::iterator begin();
	std::vector<FieldExtra>::const_iterator begin() const;
	std::vector<FieldExtra>::iterator end();
	std::vector<FieldExtra>::const_iterator end() const;
	Proxy operator[](size_t rowNum);
	const Proxy operator[](size_t rowNum) const;

	friend std::ostream& operator<<(std::ostream&, const Matrix&);

protected:
	size_t rowCount;
	size_t colCount;
	std::vector<FieldExtra> data;

private:
	class Proxy
	{
	public:
		Proxy(const Matrix* matC, size_t rowNum);
		Proxy(Matrix* mat, size_t rowNum);

		FieldExtra& operator[](size_t colNum);
		const FieldExtra& operator[](size_t colNum) const;

	private:
		const Matrix* matC;
		Matrix* mat;
		size_t rowNum;
	};
};
std::ostream& operator<<(std::ostream&, const Matrix&);


// класс контейнер-матрица с полями векторов
class MatrixWithVecField : public Matrix
{
public:
	MatrixWithVecField(size_t row, size_t col);
	MatrixWithVecField(const std::vector<FieldExtra>& rowC, const std::vector<FieldExtra>& colC);
	std::vector<FieldExtra>& getRow();
	const std::vector<FieldExtra>& getRow() const;
	std::vector<FieldExtra>& getCol();
	const std::vector<FieldExtra>& getCol() const;

protected:
	std::vector<FieldExtra> row;
	std::vector<FieldExtra> col;
};


class WeightMatrix : public MatrixWithVecField
{
public:
	WeightMatrix(const std::vector<FieldExtra>& rowC, const std::vector<FieldExtra>& colC);
	friend std::ostream& operator<<(std::ostream&, const WeightMatrix&);
};
std::ostream& operator<<(std::ostream&, const WeightMatrix&);



class PotentialMatrix : public MatrixWithVecField
{
public:
	PotentialMatrix(size_t row, size_t col);
};

#endif // !Matrixxx