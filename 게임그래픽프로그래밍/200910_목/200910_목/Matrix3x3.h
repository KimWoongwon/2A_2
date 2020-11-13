#pragma once
#include <stdio.h>
#include "Matrix1x3.h"

struct Matrix3x3
{
	static Matrix3x3 Identity;
	static Matrix3x3 Zero;
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
	static const int SIZE = 3;

	Matrix3x3();
	Matrix3x3(float, float, float, float, float, float, float, float, float);
	Matrix3x3(float*, float*, float*);
	Matrix3x3(Matrix3x3& _m);
	~Matrix3x3();

	
};



