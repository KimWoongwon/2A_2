#include "../Header/Matrix_Function.h"

#pragma region __EQUAL_FUNCTION__
bool Matrix_Function::Equal(const Matrix1x4& _m1, const Matrix1x4& _m2)
{
	return _m1._11 == _m2._11 && _m1._12 == _m2._12
		&& _m1._13 == _m2._13 && _m1._14 == _m2._14;
}
bool Matrix_Function::Equal(const Matrix4x1& _m1, const Matrix4x1& _m2)
{
	return _m1._11 == _m2._11 && _m1._21 == _m2._21
		&& _m1._31 == _m2._31 && _m1._41 == _m2._41;
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
#pragma endregion

#pragma region __PLUS_FUNCTION__
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
#pragma endregion

#pragma region __MINUS_FUNCTION__
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
#pragma endregion

#pragma region __SHOW_FUNCTION__
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
#pragma endregion

#pragma region __SCALAR_MULTIPLE_FUNCTION__
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
void Matrix_Function::ScalarMultiple(Matrix4x4& _m, const int _scalar)
{
	for (int i = 0; i < Matrix4x4::SIZE; i++)
		for (int j = 0; j < Matrix4x4::SIZE; j++)
			_m.matrix[i][j] *= _scalar;
}

#pragma endregion

#pragma region __TRANSPOSED_FUNCTION__
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
#pragma endregion

#pragma region __MULTIPLE_FUNCTION__
Matrix4x4 Multiple(const Matrix4x1& _vm, const Matrix1x4& _hm)
{
	Matrix4x4 temp = Matrix4x4::Zero;

	for (int i = 0; i < Matrix4x4::SIZE; i++)
		for (int j = 0; j < Matrix4x4::SIZE; j++)
			temp.matrix[i][j] = _vm.matrix[i][0] * _hm.matrix[0][j];

	return temp;
}

float Multiple(const Matrix1x4& _hm, const Matrix4x1& _vm)
{
	return (_hm._11 * _vm._11) + (_hm._12 * _vm._21)
		+ (_hm._13 * _vm._31) + (_hm._14 * _vm._41);
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

void Matrix_Function::Matrix1x4_Multiple(Matrix1x4& _m, const Matrix4x4& _cm)
{
	Matrix1x4 temp;
	temp._11 = (_m._11 * _cm._11) + (_m._12 * _cm._21) + (_m._13 * _cm._31) + (_m._14 * _cm._41);
	temp._12 = (_m._11 * _cm._12) + (_m._12 * _cm._22) + (_m._13 * _cm._32) + (_m._14 * _cm._42);
	temp._13 = (_m._11 * _cm._13) + (_m._12 * _cm._23) + (_m._13 * _cm._33) + (_m._14 * _cm._43);
	temp._14 = (_m._11 * _cm._14) + (_m._12 * _cm._24) + (_m._13 * _cm._34) + (_m._14 * _cm._44);
	_m = temp;
}

void Matrix_Function::Vector3D_Multiple(Vector3D& _v, const Matrix4x4& _cm)
{
	Vector3D temp;
	temp.x = (_v.x * _cm._11) + (_v.y * _cm._21) + (_v.z * _cm._31) + (_v.w * _cm._41);
	temp.y = (_v.x * _cm._12) + (_v.y * _cm._22) + (_v.z * _cm._32) + (_v.w * _cm._42);
	temp.z = (_v.x * _cm._13) + (_v.y * _cm._23) + (_v.z * _cm._33) + (_v.w * _cm._43);
	temp.w = (_v.x * _cm._14) + (_v.y * _cm._24) + (_v.z * _cm._34) + (_v.w * _cm._44);
	_v = temp;
}
#pragma endregion

#pragma region __TRANSLATION_FUNCTION__
void Matrix_Function::Rotate_X(Matrix4x4& _m, const float _radian)
{
	_m._22 = cosf(_radian);
	_m._23 = sinf(_radian);
	_m._32 = -sinf(_radian);
	_m._33 = cosf(_radian);
}
void Matrix_Function::Rotate_Y(Matrix4x4& _m, const float _radian)
{
	_m._11 = cosf(_radian);
	_m._13 = sinf(_radian);
	_m._31 = -sinf(_radian);
	_m._33 = cosf(_radian);
}
void Matrix_Function::Rotate_Z(Matrix4x4& _m, const float _radian)
{
	_m._11 = cosf(_radian);
	_m._12 = -sinf(_radian);
	_m._21 = sinf(_radian);
	_m._22 = cosf(_radian);
}

void Matrix_Function::Translate(Matrix4x4& _m, const float _dx, const float _dy, const float _dz)
{
	_m._41 = _dx;
	_m._42 = _dy;
	_m._43 = _dz;
}

void Matrix_Function::Scaling(Matrix4x4& _m, const float _sx, const float _sy, const float _sz)
{
	_m._11 = _sx;
	_m._22 = _sy;
	_m._33 = _sz;
}
#pragma endregion




