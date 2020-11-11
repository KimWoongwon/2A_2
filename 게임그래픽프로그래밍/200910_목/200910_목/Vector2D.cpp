
#include "Vector2D.h"

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

Vector2D::Vector2D()
{
	x = 0;
	y = 0;
	w = 1;
	size = 0;
}
Vector2D::Vector2D(float _x, float _y)
{
	x = _x;
	y = _y;
	w = 1;
	size = powf(x, 2) + powf(y, 2);
}

Vector2D::Vector2D(float _x, float _y, float _w)
{
	x = _x;
	y = _y;
	w = _w;
	size = powf(x, 2) + powf(y, 2);
}

Vector2D::Vector2D(Vector2D& _vec)
{
	x = _vec.x;
	y = _vec.y;
	w = _vec.w;
	size = _vec.size;
}

Vector2D::~Vector2D()
{
}

bool Vector2D::operator==(Vector2D& _val)
{
	if (x == _val.x && y == _val.y && size == _val.size)
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

float Vector2D::getx() { return x; }
float Vector2D::gety() { return y; }
float Vector2D::getw() { return w; }
Point Vector2D::getPos() { return Point(x, y); }

void Vector2D::setsize()
{
	size = powf(x, 2) + powf(y, 2);
}
void Vector2D::setZero()
{
	x = 0;
	y = 0;
	w = 1;
	size = 0;
}

void Vector2D::setx(float _val)
{
	x = _val;
	setsize();
}
void Vector2D::sety(float _val)
{
	y = _val;
	setsize();
}
void Vector2D::setw(float _val)
{
	w = _val;
}
void Vector2D::setPos(Point _val)
{
	x = _val.x;
	y = _val.y;
	w = 1;
	setsize();
}
void Vector2D::setPos(float _x, float _y)
{
	x = _x;
	y = _y;
	w = 1;
	setsize();
}

float Vector2D::getSizePow() { return size; }		// 벡터 크기 제곱값
float Vector2D::getSize() { return sqrtf(size); }	// 벡터 크기값

Vector2D Vector2D::normalizaion()	// 단위벡터 계산
{	//-1,3,4 루트26분의-1...

	Vector2D temp(x / getSize(), y / getSize());
	return temp;
}

float Vector2D::dot_Product(Vector2D& _vec)	// 내적값 계산
{
	// -7
	float dot = (this->x * _vec.getx()) + (this->y * _vec.gety());
	return dot;
}

float Vector2D::GetCos(Vector2D& _vec)			// 코사인값 계산
{
	float theta = dot_Product(_vec) / (this->getSize() * _vec.getSize());
	return cos(theta);
}

void Vector2D::Show()
{
	printf("%3.0f %3.0f %3.0f", this->x, this->y, this->w);
}


//Vector2D Vector2D::Zero()
//{
//	Vector2D temp(0,0,0);
//	return temp;
//}