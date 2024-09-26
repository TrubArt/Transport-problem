#include "Matrix.h"
#include <iomanip>
#include <Windows.h>
#include <algorithm>

extern const int shift;

FieldExtra& Matrix::Proxy::operator[](size_t colNum)
{
	return mat->data[rowNum * mat->colCount + colNum];
}

const FieldExtra& Matrix::Proxy::operator[](size_t colNum) const
{
	return matC->data[rowNum * matC->colCount + colNum];
}

Matrix::Proxy::Proxy(const Matrix* matC, size_t rowNum)
	: matC(matC), mat(nullptr), rowNum(rowNum)
{}

Matrix::Proxy::Proxy(Matrix* mat, size_t rowNum) 
	: matC(nullptr), mat(mat), rowNum(rowNum)
{}



Matrix::Matrix(size_t row, size_t col)
	: rowCount(row)
	, colCount(col) 
	, data(row * col)
{}


Matrix::Matrix(std::initializer_list<std::initializer_list<double>> list)
	: Matrix(list.size(), list.size() ? list.begin()->size() : 0)
{
	std::for_each(list.begin(), list.end(),
		[this, i{ 0 }] (auto& row) mutable
		{
			std::copy(row.begin(), row.end(), data.begin() + i++ * colCount);
		});
}

size_t Matrix::getRowSize() const
{
	return rowCount;
}

size_t Matrix::getColSize() const
{
	return colCount;
}

std::vector<FieldExtra>::iterator Matrix::begin()
{
	return data.begin();
}

std::vector<FieldExtra>::const_iterator Matrix::begin() const
{
	return data.cbegin();
}

std::vector<FieldExtra>::iterator Matrix::end()
{
	return data.end();
}

std::vector<FieldExtra>::const_iterator Matrix::end() const
{
	return data.cend();
}

Matrix::Proxy Matrix::operator[](size_t rowNum)
{
	return Proxy(this, rowNum);
}

const Matrix::Proxy Matrix::operator[](size_t rowNum) const
{
	return Proxy(this, rowNum);
}

std::ostream& operator<<(std::ostream& out, const Matrix& x)
{
	for (size_t i = 0; i < x.data.size(); ++i)
	{
		if (i && i % x.colCount == 0)
			out << "\n";
		out << " " << std::setw(shift) << x.data[i];
	}
	return out;
}



MatrixWithVecField::MatrixWithVecField(const std::vector<FieldExtra>& rowC, const std::vector<FieldExtra>& colC)
	: Matrix(rowC.size(), colC.size())
	, row(rowC)
	, col(colC)
{}

MatrixWithVecField::MatrixWithVecField(size_t row, size_t col)
	: Matrix(row, col)
	, row(row)
	, col(col)
{}

std::vector<FieldExtra>& MatrixWithVecField::getRow()
{
	return row;
}

const std::vector<FieldExtra>& MatrixWithVecField::getRow() const
{
	return row;
}

std::vector<FieldExtra>& MatrixWithVecField::getCol()
{
	return col;
}

const std::vector<FieldExtra>& MatrixWithVecField::getCol() const
{
	return col;
}



WeightMatrix::WeightMatrix(const std::vector<FieldExtra>& rowC, const std::vector<FieldExtra>& colC)
	: MatrixWithVecField(rowC, colC)
{}

std::ostream& operator<<(std::ostream& out, const WeightMatrix& x)
{
	HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);

	out << std::setw(shift) << " ";
	SetConsoleTextAttribute(console, 14);
	std::for_each(x.col.begin(), x.col.end(), [&out](const auto& ind)
		{
			out << " " << std::setw(shift) << ind;
		});
	SetConsoleTextAttribute(console, 15);

	auto itR = x.row.cbegin();
	for (size_t i = 0; i < x.data.size(); ++i)
	{
		if (i % x.colCount == 0)
		{
			SetConsoleTextAttribute(console, 14);
			out << "\n";
			out << std::setw(shift) << *itR++;
			SetConsoleTextAttribute(console, 15);
		}

		if (x.data[i])
			SetConsoleTextAttribute(console, 2);
		out << " " << std::setw(shift) << x.data[i];
		SetConsoleTextAttribute(console, 15);
	}
	return out;
}

PotentialMatrix::PotentialMatrix(size_t row, size_t col)
	: MatrixWithVecField(row, col)
{}
