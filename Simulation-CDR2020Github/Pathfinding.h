#pragma once

#include"Point.h"

class Pathfinding
{
protected:
	sf::RectangleShape m_shapeDebug;

	const sf::Vector2f m_robotSize;
	//Point m_pos;

public:
	Pathfinding(const sf::Vector2f robotSize);
	virtual ~Pathfinding();

	virtual void init(const std::vector <Point*>& robot_pos, const Point& actualPos, const Point& target, const float& rightSpeed, const float& leftSpeed) = 0; // fonction initialisant l'algorithme utilisé
	virtual void run(float timeAvailable, float& rightSpeed, float& leftSpeed, const Point& actual_pos, const Point& target, const std::vector <Point*>& robot_pos = std::vector <Point*>()) = 0;
	virtual void end(float& rightSpeed, float& leftSpeed) = 0; // fonction appliquant les commandes de vitesse des roues
	virtual std::string stateString() = 0;

	virtual void render(sf::RenderTarget& target) = 0;
};


