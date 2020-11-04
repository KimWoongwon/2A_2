#include <iostream>
#include <random>
#include <time.h>
#include "Matrix4x4.h"
using namespace std;

void main()
{
	Matrix4x4 m1(1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16);
	Matrix4x4 m2(1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16);
	Matrix4x4 m3;
	m3 = m1.Plus(m2);

	m3.Show();
	cout << endl;

	m3 = m1.Multiple(m2);

	m3.Show();
	cout << endl;

	Vector3D v1(2, 3, 4, 0);
	Vector3D v2;

	v2 = m3.Vec_Matrix_Multiple(v1);
	v2.Show();
	cout << endl;
}

