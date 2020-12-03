#include "../Header/Triangle2D.h"
//#include "Matrix_Function.h"

Triangle2D::Triangle2D()
{
	for (int i = 0; i < Vertex_Count; i++)
	{
		vertexs[i].vector = Vector2D::Zero;
		vertexs[i].index = i;
	}
		
}
Triangle2D::Triangle2D(Triangle2D& temp)
{
	for (int i = 0; i < Vertex_Count; i++)
	{
		vertexs[i] = temp.vertexs[i];
	}
		
}
Triangle2D::Triangle2D(Point a, Point b, Point c)
{
	_A.vector.setPos(a);	_A.index = 0;
	_B.vector.setPos(b);	_B.index = 1;
	_C.vector.setPos(c);	_C.index = 2;
	setCenter();
}
Triangle2D::Triangle2D(float ax, float ay, float bx, float by, float cx, float cy)
{
	_A.vector.setPos(ax, ay);	_A.index = 0;
	_B.vector.setPos(bx, by);	_B.index = 1;
	_C.vector.setPos(cx, cy);	_C.index = 2;
	setCenter();
}
Triangle2D::~Triangle2D()
{

}
void Triangle2D::setCenter()
{
	// 내심을 Center로 설정하기
	float da = sqrtf(powf(_B.vector.x - _A.vector.x, 2) + powf(_B.vector.y - _A.vector.y, 2));	// Line AB
	float db = sqrtf(powf(_C.vector.x - _B.vector.x, 2) + powf(_C.vector.y - _B.vector.y, 2));	// Line BC
	float dc = sqrtf(powf(_A.vector.x - _C.vector.x, 2) + powf(_A.vector.y - _C.vector.y, 2));	// Line CA

	float tx = ((da * _C.vector.x) + (db * _A.vector.x) + (dc * _B.vector.x)) / (da + db + dc);
	this->Center.x = tx;

	float ty = ((da * _C.vector.y) + (db * _A.vector.y) + (dc * _B.vector.y)) / (da + db + dc);
	this->Center.y = ty;
}
