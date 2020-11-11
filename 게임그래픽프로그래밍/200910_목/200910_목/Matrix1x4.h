#pragma once
#include "Vector3D.h"

class Matrix1x4;
class Matrix4x1;

class Matrix1x4
{
private:
	union
	{
		struct
		{
			float _11, _12, _13, _14;
		};
		float matrix[1][4];
	};
	int xsize = 4;
	int ysize = 1;
public:
	Matrix1x4();
	Matrix1x4(float, float, float, float);
	Matrix1x4(float*);
	Matrix1x4(Matrix1x4& _m);
	Matrix1x4(Vector3D&);
	~Matrix1x4();

	void setEntry(int, float);
	float getEntry(int);

	bool Equal(Matrix1x4& _m);
	Matrix1x4 Plus(Matrix1x4& _m);
	Matrix1x4 Minus(Matrix1x4& _m);
	Matrix1x4 ScalarMultiple(int _scalar);

	Matrix4x1 Transposed();

	void Show();
};

class Matrix4x1
{
private:
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
public:
	Matrix4x1();
	Matrix4x1(float, float, float, float);
	Matrix4x1(float*);
	Matrix4x1(Matrix4x1& _m);
	Matrix4x1(Vector3D&);
	~Matrix4x1();

	void setEntry(int, float);
	float getEntry(int);

	bool Equal(Matrix4x1& _m);
	Matrix4x1 Plus(Matrix4x1& _m);
	Matrix4x1 Minus(Matrix4x1& _m);
	Matrix4x1 ScalarMultiple(int _scalar);

	Matrix1x4 Transposed();

	void Show();
};
