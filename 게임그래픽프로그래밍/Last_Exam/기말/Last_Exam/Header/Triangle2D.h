#pragma once
#include <math.h>
#include "Vector2D.h"
//#include "Matrix_Function.h"

struct Triangle2D
{
public:
	static const int Vertex_Count = 3;

	union 
	{
		struct
		{
			Vertex2D _A;
			Vertex2D _B;
			Vertex2D _C;
		};
		Vertex2D vertexs[3];
	};
	Vector2D Center;

	Triangle2D();
	Triangle2D(Triangle2D&);
	Triangle2D(Point, Point, Point);
	Triangle2D(float, float, float, float, float, float);
	~Triangle2D();

	void setCenter();
};

