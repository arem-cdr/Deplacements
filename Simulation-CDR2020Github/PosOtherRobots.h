#pragma once

#include"Point.h"
#include"BasicFunctions.h"
#include"Constantes.h"

class PosOtherRobots
{
private:
	float* m_timeLastSeen;
	Point* m_posOtherRobots;
	const int m_nbOtherRobots;
	const int m_nbTours;

	int indClosest(const Point& point);

public:
	PosOtherRobots(int nbRobots, int nbTours);
	~PosOtherRobots();

	Point getPos(int i, int j) const;

	void updatePosOtherRobots(const std::vector <Point*>& robot_pos, const Point& actualPos);

	void testCollisionOtherRobots(const Point& pos, const int tour, float& malus, bool& collision, const float speed, const sf::Vector2f robotSize);

};

