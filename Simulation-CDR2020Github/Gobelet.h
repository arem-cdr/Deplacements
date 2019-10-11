#pragma once

#include"RenderObject.h"
#include"Point.h"

enum gobeletColor { RED = 0 , GREEN };

class Gobelet : public RenderObject
{
private:
	sf::CircleShape shape;
	bool isAvailable;

public:
	Gobelet(Point center, gobeletColor color);
	~Gobelet();
	
	virtual void render(sf::RenderTarget& target);

};

