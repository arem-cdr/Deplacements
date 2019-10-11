#pragma once

#include"Robot.h"

class InfoRobot
{
private:
	sf::RectangleShape container;

	sf::Font& font;

	std::vector <sf::Text> text;

	int nbText;

public:
	InfoRobot(sf::Vector2f pos, sf::Vector2f size, sf::Font& font);

	void updateInfo(const robot* robot);
	void render(sf::RenderTarget& target);
};

