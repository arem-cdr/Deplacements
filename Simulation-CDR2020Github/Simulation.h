#pragma once

#include"State.h"
#include"GraphicsSettings.h"
#include"MainMenuState.h"
#include"Gui.h"

class Simulation
{
private:
	//Variables
	GraphicsSettings gfxSettings;
	sf::RenderWindow* window;
	sf::Event sfEvent;

	sf::Clock dtClock;
	float dt;
	float wheelTicks;

	std::stack<State*> states;

	std::map<std::string, int> supportedKeys;

	//Initialization
	void initVariables();
	void initGraphicsSettings();
	void initWindow();
	void initKeys();
	void initStates();

public:
	//Constructors/Destructors
	Simulation();
	virtual ~Simulation();

	//Functions

	//Regular
	void endApplication();

	//Update
	void updateDt();
	void updateSFMLEvents();
	void update();

	//Render
	void render();

	//Core
	void run();
};

