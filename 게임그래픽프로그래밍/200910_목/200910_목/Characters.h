#pragma once
#include <Windows.h>
#include "Triangle2D.h"

struct Circle_Collider
{
	bool enabled = true;

	Vector2D Center;
	COLORREF color = RGB(0, 0, 0);
	float radius;
	bool hit = false;

	void Update(Vector2D _center, float _radius)
	{
		Center = _center;
		radius = _radius;
	}
};

template <typename T>
struct Characters
{
	T Model;			// ���� ��
	T Curr_Model;		// ������ ��
	Circle_Collider collider;

	// ���� ǥ�ø� ���� ��ǥ�� ȸ���� �Ǵ��� ���� ���ؼ� ���
	Point Foward;

	// �̵� ������, ũ�� ������, ȸ�� ������
	Point move_val;		// move increase value
	float rotate_val;
	float scale_val;

	Characters()
	{
		Foward = Point::Zero;
		move_val = Point::Zero;
		rotate_val = 0;
		scale_val = 1;
	}
	Characters(T _t)
	{
		Model = _t;
		Curr_Model = Model;

		Foward = Point::Zero;
		move_val = Point::Zero;
		rotate_val = 0;
		scale_val = 1;
	}
	void Reset_value()
	{
		move_val = Point::Zero;
		rotate_val = 0;
		scale_val = 1;
	}
	void Setting_Model()
	{
		Curr_Model = Model;
	}
};

template <typename T>
struct Player : Characters<T>
{
	float movecount = 0;
	bool is_keyinput = false;
	bool toggle = false;

	Player() {}
	Player(T _t) : Characters<T>(_t) {}

};

template <typename T>
struct Enemy : Characters<T>
{
	Vector2D* WayPoints;
	int nextIndex = 0;
	float movecount = 0;
	bool is_following = false;
	bool is_move = false;

	Enemy() : Characters<T>() {}
	Enemy(int waycount) : Characters<T>()
	{
		WayPoints = new Vector2D[waycount];
	}
	~Enemy()
	{
		delete[] WayPoints;
	}
};