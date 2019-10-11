#pragma once
class Terrain
{
private:
	const sf::Vector2u m_size;

	sf::RectangleShape backGround;
	sf::Texture backGroundTexture;

	std::vector<sf::RectangleShape*> sprites;

public:
	Terrain(sf::Vector2u sizeWindow);
	~Terrain();

	void render(sf::RenderTarget& target) const;

	bool collision(sf::RectangleShape Hitbox) const;
	sf::Vector2u getSize() const;
};

