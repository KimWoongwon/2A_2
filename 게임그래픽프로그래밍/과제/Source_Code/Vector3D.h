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

struct Vector3D
{
	static Vector3D Zero;
	static Vector3D Identity;

	float x, y, z, w;
	float rotsize;
	float nrotsize;

	Vector3D();
	Vector3D(float, float, float);
	Vector3D(float, float, float, float);
	Vector3D(Vector3D&);
	~Vector3D();

	bool operator==(Vector3D&);
	Vector3D operator*(int);
	Vector3D operator+(Vector3D&);
	Vector3D operator-(Vector3D&);

	Point3D getPos();
	void setPos(Point3D);
	void setPos(float, float, float);

	void setsize();

	Vector3D normalizaion();	// �������� ���
	float dot_Product(Vector3D&);	// ������ ���
	Vector3D cross_Product(Vector3D&);	// ������ ���
	float GetCos(Vector3D&);	// �ڻ��ΰ� ���

	void Console_Print();
};
