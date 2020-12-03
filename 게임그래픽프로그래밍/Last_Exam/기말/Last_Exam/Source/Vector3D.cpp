#include "../Header/Vector3D.h"

#pragma region ___Point3D___
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
#pragma endregion

#pragma region ___Vector3D___
Vector3D Vector3D::Zero = { 0,0,0 };
Vector3D Vector3D::Identity = { 1,1,1 };

Vector3D::Vector3D()
{
	x = 0;
	y = 0;
	z = 0;
	w = 1;
	setsize();
}

Vector3D::Vector3D(float _x, float _y, float _z)
{
	x = _x;
	y = _y;
	z = _z;
	w = 1;
	setsize();
}

Vector3D::Vector3D(float _x, float _y, float _z, float _w)
{
	x = _x;
	y = _y;
	z = _z;
	w = _w;
	setsize();
}

Vector3D::Vector3D(Vector3D& _vec)
{
	x = _vec.x;
	y = _vec.y;
	z = _vec.z;
	w = _vec.w;
	rotsize = _vec.rotsize;
	nrotsize = _vec.nrotsize;
}

Vector3D::~Vector3D()	{}

bool Vector3D::operator==(Vector3D& _vec)
{
	if (x == _vec.x && y == _vec.y && z == _vec.z && nrotsize == _vec.nrotsize)
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
	Vector3D temp(x - _vec.x, y - _vec.y, z - _vec.z);
	return temp;
}

Point3D Vector3D::getPos() { return Point3D(x, y, z); }

void Vector3D::setPos(Point3D _val)
{
	x = _val.x;
	y = _val.y;
	z = _val.z;
	w = 1;
	setsize();
}
void Vector3D::setPos(float _val01, float _val02, float _val03)
{
	x = _val01;
	y = _val02;
	z = _val03;
	w = 1;
	setsize();
}

void Vector3D::setsize()
{
	nrotsize = powf(x, 2) + powf(y, 2) + powf(z, 2);
	rotsize = sqrtf(nrotsize);
}

Vector3D Vector3D::normalizaion()				// 단위벡터 계산
{
	Vector3D temp(x / rotsize, y / rotsize, z / rotsize);
	return temp;
}

float Vector3D::dot_Product(Vector3D& _vec)	// 내적값 계산
{
	float dot = (this->x * _vec.x) + (this->y * _vec.y) + (this->z * _vec.z);
	return dot;
}

Vector3D Vector3D::cross_Product(Vector3D& _vec)	// 외적값 계산
{
	Vector3D temp((y * _vec.z) - (z * _vec.y),
		(z * _vec.x) - (x * _vec.z),
		(x * _vec.y) - (y * _vec.x));
	return temp;
}

float Vector3D::GetCos(Vector3D& _vec)		// cos(theta)값 계산
{
	float costheta = dot_Product(_vec) / (this->rotsize * _vec.rotsize);
	return costheta;
}

void Vector3D::Console_Print()
{
	printf("%3.0f %3.0f %3.0f %3.0f", this->x, this->y, this->z, this->w);
}
#pragma endregion



// p = v.n x n

