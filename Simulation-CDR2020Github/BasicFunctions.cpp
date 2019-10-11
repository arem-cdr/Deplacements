#include"stdafx.h"
#include "BasicFunctions.h"

float radian(float angle)
{
	return angle / 180 * PI;
}

float degre(float angle)
{
	return angle / PI * 180;
}

float val_abs(float a)
{
	if (a > 0)
		return a;
	return -a;
}

float part_pos(float a)
{
	if (a > 0)
		return a;
	return 0;
}

float dist(float a, float b)
{
	return sqrt(a * a + b * b);
}

float dist(Point p)
{
	return sqrt(p.getX() * p.getX() + p.getY() * p.getY());
}

float properAngleRad(float ang)
{
	if (ang > PI)
		return ang - 2 * PI;
	if (ang <= -PI)
		return ang + 2 * PI;
	return ang;
}

int sgn(float a)
{
	if (a >= 0)
		return 1;
	return -1;
}

float xconv(int x) { // ces fonctions effectuent les conversions de cm vers l'unite utilisee dans cette simulation
	return (float)(WINDOW_WIDTH - x * WINDOW_WIDTH / 3000);
}
float yconv(int y) {
	return (float)(y * WINDOW_HEIGHT / 2000);
}
float aconv(int angle) {
	return (float)(PI - angle * PI / 180);
}

float xconv2(int x) { // ces fonctions effectuent les conversions de cm vers l'unite utilisee dans cette simulation
	return (float)(x * WINDOW_WIDTH / 3000);
}

float CmToPx(float dist)
{
	return dist * 4;
}


Point operator+(Point const& a, Point const& b)
{
	Point result;

	result.Setall(a.getX() + b.getX(), a.getY() + b.getY(), a.getangle() + b.getangle());

	return result;
}

Point operator-(Point const& a, Point const& b)
{
	Point result;

	result.Setall(a.getX() - b.getX(), a.getY() - b.getY(), a.getangle() - b.getangle());

	return result;
}

Point operator*(float const t, Point const& a)
{
	Point result;

	result.Setall(a.getX() * t, a.getY() * t, a.getangle() * t);

	return result;
}

Point operator*(Point const& a, float const t)
{
	Point result;

	result.Setall(a.getX() * t, a.getY() * t, a.getangle() * t);

	return result;
}

bool operator!=(Point const& a, Point const& b)
{
	return a.getX() != b.getX() || a.getY() != b.getY() || a.getangle() != b.getangle();
}