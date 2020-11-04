#include "Matrix4x4.h"

Matrix4x4::Matrix4x4()
{
	for (int i = 0; i < size; i++)
	{
		for (int j = 0; j < size; j++)
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
	size = 4;
	matrix[0][0] = m00;	matrix[0][1] = m01;	matrix[0][2] = m02;	matrix[0][3] = m03;
	matrix[1][0] = m10;	matrix[1][1] = m11;	matrix[1][2] = m12;	matrix[1][3] = m13;
	matrix[2][0] = m20;	matrix[2][1] = m21;	matrix[2][2] = m22;	matrix[2][3] = m23;
	matrix[3][0] = m30;	matrix[3][1] = m31;	matrix[3][2] = m32;	matrix[3][3] = m33;
}

Matrix4x4::Matrix4x4(float* line0, float* line1, float* line2, float* line3)
{
	size = 4;
	for (int i = 0; i < size; i++)
	{
		matrix[0][i] = line0[i];
		matrix[1][i] = line1[i];
		matrix[2][i] = line2[i];
		matrix[3][i] = line3[i];
	}
}

Matrix4x4::Matrix4x4(Matrix4x4& _m)
{
	size = _m.size;
	for (int i = 0; i < size; i++)
	{
		for (int j = 0; j < size; j++)
		{
			matrix[i][j] = _m.matrix[i][j];
		}
	}
}

Matrix4x4::~Matrix4x4()
{

}

void Matrix4x4::setEntry(int a, int b, float _val)
{
	if (a >= size || a < 0 || b >= size || b < 0)
		return;

	matrix[a][b] = _val;
}

float Matrix4x4::getEntry(int a, int b)
{
	if (a >= size || a < 0 || b >= size || b < 0)
		return 0;

	return matrix[a][b];
}

bool Matrix4x4::Equal(Matrix4x4& _m)
{
	bool flag = true;
	for (int i = 0; i < size; i++)
	{
		for (int j = 0; j < size; j++)
		{
			if (matrix[i][j] != _m.matrix[i][j])
				flag = false;
		}
	}

	return flag;
}

Matrix4x4 Matrix4x4::Plus(Matrix4x4& _m)
{
	Matrix4x4 temp;

	for (int i = 0; i < _m.size; i++)
	{
		for (int j = 0; j < _m.size; j++)
		{
			temp.matrix[i][j] = matrix[i][j] + _m.matrix[i][j];
		}
	}

	return temp;
}

Matrix4x4 Matrix4x4::Minus(Matrix4x4& _m)
{
	Matrix4x4 temp;

	for (int i = 0; i < _m.size; i++)
	{
		for (int j = 0; j < _m.size; j++)
		{
			temp.matrix[i][j] = matrix[i][j] - _m.matrix[i][j];
		}
	}

	return temp;
}

Matrix4x4 Matrix4x4::Multiple(Matrix4x4& _m)
{
	Matrix4x4 temp;

	for (int i = 0; i < size; i++)
	{
		for (int j = 0; j < size; j++)
		{
			temp.matrix[i][j] = 0;
			for (int k = 0; k < size; k++)
			{
				temp.matrix[i][j] += (matrix[i][k] * _m.matrix[k][j]);
			}
		}
	}

	return temp;
}

Vector3D Matrix4x4::Vec_Matrix_Multiple(Vector3D v1)
{
	Vector3D temp;

	temp.setx((v1.getx() * matrix[0][0]) + (v1.gety() * matrix[1][0]) + (v1.getz() * matrix[2][0]) + (v1.getw() * matrix[3][0]));
	temp.sety((v1.getx() * matrix[0][1]) + (v1.gety() * matrix[1][1]) + (v1.getz() * matrix[2][1]) + (v1.getw() * matrix[3][1]));
	temp.setz((v1.getx() * matrix[0][2]) + (v1.gety() * matrix[1][2]) + (v1.getz() * matrix[2][2]) + (v1.getw() * matrix[3][2]));
	temp.setw((v1.getx() * matrix[0][3]) + (v1.gety() * matrix[1][3]) + (v1.getz() * matrix[2][3]) + (v1.getw() * matrix[3][3]));

	return temp;
}

void Matrix4x4::Show()
{
	for (int i = 0; i < size; i++)
	{
		printf("%3.0f %3.0f %3.0f %3.0f\n", matrix[i][0], matrix[i][1], matrix[i][2], matrix[i][3]);
	}
}