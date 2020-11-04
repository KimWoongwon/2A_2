#include "Vector3D.h"

Point3D::Point3D() 
{
	x = 0;
	y = 0;
	z = 0;
}

Point3D::Point3D(int x, int y, int z)
{
	this->x = x;
	this->y = y;
	this->z = z;
}
Point3D::Point3D(Point3D* P)
{
	x = P->x;
	y = P->y;
	z = P->z;
}

Vector3D::Vector3D()
{
	x = 0;
	y = 0;
	z = 0;
	w = 1;
	size = 0;
}

Vector3D::Vector3D(float _x, float _y, float _z)
{
	x = _x;
	y = _y;
	z = _z;
	w = 1;
	size = powf(x, 2) + powf(y, 2) + powf(z, 2);
}

Vector3D::Vector3D(float _x, float _y, float _z, float _w)
{
	x = _x;
	y = _y;
	z = _z;
	w = _w;
	size = powf(x, 2) + powf(y, 2) + powf(z, 2);
}

Vector3D::Vector3D(Vector3D& _vec)
{
	x = _vec.x;
	y = _vec.y;
	z = _vec.z;
	w = _vec.w;
	size = _vec.size;
}
Vector3D::~Vector3D()
{

}

bool Vector3D::operator==(Vector3D& _vec)
{
	if (x == _vec.x && y == _vec.y && z == _vec.z && size == _vec.size)
		return true;
	return false;
}

Vector3D Vector3D::operator*(int _val)
{
	Vector3D temp(x * _val, y * _val, z * _val);
	return temp;
}

Vector3D Vector3D::operator+(Vector3D& _vec)
{
	Vector3D temp(x + _vec.x, y + _vec.y, z + _vec.z);
	return temp;
}

Vector3D Vector3D::operator-(Vector3D& _vec)
{
	Vector3D temp(x + _vec.operator*(-1).x, y + _vec.operator*(-1).y, z + _vec.operator*(-1).z);
	return temp;
}

float Vector3D::getx() { return x; }
float Vector3D::gety() { return y; }
float Vector3D::getz() { return z; }
float Vector3D::getw() { return w; }
Point3D Vector3D::getPos() { return Point3D(x, y, z); }

void Vector3D::setsize()
{
	size = powf(x, 2) + powf(y, 2) + powf(z, 2);
}

void Vector3D::setx(float _val)
{
	x = _val;
	setsize();
}
void Vector3D::sety(float _val)
{
	y = _val;
	setsize();
}
void Vector3D::setz(float _val)
{
	z = _val;
	setsize();
}
void Vector3D::setw(float _val)
{
	w = _val;
}

float Vector3D::getSizePow() { return size; }		// 벡터 크기 제곱값
float Vector3D::getSize() { return sqrtf(size); }	// 벡터 크기값

Vector3D Vector3D::normalizaion()				// 단위벡터 계산
{
	Vector3D temp(x / getSize(), y / getSize(), z / getSize());
	return temp;
}

float Vector3D::dot_Product(Vector3D& _vec)	// 내적값 계산
{
	float dot = (this->x * _vec.getx()) + (this->y * _vec.gety()) + (this->z * _vec.getz());
	return dot;
}

Vector3D Vector3D::cross_Product(Vector3D& _vec)
{
	Vector3D temp((y * _vec.getz()) - (z * _vec.gety()),
		(z * _vec.getx()) - (x * _vec.getz()),
		(x * _vec.gety()) - (y * _vec.getx()));
	return temp;
}

float Vector3D::GetCos(Vector3D& _vec)		// cos(theta)값 계산
{
	float costheta = dot_Product(_vec) / (this->getSize() * _vec.getSize());
	return costheta;
}

void Vector3D::Show()
{
	printf("%3.0f %3.0f %3.0f %3.0f", this->x, this->y, this->z, this->w);
}
// p = v.n x n