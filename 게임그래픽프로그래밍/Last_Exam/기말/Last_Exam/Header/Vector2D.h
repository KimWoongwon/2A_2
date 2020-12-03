#pragma once 
#include <stdio.h>
#include <math.h>

struct Point
{
	static Point Zero;

	float x, y;
	Point();
	Point(int, int);
	Point(Point*);
};

struct Vector2D
{
	static Vector2D Zero;			// 0 ���� ����
	static Vector2D Identity;		// ���� ���� ����

	float x, y, w;					// x, y �� ��ǥ�� w�� ��ȯ����� ���� ���̰�
	float rotsize;					// ������ ũ�� ��Ʈ ���
	float nrotsize;					// ������ ũ�� ��Ʈ �Ⱦ��� ��

	Vector2D();						// ������
	Vector2D(float, float);
	Vector2D(float, float, float);
	Vector2D(Vector2D&);
	~Vector2D();

	bool operator==(Vector2D&);		// ���� ��
	Vector2D operator*(int);		// ���� ��Į��� ����
	Vector2D operator+(Vector2D&);	// ������ ���� ����
	Vector2D operator-(Vector2D&);	// ������ ���� ����

	void setsize();					// ���ο��� ���Ǵ� �Լ� ������ ũ�� ���

	Point getPos();					// Point ���� ȣȯ���� ���� �Լ�
	void setPos(Point);
	void setPos(float, float);

	Vector2D normalizaion();		// �������� ���
	float dot_Product(Vector2D&);	// ������ ���
	float GetCos(Vector2D&);		// �ڻ��ΰ� ���
	
	void Console_Print();
};

// ���Ͱ��� ������ Vertex2D��ü
struct Vertex2D
{
	Vector2D vector;
	int index;

	Point getPos();
	void setPos(Point);
	void setPos(float, float);
};


