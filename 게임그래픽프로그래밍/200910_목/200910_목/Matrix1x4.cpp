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
	for (int i = 0; i < Matrix1x4::XSIZE; i++)
		matrix[0][i] = line0[i];
}

Matrix1x4::Matrix1x4(Matrix1x4& _m)
{
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
