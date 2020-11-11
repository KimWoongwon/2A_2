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
	void setZero();
	//static Vector2D Zero();

	void setx(float);
	void sety(float);
	void setw(float);
	void setPos(Point);
	void setPos(float, float);


	float getSizePow();		// 벡터 크기 제곱값
	float getSize();	// 벡터 크기값

	Vector2D normalizaion();		// 단위벡터 계산
	float dot_Product(Vector2D&);	// 내적값 계산
	float GetCos(Vector2D&);			// 코사인값 계산
	
	void Show();
};

