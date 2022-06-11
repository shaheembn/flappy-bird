#pragma once

#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/RenderWindow.hpp>

class Obstacle
{
	public:
		std::vector<sf::Sprite> sprites;

		Obstacle(float position, float width, float blockSize, float gapIndex, sf::Texture &obstacleTexture, sf::Texture &obstacleEdgeTexture);
		void move(float speed);
		void render(sf::RenderWindow &window);
};
