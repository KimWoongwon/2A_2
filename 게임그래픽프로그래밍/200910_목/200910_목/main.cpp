#include <windows.h>
#include "Poligon_Function.h"

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
HINSTANCE g_hInst;
HWND hWndMain;
LPCTSTR lpszClass = TEXT("�����_16032020");

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

    while (GetMessage(&Message, NULL, 0, 0)) {   //Queue�� �ִ� �޼����� �о���δ�
        if ((Message.message == WM_KEYDOWN || Message.message == WM_KEYUP) && Message.hwnd != hWnd) {
            PostMessage(hWnd, Message.message, Message.wParam, Message.lParam);
        }
        TranslateMessage(&Message);   //Ű���� �Է� �޼����� �����Ͽ� ���α׷����� ���� ����� �� �ֵ���
        DispatchMessage(&Message);   //�޼����� �������� �޼��� ó�� �Լ� WndProc�� ����
    }
    return (int)Message.wParam;
}

#define AxisSize 2000

Point ConvertingWorldPos(HWND hWnd, Point pos)
{
    RECT cRect;
    GetClientRect(hWnd, &cRect);
    int CenterX = cRect.right / 2;
    int CenterY = cRect.bottom / 2;

    Point temp;
    temp.x = pos.x + CenterX;
    temp.y = CenterY - pos.y;
    return temp;
}
Point ConvertingLocalPos(HWND hWnd, Point pos)
{// ��.. 
    RECT cRect;
    GetClientRect(hWnd, &cRect);
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
    hdc = GetDC(hWnd);
    int DomainCount;

    float dx = b.x - a.x;
    float dy = b.y - a.y;

    float x = a.x;
    float y = a.y;

    DomainCount = abs((int)dx) > abs((int)dy) ? abs((int)dx):abs((int)dy);
    dx = dx / DomainCount;
    dy = dy / DomainCount;

    Point Wtemp, Ltemp;

    while (1)
    {
        Ltemp.x = x;
        Ltemp.y = y;

        Wtemp = ConvertingWorldPos(hWnd, Ltemp);
        SetPixel(hdc, Wtemp.x, Wtemp.y, color);

        x += dx;
        y += dy;

        DomainCount--;
        if (DomainCount < 0)
            break;
    }

    ReleaseDC(hWnd, hdc);
}


void DrawTriangle(HWND hWnd, Point a, Point b, Point c) 
{
    DrawLine(hWnd, a, b, RGB(0, 0, 0));
    DrawLine(hWnd, b, c, RGB(0, 0, 0));
    DrawLine(hWnd, c, a, RGB(0, 0, 0));
}

void DrawTriangle(HWND hWnd, Triangle2D& T)
{
    DrawLine(hWnd, T._A.getPos(), T._B.getPos(), RGB(0, 0, 0));
    DrawLine(hWnd, T._B.getPos(), T._C.getPos(), RGB(0, 0, 0));
    DrawLine(hWnd, T._C.getPos(), T._A.getPos(), RGB(0, 0, 0));
}

void SetMoveValue(Point a, Point b, Point* _val, int size)
{
    *_val = Point(0, 0);

    float dx = b.x - a.x;
    float dy = b.y - a.y;

    dx = dx / (float)size;
    dy = dy / (float)size;

    _val->x = dx;
    _val->y = dy;
}

void RotateZ(Triangle2D& _T, float theta)
{

}

LRESULT CALLBACK WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam)
{
    PAINTSTRUCT ps;
    HDC hdc;
    static RECT rect;
    static Point xAxis[AxisSize];
    static Point yAxis[AxisSize];

    static Point mpos, convert;
    static Triangle2D T;       // ���� ����
    static Triangle2D CurrT;   // Paint ����

    static float scale_value = 1;
    static float rotValue;
    static Point moveValue; // ������
    // �׻� �ڷᱸ�� �����ϱ�

    static bool toggle = 0;
    static bool is_keyinput = false;
    static int i = 0;
    switch (iMessage) {
    case WM_CREATE:
        GetClientRect(hWnd, &rect);

        T = Triangle2D(0,125,-75,0,75,0);
        CurrT = T;

        /*T[0].setPos(0, 125);
        T[1].setPos(-75, 0);
        T[2].setPos(75, 0);
        Triangle_Copy(T, CurrT);*/

        SetTimer(hWnd, 1, 50, NULL);
        break;
    case WM_KEYDOWN:
        is_keyinput = true;
        break;
    case WM_CHAR:
        moveValue = Point(0, 0);
        switch (wParam)
        {
        case 'z':
        case 'Z':
            scale_value = 1.1f;
            break;
        case 'c':
        case 'C':
            scale_value = 0.9f;
            break;
        case 'e':
        case 'E':
            rotValue = 0.3f;
            break;
        case 'q':
        case 'Q':
            rotValue = -0.3f;
            break;
        case 'w':
        case 'W':
            moveValue.y = 10.0f;
            break;
        case 's':
        case 'S':
            moveValue.y = -10.0f;
            break;
        case 'a':
        case 'A':
            moveValue.x = -10.0f;
            break;
        case 'd':
        case 'D':
            moveValue.x = 10.0f;
            break;
        }
        break;
    case WM_KEYUP:
        is_keyinput = false;
        scale_value = 1;
        rotValue = 0;
        moveValue = Point(0, 0);
        break;
    case WM_LBUTTONDOWN:
        if (!toggle)
        {
            moveValue = Point(0, 0);
            i = 0;
            mpos.x = LOWORD(lParam);
            mpos.y = HIWORD(lParam);
            convert = ConvertingLocalPos(hWnd, mpos);
            SetMoveValue(T.Center.getPos(), convert, &moveValue, 10);   // ������ ���
            toggle = true;
        }
        break;  //
    case WM_TIMER:
        if (toggle)
        {
            if (i++ >= 10)
            {
                toggle = false;
                break;
            }
            Poligon_Function::Move(T, moveValue);
            CurrT = T;
        }

        if (is_keyinput)
        {
            Poligon_Function::Translate(T, moveValue);
            Poligon_Function::Rotate(T, rotValue);
            Poligon_Function::Scale(T, scale_value, scale_value, (int)scale_value);
            CurrT = T;
        }

        InvalidateRect(hWnd, NULL, TRUE);
        break;
    case WM_PAINT:
        hdc = BeginPaint(hWnd, &ps);

        TextOut(hdc, 10, 10, TEXT("Translate : W, A, S, D"), strlen("Translate : W, A, S, D"));
        TextOut(hdc, 10, 30, TEXT("Rotate : Q, E"), strlen("Rotate : Q, E"));
        TextOut(hdc, 10, 50, TEXT("Scale : Z, C"), strlen("Scale : Z, C"));
        TextOut(hdc, 10, 70, TEXT("Move to Point : LButtonDown"), strlen("Move to Point : LButtonDown"));

        for (int i = 0; i < AxisSize; i++)
        {
            SetPixel(hdc, i, rect.bottom / 2, RGB(0, 0, 0));
            SetPixel(hdc, rect.right / 2, i, RGB(0, 0, 0));
        }

        DrawTriangle(hWnd, CurrT);

        EndPaint(hWnd, &ps);
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;
    }
    return(DefWindowProc(hWnd, iMessage, wParam, lParam));
}