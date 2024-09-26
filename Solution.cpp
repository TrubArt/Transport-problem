#include "Solution.h"
#define NOMINMAX
#include <Windows.h>
#include <iomanip>

extern const int shift;

namespace
{
	bool isBadPotential(const PotentialMatrix& PotM, const Matrix& PriM, size_t i, size_t j)
	{
		return PotM[i][j] - PriM[i][j] > 0 ? true : false;
	}

	Solution::coord findMinUnDefEl(const WeightMatrix& weightM, const Matrix& priceMatrix)
	{
		auto findFirstUndefValue = [&]() {
			for (size_t i = 0; i < priceMatrix.getRowSize(); ++i)
			{
				for (size_t j = 0; j < priceMatrix.getColSize(); ++j)
				{
					if (!weightM[i][j].getIsDef())
						return std::pair<std::pair<size_t, size_t>, double> { std::pair<size_t, size_t>{i, j}, priceMatrix[i][j]};
				}
			}
			return std::pair<Solution::coord, double>{ Solution::coord{ priceMatrix.getRowSize(), priceMatrix.getColSize()}, -10.0 };
		};

		auto FirstUndefValue = findFirstUndefValue();
		double min = FirstUndefValue.second;
		Solution::coord answer{ FirstUndefValue.first.first ,FirstUndefValue.first.second };

		for (size_t i = 0; i < priceMatrix.getRowSize(); ++i)
		{
			for (size_t j = 0; j < priceMatrix.getColSize(); ++j)
			{
				if (!weightM[i][j].getIsDef() && priceMatrix[i][j] < min)
				{
					min = priceMatrix[i][j].get();
					answer = { i,j };
				}
			}
		}
		return answer;
	}

	void makeLineMarked(WeightMatrix& weightMatrix, Solution::StrOrCol choice, size_t index)
	{
		if (choice == Solution::StrOrCol::str)
		{
			for (size_t j = 0; j < weightMatrix.getColSize(); ++j)
			{
				weightMatrix[index][j].setIsDef(true);
			}
		}
		else
		{
			for (size_t i = 0; i < weightMatrix.getRowSize(); ++i)
			{
				weightMatrix[i][index].setIsDef(true);
			}
		}
	}

	bool haveNotNullComponents(const std::vector<FieldExtra>& x)
	{
		for (const auto& i : x)
		{
			if (i.getIsDef())
				return true;
		}
		return false;
	}
}

void Solution::setFirstWeight(WeightMatrix& weightM, const Matrix& priceMatrix)
{
	std::vector<FieldExtra> row = weightM.getRow();
	std::vector<FieldExtra> col = weightM.getCol();

	while (haveNotNullComponents(row) || haveNotNullComponents(col))
	{
		Solution::coord coordMinEl = findMinUnDefEl(weightM, priceMatrix);
		if (coordMinEl.first == priceMatrix.getRowSize())
			std::cout << "В матрице весов нет нулевых элементов. Нельзя найти минимальный из нулевых\n";
		// если сообщение вывелось, то дальше будет неизбежная ошибка выхода за границы vector

		double minValue = 0.0;
		if (!row[coordMinEl.first].getIsDef())
		{
			minValue = (col[coordMinEl.second]).get();
			col[coordMinEl.second] = 0.0;
		}
		else if (!col[coordMinEl.second].getIsDef())
		{
			minValue = (row[coordMinEl.first]).get();
			row[coordMinEl.first] = 0.0;
		}
		else
		{
			minValue = (std::min(row[coordMinEl.first], col[coordMinEl.second])).get();
			row[coordMinEl.first] -= minValue;
			col[coordMinEl.second] -= minValue;
		}

		if (row[coordMinEl.first] == 0.0 && row[coordMinEl.first].getIsDef())
		{
			row[coordMinEl.first].setIsDef(false);
			makeLineMarked(weightM, StrOrCol::str, coordMinEl.first);
		}
		if (col[coordMinEl.second] == 0.0 && col[coordMinEl.second].getIsDef())
		{
			col[coordMinEl.second].setIsDef(false);
			makeLineMarked(weightM, StrOrCol::col, coordMinEl.second);
		}

		weightM[coordMinEl.first][coordMinEl.second] = minValue;
		//std::cout << weightM << "\n";
	}
}

double Solution::getFullPrice(const WeightMatrix& weightM, const Matrix& priceMatrix)
{
	double sum = 0.0;
	auto itPM = priceMatrix.begin();
	auto itWM = weightM.begin();

	while (itPM < priceMatrix.end())
	{
		if (*itWM)
			sum += itWM->get() * itPM->get();
		++itPM;
		++itWM;
	}
	return sum;
}

void Solution::findPotentialMarks(const Matrix& priceMatrix, const WeightMatrix& weightM, PotentialMatrix& potMatrix)
{
	potMatrix.getRow().front() = 0;
	recursiveFind(priceMatrix, weightM, potMatrix, StrOrCol::str, 0);

	for (const auto& i : potMatrix.getRow())
		if (!i.getIsDef())
			std::cout << "After findPotentialMarks() not define one of rowPotentials!\n";
	for (const auto& i : potMatrix.getCol())
		if (!i.getIsDef())
			std::cout << "After findPotentialMarks() not define one of colPotentials!\n";
}

void Solution::findAllPotential(PotentialMatrix& matrix)
{
	for (size_t i = 0; i < matrix.getRowSize(); ++i)
	{
		for (size_t j = 0; j < matrix.getColSize(); ++j)
		{
			matrix[i][j] = (matrix.getRow()[i] + matrix.getCol()[j]).get();
		}
	}
}

Solution::coord Solution::findWorstPotential(const PotentialMatrix& potenMatrix, const Matrix& priceM)
{
	double max = 0.0;
	std::pair<size_t, size_t> indexs{ potenMatrix.getRowSize(), 0 };

	for (size_t i = 0; i < potenMatrix.getRowSize(); ++i)
	{
		for (size_t j = 0; j < potenMatrix.getColSize(); ++j)
		{
			if (isBadPotential(potenMatrix, priceM, i, j) && potenMatrix[i][j] - priceM[i][j] > max)
			{
				indexs = { i, j };
				max = (potenMatrix[i][j] - priceM[i][j]).get();
			}
		}
	}
	return indexs;
}

Solution::coord Solution::findCycle(const WeightMatrix& matrix, const coord& downRightAngleCoord)
{
	for (size_t rowIndex = 0; rowIndex < matrix.getRowSize(); ++rowIndex)
	{
		if (rowIndex == downRightAngleCoord.first || !matrix[rowIndex][downRightAngleCoord.second])
			continue;
		for (size_t colIndex = 0; colIndex < matrix.getColSize(); ++colIndex)
		{
			if (colIndex == downRightAngleCoord.second || !matrix[downRightAngleCoord.first][colIndex])
				continue;

			// просмотр ячейки на пересечении подходящих базовых
			if (matrix[rowIndex][colIndex])
				return { rowIndex , colIndex };
		}
	}
	return { matrix.getRowSize(), matrix.getColSize() };
}

void Solution::changeWeightMatrix(WeightMatrix& potenMatrix, const coord& coor1, const coord& coor2)
{
	double min = std::min(potenMatrix[coor1.first][coor2.second], potenMatrix[coor2.first][coor1.second]).get();
	potenMatrix[coor1.first][coor1.second] += min;
	potenMatrix[coor2.first][coor2.second] += min;
	potenMatrix[coor1.first][coor2.second] -= min;
	potenMatrix[coor2.first][coor1.second] -= min;
}

void Solution::printAllBadPotential(const PotentialMatrix& potenMatrix, const Matrix& priceM)
{
	HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);

	std::cout << std::setw(shift) << " ";
	for (const auto& ind : potenMatrix.getCol())
	{
		if (!ind.getIsDef())
			SetConsoleTextAttribute(console, 3);
		std::cout << " " << std::setw(shift) << ind ;
		SetConsoleTextAttribute(console, 15);
	}

	for (size_t i = 0; i < potenMatrix.getRowSize(); ++i)
	{
		if (!potenMatrix.getRow()[i].getIsDef())
			SetConsoleTextAttribute(console, 3);
		std::cout << "\n" << std::setw(shift) << potenMatrix.getRow()[i];
		SetConsoleTextAttribute(console, 15);

		for (size_t j = 0; j < potenMatrix.getColSize(); ++j)
		{
			if (isBadPotential(potenMatrix, priceM, i, j))
				SetConsoleTextAttribute(console, 4);
			std::cout << " " << std::setw(shift) << potenMatrix[i][j];
			SetConsoleTextAttribute(console, 15);
		}
	}
	std::cout << "\n";
}

void Solution::printAllBadPotential(const PotentialMatrix& potenMatrix, const Matrix& priceM, size_t indi, size_t indj)
{
	HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);

	std::cout << std::setw(shift) << " ";
	for (const auto& ind : potenMatrix.getCol())
	{
		SetConsoleTextAttribute(console, 14);
		if (!ind.getIsDef())
			SetConsoleTextAttribute(console, 3);
		std::cout << " " << std::setw(shift) << ind;
		SetConsoleTextAttribute(console, 15);
	}

	for (size_t i = 0; i < potenMatrix.getRowSize(); ++i)
	{
		SetConsoleTextAttribute(console, 14);
		if (!potenMatrix.getRow()[i].getIsDef())
			SetConsoleTextAttribute(console, 3);
		std::cout << "\n" << std::setw(shift) << potenMatrix.getRow()[i];
		SetConsoleTextAttribute(console, 15);

		for (size_t j = 0; j < potenMatrix.getColSize(); ++j)
		{
			if (isBadPotential(potenMatrix, priceM, i, j))
				SetConsoleTextAttribute(console, 4);
			if (i == indi && j == indj)
				SetConsoleTextAttribute(console, 5);
			std::cout << " " << std::setw(shift) << potenMatrix[i][j];
			SetConsoleTextAttribute(console, 15);
		}
	}
	std::cout << "\n";
}

void Solution::printFoundCycle(const WeightMatrix& potenMatrix, const coord& coor1, const coord& coor2)
{
	HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);
	for (size_t i = 0; i < potenMatrix.getRowSize(); ++i)
	{
		std::cout << std::setw(shift) << " ";
		for (size_t j = 0; j < potenMatrix.getColSize(); ++j)
		{
			if ((i == coor1.first && (j == coor1.second || j == coor2.second))
				|| i == coor2.first && (j == coor1.second || j == coor2.second))
				SetConsoleTextAttribute(console, 4);
			std::cout << " " << std::setw(shift) << potenMatrix[i][j];
			SetConsoleTextAttribute(console, 15);
		}
		std::cout << "\n";
	}
}

void Solution::recursiveFind(const Matrix& priceMatrix, const WeightMatrix& weightM, PotentialMatrix& potMatrix, StrOrCol choiсe, size_t index)
{
	if (choiсe == StrOrCol::str)
	{
		for (size_t i = 0; i < potMatrix.getColSize(); ++i)
		{
			if (weightM[index][i] && !potMatrix.getCol()[i].getIsDef())
			{
				potMatrix.getCol()[i].setIsDef(true);
				potMatrix.getCol()[i] = (priceMatrix[index][i] - potMatrix.getRow()[index]).get();
				recursiveFind(priceMatrix, weightM, potMatrix, StrOrCol::col, i);
			}
		}
	}
	else
	{
		for (size_t i = 0; i < potMatrix.getRowSize(); ++i)
		{
			if (weightM[i][index] && !potMatrix.getRow()[i].getIsDef())
			{
				potMatrix.getRow()[i].setIsDef(true);
				potMatrix.getRow()[i] = (priceMatrix[i][index] - potMatrix.getCol()[index]).get();
				recursiveFind(priceMatrix, weightM, potMatrix, StrOrCol::str, i);
			}
		}
	}
}