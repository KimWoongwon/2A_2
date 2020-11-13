#include "Matrix3x3.h"

Matrix3x3 Matrix3x3::Identity = { 1,0,0,0,1,0,0,0,1 };
Matrix3x3 Matrix3x3::Zero = { 0,0,0,0,0,0,0,0,0 };

Matrix3x3::Matrix3x3()
{
	for (int i = 0; i < Matrix3x3::SIZE; i++)
	{
		for (int j = 0; j < Matrix3x3::SIZE; j++)
		{
			if (i == j)
				matrix[i][j] = 1;
			else
				matrix[i][j] = 0;
		}
	}
}

Matrix3x3::Matrix3x3(float m00, float m01, float m02, float m10, float m11,
	float m12, float m20, float m21, float m22)
{
	_11 = m00;	_12 = m01;	_13 = m02;
	_21 = m10;	_22 = m11;	_23 = m12;
	_31 = m20;	_32 = m21;	_33 = m22;
}

Matrix3x3::Matrix3x3(float* line0, float* line1, float* line2)
{
	for (int i = 0; i < Matrix3x3::SIZE; i++)
	{
		matrix[0][i] = line0[i];
		matrix[1][i] = line1[i];
		matrix[2][i] = line2[i];
	}
}

Matrix3x3::Matrix3x3(Matrix3x3& _m)
{
	for (int i = 0; i < Matrix3x3::SIZE; i++)
		for (int j = 0; j < Matrix3x3::SIZE; j++)
			matrix[i][j] = _m.matrix[i][j];
}

Matrix3x3::~Matrix3x3()
{

}