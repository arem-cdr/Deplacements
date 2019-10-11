#include"stdafx.h"

#include "SimulationState.h"

void SimulationState::initKeybinds()
{
	std::ifstream ifs("Config/gamestate_keybinds.ini");

	if (ifs.is_open())
	{
		std::string key = "";
		std::string key2 = "";

		while (ifs >> key >> key2)
		{
			this->keybinds[key] = this->supportedKeys->at(key2);
		}
	}

	ifs.close();
}

void SimulationState::initRobot(sf::RenderWindow* window)
{
	for (int i = 0; i < m_nbRobotPurple; i++)
	{
		m_robotTeamPurple.push_back(new robot(m_nbRobotPurple + m_nbRobotYellow, team::PURPLE, sf::Vector2f(27.f * (int)cmToPx.x, 27.f * (int)cmToPx.y), i, &this->terrain, i));
		infoRobot.push_back(InfoRobot(
			sf::Vector2f(50.f, 50.f + window->getSize().y * 0.5f * i),
			sf::Vector2f(window->getSize().x * 0.3f, window->getSize().y * 0.43f),
			this->pauseMenuFont
		)
		);
	}
	for (int i = 0; i < m_nbRobotYellow; i++)
	{
		m_robotTeamYellow.push_back(new robot(m_nbRobotPurple + m_nbRobotYellow, team::YELLOW, sf::Vector2f(27.f * (int)cmToPx.x, 27.f * (int)cmToPx.y), i, &this->terrain, i));
		infoRobot.push_back(InfoRobot(
			sf::Vector2f(window->getSize().x - window->getSize().x * 0.3f - 50.f, 50.f + window->getSize().y * 0.5f * i),
			sf::Vector2f(window->getSize().x * 0.3f, window->getSize().y * 0.43f),
			this->pauseMenuFont
		)
		);
	}

	this->frameTime = frameClock.getElapsedTime();
	this->genTime = genClock.getElapsedTime();
}

void SimulationState::initGobelets()
{
	//m_gobelets.push_back(Gobelet(Point(500, 500), RED));
}

void SimulationState::initPauseMenu()
{
	this->pauseMenu = new PauseMenu(*this->window, this->pauseMenuFont);
}

void SimulationState::initFont()
{
	if (!this->pauseMenuFont.loadFromFile("Ressources/Fonts/Dosis-Light.ttf"))
	{
		throw("ERROR::MAINMENUSTATE::COULD NOT LOAD FONT");
	}
}

void SimulationState::resetPauseMenu()
{
	if (pauseMenu)
		delete this->pauseMenu;

	this->pauseMenu = new PauseMenu(*this->window, this->pauseMenuFont);
}

void SimulationState::updatePauseMenuTextButtons()
{
	if (this->pauseMenu->isTextButtonPressed("QUIT"))
		this->endState();
}

void SimulationState::updateRobots(const float& dt)
{
	if (totalClock <= 100.f)
	{
		for (size_t i = 0; i < m_robotTeamPurple.size(); ++i)
		{
			m_robotTeamPurple[i]->play(16.f / (m_nbRobotPurple + m_nbRobotYellow), posRobots());
		}
		for (size_t i = 0; i < m_robotTeamYellow.size(); ++i)
		{
			m_robotTeamYellow[i]->play(16.f / (m_nbRobotPurple + m_nbRobotYellow), posRobots());

		}

		for (size_t i = 0; i < m_robotTeamPurple.size(); ++i)
		{
			m_robotTeamPurple[i]->update(dt);
			infoRobot[i].updateInfo(m_robotTeamPurple[i]);
		}

		for (size_t i = 0; i < m_robotTeamYellow.size(); ++i)
		{
			m_robotTeamYellow[i]->update(dt);
			infoRobot[i + m_nbRobotPurple].updateInfo(m_robotTeamYellow[i]);
		}
	}
	else
	{
	}
}

// Constructor/Destructor
SimulationState::SimulationState(sf::RenderWindow* window, std::map<std::string, int>* supportedKeys, std::stack<State*>* states)
	: State(window, supportedKeys, states), 
	terrain(window->getSize()), 
	m_nbRobotPurple(2), m_nbRobotYellow(2), 
	cmToPx((float) window->getSize().x / 300.f, (float)window->getSize().y / 200.f),
	debug(true)
{
	this->initFont();
	this->initKeybinds();
	this->initPauseMenu();
	this->initRobot(window);
	this->initGobelets();
}

SimulationState::~SimulationState()
{
	for (size_t i = 0; i < m_robotTeamPurple.size(); ++i)
	{
		delete m_robotTeamPurple[i];
	}
	for (size_t i = 0; i < m_robotTeamYellow.size(); ++i)
	{
		delete m_robotTeamYellow[i];
	}

	if (this->pauseMenu)
		delete this->pauseMenu;
}

// Public Functions
std::vector<Point> SimulationState::posRobots()
{
	std::vector<Point> ret;

	for (int i = 0; i < m_nbRobotPurple; i++)
	{
		if (m_robotTeamPurple[i]){}
			ret.push_back(m_robotTeamPurple[i]->getPos());
	}

	for (int i = 0; i < m_nbRobotYellow; i++)
	{
		if (m_robotTeamYellow[i]){}
			ret.push_back(m_robotTeamYellow[i]->getPos());
		
	}

	return ret;
}

void SimulationState::updateInput(const float& dt)
{
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key(this->keybinds.at("CLOSE"))) && this->getKeytime())
	{
		if (!this->paused)
		{
			//resetPauseMenu();
			this->pauseState();
		}
		else
			this->unPauseState();
	}
}

void SimulationState::update(const float& dt, const float wheelTicks)
{
	this->totalClock += dt;
	this->updateKeytime(dt);

	this->updateMousePositions();
	this->updateInput(dt);

	if (!this->paused)
	{
		this->updateRobots(dt);
	}
	else
	{
		this->pauseMenu->update(this->mousePosView);
		this->updatePauseMenuTextButtons();
	}
}

void SimulationState::render(sf::RenderTarget* target)
{
	if (!target)
		target = this->window;

	terrain.render(*target);

	for (size_t i = 0; i < m_robotTeamPurple.size(); ++i) {
		m_robotTeamPurple[i]->render(*target);
	}
	for (size_t i = 0; i < m_robotTeamYellow.size(); ++i) {
		m_robotTeamYellow[i]->render(*target);
	}

	for (size_t i = 0; i < m_gobelets.size(); i++)
	{
		m_gobelets[i].render(*target);
	}

	for (size_t i = 0; i < infoRobot.size(); i++)
	{
		infoRobot[i].render(*target);
	}

	if (this->paused) // Pause menu render
	{
		this->pauseMenu->render(*target);

		
		//REMOVE LATER
		/*sf::Text mouseText;
		mouseText.setPosition(this->mousePosView.x, this->mousePosView.y - 15);
		mouseText.setFont(this->pauseMenuFont);
		mouseText.setCharacterSize(12);
		std::stringstream ss;
		ss << this->mousePosView.x << " " << this->mousePosView.y;
		mouseText.setString(ss.str());
		target->draw(mouseText);*/
	}
}
