#pragma once
#include <stdio.h>
#include "Matrix1x4.h"

class Matrix4x4
{
private:
	union
	{
		struct
		{
			float _11, _12, _13, _14;
			float _21, _22, _23, _24;
			float _31, _32, _33, _34;
			float _41, _42, _43, _44;
		};
		float matrix[4][4];
	};
	int size = 4;
public:
	Matrix4x4();
	Matrix4x4(float, float, float, float, float, float, float, float, float, float, float, float, float, float, float, float);
	Matrix4x4(float*, float*, float*, float*);
	Matrix4x4(Matrix4x4& _m);
	~Matrix4x4();

	void Init();
	void setEntry(int, int, float);
	float getEntry(int, int);

	bool Equal(Matrix4x4& _m);

	Matrix4x4 Plus(Matrix4x4& _m);
	Matrix4x4 Minus(Matrix4x4& _m);
	Matrix4x4 Multiple(Matrix4x4& _m);

	void MatrixRotate_X(float);
	void MatrixRotate_Y(float);
	void MatrixRotate_Z(float);
	void MatrixTranslate(float, float, float);
	void MatrixScaling(float, float, float);

	Matrix1x4 Matrix1x4_Multiple(Matrix1x4&);
	Vector3D Vector3D_Multiple(Vector3D&);

	void Show();
};