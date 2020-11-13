#include "Poligon_Function.h"

void Poligon_Function::Move(Triangle2D& _tri, const Point& _moveval)
{
	for (int i = 0; i < Triangle2D::Vertex_Count; i++)
	{
		_tri.vertex[i].setx(_tri.vertex[i].getx() + _moveval.x);
		_tri.vertex[i].sety(_tri.vertex[i].gety() + _moveval.y);
	}
	_tri.setCenter();
}

void Poligon_Function::Translate(Triangle2D& _tri, const Point& _moveval)
{
	Matrix4x4 Trans_value = Matrix4x4::Identity;
	Matrix_Function::Translate(Trans_value, _moveval.x, _moveval.y, 0);

	for (int i = 0; i < Triangle2D::Vertex_Count; i++)
	{
		Vector3D temp(_tri.vertex[i].getx(), _tri.vertex[i].gety(), 1, 1);
		Matrix_Function::Vector3D_Multiple(temp, Trans_value);
		_tri.vertex[i].setPos(temp.getx(), temp.gety());
	}
	_tri.setCenter();
}

void Poligon_Function::Rotate(Triangle2D& _tri, const float _angle)
{
	Matrix4x4 trans_matrix, rotate_matrix, result_matrix;

	Matrix_Function::Translate(trans_matrix, -_tri.Center.getx(), -_tri.Center.gety(), 1);
	Matrix_Function::Multiple(result_matrix, trans_matrix);

	Matrix_Function::Rotate_Z(rotate_matrix, _angle);
	Matrix_Function::Multiple(result_matrix, rotate_matrix);

	Matrix_Function::Translate(trans_matrix, _tri.Center.getx(), _tri.Center.gety(), 1);
	Matrix_Function::Multiple(result_matrix, trans_matrix);

	for (int i = 0; i < Triangle2D::Vertex_Count; i++)
	{
		Vector3D temp(_tri.vertex[i].getx(), _tri.vertex[i].gety(), 1, 1);
		Matrix_Function::Vector3D_Multiple(temp, result_matrix);
		_tri.vertex[i].setPos(temp.getx(), temp.gety());
	}
	_tri.setCenter();
}

void Poligon_Function::Scale(Triangle2D& _tri, const float _sx, const float _sy, const float _sz)
{
	Matrix4x4 trans_matrix, scale_matrix, result_matrix;

	Matrix_Function::Translate(trans_matrix, -_tri.Center.getx(), -_tri.Center.gety(), 1);
	Matrix_Function::Multiple(result_matrix, trans_matrix);

	Matrix_Function::Scaling(scale_matrix, _sx, _sy, _sz);
	Matrix_Function::Multiple(result_matrix, scale_matrix);

	Matrix_Function::Translate(trans_matrix, _tri.Center.getx(), _tri.Center.gety(), 1);
	Matrix_Function::Multiple(result_matrix, trans_matrix);

	for (int i = 0; i < Triangle2D::Vertex_Count; i++)
	{
		Vector3D temp(_tri.vertex[i].getx(), _tri.vertex[i].gety(), 1, 1);
		Matrix_Function::Vector3D_Multiple(temp, result_matrix);
		_tri.vertex[i].setPos(temp.getx(), temp.gety());
	}
	_tri.setCenter();
}