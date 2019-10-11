#pragma once
#include "BasicFunctions.h"

class RenderObject
{
protected:
	Point m_pos; // position du robot

public:
	RenderObject();
	RenderObject(Point pos);
	~RenderObject();

	Point getPos() const;

	virtual void render(sf::RenderTarget& target) = 0;
};

