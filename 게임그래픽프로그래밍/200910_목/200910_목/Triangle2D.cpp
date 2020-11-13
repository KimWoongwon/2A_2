#include "Triangle2D.h"
#include "Matrix_Function.h"

Triangle2D::Triangle2D()
{
	for (int i = 0; i < Vertex_Count; i++)
		vertex[i].setZero();
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
	float da = sqrtf(powf(_B.getx() - _A.getx(), 2) + powf(_B.gety() - _A.gety(), 2));	// Line AB
	float db = sqrtf(powf(_C.getx() - _B.getx(), 2) + powf(_C.gety() - _B.gety(), 2));	// Line BC
	float dc = sqrtf(powf(_A.getx() - _C.getx(), 2) + powf(_A.gety() - _C.gety(), 2));	// Line CA

	float tx = ((da * _C.getx()) + (db * _A.getx()) + (dc * _B.getx())) / (da + db + dc);
	this->Center.setx(tx);

	float ty = ((da * _C.gety()) + (db * _A.gety()) + (dc * _B.gety())) / (da + db + dc);
	this->Center.sety(ty);
}
