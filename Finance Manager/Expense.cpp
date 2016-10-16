#include "stdafx.h"
#include "Expense.h"

int CExpense::count;
int CExpense::typeCount;

CExpense::CExpense()
{
	Money = 0.0f;
}


CExpense::~CExpense()
{
	delete[]Category;
}


LPTSTR CExpense::getCategory()
{
	return Category;
}

LPTSTR CExpense::setCategory(LPTSTR newCategory)
{
	Category = new TCHAR[255];
	wcscpy(Category, newCategory);
	return Category;
}


float CExpense::getMoney()
{
	return Money;
}

float CExpense::setMoney(float newMoney)
{
	Money = newMoney;
	return Money;
}

int CExpense::getCount()
{
	return count;
}

int CExpense::setCount(int newCount)
{
	count = newCount;
	return newCount;
}

int CExpense::getTypeCount(CExpense *pExpense)
{
	typeCount = 0;
	int food = 0;
	int travel = 0;
	int home = 0;
	int car = 0;
	int groceries = 0;
	int service = 0;

	for (int i = 0; i < count; i++) {

		CompareType(food, L"Food", typeCount, pExpense, i);
		CompareType(travel, L"Travel", typeCount, pExpense, i);
		CompareType(home, L"Home", typeCount, pExpense, i);
		CompareType(car, L"Car", typeCount, pExpense, i);
		CompareType(groceries, L"Groceries", typeCount, pExpense, i);
		CompareType(service, L"Service", typeCount, pExpense, i);

	}

	return typeCount;
}

int CompareType(int& flag, LPTSTR type, int& count, CExpense *pExpense, int index) {

	if (flag) {
		return 0;
	}
	else {

		LPTSTR tmp = pExpense[index].getCategory();

		if (!wcscmp(pExpense[index].getCategory(), type)) {
			flag = 1;
			count++;
			return 1;
		}

		return 0;
	}

}


