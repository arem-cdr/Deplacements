#include "stdafx.h"
#include "Gobelet.h"

Gobelet::Gobelet(Point center, gobeletColor color)
	: RenderObject(center)
{
	shape.setRadius(12.f);
	shape.setPosition(center.getX(), center.getY());
	shape.setOutlineColor(sf::Color::Black);
	shape.setOutlineThickness(1.f);

	switch (color)
	{
	case RED:
		shape.setFillColor(sf::Color::Red);
		break;
	case GREEN:
		shape.setFillColor(sf::Color(10, 170, 10));
		break;
	default:
		break;
	}

	isAvailable = true;
}

Gobelet::~Gobelet()
{
}

void Gobelet::render(sf::RenderTarget& target)
{
	target.draw(shape);
}
