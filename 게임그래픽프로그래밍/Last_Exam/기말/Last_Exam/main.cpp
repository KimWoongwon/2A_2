#include <windows.h>
#include "../Last_Exam/Header/Polygon_Function.h"
#include "../Last_Exam/Header/Characters.h"

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

Point ConvertingPos_Descartes(HWND hWnd, Point pos)
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
Point ConvertingPos_Windows(HWND hWnd, Point pos)
{
    RECT cRect;
    GetClientRect(hWnd, &cRect);
    int CenterX = cRect.right / 2;
    int CenterY = cRect.bottom / 2;

    Point temp;
    temp.x = pos.x - CenterX;
    temp.y = CenterY - pos.y;
    return temp;
}

float GetDistance(Point a, Point b)     // �� �������� �Ÿ� ��� �Լ�
{
    float dx = b.x - a.x;
    float dy = b.y - a.y;

    float distance = sqrtf(powf(dx, 2) + powf(dy, 2));

    return distance;
}

float GetAtan2(Vector2D& a, Vector2D& b)        // a ���Ͱ� b���͸� �ٶ󺸰� ȸ������ ��� �Լ�
{
    Vector2D vec;
    vec = b - a;
    float angle = atan2f(vec.y, vec.x) * 180 / 3.141592;
    if (angle < 0)
        angle += 360;
    return angle;
}

void DrawLine(HWND hWnd, HDC hdc, Point a, Point b, COLORREF color)     // �� �׷��ִ� �Լ�
{
    int DomainCount;

    float dx = b.x - a.x;
    float dy = b.y - a.y;

    float x = a.x;
    float y = a.y;

    DomainCount = abs((int)dx) > abs((int)dy) ? abs((int)dx) : abs((int)dy);
    dx = dx / DomainCount;
    dy = dy / DomainCount;

    Point Wtemp, Ltemp;

    while (1)
    {
        Ltemp.x = x;
        Ltemp.y = y;

        Wtemp = ConvertingPos_Descartes(hWnd, Ltemp);
        SetPixel(hdc, Wtemp.x, Wtemp.y, color);

        x += dx;
        y += dy;

        DomainCount--;
        if (DomainCount < 0)
            break;
    }

}

// ���� 1�� �ﰢ�� �׸��� �Լ�
void DrawTriangle(HWND hWnd, HDC hdc, Triangle2D& T, COLORREF color = RGB(0, 0, 0))     // �ﰢ�� �׷��ִ� �Լ�
{
    DrawLine(hWnd, hdc, T._A.getPos(), T._B.getPos(), color);
    DrawLine(hWnd, hdc, T._B.getPos(), T._C.getPos(), color);
    DrawLine(hWnd, hdc, T._C.getPos(), T._A.getPos(), color);
}

void DrawCircleCollider(HDC hdc, Circle_Collider& collider)     // ���� �ݸ��� �׷��ִ� �Լ�
{
    Point temp;
    float dx, dy;

    for (float i = 0; i < 360; i += 0.25)
    {
        float angle = i * 3.141592 / 180;
        dx = collider.radius * cosf(angle);
        dy = collider.radius * sinf(angle);

        dx += collider.Center.x;
        dy += collider.Center.y;

        SetPixel(hdc, dx, dy, collider.color);
    }
}

// ���� 2�� Ŭ���� �������� �̵��� ��� �Լ�
void SetMoveValue(Point a, Point b, Point* _val, float speed, float& size)  // �̵��� ���� �Լ�
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
    // Triangle�� �ִ� Center���� ������ ��ǥ���̹Ƿ� ��ī��Ʈ ��ǥ��� ��ȯ
    Point temp;
    temp = ConvertingPos_Descartes(hWnd, _t.Center.getPos());

    // collider�� Center���� Triangle�� Center���̰� ����
    // �������� Triangle�� Center ���� A�� (�ﰢ���� �����)�� �Ÿ��� ����
    // ���� ���� �ﰢ���� �� �´� ������ ����
    Vector2D vec_temp(temp.x, temp.y);
    collider.Update(vec_temp, GetDistance(_t.Center.getPos(), _t._A.getPos()));
}

// ���� 3�� �浹 ó�� �Լ�
bool Collision(Circle_Collider& _c1, Circle_Collider& _c2)
{
    // �浹 �ּ� ���� ���� -> ���� �浹ü �̱� ������ �� ��ü�� ������ ���� ���� ���� �ּ� �浹 �Ÿ�
    float min_distance = _c1.radius + _c2.radius;

    if (!_c1.enabled || !_c2.enabled)
        return false;

    // �� ���Ϳ��� �Ÿ��� �ּ� �浹 �Ÿ����� �۴ٸ� �浹 ����
    if (min_distance >= GetDistance(_c1.Center.getPos(), _c2.Center.getPos()))
        return true;
    // �� ���Ϳ��� �Ÿ��� �ּ� �浹 �Ÿ����� ũ�ٸ� �浹���� �ʾҴٰ� ����
    else
        return false;

}
#define MOVESPEED 10
#define MIN_DISTANCE 200
#define MAX_DISTANCE 400

#define IDT_PLAYER_MOVE 1
#define IDT_ENEMY_MOVE 2
#define IDT_COLLISION 3

#define Enemy_Count 3

LRESULT CALLBACK WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam)
{
    PAINTSTRUCT ps;
    HDC hdc, BACKDC;
    HBITMAP backBitmap, OldBackBitmap;
    static RECT rect;

    static Point mpos, convert;
    static int ei = 0;
    static int pi = 0;

    // Triangle2D <- Vertex2D�� ������ ��ü
    static Player<Triangle2D> player;
    static float player_angle;

    static Enemy<Triangle2D> enemy[Enemy_Count];
    static float angle[Enemy_Count];

    static Point temp;

    switch (iMessage) {
    case WM_CREATE:
        GetClientRect(hWnd, &rect);

#pragma region ___________���� 1�� Initalize Player & Enemy___________
        // Player Initalize
        player.Model = Triangle2D(0, 50, -30, 0, 30, 0);
        player.Setting_Model();

        CreateCollider(hWnd, player.Model, player.collider);
        player.collider.color = RGB(0, 255, 0);

        // Enemy Initalize

        for (int i = 0; i < Enemy_Count; i++)
        {
            enemy[i].Model = Triangle2D(200 - i * 50, 100, 170 - i * 50, 50, 230 - i * 50, 50);
            enemy[i].Setting_Model();

            CreateCollider(hWnd, enemy[i].Model, enemy[i].collider);
            enemy[i].collider.color = RGB(0, 255, 0);

            enemy[i].Set_WayPoints(4);
            enemy[i].WayPoints[0] = Point(500 - i * 500, 100);
            enemy[i].WayPoints[1] = Point(500 - i * 500, 300);
            enemy[i].WayPoints[2] = Point(700 - i * 500, 100);
            enemy[i].WayPoints[3] = Point(700 - i * 500, 300);
        }
#pragma endregion
        
        SetTimer(hWnd, IDT_PLAYER_MOVE, 50, NULL);
        SetTimer(hWnd, IDT_ENEMY_MOVE, 50, NULL);
        SetTimer(hWnd, IDT_COLLISION, 50, NULL);
        break;
    case WM_LBUTTONDOWN:
#pragma region ___________���� 2�� Player Mouse Click Move___________
        // Mouse Move
        if (!player.is_keyinput)
        {
            player.move_val = Point::Zero;
            player.toggle = true;

            pi = 0;
            mpos.x = LOWORD(lParam);
            mpos.y = HIWORD(lParam);
            convert = ConvertingPos_Windows(hWnd, mpos);
            SetMoveValue(player.Model.Center.getPos(), convert, &player.move_val, MOVESPEED * 1.5f, player.movecount);   // ������ ���

            Vector2D temp(convert.x, convert.y);
            player_angle = -GetAtan2(player.Curr_Model.Center, temp) + 90;
            
        }
#pragma endregion
        break;
    case WM_TIMER:
        switch (wParam)
        {
        case IDT_PLAYER_MOVE:
#pragma region ___________���� 2�� Player Mouse Clicked Move___________
            if (player.toggle)     // player mouse cliek move
            {
                if (pi++ <= player.movecount)
                {
                    Polygon_Function::Translate(player.Model, player.move_val);  // �̵� ��ȯ��� ������ ����
                    CreateCollider(hWnd, player.Model, player.collider);
                    player.Setting_Model();
                    Polygon_Function::Rotate(player.Curr_Model, player_angle);
                }
            }
#pragma endregion
            break;
        case IDT_ENEMY_MOVE:
#pragma region ___________���� 3�� Enemy Roaming AI___________
            for (int i = 0; i < Enemy_Count; i++)
            {
                float distance = GetDistance(player.Model.Center.getPos(), enemy[i].Model.Center.getPos());
                if (enemy[i].is_move)   // enemy romaing
                {
                    // enemy Following Start
                    // �Ÿ� ��� �� �νĹ��� ���� �÷��̾��� ���Ͱ����� �̵��� ���
                    if (distance <= MIN_DISTANCE && distance > 10)
                    {
                        SetMoveValue(enemy[i].Model.Center.getPos(), player.Model.Center.getPos(), &enemy[i].move_val, MOVESPEED, enemy[i].movecount);
                        ei = 0;
                        enemy[i].is_following = true;
                        enemy[i].color = RGB(255, 0, 0);
                        angle[i] = -GetAtan2(enemy[i].Model.Center, player.Curr_Model.Center) + 90;
                    }

                    // enemy Following End
                    // �Ÿ� ��� �� �νĹ��� ���̶�� WayPoint 1���� �̵�
                    else if (enemy[i].is_following && distance < MAX_DISTANCE)
                    {
                        enemy[i].is_move = false;
                        enemy[i].color = RGB(0, 0, 255);
                        enemy[i].nextIndex = 0;
                    }

                    // Enemy Roming End
                    // WayPoint�� �����ϸ� ���� WayPoint�� �̵� ����
                    else if (enemy[i].ei++ >= enemy[i].movecount)
                    {
                        enemy[i].is_move = false;
                        break;
                    }

                    // �̵�
                    Polygon_Function::Translate(enemy[i].Model, enemy[i].move_val);
                    CreateCollider(hWnd, enemy[i].Model, enemy[i].collider);
                    enemy[i].Setting_Model();
                    Polygon_Function::Rotate(enemy[i].Curr_Model, angle[i]);
                }
                else
                {
                    // ���� WayPoint�� �̵� �����ϱ�

                    if (distance <= 10)
                        break;
                    
                    enemy[i].ei = 0;
                    if (enemy[i].is_following)
                        enemy[i].is_following = false;
                    else
                        enemy[i].nextIndex++;   // �����ϸ� ���� ���� ����Ʈ��

                    enemy[i].nextIndex %= 4;

                    Vector2D temp(enemy[i].WayPoints[enemy[i].nextIndex].x, enemy[i].WayPoints[enemy[i].nextIndex].y);
                    angle[i] = -GetAtan2(enemy[i].Model.Center, temp) + 90;  // ȸ�� ���

                    SetMoveValue(enemy[i].Model.Center.getPos(), enemy[i].WayPoints[enemy[i].nextIndex], &enemy[i].move_val, MOVESPEED, enemy[i].movecount);
                    enemy[i].is_move = true;
                }

                
            }
#pragma endregion
            break;
        case IDT_COLLISION:
#pragma region ___________���� 3�� Collision___________
            for (int i = 0; i < Enemy_Count; i++)
            {
                player.collider.hit = Collision(player.collider, enemy[i].collider);
                if (player.collider.hit)
                    break;
            }

            if (player.collider.hit)
                player.color = RGB(255, 0, 0);
            else
                player.color = RGB(0, 0, 0);
#pragma endregion
            break;
        }
        
        InvalidateRect(hWnd, NULL, FALSE);
        break;
    case WM_PAINT:  // ���� ���۸�
        hdc = BeginPaint(hWnd, &ps);
        BACKDC = CreateCompatibleDC(hdc);
        backBitmap = CreateCompatibleBitmap(hdc, rect.right, rect.bottom);
        OldBackBitmap = (HBITMAP)SelectObject(BACKDC, backBitmap);

        PatBlt(BACKDC, 0, 0, rect.right, rect.bottom, WHITENESS);

        for (int i = 0; i < Enemy_Count; i++)
        {
            for (int j = 0; j < 4; j++)
            {
                TCHAR waytext[20];
                Point temp = ConvertingPos_Descartes(hWnd, enemy[i].WayPoints[j]);
                wsprintf(waytext, TEXT("E%d WP %d"), i + 1, j + 1);

                TextOut(BACKDC, temp.x - lstrlen(waytext) * 5, temp.y - 20, waytext, lstrlen(waytext));
                Ellipse(BACKDC, temp.x - 5, temp.y - 5, temp.x + 5, temp.y + 5);
            }
        }

#pragma region ___________���� 1�� Draw Player & Enemy___________
        // Draw Enemy Entry
        for (int i = 0; i < Enemy_Count; i++)
        {
            DrawTriangle(hWnd, BACKDC, enemy[i].Curr_Model, enemy[i].color);

            enemy[i].Foward = ConvertingPos_Descartes(hWnd, enemy[i].Curr_Model._A.getPos());
            Ellipse(BACKDC, enemy[i].Foward.x - 5, enemy[i].Foward.y - 5, enemy[i].Foward.x + 5, enemy[i].Foward.y + 5);

            DrawCircleCollider(BACKDC, enemy[i].collider);
            Ellipse(BACKDC, temp.x - 5, temp.y - 5, temp.x + 5, temp.y + 5);
        }


        // Draw Player Entry
        DrawTriangle(hWnd, BACKDC, player.Curr_Model, player.color);

        player.Foward = ConvertingPos_Descartes(hWnd, player.Curr_Model._A.getPos());
        Ellipse(BACKDC, player.Foward.x - 5, player.Foward.y - 5, player.Foward.x + 5, player.Foward.y + 5);

        DrawCircleCollider(BACKDC, player.collider);
#pragma endregion

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
