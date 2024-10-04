#include <iostream>
#include <Windows.h>
#include "Creator.h"

extern const int shift = 3;

void myTask();
void testTask();
void classTask();

int main()
{
	SetConsoleCP(1251);
	SetConsoleOutputCP(1251);

	classTask();
	std::cin.get();
}




void myTask()
{
	std::vector<FieldExtra> rowCondition = { 22, 10, 89, 91, 54 };
	std::vector<FieldExtra> colCondition = { 28, 16, 55, 52, 115 };
	Matrix price
	{
		{ 3, 4, 5, 7, 5 },
		{ 4, 8, 6, 8, 6 },
		{ 4, 4, 7, 4, 2 },
		{ 7, 5, 2, 2, 7 },
		{ 8, 3, 1, 5, 7 }
	};
	std::cout << price << "\n";
	Creator creat(price, rowCondition, colCondition);
	std::cout << "Ответ задачи: " << creat.solution();
}

void testTask()
{
	std::vector<FieldExtra> rowCondition = { 10, 20, 30 };
	std::vector<FieldExtra> colCondition = { 15, 20, 25 };
	Matrix price
	{
		{ 5, 3, 1 },
		{ 3, 2, 4 },
		{ 4, 1, 2 }
	};

	Creator creat(price, rowCondition, colCondition);
	std::cout << "Ответ задачи: " << creat.solution();
}

void classTask()
{
	std::vector<FieldExtra> rowCondition = { 7, 8, 10, 20 };
	std::vector<FieldExtra> colCondition = { 8, 10, 7, 11, 9 };
	Matrix price
	{
		{ 5, 1, 3, 1, 2 },
		{ 3, 6, 2, 4, 5 },
		{ 2, 4, 5, 3, 1 },
		{ 1, 2, 3, 4, 5 }
	};

	Creator creat(price, rowCondition, colCondition);
	std::cout << "Ответ задачи: " << creat.solution();
}