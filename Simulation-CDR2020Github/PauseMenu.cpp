#include "stdafx.h"
#include "PauseMenu.h"

PauseMenu::PauseMenu(sf::RenderWindow& window, sf::Font& font)
	: font(font)
{
	sf::Vector2f pos = sf::Vector2f(0.f, 0.f);

	//Init background
	this->background.setSize(
		sf::Vector2f(
			static_cast<float>(window.getSize().x),
			static_cast<float>(window.getSize().y)
		)
	);
	this->background.setFillColor(sf::Color(20, 20, 20, 100));
	this->background.setPosition(pos);

	//Init container
	this->container.setSize(
		sf::Vector2f(
			static_cast<float>(window.getSize().x) / 4.f,
			static_cast<float>(window.getSize().y) * 0.93f
		)
	);
	this->container.setFillColor(sf::Color(20, 20, 20, 200));
	this->container.setPosition(
		pos +
		sf::Vector2f(
			window.getSize().x / 2.f - this->container.getSize().x / 2.f,
			window.getSize().y * 0.04f
		)
	);

	//Init text
	this->menuText.setFont(font);
	this->menuText.setFillColor(sf::Color(255, 255, 255, 200));
	this->menuText.setCharacterSize(static_cast <int> (60.f * container.getSize().x / 480.f));
	this->menuText.setString("PAUSED");
	this->menuText.setPosition(
		sf::Vector2f(
			this->container.getPosition().x + container.getSize().x / 2.f - this->menuText.getGlobalBounds().width / 2.f,
			this->container.getPosition().y * 1.05f
		)
	);

	//Init Quit Button
	addTextButton("QUIT", pos.y + container.getSize().y * 0.85f, "Quit", static_cast <int> (40.f * container.getSize().x / 480.f));

}

PauseMenu::~PauseMenu()
{
	auto it = this->buttons.begin();
	for (it = this->buttons.begin(); it != this->buttons.end(); ++it)
	{
		delete it->second;
	}
}

std::map<std::string, gui::TextButton*>& PauseMenu::getTextButtons()
{
	return this->buttons;
}

const bool PauseMenu::isTextButtonPressed(const std::string key)
{
	return this->buttons[key]->isPressed();
}

void PauseMenu::addTextButton(const std::string key, const float y, const std::string text, const int characterSize)
{
	float width = 250.f;
	float height = 50.f;
	float x = this->container.getPosition().x + this->container.getSize().x / 2.f - width / 2.f;

	this->buttons[key] = new gui::TextButton(
		x, y, width, height,
		&this->font, text, characterSize,
		sf::Color(70, 70, 70, 200), sf::Color(250, 250, 250, 250), sf::Color(20, 20, 20, 50),
		sf::Color(70, 70, 70, 0), sf::Color(150, 150, 150, 0), sf::Color(20, 20, 20, 0)
	);
}

// Functions
void PauseMenu::update(const sf::Vector2f mousePos)
{
	for (auto& i : this->buttons)
	{
		i.second->update(mousePos);
	}
}

void PauseMenu::render(sf::RenderTarget& target)
{
	target.draw(this->background);
	target.draw(this->container);

	for (auto& i : this->buttons)
	{
		i.second->render(target);
	}

	target.draw(this->menuText);
}