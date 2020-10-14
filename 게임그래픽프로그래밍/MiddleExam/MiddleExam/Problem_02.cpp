#include <windows.h>
#include "Vector2D.h"

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

void ConvertingArrPos_Descartes(Point* _posArr, RECT cRect, int Arrsize)
{
	int CenterX = cRect.right / 2;
	int CenterY = cRect.bottom / 2;
	for (int i = 0; i < Arrsize; i++)
	{
		_posArr[i].x = CenterX + _posArr[i].x;
		_posArr[i].y = CenterY - _posArr[i].y;
	}
}

void ConvertingPos_Descartes(Point* _pos, RECT cRect)
{
	int CenterX = cRect.right / 2;
	int CenterY = cRect.bottom / 2;
	_pos->x = CenterX + _pos->x;
	_pos->y = CenterY - _pos->y;
}


void PtoPSetLine(Point _first, Point _sceond, Point* _posArr, int Arrsize)
{
	if (_sceond.x - _first.x == 0)
		return;
	if (_sceond.y - _first.y == 0)
		return;

	float m = (_sceond.y - _first.y) / (_sceond.x - _first.x);	// 기울기
	float b = _first.y - m * _first.x;

	for (int i = 0; i < Arrsize; i++)
	{
		_posArr[i].y = m * _posArr[i].x + b;
	}

}

LRESULT CALLBACK WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam)
{
	HDC hdc;
	PAINTSTRUCT ps;
	static RECT rect;
	static Point xAxis[AxisSize];
	static Point yAxis[AxisSize];

	static Point First, Scenod;
	static Point Line_01[AxisSize];

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

			Line_01[i + (AxisSize / 2)].x = i;
			Line_01[i + (AxisSize / 2)].y = 0;
		}

		First.x = 300;
		First.y = 300;
		Scenod.x = -54;
		Scenod.y = -318;

		PtoPSetLine(First, Scenod, Line_01, AxisSize);
		ConvertingArrPos_Descartes(Line_01, rect, AxisSize);

		ConvertingPos_Descartes(&First, rect);
		ConvertingPos_Descartes(&Scenod, rect);

		ConvertingArrPos_Descartes(xAxis, rect, AxisSize);
		ConvertingArrPos_Descartes(yAxis, rect, AxisSize);

		InvalidateRect(hWnd, NULL, TRUE);
		break;
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);

		for (int i = 0; i < AxisSize; i++)
		{
			//x축 y축 그리기
			SetPixel(hdc, xAxis[i].x, xAxis[i].y, RGB(0, 0, 0));
			SetPixel(hdc, yAxis[i].x, yAxis[i].y, RGB(0, 0, 0));
			SetPixel(hdc, Line_01[i].x, Line_01[i].y, RGB(255, 0, 0));
		}

		Ellipse(hdc, First.x - 5, First.y - 5, First.x + 5, First.y + 5);
		TextOut(hdc, First.x, First.y - 20, TEXT("First"), 5);
		Ellipse(hdc, Scenod.x - 5, Scenod.y - 5, Scenod.x + 5, Scenod.y + 5);
		TextOut(hdc, Scenod.x, Scenod.y - 20, TEXT("Sceond"), 6);

		EndPaint(hWnd, &ps);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}
	return(DefWindowProc(hWnd, iMessage, wParam, lParam));
}

