#pragma once 
#include <stdio.h>
#include <math.h>

struct Point
{
	static Point Zero;

	float x, y;
	Point();
	Point(int, int);
	Point(Point*);
};

struct Vector2D
{
	static Vector2D Zero;			// 0 벡터 정의
	static Vector2D Identity;		// 단위 벡터 정의

	float x, y, w;					// x, y 는 좌표값 w는 변환행렬을 위한 더미값
	float rotsize;					// 벡터의 크기 루트 씌운값
	float nrotsize;					// 벡터의 크기 루트 안씌운 값

	Vector2D();
	Vector2D(float, float);
	Vector2D(float, float, float);
	Vector2D(Vector2D&);
	~Vector2D();

	bool operator==(Vector2D&);		// 벡터 비교
	Vector2D operator*(int);		// 벡터 스칼라곱 연산
	Vector2D operator+(Vector2D&);	// 벡터의 덧셈 연산
	Vector2D operator-(Vector2D&);	// 벡터의 뺄셈 연산

	Point getPos();
	void setPos(Point);
	void setPos(float, float);

	void setsize();

	Vector2D normalizaion();		// 단위벡터 계산
	float dot_Product(Vector2D&);	// 내적값 계산
	float GetCos(Vector2D&);		// 코사인값 계산
	
	void Console_Print();
};


