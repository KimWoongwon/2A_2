#pragma once
#include "Matrix3x3.h"

#define Triangle_Vertex_Count 3

class Triangle2D
{
private:
	union 
	{
		struct
		{
			Vector2D A;
			Vector2D B;
			Vector2D C;
		};
		Vector2D vertex[3];
	};
	Vector2D Center;

public:
	Triangle2D();
	Triangle2D(Triangle2D&);
	Triangle2D(Point, Point, Point);
	Triangle2D(float, float, float, float, float, float);
	~Triangle2D();

	Vector2D getA();
	Vector2D getB();
	Vector2D getC();
	Vector2D getCenter();

	void setCenter();
	void setA(Vector2D);
	void setB(Vector2D);
	void setC(Vector2D);

	//void Copy(Triangle2D&);
	void Move(Point&);
	void Translate(Point&);
};