#include "Triangle2D.h"
#include "Matrix_Function.h"

Triangle2D::Triangle2D()
{
	for (int i = 0; i < Vertex_Count; i++)
		vertex[i] = Vector2D::Zero;
}
Triangle2D::Triangle2D(Triangle2D& temp)
{
	for (int i = 0; i < Vertex_Count; i++)
		vertex[i] = temp.vertex[i];
}
Triangle2D::Triangle2D(Point a, Point b, Point c)
{
	_A.setPos(a);
	_B.setPos(b);
	_C.setPos(c);
	setCenter();
}
Triangle2D::Triangle2D(float ax, float ay, float bx, float by, float cx, float cy)
{
	_A.setPos(ax, ay);
	_B.setPos(bx, by);
	_C.setPos(cx, cy);
	setCenter();
}
Triangle2D::~Triangle2D()
{

}
void Triangle2D::setCenter()
{
	float da = sqrtf(powf(_B.x - _A.x, 2) + powf(_B.y - _A.y, 2));	// Line AB
	float db = sqrtf(powf(_C.x - _B.x, 2) + powf(_C.y - _B.y, 2));	// Line BC
	float dc = sqrtf(powf(_A.x - _C.x, 2) + powf(_A.y - _C.y, 2));	// Line CA

	float tx = ((da * _C.x) + (db * _A.x) + (dc * _B.x)) / (da + db + dc);
	this->Center.x = tx;

	float ty = ((da * _C.y) + (db * _A.y) + (dc * _B.y)) / (da + db + dc);
	this->Center.y = ty;
}
