#pragma once 
#include <stdio.h>
#include <math.h>

struct Point
{
	float x, y;
	Point();
	Point(int, int);
	Point(Point*);
};

class Vector2D
{
	float x, y, w;
	float size;

public:
	Vector2D();
	Vector2D(float, float);
	Vector2D(float, float, float);
	Vector2D(Vector2D&);
	~Vector2D();

	bool operator==(Vector2D&);
	Vector2D operator*(int);
	Vector2D operator+(Vector2D&);
	Vector2D operator-(Vector2D&);

	float getx();
	float gety();
	float getw();
	Point getPos();

	void setsize();
	void setx(float);
	void sety(float);
	void setw(float);

	float getSizePow();		// ���� ũ�� ������
	float getSize();	// ���� ũ�Ⱚ

	Vector2D normalizaion();		// �������� ���
	float dot_Product(Vector2D);	// ������ ���
	float GetCos(Vector2D);			// �ڻ��ΰ� ���
	
	void Show();
};

