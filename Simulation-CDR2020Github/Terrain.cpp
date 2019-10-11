#include"stdafx.h"
#include "Terrain.h"

Terrain::Terrain(sf::Vector2u sizeWindow)
	: m_size(sizeWindow)
{
	this->backGround.setSize(
		sf::Vector2f(
			static_cast<float>(sizeWindow.x),
			static_cast<float>(sizeWindow.y)
		)
	);
	if (!this->backGroundTexture.loadFromFile("Ressources/Images/Terrain.png"))
	{
		std::cout << "loading error" << std::endl;
	}

	this->backGround.setTexture(&backGroundTexture);

	// mur du haut
	sprites.push_back(new sf::RectangleShape(sf::Vector2f((float)sizeWindow.x, (float)sizeWindow.y * 0.1f)));
	sprites[0]->setPosition(0.f, 0.f);
	sprites[0]->setFillColor(sf::Color(150, 50, 50, 200));

	// mur de gauche
	sprites.push_back(new sf::RectangleShape(sf::Vector2f((float)sizeWindow.x * 0.058f, (float)sizeWindow.y)));
	sprites[1]->setPosition(0.f, 0.f);
	sprites[1]->setFillColor(sf::Color(150, 50, 50, 200));

	// mur de droite
	sprites.push_back(new sf::RectangleShape(sf::Vector2f((float)sizeWindow.x * 0.058f, (float)sizeWindow.y)));
	sprites[2]->setPosition((float)sizeWindow.x * 0.942f, 0.f);
	sprites[2]->setFillColor(sf::Color(150, 50, 50, 200));

	// mur du bas
	sprites.push_back(new sf::RectangleShape(sf::Vector2f((float)sizeWindow.x, (float)sizeWindow.y * 0.04f)));
	sprites[3]->setPosition(sf::Vector2f(0.f, (float)sizeWindow.y * 0.96f));
	sprites[3]->setFillColor(sf::Color(150, 50, 50, 200));

	// rochers centraux
	sprites.push_back(new sf::RectangleShape(sf::Vector2f(13.f, (float)sizeWindow.y * 0.125f)));
	sprites[4]->setPosition(sf::Vector2f((float)sizeWindow.x * 0.496f, (float)sizeWindow.y * 0.835f));
	sprites[4]->setFillColor(sf::Color(150, 50, 50, 200));
	// rochers de gauche
	sprites.push_back(new sf::RectangleShape(sf::Vector2f(13.f, (float)sizeWindow.y * 0.125f)));
	sprites[5]->setPosition(sf::Vector2f((float)sizeWindow.x * 0.318f, (float)sizeWindow.y * 0.9f));
	sprites[5]->setFillColor(sf::Color(150, 50, 50, 200));

	sprites.push_back(new sf::RectangleShape(sf::Vector2f(13.f, (float)sizeWindow.y * 0.125f)));
	sprites[6]->setPosition(sf::Vector2f((float)sizeWindow.x * 0.674f, (float)sizeWindow.y * 0.9f));
	sprites[6]->setFillColor(sf::Color(150, 50, 50, 200));
}

Terrain::~Terrain()
{
	for (size_t i = 0; i < sprites.size(); ++i)
	{
		delete sprites[i];
		sprites[i] = NULL;
	}
}

void Terrain::render(sf::RenderTarget& target) const
{
	target.draw(this->backGround); 
	
	for (size_t i = 0; i < sprites.size(); i++)
	{
		target.draw(*sprites[i]);
	}
}

bool Terrain::collision(sf::RectangleShape Hitbox) const
{
	for (size_t i = 0; i < sprites.size(); ++i)
	{
		if (sprites[i]->getGlobalBounds().intersects(Hitbox.getGlobalBounds()))
		{
			return true;
		}
	}

	return false;
}

sf::Vector2u Terrain::getSize() const
{
	return m_size;
}