#include <windows.h>
#include <vector>
using namespace std;

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
HINSTANCE g_hInst;
HWND hWndMain;
LPCTSTR lpszClass = TEXT("조남혁_19032081");

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

struct Point
{
    float x;
    float y;
};

vector<vector<Point>> lines;

void MakeLineFunc(Point a, Point b)
{
    vector<Point> line;
    Point v;
    float dx, dy, l;

    dx = b.x - a.x;
    dy = b.y - a.y;
    if (!(abs(dx) < 1 && abs(dy) < 1))
    {
        if (abs(dx) > abs(dy)) // 정의역 설정
        {
            l = abs(dx);
            dx = dx / l;
            dy = dy / l;
        }
        else
        {
            l = abs(dy);
            dx = dx / l;
            dy = dy / l;
        }

        v.x = a.x;
        v.y = a.y;

        while (1)
        {
            line.push_back(v);

            v.x += dx;
            v.y += dy;

            if (v.x == b.x || v.y == b.y)
            {
                break;
            }
        }

        lines.push_back(line);
    }
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam)
{
    static Point a, b;
    static bool toggle;

    vector<vector<Point>>::iterator L_it;
    vector<Point>::iterator V_it;

    PAINTSTRUCT ps;
    HDC hdc;

    switch (iMessage) {

    case WM_CREATE:
        toggle = false;
        break;

    case WM_LBUTTONDOWN:
        if (toggle)
        {
            b.x = LOWORD(lParam);
            b.y = HIWORD(lParam);
            toggle = false;

            MakeLineFunc(a, b);

            InvalidateRect(hWnd, NULL, TRUE);
        }
        else
        {
            a.x = LOWORD(lParam);
            a.y = HIWORD(lParam);
            toggle = true;
        }
        break;

    case WM_PAINT:
        hdc = BeginPaint(hWnd, &ps);

        for (L_it = lines.begin(); L_it != lines.end(); L_it++)
        {
            for (V_it = L_it->begin(); V_it != L_it->end(); V_it++)
            {
                SetPixel(hdc, (int)V_it->x, (int)V_it->y, RGB(255, 0, 0));
            }
        }

        EndPaint(hWnd, &ps);
        break;

    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;
    }
    return(DefWindowProc(hWnd, iMessage, wParam, lParam));
}