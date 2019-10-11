#pragma once

#include"Gui.h"

class PauseMenu
{
private:
	sf::Font& font;
	sf::Text menuText;

	sf::RectangleShape background;
	sf::RectangleShape container;

	std::map<std::string, gui::TextButton*> buttons;

public:
	PauseMenu(sf::RenderWindow& window, sf::Font& font);
	virtual ~PauseMenu();

	//Accessor
	std::map<std::string, gui::TextButton*>& getTextButtons();

	// Functions
	const bool isTextButtonPressed(const std::string key);
	void addTextButton(const std::string key, float y, const std::string text, const int characterSize = 50);
	void update(const sf::Vector2f mousePos);
	void render(sf::RenderTarget& target);
};

