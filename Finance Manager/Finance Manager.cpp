// Finance Manager.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "Finance Manager.h"

#define MAX_LOADSTRING 100

// Global Variables:
HINSTANCE hInst;                                // current instance
WCHAR szTitle[MAX_LOADSTRING];                  // The title bar text
WCHAR szWindowClass[MAX_LOADSTRING];            // the main window class name
CExpense *AllExpense;


// Forward declarations of functions included in this code module:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);




int ChangeFont(HWND hwnd);

int AddExpenseCategory(HWND hwnd);
int AddItemToBox(HWND hwnd, LPARAM text);

int createAddWindow(HINSTANCE hInstance, int nCmdShow, HWND hWnd);
int createResultWindow(HINSTANCE hInstance, int nCmdShow, HWND hWnd);
int createSummary(HINSTANCE hInstance, int nCmdShow, HWND hWnd);

float calTotalExpense();

int drawPieChart(HINSTANCE hInstance, int nCmdShow, HWND hWnd, HDC &hdc);
int CreatePiePart(CPiePart *PiePart, HINSTANCE hInstance, HWND hWnd);
float calSeparateExpense(LPTSTR type);
int InitPie(CPiePart *PiePart, int &index, float percentage);

int createListView(HINSTANCE hInstance, int nCmdShow, HWND hWnd);
int addListViewItem(HWND hwnd);

int GetNumberOfRow(std::string str);

int AddToTextFile(LPTSTR category, LPTSTR content, LPTSTR money);
BOOL ValidateData(LPTSTR money);

wchar_t * AttatchResultString(wchar_t * notificationString, float input);
int getSlideCount();



int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPWSTR    lpCmdLine,
	_In_ int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	// TODO: Place code here.

	// Initialize global strings
	LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadStringW(hInstance, IDC_FINANCEMANAGER, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// Perform application initialization:
	if (!InitInstance(hInstance, nCmdShow))
	{
		return FALSE;
	}

	HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_FINANCEMANAGER));

	MSG msg;

	// Main message loop:
	while (GetMessage(&msg, nullptr, 0, 0))
	{
		if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	return (int)msg.wParam;
}



//
//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: Registers the window class.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEXW wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_FINANCEMANAGER));
	wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = MAKEINTRESOURCEW(IDC_FINANCEMANAGER);
	wcex.lpszClassName = szWindowClass;
	wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassExW(&wcex);
}

//
//   FUNCTION: InitInstance(HINSTANCE, int)
//
//   PURPOSE: Saves instance handle and creates main window
//
//   COMMENTS:
//
//        In this function, we save the instance handle in a global variable and
//        create and display the main program window.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
	hInst = hInstance; // Store instance handle in our global variable

	HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

	if (!hWnd)
	{
		return FALSE;
	}

	
	createAddWindow(hInstance, nCmdShow, hWnd);

	createResultWindow(hInstance, nCmdShow, hWnd);

	createSummary(hInstance, nCmdShow, hWnd);

	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	return TRUE;
}

//
//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE:  Processes messages for the main window.
//
//  WM_COMMAND  - process the application menu
//  WM_PAINT    - Paint the main window
//  WM_DESTROY  - post a quit message and return
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_COMMAND:
	{
		int wmId = LOWORD(wParam);
		// Parse the menu selections:
		switch (wmId)
		{
		case IDM_ABOUT:
			DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
			break;
		case IDM_EXIT:
			DestroyWindow(hWnd);
			break;
		case IDB_ADD_BUTTON: {

			int bufSize = 1024;

			LPTSTR category = new TCHAR[bufSize];
			LPTSTR content = new TCHAR[bufSize];
			LPTSTR money = new TCHAR[bufSize];

			GetDlgItemText(hWnd, IDD_ADD_ITEM_COMBO_BOX, category, bufSize);
			GetDlgItemText(hWnd, IDD_CONTENT_BOX, content, bufSize);
			GetDlgItemText(hWnd, IDD_INPUT_MONEY_BOX, money, bufSize);

			if (ValidateData(money)) {
				AddToTextFile(category, content, money);
			}
			delete[] category;
			delete[] content;
			delete[] money;
			break;
		}
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
	}
	break;
	case WM_PAINT:
	{
		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(hWnd, &ps);
		drawPieChart(nullptr, NULL, hWnd, hdc);
		// TODO: Add any drawing code that uses hdc here...
		
	}
	break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

// Message handler for about box.
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		return (INT_PTR)TRUE;

	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
		{
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		break;
	}
	return (INT_PTR)FALSE;
}

int ChangeFont(HWND hwnd) {

	try {
		HFONT hFont = CreateFont(13, 0, 0, 0, FW_DONTCARE, FALSE, FALSE, FALSE, ANSI_CHARSET,
			OUT_TT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY,
			DEFAULT_PITCH | FF_DONTCARE, TEXT("Tahoma"));

		SendMessage(hwnd, WM_SETFONT, (WPARAM)hFont, TRUE);

		return 1;
	}
	catch (const std::exception&) {
		throw;
		return 0;
	}


}

int AddExpenseCategory(HWND hwnd) {

	try
	{
		AddItemToBox(hwnd, (LPARAM)TEXT("Food"));
		AddItemToBox(hwnd, (LPARAM)TEXT("Travel"));
		AddItemToBox(hwnd, (LPARAM)TEXT("Home"));
		AddItemToBox(hwnd, (LPARAM)TEXT("Car"));
		AddItemToBox(hwnd, (LPARAM)TEXT("Groceries"));
		AddItemToBox(hwnd, (LPARAM)TEXT("Service"));

		return 1;
	}
	catch (const std::exception&)
	{
		throw;
		return 0;
	}

}

int AddItemToBox(HWND hwnd, LPARAM text)
{
	try
	{
		SendMessage(hwnd, (UINT)CB_ADDSTRING, (WPARAM)0, text);
		return 1;
	}
	catch (const std::exception&)
	{
		throw;
		return 0;
	}
}

int createAddWindow(HINSTANCE hInstance, int nCmdShow, HWND hWnd) {

	HWND hwndAddItemWindow = CreateWindow(szWindowClass, L"Adding Expense",
		WS_THICKFRAME | WS_MINIMIZEBOX |
		WS_MAXIMIZEBOX | WS_VISIBLE | WS_CHILD,
		10, 10, 300, 250, hWnd,
		(HMENU)IDD_ADD_ITEM_WINDOW, hInstance, NULL);

	HWND hwndCategoryBox = CreateWindow(L"COMBOBOX", L"Type Of Expense",
		CBS_DROPDOWNLIST | WS_CHILDWINDOW | WS_VISIBLE,
		10, 10, 150, 400, hwndAddItemWindow,
		(HMENU)IDD_ADD_ITEM_COMBO_BOX, hInstance, NULL);

	AddExpenseCategory(hwndCategoryBox);

	SendMessage(hwndCategoryBox, CB_SETCURSEL, (WPARAM)0, (LPARAM)0);

	ChangeFont(hwndCategoryBox);

	HWND hwndExType = CreateWindow(L"STATIC", L"Category",
		WS_CHILDWINDOW | WS_VISIBLE | WS_ACTIVECAPTION,
		170, 15, 100, 15, hwndAddItemWindow,
		nullptr, hInstance, NULL);

	ChangeFont(hwndExType);

	HWND hwndContentBox = CreateWindow(L"Edit", L"",
		WS_CHILD | WS_VISIBLE | WS_BORDER | ES_LEFT,
		10, 50, 250, 20, hwndAddItemWindow,
		(HMENU)IDD_CONTENT_BOX, hInstance, NULL);

	ChangeFont(hwndContentBox);

	HWND hwndContentTitle = CreateWindow(L"STATIC", L"Content\r\nWhat did you spent for?\r\nEx: Eating out, Reparing car...",
		WS_CHILDWINDOW | WS_VISIBLE | WS_ACTIVECAPTION,
		10, 80, 250, 50, hwndAddItemWindow,
		nullptr, hInstance, NULL);

	ChangeFont(hwndContentTitle);

	HWND hwndMoneyBox = CreateWindow(L"Edit", L"Money Spent",
		WS_CHILDWINDOW | WS_VISIBLE | WS_BORDER | ES_AUTOHSCROLL,
		10, 170, 150, 20, hwndAddItemWindow,
		(HMENU)IDD_INPUT_MONEY_BOX, hInstance, NULL);

	ChangeFont(hwndMoneyBox);

	HWND hwndMoneyBoxTitle = CreateWindow(L"STATIC", L"Money Spent",
		WS_CHILDWINDOW | WS_VISIBLE | WS_ACTIVECAPTION,
		170, 170, 100, 15, hwndAddItemWindow,
		nullptr, hInstance, NULL);

	ChangeFont(hwndMoneyBoxTitle);

	HWND hwndAddButton = CreateWindow(L"BUTTON", L"Save",
		WS_CHILD | WS_VISIBLE | WS_BORDER,
		90, 210, 100, 25, hwndAddItemWindow,
		(HMENU)IDB_ADD_BUTTON, hInstance, NULL);

	ChangeFont(hwndAddButton);

	return 1;
}

int createResultWindow(HINSTANCE hInstance, int nCmdShow, HWND hWnd)
{
	HWND hwndShowItemWindow = CreateWindow(szWindowClass, L"Show Expense",
		WS_THICKFRAME | WS_MINIMIZEBOX |
		WS_MAXIMIZEBOX | WS_VISIBLE | WS_CHILD,
		330, 10, 660, 250, hWnd,
		(HMENU)IDD_SHOW_ITEM_WINDOW, hInstance, NULL);

	createListView(hInstance, nCmdShow, hwndShowItemWindow);

	ChangeFont(hwndShowItemWindow);

	return 1;
}

int createSummary(HINSTANCE hInstance, int nCmdShow, HWND hWnd)
{

	HWND hwndShowTotalExpense = CreateWindow(L"STATIC", L"Total Expense: ",
		WS_VISIBLE | WS_CHILD,
		10, 270, 300, 20, hWnd,
		(HMENU)IDD_SHOW_EXPENSE, hInstance, NULL);

	float total = calTotalExpense();

	SetDlgItemText(hWnd, IDD_SHOW_EXPENSE, AttatchResultString(L"Total Expense: ", total));

	ChangeFont(hwndShowTotalExpense);

	return 0;
}

float calTotalExpense()
{
	float res = 0;
	for (int i = 0; i < AllExpense[0].getCount(); i++) {
		res = res + AllExpense[i].getMoney();
	}
	return res;
}

int drawPieChart(HINSTANCE hInstance, int nCmdShow, HWND hWnd, HDC &hdc)
{
	int slideCount = getSlideCount();
	CPiePart *PiePart = new CPiePart[slideCount];

	CreatePiePart(PiePart, hInstance, hWnd);

	for (int i = 0; i < slideCount; i++) {
		hdc = GetDC(hWnd);
		BeginPath(hdc);
		if (1 == i)
			SelectObject(hdc, TRAVEL_COLOR);
		if (2 == i)
			SelectObject(hdc, FOOD_COLOR);
		if (3 == i)
			SelectObject(hdc, HOME_COLOR);
		if (4 == i)
			SelectObject(hdc, CAR_COLOR);
		if (5 == i)
			SelectObject(hdc, SERVICE_COLOR);
		if (6 == i)
			SelectObject(hdc, GROCERIES_COLOR);
		MoveToEx(hdc, PiePart[i].getX(), PiePart[i].getY(), (LPPOINT)NULL);
		AngleArc(hdc, PiePart[i].getX(), PiePart[i].getY(), PiePart[i].getR(),
			PiePart[i].getStartAngle(), PiePart[i].getSweepAngle());
		LineTo(hdc, PiePart[i].getX(), PiePart[i].getY());
		EndPath(hdc);
		StrokeAndFillPath(hdc);
		ReleaseDC(hWnd, hdc);
	}

	return 0;
}

int CreatePiePart(CPiePart * PiePart, HINSTANCE hInstance, HWND hWnd)
{
	int slideCount = getSlideCount();
	int index = 0;

	float total = calTotalExpense();

	if (0 == total) {
		return 0;
	}

	float food = calSeparateExpense(L"Food");
	float travel = calSeparateExpense(L"Travel");
	float home = calSeparateExpense(L"Home");
	float groceries = calSeparateExpense(L"Groceries");
	float service = calSeparateExpense(L"Service");
	float car = calSeparateExpense(L"Car");


	if (food != 0) {
		food = (food / total);
		InitPie(PiePart, index, food);
	}
	if (travel != 0) {
		travel = (travel / total);
		InitPie(PiePart, index, travel);
	}
	if (home != 0) {
		home = (home / total);
		InitPie(PiePart, index, home);
	}
	if (car != 0) {
		car = (car / total);
		InitPie(PiePart, index, car);
	}
	if (groceries != 0) {
		groceries = (groceries / total);
		InitPie(PiePart, index, groceries);
	}
	if (service != 0) {
		service = (service / total);
		InitPie(PiePart, index, service);
	}


	HWND hwndShowTSeparateExpense = CreateWindow(L"COMBOBOX", L"",
		 WS_CHILDWINDOW | WS_VISIBLE | ES_AUTOVSCROLL | ES_MULTILINE,
		10, 290, 200, 100, hWnd,
		(HMENU)IDD_SHOW_SEPARATE_EXPENSE, hInstance, NULL);


	AddItemToBox(hwndShowTSeparateExpense, (LPARAM)AttatchResultString(L"Food(%): ", food*100));
	AddItemToBox(hwndShowTSeparateExpense, (LPARAM)AttatchResultString(L"Travel(%): ", travel*100));
	AddItemToBox(hwndShowTSeparateExpense, (LPARAM)AttatchResultString(L"Home(%): ", home*100));
	AddItemToBox(hwndShowTSeparateExpense, (LPARAM)AttatchResultString(L"Car(%): ", car*100));
	AddItemToBox(hwndShowTSeparateExpense, (LPARAM)AttatchResultString(L"Groceries(%): ", groceries*100));
	AddItemToBox(hwndShowTSeparateExpense, (LPARAM)AttatchResultString(L"Service(%): ", service*100));

	SendMessage(hwndShowTSeparateExpense, CB_SETCURSEL, (WPARAM)0, (LPARAM)0);

	ChangeFont(hwndShowTSeparateExpense);

	return 1;
}

float calSeparateExpense(LPTSTR type)
{
	float result = 0;

	for (int i = 0; i < AllExpense[0].getCount(); i++) {

		if (!wcscmp(AllExpense[i].getCategory(), type)) {
			result += AllExpense[i].getMoney();
		}

	}

	return result;
}

int InitPie(CPiePart *PiePart, int &index, float percentage)
{
	if (0 == index) {

		float swAng = roundf(360.0 * percentage);
		PiePart[0].setStartAngle(0);
		PiePart[0].setSweepAngle(swAng);

	}
	else {

		float preStAng = PiePart[index - 1].getStartAngle();
		float preSwAng = PiePart[index - 1].getSweepAngle();
		float stAng = preStAng + preSwAng;
		float swAng = roundf(360.0 * percentage);
		PiePart[index].setStartAngle(stAng);
		PiePart[index].setSweepAngle(swAng);
	
	}


	index++;
	return 0;
}

int createListView(HINSTANCE hInstance, int nCmdShow, HWND hWnd)
{
	RECT rcClient;                       // The parent window's client area.

	GetClientRect(hWnd, &rcClient);

	HWND hwndListView = CreateWindow(L"COMBOBOX", L"",
		CBS_DROPDOWNLIST | WS_CHILDWINDOW | WS_VISIBLE | ES_AUTOVSCROLL | ES_MULTILINE,
		0, 0, rcClient.right - rcClient.left,
		rcClient.bottom - rcClient.top, hWnd,
		(HMENU)IDL_EXPENSE_LIST, hInstance, NULL);


	ChangeFont(hwndListView);

	addListViewItem(hwndListView);

	SendMessage(hwndListView, CB_SETCURSEL, (WPARAM)0, (LPARAM)0);

	return 0;
}

int addListViewItem(HWND hwnd)
{
	int row;
	std::wifstream inCategory;
	std::wifstream inMoney;
	std::wifstream inContent;

	row = GetNumberOfRow("row.txt");
	AllExpense = new CExpense[row];
	AllExpense[0].setCount(row);

	inCategory.open("data_Category.txt", std::ios::in);
	inContent.open("data_Content.txt", std::ios::in);
	inMoney.open("data_Money.txt", std::ios::in);

	for (int i = 0; i < row; i++) {

		LPTSTR category = new TCHAR[255];
		LPTSTR content = new TCHAR[255];
		LPTSTR money = new TCHAR[255];
		LPTSTR lines = new TCHAR[1024];

		inCategory.getline(category, 255);
		inContent.getline(content, 255);
		inMoney.getline(money, 255);

		AllExpense[i].setCategory(category);
		AllExpense[i].setMoney(_tstof(money));

		wcscat(content, L" - Category: ");
		wcscat(content, category);
		wcscat(content, L" - Money Spent: ");
		wcscat(content, money);

		AddItemToBox(hwnd, (LPARAM)content);

		delete[]category;
		delete[]content;
		delete[]money;
		delete[]lines;

	}


	inCategory.close();
	inContent.close();
	inMoney.close();

	return 1;
}

int GetNumberOfRow(std::string str)
{
	std::wifstream in;
	int count;

	in.open("row.txt", std::ios::in);

	if (in.is_open()) {
		in >> count;
		in.close();
	}
	else {
		count = 0;
	}

	return count;
}

int AddToTextFile(LPTSTR category, LPTSTR content, LPTSTR money)
{
	std::wofstream outCategory;
	std::wofstream outContent;
	std::wofstream outMoney;
	std::wofstream out;
	std::wifstream in;
	int count;

	outCategory.open("data_Category.txt", std::ios::app);
	outContent.open("data_Content.txt", std::ios::app);
	outMoney.open("data_Money.txt", std::ios::app);

	outCategory << category << "\n";
	outContent << content << "\n";
	outMoney << money << "\n";

	outCategory.close();
	outContent.close();
	outMoney.close();

	in.open("row.txt", std::ios::in);

	if (in.is_open()) {
		in >> count;
		in.close();
	}
	else {
		count = 0;
	}

	count++;
	out.open("row.txt", std::ios::trunc);
	out << count;
	out.close();

	return 0;
}

BOOL ValidateData(LPTSTR money) {

	int lenMoney;

	lenMoney = _tcsclen(money);

	if (0 == lenMoney) {

		MessageBox(NULL, L"Input is empty", L"Error", NULL);
		return false;

	}

	for (int i = 0; i < lenMoney; i++) {
		if (!iswdigit(money[i])) {
			MessageBox(NULL, L"Money box had incorrect input", L"Error", NULL);
			return false;
		}
	}
	return true;
}

wchar_t* AttatchResultString(wchar_t* notificationString, float input) {

	std::wostringstream oss;
	oss << std::fixed << std::setprecision(2) << input;

	std::wstring ws = oss.str();

	wchar_t result[1024];

	wcscpy(result, notificationString);

	wcscat(result, ws.c_str());

	return result;
}

int getSlideCount()
{
	int typeCount = CExpense::getTypeCount(AllExpense);
	return typeCount;
}
