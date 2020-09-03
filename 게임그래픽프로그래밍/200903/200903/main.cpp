#include <windows.h>

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
HINSTANCE g_hInst;
HWND hWndMain;
LPCTSTR lpszClass = TEXT("김웅원_16032020");

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance
	, LPSTR lpszCmdParam, int nCmdShow)
{
	HWND hWnd;
	MSG Message;
	WNDCLASS WndClass;
	g_hInst = hInstance;

	WndClass.cbClsExtra = 0;
	WndClass.cbWndExtra = 0;
	WndClass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	WndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	WndClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	WndClass.hInstance = hInstance;
	WndClass.lpfnWndProc = WndProc;
	WndClass.lpszClassName = lpszClass;
	WndClass.lpszMenuName = NULL;
	WndClass.style = CS_HREDRAW | CS_VREDRAW;
	RegisterClass(&WndClass);

	hWnd = CreateWindow(lpszClass, lpszClass, WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,
		NULL, (HMENU)NULL, hInstance, NULL);
	ShowWindow(hWnd, nCmdShow);

	while (GetMessage(&Message, NULL, 0, 0)) {
		TranslateMessage(&Message);
		DispatchMessage(&Message);
	}
	return (int)Message.wParam;
}

#define AxisSize 2000
class Equation
{
	float a;
	float b;
	float c;
	
public:
	Equation(int _a, int _b, int _c)
	{
		a = (float)_a;
		b = (float)_b;
		c = -(float)_c;
	}
	Equation(Equation& _val)
	{
		a = _val.a;
		b = _val.b;
		c = _val.c;
	}
	float getA() { return a; }
	float getB() { return b; }
	float getC() { return c; }
	void setA(int _val) { a = (float)_val; }
	void setB(int _val) { b = (float)_val; }
	void setC(int _val) { c = -(float)_val; }
};
void SetPos(POINT* _posArr, int* count, int xpos, int ypos)
{
	if (*count >= 10)
		return;
	_posArr[*count].x = xpos;
	_posArr[(*count)++].y = ypos;
}

void ConvertingPos(POINT* _posArr, RECT cRect, int Arrsize)
{
	int CenterX = cRect.right / 2;
	int CenterY = cRect.bottom / 2;
	for (int i = 0; i < Arrsize; i++)
	{
		_posArr[i].x = CenterX + _posArr[i].x;
		_posArr[i].y = CenterY - _posArr[i].y;
	}
}

void SetLinePos(Equation& _val, POINT* xArr, int Arrsize)
{
	// y = (c/b) - (a/b)x	x-y=3
	for (int i = 0; i < Arrsize; i++)
	{
		xArr[i].y = (_val.getC() / _val.getB()) - ((_val.getA() / _val.getB()) * xArr[i].x);
	}
}

bool GaussFunc(Equation& Line01, Equation& Line02, POINT outvalue)
{
	int a1, a2;
	int b1, b2;
	int c1, c2;
	int i = 2;
	while (1)
	{
		if (Line01.getA() * i == Line02.getA())
		{
			int b = (Line01.getB() * i) + (Line02.getB() * i);
			int c = (Line01.getC() * i) + (Line02.getC() * i);

			int result = (c/b) % 1 == 0 ? (c/b) : 0;


		}
		else if (Line02.getA() * i == Line01.getA())
		{

		}

	}
}

bool Substitution(Equation& Line01, Equation& Line02)
{
	return true;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam)
{
	HDC hdc;
	PAINTSTRUCT ps;
	static RECT rect;
	static POINT xAxis[AxisSize];
	static POINT yAxis[AxisSize];
	static int i = 0;

	Equation LineEquation_01 = { 1,1,10 };
	Equation LineEquation_02 = { 1,-1,30 };
	Equation LineEquation_03 = { 2,3,60 };
	static POINT Line_01[200];
	static POINT Line_02[200];
	static POINT Line_03[200];

	switch (iMessage) 
	{
	case WM_CREATE:
		GetClientRect(hWnd, &rect);

		for (int i = -(AxisSize / 2); i < (AxisSize / 2); i++)
		{
			xAxis[i + (AxisSize / 2)].x = i;
			xAxis[i + (AxisSize / 2)].y = 0;
			yAxis[i + (AxisSize / 2)].x = 0;
			yAxis[i + (AxisSize / 2)].y = i;
		}

		for (int i = -100; i < 100; i++)
		{
			Line_01[i + 100].x = i;
			Line_01[i + 100].y = 0;
			Line_02[i + 100].x = i;
			Line_02[i + 100].y = 0;
			Line_03[i + 100].x = i;
			Line_03[i + 100].y = 0;
		}
		

		// 각 그래프 셋팅후 원점기준 좌표로 변경
		SetLinePos(LineEquation_01, Line_01, 200);
		ConvertingPos(Line_01, rect, 200);

		SetLinePos(LineEquation_02, Line_02, 200);
		ConvertingPos(Line_02, rect, 200);

		SetLinePos(LineEquation_03, Line_03, 200);
		ConvertingPos(Line_03, rect, 200);
		
		ConvertingPos(xAxis, rect, AxisSize);
		ConvertingPos(yAxis, rect, AxisSize);

		break;
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);

		for (int i = 0; i < AxisSize; i++)
		{
			//x축 y축 그리기
			SetPixel(hdc, xAxis[i].x, xAxis[i].y, RGB(0, 0, 0));
			SetPixel(hdc, yAxis[i].x, yAxis[i].y, RGB(0, 0, 0));
		}

		for (int i = 0; i < 200; i++)
		{
			SetPixel(hdc, Line_01[i].x, Line_01[i].y, RGB(255, 0, 0));
			SetPixel(hdc, Line_02[i].x, Line_02[i].y, RGB(0, 255, 0));
			SetPixel(hdc, Line_03[i].x, Line_03[i].y, RGB(0, 0, 255));
		}

		EndPaint(hWnd, &ps);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}
	return(DefWindowProc(hWnd, iMessage, wParam, lParam));
}
