#include "Matrix1x4.h"

Matrix1x4::Matrix1x4()
{
	_11 = 0;
	_12 = 0;
	_13 = 0;
	_14 = 0;
}

Matrix1x4::Matrix1x4(float m11, float m12, float m13, float m14)
{
	_11 = m11;
	_12 = m12;
	_13 = m13;
	_14 = m14;
}

Matrix1x4::Matrix1x4(float* line0)
{
	for (int i = 0; i < xsize; i++)
		matrix[0][i] = line0[i];
}

Matrix1x4::Matrix1x4(Matrix1x4& _m)
{
	xsize = _m.xsize;
	ysize = _m.ysize;

	_11 = _m._11;
	_12 = _m._12;
	_13 = _m._13;
	_14 = _m._14;
}
Matrix1x4::Matrix1x4(Vector3D& _v)
{
	_11 = _v.getx();
	_12 = _v.gety();
	_13 = _v.getz();
	_14 = _v.getw();
}

Matrix1x4::~Matrix1x4()
{

}

void Matrix1x4::setEntry(int idx, float _val)
{
	matrix[0][idx] = _val;
}
float Matrix1x4::getEntry(int idx)
{
	return matrix[0][idx];
}

bool Matrix1x4::Equal(Matrix1x4& _m)
{
	return _11 == _m._11 && _12 == _m._12 && _13 == _m._13 && _14 == _m._14;
}

Matrix1x4 Matrix1x4::Plus(Matrix1x4& _m)
{
	Matrix1x4 temp;

	temp._11 = _11 + _m._11;
	temp._12 = _12 + _m._12;
	temp._13 = _13 + _m._13;
	temp._14 = _14 + _m._14;

	return temp;
}

Matrix1x4 Matrix1x4::Minus(Matrix1x4& _m)
{
	Matrix1x4 temp;

	temp._11 = _11 - _m._11;
	temp._12 = _12 - _m._12;
	temp._13 = _13 - _m._13;
	temp._14 = _14 - _m._14;

	return temp;
}

Matrix1x4 Matrix1x4::ScalarMultiple(int _scalar)
{
	Matrix1x4 temp;

	temp._11 = this->_11 * _scalar;
	temp._12 = this->_12 * _scalar;
	temp._13 = this->_13 * _scalar;
	temp._14 = this->_14 + _scalar;

	return temp;
}

Matrix4x1 Matrix1x4::Transposed()
{
	Matrix4x1 temp(_11, _12, _13, _14);

	return temp;
}

void Matrix1x4::Show()
{
	printf("%3.0f %3.0f %3.0f %3.0f\n", _11, _12, _13, _14);
}

//--------------------------------------------------------------------------

Matrix4x1::Matrix4x1()
{
	_11 = 0;
	_21 = 0;
	_31 = 0;
	_41 = 0;
}

Matrix4x1::Matrix4x1(float m11, float m21, float m31, float m41)
{
	_11 = m11;
	_21 = m21;
	_31 = m31;
	_41 = m41;
}

Matrix4x1::Matrix4x1(float* line0)
{
	for (int i = 0; i < xsize; i++)
		matrix[0][i] = line0[i];
}

Matrix4x1::Matrix4x1(Matrix4x1& _m)
{
	xsize = _m.xsize;
	ysize = _m.ysize;

	_11 = _m._11;
	_21 = _m._21;
	_31 = _m._31;
	_41 = _m._41;
}

Matrix4x1::Matrix4x1(Vector3D& _v)
{
	_11 = _v.getx();
	_21 = _v.gety();
	_31 = _v.getz();
	_41 = _v.getw();
}

Matrix4x1::~Matrix4x1()
{

}

void Matrix4x1::setEntry(int idx, float _val)
{
	matrix[idx][0] = _val;
}
float Matrix4x1::getEntry(int idx)
{
	return matrix[idx][0];
}

bool Matrix4x1::Equal(Matrix4x1& _m)
{
	return _11 == _m._11 && _21 == _m._21 && _31 == _m._31 && _41 == _m._41;
}

Matrix4x1 Matrix4x1::Plus(Matrix4x1& _m)
{
	Matrix4x1 temp;

	temp._11 = _11 + _m._11;
	temp._21 = _21 + _m._21;
	temp._31 = _31 + _m._31;
	temp._41 = _41 + _m._41;

	return temp;
}

Matrix4x1 Matrix4x1::Minus(Matrix4x1& _m)
{
	Matrix4x1 temp;

	temp._11 = _11 - _m._11;
	temp._21 = _21 - _m._21;
	temp._31 = _31 - _m._31;
	temp._41 = _41 - _m._41;

	return temp;
}

Matrix4x1 Matrix4x1::ScalarMultiple(int _scalar)
{
	Matrix4x1 temp;

	temp._11 = _11 * _scalar;
	temp._21 = _21 * _scalar;
	temp._31 = _31 * _scalar;
	temp._41 = _41 + _scalar;

	return temp;
}

Matrix1x4 Matrix4x1::Transposed()
{
	Matrix1x4 temp(_11, _21, _31, _41);

	return temp;
}

void Matrix4x1::Show()
{
	printf("%3.0f %3.0f %3.0f %3.0f\n", _11, _21, _31, _41);
}
