#include "obstacle.hpp"

Obstacle::Obstacle(float position, float width, float blockSize, float gapIndex, sf::Texture &obstacleTexture, sf::Texture &obstacleEdgeTexture)
{
	// Each obstacle column is made up of 6 blocks (including gap) to make it easier to texture
	for(int i = 0; i < 6; i++)
	{
		// Create a sprite for each block except for the gap
		if(i != gapIndex)
		{
			// Apply the edge texture if the block is near the gap
			if(i == gapIndex - 1)
				sprites.push_back(sf::Sprite(obstacleEdgeTexture));
			else if(i == gapIndex + 1)
			{
				sprites.push_back(sf::Sprite(obstacleEdgeTexture));
				// Flip the sprite vertically if the edge is below
				sprites[sprites.size() - 1].setTextureRect(sf::IntRect(0, obstacleTexture.getSize().x, obstacleTexture.getSize().x, -obstacleTexture.getSize().y));
			}
			else
				sprites.push_back(sf::Sprite(obstacleTexture));
			
			// Scale the sprites (according to obstacle scale and texture scale)
			sprites[sprites.size() - 1].setScale(sf::Vector2f(width / obstacleTexture.getSize().x, blockSize / obstacleTexture.getSize().y));
			// Position the blocks according to their index
			sprites[sprites.size() - 1].setPosition(sf::Vector2f(position, i * blockSize));
		}
	}
}

// Move all blocks in obstacle to the left with given speed
void Obstacle::move(float speed)
{
	for(int i = 0; i < sprites.size(); i++)
	{
		sprites[i].move(sf::Vector2f(-speed, 0.0f));
	}
}

// Render all blocks of obstacle
void Obstacle::render(sf::RenderWindow &window) {
	for(int i = 0; i < sprites.size(); i++)
	{
		window.draw(sprites[i]);
	}
}
