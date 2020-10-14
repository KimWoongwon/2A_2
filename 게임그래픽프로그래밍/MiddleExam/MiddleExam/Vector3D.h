#pragma once
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
	float x, y, z;
	float size;

public:
	Vector3D();
	Vector3D(float, float, float);
	Vector3D(Vector3D&);
	~Vector3D();

	bool operator==(Vector3D&);
	Vector3D operator*(int);
	Vector3D operator+(Vector3D&);
	Vector3D operator-(Vector3D&);

	float getx();
	float gety();
	float getz();
	Point3D getPos();

	void setsize();
	void setx(float);
	void sety(float);
	void setz(float);

	float getSizePow();		// ���� ũ�� ������
	float getSize();	// ���� ũ�Ⱚ

	Vector3D normalizaion();	// �������� ���
	float dot_Product(Vector3D);	// ������ ���
	Vector3D cross_Product(Vector3D);	// ������ ���

	float GetCos(Vector3D);	// �ڻ��ΰ� ���


};
