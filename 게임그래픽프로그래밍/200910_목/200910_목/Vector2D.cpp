
#include "Vector2D.h"

Point Point::Zero = { 0, 0 };

Point::Point()
{
}
Point::Point(int x, int y)
{
	this->x = x;
	this->y = y;
}
Point::Point(Point* P)
{
	x = P->x;
	y = P->y;
}

Vector2D Vector2D::Zero = { 0 , 0 };
Vector2D Vector2D::Identity = { 1 , 1 };

Vector2D::Vector2D()
{
	x = 0;
	y = 0;
	w = 1;
	setsize();
}
Vector2D::Vector2D(float _x, float _y)
{
	x = _x;
	y = _y;
	w = 1;
	setsize();
}

Vector2D::Vector2D(float _x, float _y, float _w)
{
	x = _x;
	y = _y;
	w = _w;
	setsize();
}

Vector2D::Vector2D(Vector2D& _vec)
{
	x = _vec.x;
	y = _vec.y;
	w = _vec.w;
	rotsize = _vec.rotsize;
	nrotsize = _vec.nrotsize;
}

Vector2D::~Vector2D()
{
}

bool Vector2D::operator==(Vector2D& _val)
{
	if (x == _val.x && y == _val.y && nrotsize == _val.nrotsize)
		return true;
	return false;
}
Vector2D Vector2D::operator*(int _val)
{
	Vector2D temp(x * _val, y * _val);
	return temp;
}
Vector2D Vector2D::operator+(Vector2D& _val)
{
	Vector2D temp(x + _val.x, y + _val.y);
	return temp;
}
Vector2D Vector2D::operator-(Vector2D& _val)
{
	Vector2D temp(x + _val.operator*(-1).x, y + _val.operator*(-1).y);
	return temp;
}

Point Vector2D::getPos() { return Point(x, y); }

void Vector2D::setPos(Point _val)
{
	x = _val.x;
	y = _val.y;
	w = 1;
	setsize();
}

void Vector2D::setPos(float _val01, float _val02)
{
	x = _val01;
	y = _val02;
	w = 1;
	setsize();
}

void Vector2D::setsize()
{
	nrotsize = powf(x, 2) + powf(y, 2);
	rotsize = sqrtf(nrotsize);
}

Vector2D Vector2D::normalizaion()	// 단위벡터 계산
{	//-1,3,4 루트26분의-1...

	Vector2D temp(x / rotsize, y / rotsize);
	return temp;
}

float Vector2D::dot_Product(Vector2D& _vec)	// 내적값 계산
{
	// -7
	float dot = (this->x * _vec.x) + (this->y * _vec.y);
	return dot;
}

float Vector2D::GetCos(Vector2D& _vec)			// 코사인값 계산
{
	float theta = dot_Product(_vec) / (this->rotsize * _vec.rotsize);
	return cos(theta);
}

void Vector2D::Console_Print()
{
	printf("%3.0f %3.0f %3.0f", this->x, this->y, this->w);
}
