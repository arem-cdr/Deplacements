#include "stdafx.h"
#include "MainMenuState.h"
#include "stdafx.h"
#include "MainMenuState.h"

//Initializer functions
void MainMenuState::initVariables()
{

}

void MainMenuState::initBackground()
{
	this->background.setSize(
		sf::Vector2f(
			static_cast<float>(this->window->getSize().x),
			static_cast<float>(this->window->getSize().y)
		)
	);

	if (!this->backgroundTexture.loadFromFile("Ressources/Images/BackgroundSettings.png"))
	{
		throw "ERROR::MAIN_MENU_STATE::FAILED_TO_LOAD_BACKGROUND_TEXTURE";
	}
	this->background.setTexture(&backgroundTexture);
}

void MainMenuState::initFonts()
{
	if (!this->font.loadFromFile("Ressources/Fonts/Dosis-Light.ttf"))
	{
		throw("ERROR::MAINMENUSTATE::COULD NOT LOAD FONT");
	}
}

void MainMenuState::initKeybinds()
{
	std::ifstream ifs("Config/mainmenustate_keybinds.ini");

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

void MainMenuState::initSound()
{
	//if (!backgroundMusic.openFromFile("Ressources/Audio/airtone_reCreation.ogg"))
	//	throw("ERROR::MAINMENUSTATE::COULD NOT LOAD BACKGROUNDMUSIC");

	if (!buttonHoverSoundBuffer.loadFromFile("Ressources/Audio/button_mainMenu.ogg"))
		throw("ERROR::MAINMENUSTATE::COULD NOT LOAD BUTTONHOVERSOUND");

	if (!buttonActiveSoundBuffer.loadFromFile("Ressources/Audio/button_mainMenu.ogg"))
		throw("ERROR::MAINMENUSTATE::COULD NOT LOAD BUTTONACTIVESOUND");
}

void MainMenuState::initTextButtons()
{
	this->buttons["GAME_STATE"] = new gui::TextButton(
		880.f, 400.f, 150.f, 50.f,
		&this->font, "Simulation", 50,
		sf::Color(70, 70, 70, 200), sf::Color(250, 250, 250, 250), sf::Color(20, 20, 20, 50),
		sf::Color(70, 70, 70, 0), sf::Color(150, 150, 150, 0), sf::Color(20, 20, 20, 0),
		&this->buttonHoverSoundBuffer, &this->buttonActiveSoundBuffer
	);

	this->buttons["SETTINGS_STATE"] = new gui::TextButton(
		880, 470.f, 150.f, 50.f,
		&this->font, "Settings", 50,
		sf::Color(70, 70, 70, 200), sf::Color(250, 250, 250, 250), sf::Color(20, 20, 20, 50),
		sf::Color(70, 70, 70, 0), sf::Color(150, 150, 150, 0), sf::Color(20, 20, 20, 0),
		&this->buttonHoverSoundBuffer, &this->buttonActiveSoundBuffer
	);

	this->buttons["EDITOR_STATE"] = new gui::TextButton(
		880, 540.f, 150.f, 50.f,
		&this->font, "Editor", 50,
		sf::Color(70, 70, 70, 200), sf::Color(250, 250, 250, 250), sf::Color(20, 20, 20, 50),
		sf::Color(70, 70, 70, 0), sf::Color(150, 150, 150, 0), sf::Color(20, 20, 20, 0),
		&this->buttonHoverSoundBuffer, &this->buttonActiveSoundBuffer
	);

	this->buttons["EXIT_STATE"] = new gui::TextButton(
		880, 630.f, 150.f, 50.f,
		&this->font, "Quit", 50,
		sf::Color(70, 70, 70, 200), sf::Color(250, 250, 250, 250), sf::Color(20, 20, 20, 50),
		sf::Color(70, 70, 70, 0), sf::Color(150, 150, 150, 0), sf::Color(20, 20, 20, 0),
		&this->buttonHoverSoundBuffer, &this->buttonActiveSoundBuffer
	);
}

MainMenuState::MainMenuState(sf::RenderWindow* window, GraphicsSettings& gfxSettings, std::map<std::string, int>* supportedKeys, std::stack<State*>* states)
	: State(window, supportedKeys, states), gfxSettings(gfxSettings)
{
	this->initVariables();
	this->initBackground();
	this->initFonts();
	this->initKeybinds();
	this->initSound();
	this->initTextButtons();
}

MainMenuState::~MainMenuState()
{
	//this->backgroundMusic.stop();

	auto it = this->buttons.begin();
	for (it = this->buttons.begin(); it != this->buttons.end(); ++it)
	{
		delete it->second;
	}
}

void MainMenuState::updateInput(const float& dt)
{

}

void MainMenuState::updateTextButtons()
{
	/*Updates all the buttons in the state and handles their functionlaity.*/

	for (auto& it : this->buttons)
	{
		it.second->update(this->mousePosView);
	}

	//New game
	if (this->buttons["GAME_STATE"]->isPressed())
	{
		//this->backgroundMusic.stop();
		this->states->push(new SimulationState(this->window, this->supportedKeys, this->states));//new GameState(this->window, &this->supportedKeys, this->states));
	}

	//Settings
	if (this->buttons["SETTINGS_STATE"]->isPressed())
	{
		this->states->push(new SettingsState(this->window, this->gfxSettings, this->supportedKeys, this->states));//new GameState(this->window, &this->supportedKeys, this->states));
	}

	//Editor
	if (this->buttons["EDITOR_STATE"]->isPressed())
	{
		//this->states->push(new EditorState(this->window, this->supportedKeys, this->states));//new GameState(this->window, &this->supportedKeys, this->states));
	}

	//Quit the game
	if (this->buttons["EXIT_STATE"]->isPressed())
	{
		this->backgroundMusic.stop();
		this->endState();
	}
}

void MainMenuState::updateMusic()
{
	//if (this->backgroundMusic.getStatus() == sf::SoundSource::Stopped)
	//{
	//	this->backgroundMusic.setPlayingOffset(sf::seconds(5.2f));
	//	this->backgroundMusic.play();
	//}
}

void MainMenuState::update(const float& dt, const float wheelTicks)
{
	this->updateMusic();

	this->updateMousePositions();
	this->updateInput(dt);

	this->updateTextButtons();
}

void MainMenuState::renderTextButtons(sf::RenderTarget& target)
{
	for (auto& it : this->buttons)
	{
		it.second->render(target);
	}
}

void MainMenuState::render(sf::RenderTarget* target)
{
	if (!target)
		target = this->window;

	target->draw(this->background);

	this->renderTextButtons(*target);

	////REMOVE LATER
	//sf::Text mouseText;
	//mouseText.setPosition(this->mousePosView.x, this->mousePosView.y - 15);
	//mouseText.setFont(this->font);
	//mouseText.setCharacterSize(12);
	//std::stringstream ss;
	//ss << this->mousePosView.x << " " << this->mousePosView.y;
	//mouseText.setString(ss.str());
	//target->draw(mouseText);
}
