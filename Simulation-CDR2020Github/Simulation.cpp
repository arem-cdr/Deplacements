#include"stdafx.h"

#include "Simulation.h"

//Static functions

//Initializer functions
void Simulation::initVariables()
{
	this->window = NULL;

	this->dt = 0.f;

	this->wheelTicks = 0;
}

void Simulation::initGraphicsSettings()
{
	this->gfxSettings.loadFromFile("Config/graphics.ini");
}

void Simulation::initWindow()
{
	//Creates a SFML window

	if (this->gfxSettings.fullScreen) {
		this->window = new sf::RenderWindow(
			this->gfxSettings.resolution,
			this->gfxSettings.title,
			sf::Style::Fullscreen,
			this->gfxSettings.contextSettings);
	}
	else {
		this->window = new sf::RenderWindow(
			this->gfxSettings.resolution,
			this->gfxSettings.title,
			sf::Style::Titlebar | sf::Style::Close,
			this->gfxSettings.contextSettings);
	}

	this->window->setFramerateLimit(this->gfxSettings.frameRateLimit);
	this->window->setVerticalSyncEnabled(this->gfxSettings.verticalSync);
}

void Simulation::initKeys()
{
	std::ifstream ifs("Config/supported_keys.ini");

	if (ifs.is_open())
	{
		std::string key = "";
		int key_value = 0;

		while (ifs >> key >> key_value)
		{
			this->supportedKeys[key] = key_value;
		}
	}

	ifs.close();
}

void Simulation::initStates()
{
	this->states.push(new MainMenuState(this->window, this->gfxSettings, &this->supportedKeys, &this->states));
}

//Constructors/Destructors
Simulation::Simulation()
{
	this->initVariables();
	this->initGraphicsSettings();
	this->initWindow();
	this->initKeys();
	this->initStates();
}

Simulation::~Simulation()
{
	delete this->window;

	while (!this->states.empty())
	{
		delete this->states.top();
		this->states.pop();
	}
}

//Functions
void Simulation::endApplication()
{
	std::cout << "Ending Application!" << "\n";
}

void Simulation::updateDt()
{
	//Updates the dt variable with the time it takes to update and render one frame

	this->dt = this->dtClock.restart().asSeconds();
}

void Simulation::updateSFMLEvents()
{
	wheelTicks = 0.f;

	while (this->window->pollEvent(this->sfEvent))
	{
		if (this->sfEvent.type == sf::Event::Closed)
			this->window->close();
		else if (this->sfEvent.type == sf::Event::MouseWheelScrolled)
			this->wheelTicks = sfEvent.mouseWheelScroll.delta;
	}
}

void Simulation::update()
{
	this->updateSFMLEvents();

	if (!this->states.empty())
	{
		if (this->window->hasFocus())
		{
			this->states.top()->update(this->dt, wheelTicks);

			if (this->states.top()->getQuit())
			{
				//this->states.top()->endState();
				delete this->states.top();
				this->states.pop();
			}
		}
	}
	//Application end
	else
	{
		std::cout << "End Of Application" << std::endl;
		this->endApplication();
		this->window->close();
	}
}

void Simulation::render()
{
	this->window->clear();

	//Render items
	if (!this->states.empty()) {
		this->states.top()->render();
	}

	this->window->display();
}

void Simulation::run()
{
	while (this->window->isOpen())
	{
		this->updateDt();
		this->update();
		this->render();
	}
}

