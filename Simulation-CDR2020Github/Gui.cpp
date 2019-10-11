#include "stdafx.h"
#include "Gui.h"

gui::TextButton::TextButton(float x, float y, float width, float height,
	sf::Font* font, std::string text, unsigned character_size,
	sf::Color text_idle_color, sf::Color text_hover_color, sf::Color text_active_color,
	sf::Color idle_color, sf::Color hover_color, sf::Color active_color,
	sf::SoundBuffer* hover_sound_buffer, sf::SoundBuffer* active_sound_buffer,
	sf::Color outline_idle_color, sf::Color outline_hover_color, sf::Color outline_active_color,
	short unsigned id)
{
	this->buttonState = BTN_IDLE;
	this->id = id;

	this->shape.setPosition(sf::Vector2f(x, y));
	this->shape.setSize(sf::Vector2f(width, height));
	this->shape.setFillColor(idle_color);
	this->shape.setOutlineThickness(1.f);
	this->shape.setOutlineColor(outline_idle_color);

	this->font = font;
	this->text.setFont(*this->font);
	this->text.setString(text);
	this->text.setFillColor(text_idle_color);
	this->text.setCharacterSize(character_size);
	this->text.setPosition(
		this->shape.getPosition().x + (this->shape.getGlobalBounds().width / 2.f) - this->text.getGlobalBounds().width / 2.f,
		this->shape.getPosition().y + (this->shape.getGlobalBounds().height / 2.f) - this->text.getCharacterSize() / 1.7f//this->text.getGlobalBounds().height / 2.f
	);

	if (hover_sound_buffer)
		this->hoverSound.setBuffer(*hover_sound_buffer);
	if (active_sound_buffer)
		this->activeSound.setBuffer(*active_sound_buffer);

	this->textIdleColor = text_idle_color;
	this->textHoverColor = text_hover_color;
	this->textActiveColor = text_active_color;

	this->idleColor = idle_color;
	this->hoverColor = hover_color;
	this->activeColor = active_color;

	this->outlineIdleColor = outline_idle_color;
	this->outlineHoverColor = outline_hover_color;
	this->outlineActiveColor = outline_active_color;
}

gui::TextButton::~TextButton()
{
	//this->activeSound.stop();
	//this->hoverSound.stop();
}

//Accessors
const bool gui::TextButton::isPressed() const
{
	if (this->buttonState == BTN_ACTIVE)
		return true;

	return false;
}

const bool gui::TextButton::isHover() const
{
	if (this->buttonState == BTN_HOVER)
		return true;

	return false;
}

const sf::Text gui::TextButton::getText() const
{
	return this->text;
}

const short unsigned& gui::TextButton::getId() const
{
	return this->id;
}

//Modifiers
void gui::TextButton::setText(const sf::Text text)
{
	this->text.setString(text.getString());
}

void gui::TextButton::setId(const short unsigned id)
{
	this->id = id;
}

//Functions
void gui::TextButton::update(const sf::Vector2f& mousePos)
{
	/*Update the booleans for hover and pressed*/
	//Storing previous state
	short unsigned prevButtonState = buttonState;

	//Idle
	this->buttonState = BTN_IDLE;

	//Hover
	if (this->shape.getGlobalBounds().contains(mousePos))
	{
		this->buttonState = BTN_HOVER;

		//Pressed
		if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
		{
			this->buttonState = BTN_ACTIVE;
		}
	}

	if (buttonState != prevButtonState)
	{
		switch (this->buttonState)
		{
		case BTN_IDLE:
			this->shape.setFillColor(this->idleColor);
			this->text.setFillColor(this->textIdleColor);
			this->shape.setOutlineColor(this->outlineIdleColor);
			break;

		case BTN_HOVER:
			this->shape.setFillColor(this->hoverColor);
			this->text.setFillColor(this->textHoverColor);
			this->shape.setOutlineColor(this->outlineHoverColor);
			this->hoverSound.play();
			break;

		case BTN_ACTIVE:
			this->shape.setFillColor(this->activeColor);
			this->text.setFillColor(this->textActiveColor);
			this->shape.setOutlineColor(this->outlineActiveColor);
			this->activeSound.play();
			break;

		default:
			this->shape.setFillColor(sf::Color::Red);
			this->text.setFillColor(sf::Color::Blue);
			this->shape.setOutlineColor(sf::Color::Green);
			break;
		}
	}
}

void gui::TextButton::render(sf::RenderTarget& target)
{
	target.draw(this->shape);
	target.draw(this->text);
}


//DROPDOWNLIST ================================================================================

gui::DropDownList::DropDownList(float x, float y, float width, float height,
	sf::Font& font, std::string list[],
	unsigned nbOfElements, unsigned default_index)
	: font(font), showList(false), keytimeMax(1.f), keytime(0.f)
{
	//unsigned nbOfElements = sizeof(list) / sizeof(std::string);



	this->activeElement = new gui::TextButton(
		x, y, width, height,
		&this->font, list[default_index], 14,
		sf::Color(255, 255, 255, 150), sf::Color(255, 255, 255, 200), sf::Color(20, 20, 20, 50),
		sf::Color(70, 70, 70, 200), sf::Color(150, 150, 150, 200), sf::Color(20, 20, 20, 200),
		NULL, NULL,
		sf::Color(255, 255, 255, 200), sf::Color(255, 255, 255, 255), sf::Color(20, 20, 20, 50)
	);

	for (unsigned int i = 0; i < nbOfElements; i++)
	{
		this->list.push_back(
			new gui::TextButton(
				x, y + ((i + 1) * height), width, height,
				&this->font, list[i], 14,
				sf::Color(255, 255, 255, 150), sf::Color(255, 255, 255, 255), sf::Color(20, 20, 20, 50),
				sf::Color(70, 70, 70, 200), sf::Color(150, 150, 150, 200), sf::Color(20, 20, 20, 200),
				NULL, NULL,
				sf::Color(255, 255, 255, 0), sf::Color(255, 255, 255, 0), sf::Color(20, 20, 20, 0),
				i
			)
		);
	}
}

gui::DropDownList::~DropDownList()
{
	delete this->activeElement;
	for (size_t i = 0; i < this->list.size(); i++)
	{
		delete this->list[i];
	}
}

//Accessors
const bool gui::DropDownList::getKeytime()
{
	if (this->keytime >= this->keytimeMax)
	{
		this->keytime = 0.f;
		return true;
	}

	return false;
}

const unsigned short& gui::DropDownList::getActiveElementId() const
{
	return this->activeElement->getId();
}

//Functions
void gui::DropDownList::updateKeytime(const float& dt)
{
	if (this->keytime < this->keytimeMax)
		this->keytime += 10.f * dt;
}

void gui::DropDownList::update(const sf::Vector2f& mousePos, const float& dt)
{
	this->updateKeytime(dt);

	this->activeElement->update(mousePos);

	//Show and hide the list
	if (this->activeElement->isPressed() && this->getKeytime())
	{
		if (this->showList)
			this->showList = false;
		else
			this->showList = true;
	}

	if (this->showList)
	{
		for (auto& i : this->list)
		{
			i->update(mousePos);

			if (i->isPressed() && this->getKeytime())
			{
				this->showList = false;
				this->activeElement->setText(i->getText());
				this->activeElement->setId(i->getId());
			}
		}
	}
}

void gui::DropDownList::render(sf::RenderTarget& target)
{
	this->activeElement->render(target);

	if (this->showList)
	{
		for (auto& i : this->list)
		{
			i->render(target);
		}
	}
}

//TEXTURE SELECTOR ===================================================

gui::TextureSelector::TextureSelector(
	float x, float y, float width, float height,
	const sf::Texture* texture_sheet)
{
	this->bounds.setSize(sf::Vector2f(width, height));
	this->bounds.setPosition(x, y);
	this->bounds.setFillColor(sf::Color(50, 50, 50, 100));
	this->bounds.setOutlineThickness(1.f);
	this->bounds.setOutlineColor(sf::Color(255, 255, 255, 200));

	this->sheet.setTexture(*texture_sheet);
	this->sheet.setPosition(x, y);

	if (this->sheet.getGlobalBounds().width > this->bounds.getGlobalBounds().width)
	{
		this->sheet.setTextureRect(sf::IntRect(0, 0, (int)this->bounds.getGlobalBounds().width, (int)this->sheet.getGlobalBounds().height));
	}

	if (this->sheet.getGlobalBounds().height > this->bounds.getGlobalBounds().height)
	{
		this->sheet.setTextureRect(sf::IntRect(0, 0, (int)this->sheet.getGlobalBounds().width, (int)this->bounds.getGlobalBounds().height));
	}
}

gui::TextureSelector::~TextureSelector()
{

}

//Functions
void gui::TextureSelector::update()
{

}

void gui::TextureSelector::render(sf::RenderTarget& target)
{
	target.draw(this->bounds);
	target.draw(this->sheet);
}