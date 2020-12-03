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
	_11 = _v.x;
	_12 = _v.y;
	_13 = _v.w;
}

Matrix1x3::~Matrix1x3()
{

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
	_11 = _v.x;
	_21 = _v.y;
	_31 = _v.w;
}

Matrix3x1::~Matrix3x1()
{

}

