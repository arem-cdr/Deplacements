#include "stdafx.h"

#include "PosOtherRobots.h"

int PosOtherRobots::indClosest(const Point& point)
{
	float dmin = (tableLength + tableWidth) * 100;
	int indRet = -1;

	for (int i = 0; i < m_nbOtherRobots; ++i)
	{
		if (dist(point - m_posOtherRobots[i]) < dmin && m_timeLastSeen[i] != 0)
		{
			indRet = i;
		}
	}

	if (indRet == -1)
		std::cout << "ind error in posotherrobots::indclosest" << std::endl;

	return indRet;
}

PosOtherRobots::PosOtherRobots(const int nbRobots, const int nbTours)
	: m_nbOtherRobots(nbRobots- 1), m_nbTours(nbTours)
{
	m_timeLastSeen = new float[m_nbOtherRobots];
	for (int i = 0; i < m_nbOtherRobots; ++i)
	{
		m_timeLastSeen[i] = 0.f;
	}

	m_posOtherRobots = new Point[m_nbOtherRobots * nbTours];
	for (int i = 0; i < m_nbOtherRobots; ++i)
	{
		m_posOtherRobots[i] = Point();
	}
}

PosOtherRobots::~PosOtherRobots()
{
	delete[] m_posOtherRobots;
}

Point PosOtherRobots::getPos(int i, int j) const
{
	return m_posOtherRobots[i* m_nbOtherRobots + j];
}

void PosOtherRobots::updatePosOtherRobots(const std::vector<Point*>& robot_pos, const Point& actualPos)
{
	// mise à jour de la pos des autres robots

	for (int i = 0; i < m_nbOtherRobots; ++i)
	{
		m_timeLastSeen[i] += temps_gen;
	}

	for (size_t i = 0; i < robot_pos.size(); ++i)
	{
		if (*robot_pos[i] != actualPos)
		{
			//on identifie le robot dont la pos est *robot_pos[i] parmi les robots dans m_posOtherRobots
			int ind = indClosest(*robot_pos[i]);

			m_timeLastSeen[ind] = 0.f;

			//
			Point lastPos = m_posOtherRobots[ind * m_nbTours];

			m_posOtherRobots[ind * m_nbTours] = *robot_pos[i];

			for (int j = 1; j < m_nbTours; j++)
			{
				m_posOtherRobots[ind * m_nbTours + j] = m_posOtherRobots[ind * m_nbTours] + (float)j * (m_posOtherRobots[ind * m_nbTours] - lastPos);
			}
		}
	}
}

void PosOtherRobots::testCollisionOtherRobots(const Point& pos, const int tour, float& malus, bool& collision, const float speed, const sf::Vector2f robotSize)
{
	for (int j = 0; j < m_nbOtherRobots; ++j)
	{
		float d = dist(pos - m_posOtherRobots[j * m_nbTours]);
		//float d2 = dist(robotSize.x, robotSize.y); //debug

		if (d < dist(robotSize.x, robotSize.y))//min(50*(tour+1), 200))
		{
			collision = true;
			//std::cout << "collision" << std::endl;
		}
		else if (d < dist(robotSize.x, robotSize.y) * std::max(1.1f, std::min(speed / 6.f, 2.5f)))//std::min(speed / 4.f, 2.5f)))
		{
			//d2 = d2;
			float m = 1.f * ((1000 - d)) * std::max(speed - 5.f, 0.1f);
			malus += 1.f * ((1000 - d)) * std::max(speed - 5.f, 0.1f);
			//std::cout << "collision probable" << std::endl;
		}
	}
}