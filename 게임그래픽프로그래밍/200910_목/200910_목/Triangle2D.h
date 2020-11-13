#pragma once
#include <math.h>
#include "Matrix_Function.h"

struct Triangle2D
{
public:
	static const int Vertex_Count = 3;

	union 
	{
		struct
		{
			Vector2D _A;
			Vector2D _B;
			Vector2D _C;
		};
		Vector2D vertex[3];
	};
	Vector2D Center;

	Triangle2D();
	Triangle2D(Triangle2D&);
	Triangle2D(Point, Point, Point);
	Triangle2D(float, float, float, float, float, float);
	~Triangle2D();

	void setCenter();

	
};

