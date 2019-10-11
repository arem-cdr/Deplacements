
#include"stdafx.h"

#include "Pathfinding.h"

Pathfinding::Pathfinding(const sf::Vector2f robotSize) : m_robotSize(robotSize)
{
	sf::RectangleShape shape(robotSize);
	shape.setOrigin(sf::Vector2f(robotSize.x / 2.f, robotSize.y / 2.f));
	shape.setFillColor(sf::Color(70, 200, 70, 170));
	shape.setOutlineColor(sf::Color::Black);
	shape.setOutlineThickness(2);
	shape.setPosition(sf::Vector2f(-robotSize.x, -robotSize.y));
	shape.setRotation(0);

	m_shapeDebug = shape;
}

Pathfinding::~Pathfinding()
{
}
