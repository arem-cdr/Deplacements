#pragma once

#include"State.h"
#include"Robot.h"
#include"Gobelet.h"
#include"constantes.h"
#include"Terrain.h"
#include"PauseMenu.h"
#include"InfoRobot.h"

class SimulationState
	: public State
{
private:
	// variables
	bool debug;

	sf::Font pauseMenuFont;
	PauseMenu* pauseMenu;

	sf::RectangleShape backGround;
	sf::Texture backGroundTexture;

	float totalClock;

	sf::Clock frameClock;
	sf::Clock genClock;
	sf::Time frameTime;
	sf::Time genTime;

	const Terrain terrain;

	const int m_nbRobotYellow;
	const int m_nbRobotPurple;
	std::vector <robot*> m_robotTeamYellow;
	std::vector <robot*> m_robotTeamPurple;

	std::vector <Gobelet> m_gobelets;

	std::vector <InfoRobot> infoRobot;

	//Point* posRobots;

	const sf::Vector2f cmToPx;

	// private functions
	virtual void initKeybinds();
	void initRobot(sf::RenderWindow* window);
	void initGobelets();
	void initPauseMenu();
	void initFont();

	void resetPauseMenu();
	void updatePauseMenuTextButtons();

	void updateRobots(const float& dt);

public:
	SimulationState(sf::RenderWindow* window, std::map<std::string, int>* supportedKeys, std::stack<State*>* states);
	~SimulationState();

	std::vector<Point> posRobots();

	virtual void updateInput(const float& dt);
	virtual void update(const float& dt, const float wheelTicks = 0);
	virtual void render(sf::RenderTarget* target = 0);
};

