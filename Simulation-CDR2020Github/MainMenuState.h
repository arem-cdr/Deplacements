#pragma once
#include "SimulationState.h"
#include "SettingsState.h"
#include "Gui.h"
#include "GraphicsSettings.h"

class MainMenuState :
	public State
{
private:
	//Variables
	GraphicsSettings& gfxSettings;
	sf::Texture backgroundTexture;
	sf::RectangleShape background;
	sf::Font font;
	sf::Music backgroundMusic;
	sf::SoundBuffer buttonHoverSoundBuffer;
	sf::SoundBuffer buttonActiveSoundBuffer;

	std::map<std::string, gui::TextButton*> buttons;

	//Functions
	void initVariables();
	void initBackground();
	void initFonts();
	void initKeybinds();
	void initTextButtons();
	void initSound();


public:
	MainMenuState(sf::RenderWindow* window, GraphicsSettings& gfxSettings, std::map<std::string, int>* supportedKeys, std::stack<State*>* states);
	virtual ~MainMenuState();

	//Functions
	void updateInput(const float& dt);
	void updateTextButtons();
	void updateMusic();
	void update(const float& dt, const float wheelTicks = 0);
	void renderTextButtons(sf::RenderTarget& target);
	void render(sf::RenderTarget* target = NULL);
};
