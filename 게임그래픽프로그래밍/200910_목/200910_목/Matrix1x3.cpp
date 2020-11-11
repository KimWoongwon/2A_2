#include "Matrix1x3.h"

Matrix1x3::Matrix1x3()
{
	_11 = 0;
	_12 = 0;
	_13 = 0;
}

Matrix1x3::Matrix1x3(float m11, float m12, float m13)
{
	_11 = m11;
	_12 = m12;
	_13 = m13;
}

Matrix1x3::Matrix1x3(float* line0)
{
	for (int i = 0; i < xsize; i++)
		matrix[0][i] = line0[i];
}

Matrix1x3::Matrix1x3(Matrix1x3& _m)
{
	xsize = _m.xsize;
	ysize = _m.ysize;

	_11 = _m._11;
	_12 = _m._12;
	_13 = _m._13;
}
Matrix1x3::Matrix1x3(Vector2D& _v)
{
	_11 = _v.getx();
	_12 = _v.gety();
	_13 = _v.getw();
}

Matrix1x3::~Matrix1x3()
{

}

void Matrix1x3::setEntry(int idx, float _val)
{
	matrix[0][idx] = _val;
}
float Matrix1x3::getEntry(int idx)
{
	return matrix[0][idx];
}

bool Matrix1x3::Equal(Matrix1x3& _m)
{
	return _11 == _m._11 && _12 == _m._12 && _13 == _m._13;
}

Matrix1x3 Matrix1x3::Plus(Matrix1x3& _m)
{
	Matrix1x3 temp;

	temp._11 = _11 + _m._11;
	temp._12 = _12 + _m._12;
	temp._13 = _13 + _m._13;

	return temp;
}

Matrix1x3 Matrix1x3::Minus(Matrix1x3& _m)
{
	Matrix1x3 temp;

	temp._11 = _11 - _m._11;
	temp._12 = _12 - _m._12;
	temp._13 = _13 - _m._13;

	return temp;
}

Matrix1x3 Matrix1x3::ScalarMultiple(int _scalar)
{
	Matrix1x3 temp;

	temp._11 = this->_11 * _scalar;
	temp._12 = this->_12 * _scalar;
	temp._13 = this->_13 * _scalar;

	return temp;
}

Matrix3x1 Matrix1x3::Transposed()
{
	Matrix3x1 temp(_11, _12, _13);

	return temp;
}

void Matrix1x3::Show()
{
	printf("%3.0f %3.0f %3.0f\n", _11, _12, _13);
}

//--------------------------------------------------------------------------

Matrix3x1::Matrix3x1()
{
	_11 = 0;
	_21 = 0;
	_31 = 0;
}

Matrix3x1::Matrix3x1(float m11, float m21, float m31)
{
	_11 = m11;
	_21 = m21;
	_31 = m31;
}

Matrix3x1::Matrix3x1(float* line0)
{
	for (int i = 0; i < xsize; i++)
		matrix[0][i] = line0[i];
}

Matrix3x1::Matrix3x1(Matrix3x1& _m)
{
	xsize = _m.xsize;
	ysize = _m.ysize;

	_11 = _m._11;
	_21 = _m._21;
	_31 = _m._31;
}

Matrix3x1::Matrix3x1(Vector2D& _v)
{
	_11 = _v.getx();
	_21 = _v.gety();
	_31 = _v.getw();
}

Matrix3x1::~Matrix3x1()
{

}

void Matrix3x1::setEntry(int idx, float _val)
{
	matrix[idx][0] = _val;
}
float Matrix3x1::getEntry(int idx)
{
	return matrix[idx][0];
}

bool Matrix3x1::Equal(Matrix3x1& _m)
{
	return _11 == _m._11 && _21 == _m._21 && _31 == _m._31;
}

Matrix3x1 Matrix3x1::Plus(Matrix3x1& _m)
{
	Matrix3x1 temp;

	temp._11 = _11 + _m._11;
	temp._21 = _21 + _m._21;
	temp._31 = _31 + _m._31;

	return temp;
}

Matrix3x1 Matrix3x1::Minus(Matrix3x1& _m)
{
	Matrix3x1 temp;

	temp._11 = _11 - _m._11;
	temp._21 = _21 - _m._21;
	temp._31 = _31 - _m._31;
	return temp;
}

Matrix3x1 Matrix3x1::ScalarMultiple(int _scalar)
{
	Matrix3x1 temp;

	temp._11 = _11 * _scalar;
	temp._21 = _21 * _scalar;
	temp._31 = _31 * _scalar;

	return temp;
}

Matrix1x3 Matrix3x1::Transposed()
{
	Matrix1x3 temp(_11, _21, _31);

	return temp;
}

void Matrix3x1::Show()
{
	printf("%3.0f %3.0f %3.0f\n", _11, _21, _31);
}
