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
    WndClass.style = CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS;
    RegisterClass(&WndClass);

    hWnd = CreateWindow(lpszClass, lpszClass, WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,
        NULL, (HMENU)NULL, hInstance, NULL);
    ShowWindow(hWnd, nCmdShow);

    while (GetMessage(&Message, NULL, 0, 0)) {   //Queue에 있는 메세지를 읽어들인다
        if ((Message.message == WM_KEYDOWN || Message.message == WM_KEYUP) && Message.hwnd != hWnd) {
            PostMessage(hWnd, Message.message, Message.wParam, Message.lParam);
        }
        TranslateMessage(&Message);   //키보드 입력 메세지를 가공하여 프로그램에서 쉽게 사용할 수 있도록
        DispatchMessage(&Message);   //메세지를 윈도우의 메세지 처리 함수 WndProc로 전달
    }
    return (int)Message.wParam;
}

#define AxisSize 2000

Point ConvertingPos_Descartes(RECT cRect, Point pos)
{
    int CenterX = cRect.right / 2;
    int CenterY = cRect.bottom / 2;

    Point temp;
    temp.x = pos.x + CenterX;
    temp.y = CenterY - pos.y;
    return temp;
}
Point ConvertingPos_Windows(RECT cRect, Point pos)
{
    int CenterX = cRect.right / 2;
    int CenterY = cRect.bottom / 2;

    Point temp;
    temp.x = pos.x - CenterX;
    temp.y = CenterY - pos.y;
    return temp;
}

void DrawLine(HWND hWnd, Point a, Point b, COLORREF color)
{
    HDC hdc;
    RECT rect;
    hdc = GetDC(hWnd);
    GetClientRect(hWnd, &rect);
    
    float dx = b.x - a.x;
    float dy = b.y - a.y;

    int DomainCount = abs((int)dx) > abs((int)dy) ? abs((int)dx):abs((int)dy);
    dx = dx / DomainCount;
    dy = dy / DomainCount;

    
    Point Wtemp, Ltemp;
    Ltemp.x = a.x;
    Ltemp.y = a.y;

    while (1)
    {
        Wtemp = ConvertingPos_Descartes(rect, Ltemp);
        SetPixel(hdc, Wtemp.x, Wtemp.y, color);

        Ltemp.x += dx;
        Ltemp.y += dy;

        DomainCount--;
        if (DomainCount < 0)
            break;
    }

    ReleaseDC(hWnd, hdc);
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam)
{
    PAINTSTRUCT ps;
    HDC hdc;
    static RECT rect;
    static Point xAxis[AxisSize];
    static Point yAxis[AxisSize];

    static Point mpos, convert;
    static Vector2D A; 
    static Vector2D B;
    static Vector2D plus, minus;

    static bool toggle;

    switch (iMessage) {

    case WM_CREATE:
        GetClientRect(hWnd, &rect);

        toggle = true;

        break;
    case WM_LBUTTONDOWN:
        mpos.x = LOWORD(lParam);
        mpos.y = HIWORD(lParam);
        convert = ConvertingPos_Windows(rect, mpos);
        if (toggle)
        {
            A.setx(convert.x);
            A.sety(convert.y);
            toggle = !toggle;
        }
        else
        {
            B.setx(convert.x);
            B.sety(convert.y);
            toggle = !toggle;

            plus = A + B;
            minus = A - B;
            InvalidateRect(hWnd, NULL, TRUE);
        }
        
        break;
    case WM_PAINT:
        hdc = BeginPaint(hWnd, &ps);

        for (int i = 0; i < AxisSize; i++)
        {
            SetPixel(hdc, i, rect.bottom / 2, RGB(0, 0, 0));
            SetPixel(hdc, rect.right / 2, i, RGB(0, 0, 0));
        }

        DrawLine(hWnd, Point(0, 0), A.getPos(), RGB(0, 0, 0));
        DrawLine(hWnd, Point(0, 0), B.getPos(), RGB(0, 0, 0));
        DrawLine(hWnd, Point(0, 0), plus.getPos(), RGB(0, 0, 255));
        DrawLine(hWnd, Point(0, 0), minus.getPos(), RGB(255, 0, 0));

        EndPaint(hWnd, &ps);
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;
    }
    return(DefWindowProc(hWnd, iMessage, wParam, lParam));
}