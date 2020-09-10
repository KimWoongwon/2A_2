#pragma once 
#include <math.h>

struct Point
{
	float x, y;
	Point()
	{
	}
	Point(int x, int y)
	{
		this->x = x;
		this->y = y;
	}
	Point(Point* P)
	{
		x = P->x;
		y = P->y;
	}
};

class Vector2D
{
	float x, y;
	float size;
public:
	Vector2D()
	{
		x = 0;
		y = 0;
		size = 0;
	}
	Vector2D(float _x, float _y)
	{
		x = _x;
		y = _y;
		size = powf(x,2) + powf(y,2);
	}
	Vector2D(Vector2D& _vec)
	{
		x = _vec.x;
		y = _vec.y;
		size = _vec.size;
	}
	~Vector2D()
	{

	}

	bool operator==(Vector2D& _val)
	{
		if (x == _val.x && y == _val.y && size == _val.size)
			return true;
		return false;
	}

	Vector2D operator*(int _val)
	{
		Vector2D temp(x * _val, y * _val);
		return temp;
	}

	Vector2D operator+(Vector2D& _val)
	{
		Vector2D temp(x + _val.x, y + _val.y);
		return temp;
	}

	Vector2D operator-(Vector2D& _val)
	{
		Vector2D temp(x + _val.operator*(-1).x, y + _val.operator*(-1).y);
		return temp;
	}

	float getx() { return x; }
	float gety() { return y; }
	Point getPos() { return Point(x, y); }

	void setx(float _val) 
	{
		x = _val; 
		size = powf(x, 2) + powf(y, 2);
	}
	void sety(float _val) 
	{
		y = _val; 
		size = powf(x, 2) + powf(y, 2);
	}

	float SizePow() { return size; }
	float Size() { return sqrtf(size); }
	Vector2D normalizaion()
	{
		Vector2D temp(x / sqrt(size), y / sqrt(size));
		return temp;
	}

};
