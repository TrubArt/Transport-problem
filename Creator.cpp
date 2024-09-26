#include "Creator.h"
#include "Solution.h"

Creator::Creator(const Matrix& PM, const std::vector<FieldExtra>& rowC, const std::vector<FieldExtra>& colC)
	: priceMatrix(PM)
	, weigM(new WeightMatrix(rowC, colC))
{
	if (priceMatrix.getColSize() != colC.size() || priceMatrix.getRowSize() != rowC.size())
		throw std::invalid_argument("different size");
}

bool Creator::isTaskClosed() const
{
	double difference = 0.0;
	for (const auto& i : weigM->getRow())
		difference += i.get();
	for (const auto& i : weigM->getCol())
		difference -= i.get();

	if (!difference)
		return true;
	return false;
}

double Creator::solution()
{
	if (!isTaskClosed())
	{
		std::cout << "Задача не является закрытой!\n";
		return 0.0;
	}

	// получение первой оценки
	Solution::setFirstWeight(*weigM, priceMatrix);

	bool endWorkingFlag = false;
	double answer = 0.0;
	size_t iterationNumber = 0;

	while (!endWorkingFlag)
	{
		// подсчёт общего веса и вывод матрицы весов
		answer = partFindWeight(iterationNumber);

		// расчёт потенциалов
		PotentialMatrix potenM(priceMatrix.getRowSize(), priceMatrix.getColSize());
		Solution::coord worstPot = partFindPotential(potenM);

		// проверка на конец итерации
		if (worstPot.first >= priceMatrix.getRowSize())
			endWorkingFlag = true;

		// перестройка матрицы весов
		if (!endWorkingFlag)
		{
			Solution::coord rectAngleCoord = Solution::findCycle(*weigM, worstPot);	// нахождение цикла
			if (rectAngleCoord.first == weigM->getRowSize())
			{
				std::cout << "Цикл не найден!\n";
				return 0.0;
			}
			partChangeWeightMatrix(rectAngleCoord, worstPot);
		}
	}
	return answer;
}




double Creator::partFindWeight(size_t iterationNumber) const
{
	double answer = Solution::getFullPrice(*weigM, priceMatrix);	// подсчёт общего веса
	std::cout << "\n\nВес на " << ++iterationNumber << ": " << answer << "\n";
	std::cout << "Матрица весов:\n" << *weigM << "\n\n";	// печать матрицы весов
	return answer;
}

Solution::coord Creator::partFindPotential(PotentialMatrix& potenM) const
{
	Solution::findPotentialMarks(priceMatrix, *weigM, potenM);						// нахожение оценок
	Solution::findAllPotential(potenM);												// нахождение потенциалов
	Solution::coord worstPot = Solution::findWorstPotential(potenM, priceMatrix);	// нахождение индексов худшего потенциала

	std::cout << "Матрица потенциалов:\n";
	Solution::printAllBadPotential(potenM, priceMatrix, worstPot.first, worstPot.second);	// печать матрицы с потенциалами
	std::cout << "\n";

	return worstPot;
}

void Creator::partChangeWeightMatrix(const Solution::coord& rectAngleCoord, const Solution::coord& worstPot)
{
	std::cout << "Найденный цикл:\n";
	Solution::printFoundCycle(*weigM, rectAngleCoord, worstPot);	// печать цикла
	Solution::changeWeightMatrix(*weigM, rectAngleCoord, worstPot);	// изменение матрицы весов
}