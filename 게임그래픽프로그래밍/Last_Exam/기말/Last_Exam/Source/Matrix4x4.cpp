#include "../Header/Matrix4x4.h"

Matrix4x4 Matrix4x4::Identity = { 1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,1 };
Matrix4x4 Matrix4x4::Zero =		{ 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 };

Matrix4x4::Matrix4x4()
{
	for (int i = 0; i < Matrix4x4::SIZE; i++)
	{
		for (int j = 0; j < Matrix4x4::SIZE; j++)
		{
			if (i == j)
				matrix[i][j] = 1;
			else
				matrix[i][j] = 0;
		}
	}
}
Matrix4x4::Matrix4x4(float m00, float m01, float m02, float m03, float m10, float m11,
	float m12, float m13, float m20, float m21, float m22, float m23, float m30, float m31, float m32, float m33)
{
	_11 = m00;	_12 = m01;	_13 = m02;	_14 = m03;
	_21 = m10;	_22 = m11;	_23 = m12;	_24 = m13;
	_31 = m20;	_32 = m21;	_33 = m22;	_34 = m23;
	_41 = m30;	_42 = m31;	_43 = m32;	_44 = m33;
}
Matrix4x4::Matrix4x4(float* line0, float* line1, float* line2, float* line3)
{
	for (int i = 0; i < Matrix4x4::SIZE; i++)
	{
		matrix[0][i] = line0[i];
		matrix[1][i] = line1[i];
		matrix[2][i] = line2[i];
		matrix[3][i] = line3[i];
	}
}
Matrix4x4::Matrix4x4(Matrix4x4& _m)
{
	for (int i = 0; i < Matrix4x4::SIZE; i++)
		for (int j = 0; j < Matrix4x4::SIZE; j++)
			matrix[i][j] = _m.matrix[i][j];
}
Matrix4x4::~Matrix4x4()
{

}



