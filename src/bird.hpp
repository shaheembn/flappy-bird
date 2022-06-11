#pragma once

#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/RenderWindow.hpp>

class Bird
{
    public:
        sf::Vector2f position;
		sf::Vector2f scale;
        sf::Vector2f velocity;

        sf::Sprite sprite;

		Bird(sf::Vector2f Position, sf::Vector2f scale, sf::Texture &texture);

		void fall();
		void jump();

		void render(sf::RenderWindow &window);
};
