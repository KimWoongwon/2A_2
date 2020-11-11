#pragma once
#include <stdio.h>
#include <math.h>

struct Point3D
{
	float x, y, z;
	Point3D();
	Point3D(int, int, int);
	Point3D(Point3D*);
};

class Vector3D
{
	float x, y, z, w;
	float size;

public:
	Vector3D();
	Vector3D(float, float, float);
	Vector3D(float, float, float, float);
	Vector3D(Vector3D&);
	~Vector3D();

	bool operator==(Vector3D&);
	Vector3D operator*(int);
	Vector3D operator+(Vector3D&);
	Vector3D operator-(Vector3D&);

	float getx();
	float gety();
	float getz();
	float getw();
	Point3D getPos();

	void setsize();
	void setZero();
	static Vector3D Zero();

	void setx(float);
	void sety(float);
	void setz(float);
	void setw(float);

	float getSizePow();		// 벡터 크기 제곱값
	float getSize();	// 벡터 크기값

	Vector3D normalizaion();	// 단위벡터 계산
	float dot_Product(Vector3D&);	// 내적값 계산
	Vector3D cross_Product(Vector3D&);	// 외적값 계산

	float GetCos(Vector3D&);	// 코사인값 계산

	void Show();
};
