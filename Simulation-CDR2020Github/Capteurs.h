#pragma once

#include"Point.h"

class Capteurs
{
protected:
	std::vector<Point*>* m_posOtherRobot;

public:
	Capteurs(std::vector<Point*>* posOtherRobot);
	~Capteurs();

	void update();
};

