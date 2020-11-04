#pragma once
#include <stdio.h>
#include "Vector2D.h"


class Matrix3x3
{
private:
	float matrix[3][3];
	int size = 3;
public:
	Matrix3x3();
	Matrix3x3(float, float, float, float, float, float, float, float, float);
	Matrix3x3(float*, float*, float*);
	Matrix3x3(Matrix3x3& _m);
	~Matrix3x3();

	float getEntry(int, int);

	bool Equal(Matrix3x3& _m);
	Matrix3x3 Plus(Matrix3x3& _m);
	Matrix3x3 Minus(Matrix3x3& _m);
	Matrix3x3 Multiple(Matrix3x3& _m);

	Vector2D Vec_Matrix_Multiple(Vector2D v1);

	void Show();
};

