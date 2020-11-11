#pragma once
#include <stdio.h>
#include "Matrix1x3.h"

class Matrix3x3
{
private:
	union 
	{
		struct 
		{
			float _11, _12, _13;
			float _21, _22, _23;
			float _31, _32, _33;
		};
		float matrix[3][3];
	};
	int size = 3;
public:
	Matrix3x3();
	Matrix3x3(float, float, float, float, float, float, float, float, float);
	Matrix3x3(float*, float*, float*);
	Matrix3x3(Matrix3x3& _m);
	~Matrix3x3();

	void setMatrix_Delta(float, float);
	void setEntry(int, int, float);
	float getEntry(int, int);

	bool Equal(Matrix3x3& _m);
	Matrix3x3 Plus(Matrix3x3& _m);
	Matrix3x3 Minus(Matrix3x3& _m);
	Matrix3x3 Multiple(Matrix3x3& _m);

	Matrix1x3 Matrix1x3_Multiple(Matrix1x3& v1);
	Vector2D Vector2D_Multiple(Vector2D& v1);

	void Show();
};

