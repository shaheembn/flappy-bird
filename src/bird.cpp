#include "bird.hpp"

Bird::Bird(sf::Vector2f Position, sf::Vector2f Scale, sf::Texture &texture)
:   position(Position),
	scale(Scale)
{
    sprite.setTexture(texture);
	sprite.setScale(sf::Vector2f(scale.x / texture.getSize().x, scale.y / texture.getSize().y));
    velocity = sf::Vector2f(0.0f, 0.0f);
}

void Bird::fall()
{
	// Don't go above the screen
	if(position.y <= 0)
	{
		position.y = 0;
		velocity.y = 0;
	}

	// Fall with an increasing velocity
	velocity.y += 0.05f;
}

void Bird::render(sf::RenderWindow &window) {
	// Update position according to velocity
	position += velocity;
	sprite.setPosition(position);

	// Draw the bird onto the window
	window.draw(sprite);
}
