#include <windows.h>
#include "Polygon_Function.h"
#include "Characters.h"

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
{// 어.. 
    RECT cRect;
    GetClientRect(hWnd, &cRect);
    int CenterX = cRect.right / 2;
    int CenterY = cRect.bottom / 2;

    Point temp;
    temp.x = pos.x - CenterX;
    temp.y = CenterY - pos.y;
    return temp;
}

float GetDistance(Vector2D a, Vector2D b)
{
    float dx = b.x - a.x;
    float dy = b.y - a.y;

    float distance = sqrtf(powf(dx, 2) + powf(dy, 2));

    return distance;
}

float GetAtan2(Vector2D& a, Vector2D& b)
{
    Vector2D vec;
    vec = b - a;
    float angle = atan2f(vec.y, vec.x) * 180 / 3.141592;
    if (angle < 0)
        angle += 360;
    return angle;
}

void DrawLine(HWND hWnd, HDC hdc, Point a, Point b, COLORREF color)
{
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

}
void DrawCircleCollider(HDC hdc, Circle_Collider& collider)
{
    Point temp;
    float dx, dy;

    for (float i = 0; i < 360; i += 0.25)
    {
        float angle = i * 3.141592 / 180;
        dx = collider.radius * cosf(angle);
        dy = collider.radius * sinf(angle);

        dx += collider.Center.getPos().x;
        dy += collider.Center.getPos().y;

        SetPixel(hdc, dx, dy, collider.color);
    }
}

void DrawTriangle(HWND hWnd, HDC hdc, Point a, Point b, Point c) 
{
    DrawLine(hWnd, hdc, a, b, RGB(0, 0, 0));
    DrawLine(hWnd, hdc, b, c, RGB(0, 0, 0));
    DrawLine(hWnd, hdc, c, a, RGB(0, 0, 0));
}
void DrawTriangle(HWND hWnd, HDC hdc, Triangle2D& T)
{
    DrawLine(hWnd, hdc, T._A.getPos(), T._B.getPos(), RGB(0, 0, 0));
    DrawLine(hWnd, hdc, T._B.getPos(), T._C.getPos(), RGB(0, 0, 0));
    DrawLine(hWnd, hdc, T._C.getPos(), T._A.getPos(), RGB(0, 0, 0));
}


void DrawTriangle(HWND hWnd, HDC hdc, Triangle2D& T, COLORREF color)
{
    DrawLine(hWnd, hdc, T._A.getPos(), T._B.getPos(), color);
    DrawLine(hWnd, hdc, T._B.getPos(), T._C.getPos(), color);
    DrawLine(hWnd, hdc, T._C.getPos(), T._A.getPos(), color);
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

void SetMoveValue(Point a, Point b, Point* _val, float speed, float& size)
{
    *_val = Point(0, 0);

    float dx = b.x - a.x;
    float dy = b.y - a.y;

    float distance = sqrtf(powf(dx, 2) + powf(dy, 2));
    size = distance / speed;

    dx = dx / (float)size;
    dy = dy / (float)size;

    _val->x = dx;
    _val->y = dy;
}

void CreateCollider(HWND hWnd, Triangle2D& _t, Circle_Collider& collider)
{
    Point temp;
    temp = ConvertingWorldPos(hWnd, _t.Center.getPos());

    Vector2D vec_temp(temp.x, temp.y);
    collider.Update(vec_temp, GetDistance(_t.Center, _t._A));
}

void Collision(Circle_Collider& _c1, Circle_Collider& _c2)
{
    float min_distance = _c1.radius + _c2.radius;

    if (!_c1.enabled || !_c2.enabled)
        return;

    if (min_distance >= GetDistance(_c1.Center, _c2.Center))
    {
        _c1.hit = true;
        _c2.hit = true;

        _c1.color = RGB(255, 0, 255);
        _c2.color = RGB(255, 0, 255);
    }
    else
    {
        _c1.hit = false;
        _c2.hit = false;

        _c1.color = RGB(0, 255, 0);
        _c2.color = RGB(0, 255, 0);
    }
    
}
#define MOVESPEED 10
#define MIN_DISTANCE 300
#define MAX_DISTANCE 500

LRESULT CALLBACK WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam)
{
    PAINTSTRUCT ps;
    HDC hdc, BACKDC;
    HBITMAP backBitmap, OldBackBitmap;
    static RECT rect;

    static Point mpos, convert;
    static float angle;
    static int ei = 0;
    static int i = 0;
    // 항상 자료구조 생각하기

    static Player<Triangle2D> player;
    static Enemy<Triangle2D> enemy(4);
    
    static Point temp;
    
    
    switch (iMessage) {
    case WM_CREATE:
        GetClientRect(hWnd, &rect);

        // Player Initalize
        player.Model = Triangle2D(0, 125, -75, 0, 75, 0);
        player.Setting_Model();

        CreateCollider(hWnd, player.Model, player.collider);
        player.collider.color = RGB(0, 255, 0);
        
        // Enemy Initalize
        enemy.Model = Triangle2D(300, -175, 225, -300, 375, -300);
        enemy.Setting_Model();

        CreateCollider(hWnd, enemy.Model, enemy.collider);
        enemy.collider.color = RGB(0, 255, 0);

        enemy.WayPoints[0].setPos( Point(100, -250));
        enemy.WayPoints[1].setPos( Point(600, -250));
        enemy.WayPoints[2].setPos( Point(600, 250));
        enemy.WayPoints[3].setPos( Point(100, 250));

        SetTimer(hWnd, 1, 50, NULL);
        break;
    case WM_KEYDOWN:
        player.is_keyinput = true;
        
        break;
    case WM_CHAR:
        // Keyboard Move
        player.move_val = Point(0, 0);
        if (wParam == 'z' || wParam == 'Z')
            player.scale_val = 1.1f;
        else if (wParam == 'c' || wParam == 'C')
            player.scale_val = 0.9f;

        if (wParam == 'e' || wParam == 'E')
            player.rotate_val = 5;
        else if (wParam == 'q' || wParam == 'Q')
            player.rotate_val = -5;

        if (wParam == 'w' || wParam == 'W')
            player.move_val.y = MOVESPEED * 2;
        else if (wParam == 's' || wParam == 'S')
            player.move_val.y = -MOVESPEED * 2;

        if (wParam == 'a' || wParam == 'A')
            player.move_val.x = -MOVESPEED * 2;
        else if (wParam == 'd' || wParam == 'D')
            player.move_val.x = MOVESPEED * 2;

        break;
    case WM_KEYUP:
        player.is_keyinput = false;
        player.Reset_value();
        break;
    case WM_LBUTTONDOWN:
        // Mouse Move
        if (!player.is_keyinput)
        {
            player.move_val = Point::Zero;
            player.toggle = true;

            i = 0;
            mpos.x = LOWORD(lParam);
            mpos.y = HIWORD(lParam);
            convert = ConvertingLocalPos(hWnd, mpos);
            SetMoveValue(player.Model.Center.getPos(), convert, &player.move_val, MOVESPEED * 2, player.movecount);   // 증가값 계산
        }
        break;  
    case WM_TIMER:
        if (player.toggle)     // player mouse clieked move
        {
            if (i++ <= player.movecount)
            {
                Polygon_Function::Move(player.Model, player.move_val);
                CreateCollider(hWnd, player.Model, player.collider);
                player.Setting_Model();
            }
        }
        
        if (player.is_keyinput)    // player WASD move
        {
            Polygon_Function::Translate(player.Model, player.move_val);
            Polygon_Function::Rotate(player.Model, player.rotate_val);
            Polygon_Function::Scale(player.Model, player.scale_val, player.scale_val, (int)player.scale_val);
            CreateCollider(hWnd, player.Model, player.collider);
            player.Setting_Model();
        }

        if (enemy.is_move)   // enemy romaing
        {
            // enemy Following Start
            if (GetDistance(player.Model.Center, enemy.Model.Center) <= MIN_DISTANCE)  
            {
                SetMoveValue(enemy.Model.Center.getPos(), player.Model.Center.getPos(), &enemy.move_val, MOVESPEED, enemy.movecount);
                ei = 0;
                enemy.is_following = true;
                angle = -GetAtan2(enemy.Model.Center, player.Curr_Model.Center) + 90;
            }
            // enemy Following End
            else if(enemy.is_following && GetDistance(player.Model.Center, enemy.Model.Center) <= MAX_DISTANCE)
            {
                enemy.is_move = false;
                enemy.is_following = false;

                float distance = GetDistance(enemy.Model.Center, enemy.WayPoints[enemy.nextIndex]);
                for (int i = 0; i < 4; i++)
                {
                    float temp = GetDistance(enemy.Model.Center, enemy.WayPoints[i]);
                    if (distance < temp)
                    {
                        distance = temp;
                        enemy.nextIndex = i + 1;
                    }
                }
            }
            // Enemy Roming End
            else if (ei++ >= enemy.movecount)
            {
                enemy.is_move = false;
                break;
            }

            Polygon_Function::Translate(enemy.Model, enemy.move_val);
            CreateCollider(hWnd, enemy.Model, enemy.collider);
            enemy.Setting_Model();
            Polygon_Function::Rotate(enemy.Curr_Model, angle);
        }
        else  
        {
            // next waypoint set
            ei = 0;
            enemy.nextIndex = ++enemy.nextIndex % 4;  // 도착하면 다음 웨이 포인트로
            angle = -GetAtan2(enemy.Model.Center, enemy.WayPoints[enemy.nextIndex]) + 90;  // 회전 계산
            
            SetMoveValue(enemy.Model.Center.getPos(), enemy.WayPoints[enemy.nextIndex].getPos(), &enemy.move_val, MOVESPEED, enemy.movecount);
            enemy.is_move = true;
        }

        Collision(player.collider, enemy.collider);
        InvalidateRect(hWnd, NULL, TRUE);
        break;
    case WM_PAINT:  // 더블 버퍼링
        hdc = BeginPaint(hWnd, &ps);
        BACKDC = CreateCompatibleDC(hdc);
        backBitmap = CreateCompatibleBitmap(hdc, rect.right, rect.bottom);
        OldBackBitmap = (HBITMAP)SelectObject(BACKDC, backBitmap);

        PatBlt(BACKDC, 0, 0, rect.right, rect.bottom, WHITENESS);
        
        TextOut(BACKDC, 10, 10, TEXT("Translate : W, A, S, D"), strlen("Translate : W, A, S, D"));
        TextOut(BACKDC, 10, 30, TEXT("Rotate : Q, E"), strlen("Rotate : Q, E"));
        TextOut(BACKDC, 10, 50, TEXT("Scale : Z, C"), strlen("Scale : Z, C"));
        TextOut(BACKDC, 10, 70, TEXT("Move to Point : LButtonDown"), strlen("Move to Point : LButtonDown"));

        for (int i = 0; i < AxisSize; i++)
        {
            SetPixel(BACKDC, i, rect.bottom / 2, RGB(0, 0, 0));
            SetPixel(BACKDC, rect.right / 2, i, RGB(0, 0, 0));
        }
        
        for (int i = 0; i < 4; i++)
        {
            char waytext[20];
            Point temp = ConvertingWorldPos(hWnd, enemy.WayPoints[i].getPos());
            sprintf_s(waytext, "WayPoint %d", i + 1);

            TextOut(BACKDC, temp.x - strlen(waytext) * 5, temp.y - 20, waytext, strlen(waytext));
            Ellipse(BACKDC, temp.x - 5, temp.y - 5, temp.x + 5, temp.y + 5);
        }
        
        // Draw Enemy Entry
        
        if(enemy.is_following)
            DrawTriangle(hWnd, BACKDC, enemy.Curr_Model, RGB(255, 0, 0));
        else 
            DrawTriangle(hWnd, BACKDC, enemy.Curr_Model, RGB(0, 255, 0));

        enemy.Foward = ConvertingWorldPos(hWnd, enemy.Curr_Model._A.getPos());
        Ellipse(BACKDC, enemy.Foward.x - 5, enemy.Foward.y - 5, enemy.Foward.x + 5, enemy.Foward.y + 5);

        DrawCircleCollider(BACKDC, enemy.collider);
        Ellipse(BACKDC, temp.x - 5, temp.y - 5, temp.x + 5, temp.y + 5);

        

        DrawTriangle(hWnd, BACKDC, player.Curr_Model);

        player.Foward = ConvertingWorldPos(hWnd, player.Curr_Model._A.getPos());
        Ellipse(BACKDC, player.Foward.x - 5, player.Foward.y - 5, player.Foward.x + 5, player.Foward.y + 5);

        DrawCircleCollider(BACKDC, player.collider);

        BitBlt(hdc, 0, 0, rect.right, rect.bottom, BACKDC, 0, 0, SRCCOPY);

        SelectObject(BACKDC, OldBackBitmap);
        DeleteDC(BACKDC);
        DeleteObject(backBitmap);
        EndPaint(hWnd, &ps);
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;
    }
    return(DefWindowProc(hWnd, iMessage, wParam, lParam));
}
