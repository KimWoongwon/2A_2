#include "Matrix_Function.h"

bool Matrix_Function::Equal(const Matrix1x3& _m1, const Matrix1x3& _m2)
{
	return _m1._11 == _m2._11 && _m1._12 == _m2._12 && _m1._13 == _m2._13;
}
bool Matrix_Function::Equal(const Matrix3x1& _m1, const Matrix3x1& _m2)
{
	return _m1._11 == _m2._11 && _m1._21 == _m2._21 && _m1._31 == _m2._31;
}
bool Matrix_Function::Equal(const Matrix3x3& _m1, const Matrix3x3& _m2)
{
	bool flag = true;
	for (int i = 0; i < Matrix3x3::SIZE; i++)
	{
		for (int j = 0; j < Matrix3x3::SIZE; j++)
		{
			if (_m1.matrix[i][j] != _m2.matrix[i][j])
				flag = false;
		}
	}

	return flag;
}
bool Matrix_Function::Equal(const Matrix1x4& _m1, const Matrix1x4& _m2)
{
	return _m1._11 == _m2._11 && _m1._12 == _m2._12 && _m1._13 == _m2._13 && _m1._14 == _m2._14;
}
bool Matrix_Function::Equal(const Matrix4x1& _m1, const Matrix4x1& _m2)
{
	return _m1._11 == _m2._11 && _m1._21 == _m2._21 && _m1._31 == _m2._31 && _m1._41 == _m2._41;
}
bool Matrix_Function::Equal(const Matrix4x4& _m1, const Matrix4x4& _m2)
{
	bool flag = true;
	for (int i = 0; i < Matrix4x4::SIZE; i++)
	{
		for (int j = 0; j < Matrix4x4::SIZE; j++)
		{
			if (_m1.matrix[i][j] != _m2.matrix[i][j])
				flag = false;
		}
	}

	return flag;
}

void Matrix_Function::Plus(Matrix1x3& _m1, const Matrix1x3& _m2)
{
	_m1._11 += _m2._11;
	_m1._12 += _m2._12;
	_m1._13 += _m2._13;
}
void Matrix_Function::Plus(Matrix3x1& _m1, const Matrix3x1& _m2)
{
	_m1._11 += _m2._11;
	_m1._21 += _m2._21;
	_m1._31 += _m2._31;
}
void Matrix_Function::Plus(Matrix3x3& _m1, const Matrix3x3& _m2)
{
	for (int i = 0; i < Matrix3x3::SIZE; i++)
		for (int j = 0; j < Matrix3x3::SIZE; j++)
			_m1.matrix[i][j] += _m2.matrix[i][j];
}
void Matrix_Function::Plus(Matrix1x4& _m1, const Matrix1x4& _m2)
{
	_m1._11 += _m2._11;
	_m1._12 += _m2._12;
	_m1._13 += _m2._13;
	_m1._14 += _m2._14;
}
void Matrix_Function::Plus(Matrix4x1& _m1, const Matrix4x1& _m2)
{
	_m1._11 += _m2._11;
	_m1._21 += _m2._21;
	_m1._31 += _m2._31;
	_m1._41 += _m2._41;
}
void Matrix_Function::Plus(Matrix4x4& _m1, const Matrix4x4& _m2)
{
	for (int i = 0; i < Matrix4x4::SIZE; i++)
		for (int j = 0; j < Matrix4x4::SIZE; j++)
			_m1.matrix[i][j] += _m2.matrix[i][j];
}

void Matrix_Function::Minus(Matrix1x3& _m1, const Matrix1x3& _m2)
{
	_m1._11 -= _m2._11;
	_m1._12 -= _m2._12;
	_m1._13 -= _m2._13;
}
void Matrix_Function::Minus(Matrix3x1& _m1, const Matrix3x1& _m2)
{
	_m1._11 -= _m2._11;
	_m1._21 -= _m2._21;
	_m1._31 -= _m2._31;
}
void Matrix_Function::Minus(Matrix3x3& _m1, const Matrix3x3& _m2)
{
	for (int i = 0; i < Matrix3x3::SIZE; i++)
		for (int j = 0; j < Matrix3x3::SIZE; j++)
			_m1.matrix[i][j] -= _m2.matrix[i][j];
}
void Matrix_Function::Minus(Matrix1x4& _m1, const Matrix1x4& _m2)
{
	_m1._11 -= _m2._11;
	_m1._12 -= _m2._12;
	_m1._13 -= _m2._13;
	_m1._14 -= _m2._14;
}
void Matrix_Function::Minus(Matrix4x1& _m1, const Matrix4x1& _m2)
{
	_m1._11 -= _m2._11;
	_m1._21 -= _m2._21;
	_m1._31 -= _m2._31;
	_m1._41 -= _m2._41;
}
void Matrix_Function::Minus(Matrix4x4& _m1, const Matrix4x4& _m2)
{
	for (int i = 0; i < Matrix4x4::SIZE; i++)
		for (int j = 0; j < Matrix4x4::SIZE; j++)
			_m1.matrix[i][j] -= _m2.matrix[i][j];
}

void Matrix_Function::Show(const Matrix1x3& _m)
{
	printf("%3.0f %3.0f %3.0f\n", _m._11, _m._12, _m._13);
}
void Matrix_Function::Show(const Matrix3x1& _m)
{
	printf("%3.0f %3.0f %3.0f\n", _m._11, _m._21, _m._31);
}
void Matrix_Function::Show(const Matrix3x3& _m)
{
	for (int i = 0; i < Matrix3x3::SIZE; i++)
		printf("%3.0f %3.0f %3.0f\n", _m.matrix[i][0], _m.matrix[i][1], _m.matrix[i][2]);
}
void Matrix_Function::Show(const Matrix1x4& _m)
{
	printf("%3.0f %3.0f %3.0f %3.0f\n", _m._11, _m._12, _m._13, _m._14);
}
void Matrix_Function::Show(const Matrix4x1& _m)
{
	printf("%3.0f %3.0f %3.0f %3.0f\n", _m._11, _m._21, _m._31, _m._41);
}
void Matrix_Function::Show(const Matrix4x4& _m)
{
	for (int i = 0; i < Matrix4x4::SIZE; i++)
		printf("%3.0f %3.0f %3.0f %3.0f\n", _m.matrix[i][0], _m.matrix[i][1], _m.matrix[i][2], _m.matrix[i][3]);
}

void Matrix_Function::ScalarMultiple(Matrix1x3& _m, const int _scalar)
{
	_m._11 *= _scalar;
	_m._12 *= _scalar;
	_m._13 *= _scalar;
}
void Matrix_Function::ScalarMultiple(Matrix3x1& _m, const int _scalar)
{
	_m._11 *= _scalar;
	_m._21 *= _scalar;
	_m._31 *= _scalar;
}
void Matrix_Function::ScalarMultiple(Matrix1x4& _m, const int _scalar)
{
	_m._11 *= _scalar;
	_m._12 *= _scalar;
	_m._13 *= _scalar;
	_m._14 *= _scalar;
}
void Matrix_Function::ScalarMultiple(Matrix4x1& _m, const int _scalar)
{
	_m._11 *= _scalar;
	_m._21 *= _scalar;
	_m._31 *= _scalar;
	_m._41 *= _scalar;
}

Matrix3x1 Matrix_Function::Transposed(const Matrix1x3& _m)
{
	Matrix3x1 temp(_m._11, _m._12, _m._13);

	return temp;
}
Matrix1x3 Matrix_Function::Transposed(const Matrix3x1& _m)
{
	Matrix1x3 temp(_m._11, _m._21, _m._31);

	return temp;
}
Matrix4x1 Matrix_Function::Transposed(const Matrix1x4& _m)
{
	Matrix4x1 temp(_m._11, _m._12, _m._13, _m._14);

	return temp;
}
Matrix1x4 Matrix_Function::Transposed(const Matrix4x1& _m)
{
	Matrix1x4 temp(_m._11, _m._21, _m._31, _m._41);

	return temp;
}



void Matrix_Function::Multiple(Matrix3x3& _m1, const Matrix3x3& _m2)
{
	Matrix3x3 temp = Matrix3x3::Zero;

	for (int i = 0; i < Matrix3x3::SIZE; i++)
	{
		for (int j = 0; j < Matrix3x3::SIZE; j++)
		{
			temp.matrix[i][j] = 0;
			for (int k = 0; k < Matrix3x3::SIZE; k++)
			{
				temp.matrix[i][j] += (_m1.matrix[i][k] * _m2.matrix[k][j]);
			}
		}
	}

	_m1 = temp;
}
void Matrix_Function::Multiple(Matrix4x4& _m1, const Matrix4x4& _m2)
{
	Matrix4x4 temp = Matrix4x4::Zero;

	for (int i = 0; i < Matrix4x4::SIZE; i++)
	{
		for (int j = 0; j < Matrix4x4::SIZE; j++)
		{
			for (int k = 0; k < Matrix4x4::SIZE; k++)
			{
				temp.matrix[i][j] += (_m1.matrix[i][k] * _m2.matrix[k][j]);
			}
		}
	}
	
	_m1 = temp;
}

void Matrix_Function::Matrix1x3_Multiple(Matrix1x3& _m, const Matrix3x3& _cm)
{
	Matrix1x3 temp;
	temp._11 = (_m._11 * _cm._11) + (_m._12 * _cm._21) + (_m._13 * _cm._31);
	temp._12 = (_m._11 * _cm._12) + (_m._12 * _cm._22) + (_m._13 * _cm._32);
	temp._13 = (_m._11 * _cm._13) + (_m._12 * _cm._23) + (_m._13 * _cm._33);
	_m = temp;
}
void Matrix_Function::Matrix1x4_Multiple(Matrix1x4& _m, const Matrix4x4& _cm)
{
	Matrix1x4 temp;
	temp._11 = (_m._11 * _cm._11) + (_m._12 * _cm._21) + (_m._13 * _cm._31) + (_m._14 * _cm._41);
	temp._12 = (_m._11 * _cm._12) + (_m._12 * _cm._22) + (_m._13 * _cm._32) + (_m._14 * _cm._42);
	temp._13 = (_m._11 * _cm._13) + (_m._12 * _cm._23) + (_m._13 * _cm._33) + (_m._14 * _cm._43);
	temp._14 = (_m._11 * _cm._14) + (_m._12 * _cm._24) + (_m._13 * _cm._34) + (_m._14 * _cm._44);
	_m = temp;
}

void Matrix_Function::Vector2D_Multiple(Vector2D& _v, const Matrix3x3& _cm)
{
	Vector2D temp;
	temp.setx((_v.getx() * _cm._11) + (_v.gety() * _cm._21) + (_v.getw() * _cm._31));
	temp.sety((_v.getx() * _cm._12) + (_v.gety() * _cm._22) + (_v.getw() * _cm._32));
	temp.setw((_v.getx() * _cm._13) + (_v.gety() * _cm._23) + (_v.getw() * _cm._33));
	_v = temp;
}
void Matrix_Function::Vector3D_Multiple(Vector3D& _v, const Matrix4x4& _cm)
{
	Vector3D temp;
	temp.setx((_v.getx() * _cm._11) + (_v.gety() * _cm._21) + (_v.getz() * _cm._31) + (_v.getw() * _cm._41));
	temp.sety((_v.getx() * _cm._12) + (_v.gety() * _cm._22) + (_v.getz() * _cm._32) + (_v.getw() * _cm._42));
	temp.setz((_v.getx() * _cm._13) + (_v.gety() * _cm._23) + (_v.getz() * _cm._33) + (_v.getw() * _cm._43));
	temp.setw((_v.getx() * _cm._14) + (_v.gety() * _cm._24) + (_v.getz() * _cm._34) + (_v.getw() * _cm._44));
	_v = temp;
}

void Matrix_Function::Rotate_2D(Matrix3x3& _m, const float _angle)
{
	_m._11 = cosf(_angle);
	_m._12 = sinf(_angle);
	_m._21 = -sinf(_angle);
	_m._22 = cosf(_angle);
}
void Matrix_Function::Rotate_X(Matrix4x4& _m, const float _angle)
{
	_m._22 = cosf(_angle);
	_m._23 = sinf(_angle);
	_m._32 = -sinf(_angle);
	_m._33 = cosf(_angle);
}
void Matrix_Function::Rotate_Y(Matrix4x4& _m, const float _angle)
{
	_m._11 = cosf(_angle);
	_m._13 = sinf(_angle);
	_m._31 = -sinf(_angle);
	_m._33 = cosf(_angle);
}
void Matrix_Function::Rotate_Z(Matrix4x4& _m, const float _angle)
{
	_m._11 = cosf(_angle);
	_m._12 = -sinf(_angle);
	_m._21 = sinf(_angle);
	_m._22 = cosf(_angle);
}

void Matrix_Function::Translate_2D(Matrix3x3& _m, const float _dx, const float _dy)
{
	_m._21 = _dx;
	_m._22 = _dy;
}
void Matrix_Function::Translate(Matrix4x4& _m, const float _dx, const float _dy, const float _dz)
{
	_m._41 = _dx;
	_m._42 = _dy;
	_m._43 = _dz;
}

void Matrix_Function::Scaling_2D(Matrix3x3& _m, const float _sx, const float _sy)
{
	_m._11 = _sx;
	_m._22 = _sy;
}
void Matrix_Function::Scaling(Matrix4x4& _m, const float _sx, const float _sy, const float _sz)
{
	_m._11 = _sx;
	_m._22 = _sy;
	_m._33 = _sz;
}


