#include "stdafx.h"
#include "InfoRobot.h"

InfoRobot::InfoRobot(sf::Vector2f pos, sf::Vector2f size, sf::Font& font)
	: font(font), nbText(5)
{
	this->container.setSize(size);
	this->container.setFillColor(sf::Color(20, 20, 20, 120));
	this->container.setPosition(pos);

	for (int i = 0; i < nbText; i++)
	{
		text.push_back(sf::Text());

		this->text[i].setFont(font);
		this->text[i].setFillColor(sf::Color(255, 255, 255, 200));
		this->text[i].setCharacterSize(static_cast <int> (30.f * container.getSize().y / 480.f));
		this->text[i].setPosition(
			pos + sf::Vector2f(10.f, 30.f * i)
		);
		this->text[i].setString("");
	}
}

void InfoRobot::updateInfo(const robot* robot)
{
	this->text[0].setString((std::string) "Position actuelle : (" + robot->posString() + (std::string) ")");
	this->text[1].setString(robot->speedString());
	this->text[4].setString(robot->pathFindingString());
}

void InfoRobot::render(sf::RenderTarget& target)
{
	target.draw(this->container);

	for (int i = 0; i<nbText; ++i)
	{
		target.draw(this->text[i]);
	}	
}
