#include <windows.h>

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
HINSTANCE g_hInst;
HWND hWndMain;
LPCTSTR lpszClass = TEXT("±è¿õ¿ø_16032020");

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

void ConvertingArrPos_Descartes(POINT* _posArr, RECT cRect, int Arrsize)
{
	int CenterX = cRect.right / 2;
	int CenterY = cRect.bottom / 2;
	for (int i = 0; i < Arrsize; i++)
	{
		_posArr[i].x = CenterX + _posArr[i].x;
		_posArr[i].y = CenterY - _posArr[i].y;
	}
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam)
{
	HDC hdc;
	PAINTSTRUCT ps;
	static RECT rect;
	static POINT xAxis[AxisSize];
	static POINT yAxis[AxisSize];

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

		ConvertingArrPos_Descartes(xAxis, rect, AxisSize);
		ConvertingArrPos_Descartes(yAxis, rect, AxisSize);

		break;
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);

		for (int i = 0; i < AxisSize; i++)
		{
			//xÃà yÃà ±×¸®±â
			SetPixel(hdc, xAxis[i].x, xAxis[i].y, RGB(0, 0, 0));
			SetPixel(hdc, yAxis[i].x, yAxis[i].y, RGB(0, 0, 0));
		}

		EndPaint(hWnd, &ps);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}
	return(DefWindowProc(hWnd, iMessage, wParam, lParam));
}

