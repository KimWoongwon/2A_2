#pragma once
#include "Vector2D.h"

struct Matrix1x3;
struct Matrix3x1;

struct Matrix1x3
{
	union
	{
		struct
		{
			float _11, _12, _13;
		};
		float matrix[1][3];
	};
	int xsize = 3;
	int ysize = 1;

	Matrix1x3();
	Matrix1x3(float, float, float);
	Matrix1x3(float*);
	Matrix1x3(Matrix1x3& _m);
	Matrix1x3(Vector2D&);
	~Matrix1x3();
};

struct Matrix3x1
{
public:
	union
	{
		struct
		{
			float _11;
			float _21;
			float _31;
		};

		float matrix[3][1];
	};
	int xsize = 1;
	int ysize = 3;

	Matrix3x1();
	Matrix3x1(float, float, float);
	Matrix3x1(float*);
	Matrix3x1(Matrix3x1& _m);
	Matrix3x1(Vector2D&);
	~Matrix3x1();
};
