#pragma once
#include "Vector2D.h"

class Matrix1x3;
class Matrix3x1;

class Matrix1x3
{
private:
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
public:
	Matrix1x3();
	Matrix1x3(float, float, float);
	Matrix1x3(float*);
	Matrix1x3(Matrix1x3& _m);
	Matrix1x3(Vector2D&);
	~Matrix1x3();

	void setEntry(int, float);
	float getEntry(int);

	bool Equal(Matrix1x3& _m);
	Matrix1x3 Plus(Matrix1x3& _m);
	Matrix1x3 Minus(Matrix1x3& _m);
	Matrix1x3 ScalarMultiple(int _scalar);

	Matrix3x1 Transposed();

	void Show();
};

class Matrix3x1
{
private:
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
public:
	Matrix3x1();
	Matrix3x1(float, float, float);
	Matrix3x1(float*);
	Matrix3x1(Matrix3x1& _m);
	Matrix3x1(Vector2D&);
	~Matrix3x1();

	void setEntry(int, float);
	float getEntry(int);

	bool Equal(Matrix3x1& _m);
	Matrix3x1 Plus(Matrix3x1& _m);
	Matrix3x1 Minus(Matrix3x1& _m);
	Matrix3x1 ScalarMultiple(int _scalar);

	Matrix1x3 Transposed();

	void Show();
};
