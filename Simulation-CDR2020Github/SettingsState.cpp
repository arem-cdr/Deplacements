#include "stdafx.h"
#include "SettingsState.h"

//Initializer functions
void SettingsState::initVariables()
{
	this->modes = sf::VideoMode::getFullscreenModes();
}

void SettingsState::initBackground()
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

void SettingsState::initFonts()
{
	if (!this->font.loadFromFile("Ressources/Fonts/Dosis-Light.ttf"))
	{
		throw("ERROR::MAINMENUSTATE::COULD NOT LOAD FONT");
	}
}

void SettingsState::initKeybinds()
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

void SettingsState::initGui()
{
	this->buttons["BACK"] = new gui::TextButton(
		1090.f, 640.f, 150.f, 50.f,
		&this->font, "Back", 50,
		sf::Color(70, 70, 70, 200), sf::Color(250, 250, 250, 250), sf::Color(20, 20, 20, 50),
		sf::Color(70, 70, 70, 0), sf::Color(150, 150, 150, 0), sf::Color(20, 20, 20, 0)
	);

	this->buttons["APPLY"] = new gui::TextButton(
		680.f, 640.f, 150.f, 50.f,
		&this->font, "Apply", 50,
		sf::Color(70, 70, 70, 200), sf::Color(250, 250, 250, 250), sf::Color(20, 20, 20, 50),
		sf::Color(70, 70, 70, 0), sf::Color(150, 150, 150, 0), sf::Color(20, 20, 20, 0)
	);

	std::vector<std::string> modes_str;
	for (auto& i : this->modes)
	{
		modes_str.push_back(std::to_string(i.width) + 'x' + std::to_string(i.width));
	}

	this->dropDownLists["RESOLUTION"] = new gui::DropDownList(900.f, 410.f, 200.f, 40.f, font, modes_str.data(), (unsigned int)modes_str.size());

	std::string list[] = { "WINDOWED", "FULL SCREEN" };
	this->dropDownLists["WINDOW"] = new gui::DropDownList(900.f, 460.f, 200.f, 40.f, font, list, 2);
}

void SettingsState::initText()
{
	this->optionsText.setFont(font);

	this->optionsText.setPosition(sf::Vector2f(700.f, 400.f));

	this->optionsText.setCharacterSize(40);

	this->optionsText.setFillColor(sf::Color(255, 255, 255, 200));


	this->optionsText.setString(
		"Resolution \nFullscreen \nVsync \nAntialiasing \n"
	);

	//std::to_string(this->window->getSize().x) + "x");
}

SettingsState::SettingsState(sf::RenderWindow* window, GraphicsSettings& gfxSettings, std::map<std::string, int>* supportedKeys, std::stack<State*>* states)
	: State(window, supportedKeys, states), gfxSettings(gfxSettings)
{
	this->initVariables();
	this->initBackground();
	this->initFonts();
	this->initKeybinds();
	this->initGui();
	this->initText();

}

SettingsState::~SettingsState()
{
	auto it = this->buttons.begin();
	for (it = this->buttons.begin(); it != this->buttons.end(); ++it)
	{
		delete it->second;
	}

	auto it2 = this->dropDownLists.begin();
	for (it2 = this->dropDownLists.begin(); it2 != this->dropDownLists.end(); ++it2)
	{
		delete it2->second;
	}
}

//Accessors

//Functions
void SettingsState::updateInput(const float& dt)
{

}

void SettingsState::updateGui(const float& dt)
{
	/*Updates all the gui in the state and handles their functionlaity.*/
	//TextButtons
	for (auto& it : this->buttons)
	{
		it.second->update(this->mousePosView);
	}

	//TextButton functionnality
	//Quit the game
	if (this->buttons["BACK"]->isPressed())
	{
		this->endState();
	}
	//Apply selected Settings
	if (this->buttons["APPLY"]->isPressed())
	{
		//TEST
		this->gfxSettings.resolution = this->modes[dropDownLists["RESOLUTION"]->getActiveElementId()];
		this->gfxSettings.fullScreen = !(bool)dropDownLists["WINDOW"]->getActiveElementId();

		if (this->gfxSettings.fullScreen)
		{
			this->window->create(this->gfxSettings.resolution, this->gfxSettings.title, sf::Style::Default);
		}
		else
		{
			this->window->create(this->gfxSettings.resolution, this->gfxSettings.title, sf::Style::Fullscreen);
		}

		//On sauvegarde les info dans graphics.ini
		this->gfxSettings.saveToFile("Config/graphics.ini");
	}

	//DropDownLists
	for (auto& it : this->dropDownLists)
	{
		it.second->update(this->mousePosView, dt);
	}

	//DropDownLists functionnality
}

void SettingsState::update(const float& dt, const float wheelTicks)
{
	this->updateMousePositions();
	this->updateInput(dt);

	this->updateGui(dt);
}

void SettingsState::renderGui(sf::RenderTarget& target)
{
	for (auto& it : this->buttons)
	{
		it.second->render(target);
	}

	for (auto& it : this->dropDownLists)
	{
		it.second->render(target);
	}
}

void SettingsState::render(sf::RenderTarget* target)
{
	if (!target)
		target = this->window;

	target->draw(this->background);

	this->renderGui(*target);

	target->draw(this->optionsText);

	//REMOVE LATER
	sf::Text mouseText;
	mouseText.setPosition(this->mousePosView.x, this->mousePosView.y - 15);
	mouseText.setFont(this->font);
	mouseText.setCharacterSize(12);
	std::stringstream ss;
	ss << this->mousePosView.x << " " << this->mousePosView.y;
	mouseText.setString(ss.str());
	target->draw(mouseText);
}
