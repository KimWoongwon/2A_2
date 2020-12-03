#pragma once
#include "Vector2D.h"
#include "Vector3D.h"

#include "Matrix1x4.h"
#include "Matrix4x4.h"
namespace Matrix_Function
{
	//=======================================================================================
	
	bool Equal(const Matrix1x4&, const Matrix1x4&);
	bool Equal(const Matrix4x1&, const Matrix4x1&);
	bool Equal(const Matrix4x4&, const Matrix4x4&);

	void Plus(Matrix1x4&, const Matrix1x4&);
	void Plus(Matrix4x1&, const Matrix4x1&);
	void Plus(Matrix4x4&, const Matrix4x4&);

	void Minus(Matrix1x4&, const Matrix1x4&);
	void Minus(Matrix4x1&, const Matrix4x1&);
	void Minus(Matrix4x4&, const Matrix4x4&);

	void Show(const Matrix1x4&);
	void Show(const Matrix4x1&);
	void Show(const Matrix4x4&);

	void ScalarMultiple(Matrix1x4&, const int);
	void ScalarMultiple(Matrix4x1&, const int);
	void ScalarMultiple(Matrix4x4&, const int);

	Matrix4x1 Transposed(const Matrix1x4&);
	Matrix1x4 Transposed(const Matrix4x1&);

	//=======================================================================================
	Matrix4x4 Multiple(const Matrix4x1&, const Matrix1x4&);
	float Multiple(const Matrix1x4&, const Matrix4x1&);
	void Multiple(Matrix4x4&, const Matrix4x4&);

	void Matrix1x4_Multiple(Matrix1x4&, const Matrix4x4&);

	void Vector3D_Multiple(Vector3D&, const Matrix4x4&);

	void Rotate_X(Matrix4x4&, const float);
	void Rotate_Y(Matrix4x4&, const float);
	void Rotate_Z(Matrix4x4&, const float);
	
	void Translate(Matrix4x4&, const float, const float, const float);

	void Scaling(Matrix4x4&, const float, const float, const float);


}