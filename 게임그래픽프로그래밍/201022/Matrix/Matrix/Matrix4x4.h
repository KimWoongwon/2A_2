#pragma once
#include <stdio.h>
#include "Vector3D.h"

class Matrix4x4
{
private:
	float matrix[4][4];
	int size = 4;
public:
	Matrix4x4();
	Matrix4x4(float, float, float, float, float, float, float, float, float, float, float, float, float, float, float, float);
	Matrix4x4(float*, float*, float*, float*);
	Matrix4x4(Matrix4x4& _m);
	~Matrix4x4();

	float getEntry(int, int);

	bool Equal(Matrix4x4& _m);

	Matrix4x4 Plus(Matrix4x4& _m);
	Matrix4x4 Minus(Matrix4x4& _m);
	Matrix4x4 Multiple(Matrix4x4& _m);

	Vector3D Vec_Matrix_Multiple(Vector3D v1);

	void Show();
};