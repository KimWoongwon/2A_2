#pragma once
#include "Triangle2D.h"
#include "Matrix_Function.h"

namespace Polygon_Function
{
	void Move(Triangle2D&, const Point&);
	void Translate(Triangle2D&, const Point&);
	void Rotate(Triangle2D&, const float);
	void Scale(Triangle2D&, const float, const float, const float);
}

