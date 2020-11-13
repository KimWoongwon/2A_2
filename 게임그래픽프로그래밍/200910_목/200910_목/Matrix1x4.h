#pragma once
#include "Vector3D.h"

struct Matrix1x4;
struct Matrix4x1;

struct Matrix1x4
{
public:
	union
	{
		struct
		{
			float _11, _12, _13, _14;
		};
		float matrix[1][4];
	};
	static const int XSIZE = 4;
	static const int YSIZE = 1;

	Matrix1x4();
	Matrix1x4(float, float, float, float);
	Matrix1x4(float*);
	Matrix1x4(Matrix1x4& _m);
	Matrix1x4(Vector3D&);
	~Matrix1x4();
};

struct Matrix4x1
{
public:
	union
	{
		struct
		{
			float _11;
			float _21;
			float _31;
			float _41;
		};

		float matrix[4][1];
	};
	int xsize = 1;
	int ysize = 4;

	Matrix4x1();
	Matrix4x1(float, float, float, float);
	Matrix4x1(float*);
	Matrix4x1(Matrix4x1& _m);
	Matrix4x1(Vector3D&);
	~Matrix4x1();
};
