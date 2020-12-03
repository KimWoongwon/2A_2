#pragma once
#include <stdio.h>
#include "Matrix1x4.h"

struct Matrix4x4
{
	static Matrix4x4 Identity;
	static Matrix4x4 Zero;

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
	static const int SIZE = 4;

	Matrix4x4();
	Matrix4x4(	float, float, float, float,
				float, float, float, float,
				float, float, float, float,
				float, float, float, float	);

	Matrix4x4(float*, float*, float*, float*);
	Matrix4x4(Matrix4x4& _m);
	~Matrix4x4();
};

