#include "Triangle2D.h"

Triangle2D::Triangle2D()
{
	for (int i = 0; i < Triangle_Vertex_Count; i++)
		vertex[i].setZero();
}
Triangle2D::Triangle2D(Triangle2D& temp)
{
	for (int i = 0; i < Triangle_Vertex_Count; i++)
		vertex[i] = temp.vertex[i];
}
Triangle2D::Triangle2D(Point a, Point b, Point c)
{
	A.setPos(a);
	B.setPos(b);
	C.setPos(c);
	setCenter();
}
Triangle2D::Triangle2D(float ax, float ay, float bx, float by, float cx, float cy)
{
	A.setPos(ax, ay);
	B.setPos(bx, by);
	C.setPos(cx, cy);
	setCenter();
}
Triangle2D::~Triangle2D()
{

}

Vector2D Triangle2D::getA()
{
	return A;
}
Vector2D Triangle2D::getB()
{
	return B;
}
Vector2D Triangle2D::getC()
{
	return C;
}
Vector2D Triangle2D::getCenter()
{
	return Center;
}

void Triangle2D::setCenter()
{
	float Cx = (A.getx() + B.getx() + C.getx()) / Triangle_Vertex_Count;
	this->Center.setx(Cx);

	float Cy = (A.gety() + B.gety() + C.gety()) / Triangle_Vertex_Count;
	this->Center.sety(Cy);
}
void Triangle2D::setA(Vector2D _val)
{
	A = _val;
	setCenter();
}
void Triangle2D::setB(Vector2D _val)
{
	B = _val;
	setCenter();
}
void Triangle2D::setC(Vector2D _val)
{
	C = _val;
	setCenter();
}

void Triangle2D::Move(Point& _moveval)
{
	for (int i = 0; i < Triangle_Vertex_Count; i++)
	{
		vertex[i].setx(vertex[i].getx() + _moveval.x);
		vertex[i].sety(vertex[i].gety() + _moveval.y);
	}
	setCenter();
}
void Triangle2D::Translate(Point& _moveval)
{
	Matrix3x3 Trans_value;
	Trans_value.setEntry(2, 0, _moveval.x);
	Trans_value.setEntry(2, 1, _moveval.y);

	for (int i = 0; i < Triangle_Vertex_Count; i++)
	{
		vertex[i] = Trans_value.Vec_Matrix_Multiple(vertex[i]);
	}
	setCenter();
}