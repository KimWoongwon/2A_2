#include "Matrix3x3.h"

Matrix3x3::Matrix3x3()
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

Matrix3x3::Matrix3x3(float m00, float m01, float m02, float m10, float m11,
	float m12, float m20, float m21, float m22)
{
	size = 3;
	matrix[0][0] = m00;	matrix[0][1] = m01;	matrix[0][2] = m02;
	matrix[1][0] = m10;	matrix[1][1] = m11;	matrix[1][2] = m12;
	matrix[2][0] = m20;	matrix[2][1] = m21;	matrix[2][2] = m22;
}

Matrix3x3::Matrix3x3(float* line0, float* line1, float* line2)
{
	size = 3;
	for (int i = 0; i < size; i++)
	{
		matrix[0][i] = line0[i];
		matrix[1][i] = line1[i];
		matrix[2][i] = line2[i];
	}
}

Matrix3x3::Matrix3x3(Matrix3x3& _m)
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

Matrix3x3::~Matrix3x3()
{

}

void Matrix3x3::setMatrix_Delta(float _dx, float _dy)
{
	_21 = _dx;
	_22 = _dy;
}

void Matrix3x3::setEntry(int a, int b, float _val)
{
	if (a >= size || a < 0 || b >= size || b < 0)
		return;

	matrix[a][b] = _val;
}

float Matrix3x3::getEntry(int a, int b)
{
	if (a >= size || a < 0 || b >= size || b < 0)
		return 0;

	return matrix[a][b];
}

bool Matrix3x3::Equal(Matrix3x3& _m)
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

Matrix3x3 Matrix3x3::Plus(Matrix3x3& _m)
{
	Matrix3x3 temp;

	for (int i = 0; i < _m.size; i++)
	{
		for (int j = 0; j < _m.size; j++)
		{
			temp.matrix[i][j] = matrix[i][j] + _m.matrix[i][j];
		}
	}

	return temp;
}

Matrix3x3 Matrix3x3::Minus(Matrix3x3& _m)
{
	Matrix3x3 temp;

	for (int i = 0; i < _m.size; i++)
	{
		for (int j = 0; j < _m.size; j++)
		{
			temp.matrix[i][j] = matrix[i][j] - _m.matrix[i][j];
		}
	}

	return temp;
}

Matrix3x3 Matrix3x3::Multiple(Matrix3x3& _m)
{
	Matrix3x3 temp;

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

Matrix1x3 Matrix3x3::Matrix1x3_Multiple(Matrix1x3& _m)
{
	Matrix1x3 temp;
	temp.setEntry(0, ((_m.getEntry(0) * matrix[0][0]) + (_m.getEntry(1) * matrix[1][0]) + (_m.getEntry(2) * matrix[2][0])));
	temp.setEntry(1, ((_m.getEntry(0) * matrix[0][1]) + (_m.getEntry(1) * matrix[1][1]) + (_m.getEntry(2) * matrix[2][1])));
	temp.setEntry(2, ((_m.getEntry(0) * matrix[0][2]) + (_m.getEntry(1) * matrix[1][2]) + (_m.getEntry(2) * matrix[2][2])));
	return temp;
}

Vector2D Matrix3x3::Vector2D_Multiple(Vector2D& v1)
{
	Vector2D temp;

	temp.setx((v1.getx() * matrix[0][0]) + (v1.gety() * matrix[1][0]) + (v1.getw() * matrix[2][0]));
	temp.sety((v1.getx() * matrix[0][1]) + (v1.gety() * matrix[1][1]) + (v1.getw() * matrix[2][1]));
	temp.setw((v1.getx() * matrix[0][2]) + (v1.gety() * matrix[1][2]) + (v1.getw() * matrix[2][2]));

	return temp;
}

void Matrix3x3::Show()
{
	for (int i = 0; i < size; i++)
	{
		printf("%3.0f %3.0f %3.0f\n", matrix[i][0], matrix[i][1], matrix[i][2]);
	}
}