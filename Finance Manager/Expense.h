#pragma once
class CExpense
{
public:
	CExpense();
	~CExpense();
private:
	LPTSTR Category;
	float Money;
	static int count;
	static int typeCount;
public:
	LPTSTR getCategory();
	LPTSTR setCategory(LPTSTR newCategory);
	float getMoney();
	float setMoney(float newMoney);
	static int getCount();
	static int setCount(int newCount);
	static int getTypeCount(CExpense *pExpense);
};

int CompareType(int & flag, LPTSTR type, int & count, CExpense * pExpense, int index);
