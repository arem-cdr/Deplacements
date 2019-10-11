#pragma once
#include"constantes.h"


class Point
{
public:
	//Constructor/destructor
	Point()
	{
		m_x = 0.f;
		m_y = 0.f;
		m_angle = 0.f;
	}
	Point(float x0, float y0, float angle = 0.f)
	{
		m_x = x0;
		m_y = y0;
		m_angle = angle;
	}
	~Point() {}

	//Accessors
	float getX() const { return m_x; }

	float getY() const { return m_y; }
	float getangle() const { return m_angle; }

	//Set
	void setX(const float nx) { m_x = nx; }
	void setY(const float ny) { m_y = ny; }
	void Setangle(const float val) { m_angle = val; }
	void Setall(const float x, const float y, const float angle) { m_x = x; m_y = y; m_angle = angle; }

	//Functions
	bool egal(const Point& p) const { return m_x == p.m_x && m_y == p.m_y; }
	void afficher() const { std::cout << "(" << m_x << ";" << m_y << ")" << std::endl; }

private:
	float m_x, m_y;
	float m_angle;
};

