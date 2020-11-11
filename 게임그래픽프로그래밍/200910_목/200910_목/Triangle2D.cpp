#include "Triangle2D.h"


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
	float Cx = (A.getx() + B.getx() + C.getx()) / Vertex_Count;
	this->Center.setx(Cx);

	float Cy = (A.gety() + B.gety() + C.gety()) / Vertex_Count;
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
	for (int i = 0; i < Vertex_Count; i++)
	{
		vertex[i].setx(vertex[i].getx() + _moveval.x);
		vertex[i].sety(vertex[i].gety() + _moveval.y);
	}
	setCenter();
}

void Triangle2D::Translate(Point& _moveval)
{
	Matrix4x4 Trans_value;
	Trans_value.MatrixTranslate(_moveval.x, _moveval.y, 0);

	for (int i = 0; i < Vertex_Count; i++)
	{
		Vector3D temp(vertex[i].getx(), vertex[i].gety(), 1, 1);
		temp = Trans_value.Vector3D_Multiple(temp);
		vertex[i].setPos(temp.getx(), temp.gety());
	}
	setCenter();
}

void Triangle2D::Rotate_Z(float theta)
{
	Matrix4x4 Trans_01;
	Matrix4x4 Trans_02;
	Matrix4x4 Rot_value;
	// 요기서 Center기준으로
	Trans_01.MatrixTranslate(-Center.getx(), -Center.gety(), 1);
	Trans_02.MatrixTranslate(Center.getx(), Center.gety(), 1);
	Rot_value.MatrixRotate_Z(theta);

	Matrix4x4 result_value;	// 네네
	result_value = Trans_01.Multiple(Rot_value);	// mt1 * mr
	result_value = result_value.Multiple(Trans_02);	// (mt1 * mr) * mt2
	
	for (int i = 0; i < Vertex_Count; i++)
	{
		Vector3D temp(vertex[i].getx(), vertex[i].gety(), 1, 1);
		temp = result_value.Vector3D_Multiple(temp);
		vertex[i].setPos(temp.getx(), temp.gety());
	}
	setCenter();
}

//void Triangle2D::Rotate_X(float theta)
//{
//	Matrix4x4 Trans_01;
//	Matrix4x4 Trans_02;
//	Matrix4x4 Rot_value;
//	Trans_01.MatrixTranslate(-Center.getx(), -Center.gety(), 1);
//	Trans_02.MatrixTranslate(Center.getx(), Center.gety(), 1);
//	Rot_value.MatrixRotate_X(theta);
//
//	Matrix4x4 result_value;
//	result_value = Trans_01.Multiple(Rot_value);
//	result_value = result_value.Multiple(Trans_02);
//
//	for (int i = 0; i < Vertex_Count; i++)
//	{
//		Vector3D temp(vertex[i].getx(), vertex[i].gety(), 1, 1);
//		temp = result_value.Vector3D_Multiple(temp);
//		vertex[i].setPos(temp.getx(), temp.gety());
//	}
//	setCenter();
//}
//
//void Triangle2D::Rotate_Y(float theta)
//{
//	Matrix4x4 Trans_01;
//	Matrix4x4 Trans_02;
//	Matrix4x4 Rot_value;
//	Trans_01.MatrixTranslate(-Center.getx(), -Center.gety(), 1);
//	Trans_02.MatrixTranslate(Center.getx(), Center.gety(), 1);
//	Rot_value.MatrixRotate_Y(theta);
//
//	Matrix4x4 result_value;
//	result_value = Trans_01.Multiple(Rot_value);
//	result_value = result_value.Multiple(Trans_02);
//
//	for (int i = 0; i < Vertex_Count; i++)
//	{
//		Vector3D temp(vertex[i].getx(), vertex[i].gety(), 1, 1);
//		temp = result_value.Vector3D_Multiple(temp);
//		vertex[i].setPos(temp.getx(), temp.gety());
//	}
//	setCenter();
//}

void Triangle2D::Scale(float _sx, float _sy, float _sz)
{
	Matrix4x4 Trans_01;
	Matrix4x4 Trans_02;
	Matrix4x4 scale_value;
	Trans_01.MatrixTranslate(-Center.getx(), -Center.gety(), 1);
	Trans_02.MatrixTranslate(Center.getx(), Center.gety(), 1);
	scale_value.MatrixScaling(_sx, _sy, _sz);

	Matrix4x4 result_value;
	result_value = Trans_01.Multiple(scale_value);
	result_value = result_value.Multiple(Trans_02);

	for (int i = 0; i < Vertex_Count; i++)
	{
		Vector3D temp(vertex[i].getx(), vertex[i].gety(), 1, 1);
		temp = result_value.Vector3D_Multiple(temp);
		vertex[i].setPos(temp.getx(), temp.gety());
	}
	setCenter();
}