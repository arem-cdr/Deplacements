#pragma once

enum button_states { BTN_IDLE = 0, BTN_HOVER, BTN_ACTIVE };

namespace gui
{
	class Button
	{
		// A definir : pour les boutons play, pause...
	};

	class TextButton
	{
	private:
		short unsigned buttonState;
		short unsigned id;
		sf::Sound hoverSound;
		sf::Sound activeSound;

		sf::RectangleShape shape;
		sf::Font* font;
		sf::Text text;

		sf::Color textIdleColor;
		sf::Color textHoverColor;
		sf::Color textActiveColor;

		sf::Color idleColor;
		sf::Color hoverColor;
		sf::Color activeColor;

		sf::Color outlineIdleColor;
		sf::Color outlineHoverColor;
		sf::Color outlineActiveColor;

	public:
		TextButton(float x, float y, float width, float height,
			sf::Font* font, std::string text, unsigned character_size,
			sf::Color text_idle_color, sf::Color text_hover_color, sf::Color text_active_color,
			sf::Color idle_color, sf::Color hover_color, sf::Color active_color,
			sf::SoundBuffer* hover_sound_buffer = NULL, sf::SoundBuffer* active_sound_buffer = NULL,
			sf::Color outline_idle_color = sf::Color::Transparent, sf::Color outline_hover_color = sf::Color::Transparent, sf::Color outline_active_color = sf::Color::Transparent,
			short unsigned id = 0);
		~TextButton();

		//Accessors
		const bool isPressed() const;
		const bool isHover() const;
		const sf::Text getText() const;
		const short unsigned& getId() const;

		//Modifiers
		void setText(const sf::Text text);
		void setId(const short unsigned id);

		//Functions
		void update(const sf::Vector2f& mousePos);
		void render(sf::RenderTarget& target);
	};

	class DropDownList
	{
	private:
		float keytime;
		float keytimeMax;

		sf::Font& font;
		gui::TextButton* activeElement;
		std::vector<gui::TextButton*> list;
		bool showList;


	public:
		DropDownList(float x, float y, float width, float height,
			sf::Font& font, std::string list[],
			unsigned nrOfElements, unsigned default_index = 0);
		~DropDownList();

		//Accessors
		const unsigned short& getActiveElementId() const;

		//Functions
		const bool getKeytime();
		void updateKeytime(const float& dt);
		void update(const sf::Vector2f& mousePos, const float& dt);
		void render(sf::RenderTarget& target);
	};

	class TextureSelector
	{
	private:
		sf::RectangleShape bounds;
		sf::Sprite sheet;

	public:
		TextureSelector(float x, float y, float width, float height, const sf::Texture* texture_sheet);
		~TextureSelector();

		//Functions
		void update();
		void render(sf::RenderTarget& target);
	};
}
