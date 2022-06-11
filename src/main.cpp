#include <iostream>

#include "bird.hpp"
#include "obstacle.hpp"

#include <SFML/Graphics.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/Window/Keyboard.hpp>

int main()
{
	// Create window
	sf::RenderWindow window(sf::VideoMode(600, 600), "Flappy Bird");
	
	// Initialize game clock
	sf::Clock clock;

	// Set the seed for random number generators
	srand(time(0));

	// Setup textures from image files
	sf::Texture skyTexture;
	sf::Texture birdTexture;
	sf::Texture obstacleTexture;
	sf::Texture obstacleEdgeTexture;
	if(
		!skyTexture.loadFromFile("../assets/images/sky.png") ||
		!birdTexture.loadFromFile("../assets/images/flappy.png") ||
		!obstacleTexture.loadFromFile("../assets/images/tube.png") ||
		!obstacleEdgeTexture.loadFromFile("../assets/images/tube-end.png")
	)
	{
		std::cerr << "Failed to load image file\n";
	}
	
	// Setup text font
	sf::Font pixeloidFont;
	if(!pixeloidFont.loadFromFile("../assets/fonts/PixeloidSans.ttf"))
	{
		std::cerr << "Failed to load font file\n";
	}

	// Backgrounds that scroll to give illusion of movement
	sf::Sprite background(skyTexture);
	sf::Sprite background2(skyTexture);
	background2.setPosition(sf::Vector2f(window.getSize().x, 0.0f));

	// Player bird
	Bird bird(sf::Vector2f(100.0f, 100.0f), sf::Vector2f(50.f, 50.0f), birdTexture);

	// Obstacles to fly through
	std::vector<Obstacle> obstacles;
	float obstacleWidth = 100.0f;
	float obstacleBlockHeight = 100.0f;
	obstacles.push_back(Obstacle(300.0f, obstacleWidth, obstacleBlockHeight, 1 + (rand() % 4), obstacleTexture, obstacleEdgeTexture));
	obstacles.push_back(Obstacle(600.0f, obstacleWidth, obstacleBlockHeight, 1 + (rand() % 4), obstacleTexture, obstacleEdgeTexture));

	// Game variables
	int points = 0;

	bool pointLock = false;
	bool gameOver = false;
	bool gamePaused = false;

	// Texts to display
	sf::Text scoreText("Score: " + std::to_string(points), pixeloidFont);
	sf::Text gameOverText("Game Over!", pixeloidFont, 50);
	gameOverText.setPosition((window.getSize().x / 2.0f) - (gameOverText.getLocalBounds().width / 2.0f), (window.getSize().y / 2.0f) - (gameOverText.getLocalBounds().height / 2.0f));
	sf::Text gameOverTipText("Press R to restart or Q to quit the game", pixeloidFont, 25);
	gameOverTipText.setPosition((window.getSize().x / 2.0f) - (gameOverTipText.getLocalBounds().width / 2.0f), (window.getSize().y / 2.0f) + gameOverTipText.getLocalBounds().height + 25.0f);

	// Game loop
	while(window.isOpen())
	{
		// Reset RNG seed
		srand(time(0));

		// Event loop
		sf::Event event;
		while(window.pollEvent(event))
		{
			// Close window
			if(event.type == sf::Event::Closed || (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Q))
				window.close();

			// Make the bird jump
			if(event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Space)
				bird.velocity.y = -2.5f;

			// Restart game after game is over
			if(event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::R)
			{
				gameOver = false;
				points = 0;
				background.setPosition(sf::Vector2f(0.0f, 0.0f));
				background2.setPosition(sf::Vector2f(window.getSize().x, 0.0f));
				bird.position.y = 100.0f;
				bird.velocity.y = 0.0f;
				obstacles.clear();
				obstacles.push_back(Obstacle(300.0f, obstacleWidth, obstacleBlockHeight, 1 + (rand() % 4), obstacleTexture, obstacleEdgeTexture));
				obstacles.push_back(Obstacle(600.0f, obstacleWidth, obstacleBlockHeight, 1 + (rand() % 4), obstacleTexture, obstacleEdgeTexture));
				gamePaused = false;
			}
		}
	
		// Update score text
		scoreText.setString("Score: " + std::to_string(points));

		// Stop the game if game over and display game over text
		if(gameOver)
		{
			gamePaused = true;
			window.draw(gameOverText);
			window.draw(gameOverTipText);
			window.display();
		}

		if(!gamePaused)
		{
			// Game over if you hit the bottom
			if(bird.position.y + bird.sprite.getLocalBounds().height >= 600)
			{
				gameOver = true;
			}
			
			// Check for collision with obstacles
			// TODO: A bit broken
			for(int i = 0; i < obstacles.size(); i++)
			{
				for(int j = 0; j < obstacles[i].sprites.size(); j++)
				{
					if(
						bird.sprite.getPosition().x + bird.sprite.getLocalBounds().width	>= obstacles[i].sprites[j].getPosition().x														&& 
						bird.sprite.getPosition().x											<= obstacles[i].sprites[j].getPosition().x + obstacles[i].sprites[j].getLocalBounds().width		&& 
						bird.sprite.getPosition().y											<= obstacles[i].sprites[j].getPosition().y + obstacles[i].sprites[j].getLocalBounds().height	&&
						bird.sprite.getPosition().y + bird.sprite.getLocalBounds().height	>= obstacles[i].sprites[j].getPosition().y
					)
					{
						gameOver = true;
						break;
					}
				}

				if(gameOver)
					break;
			}
			
			// Increment points if bird passes obstacle
			if(bird.position.x >= obstacles[0].sprites[0].getPosition().x + obstacleWidth && !pointLock)
			{
				points++;
				pointLock = true;
			}
			
			// Remove obstacle when it goes off-screen, and spawn a new one
			if(obstacles[0].sprites[0].getPosition().x + obstacles[0].sprites[0].getLocalBounds().width <= 0)
			{
				obstacles.erase(obstacles.begin());
				obstacles.push_back(Obstacle(600.0f, obstacleWidth, obstacleBlockHeight, 1 + (rand() % 4), obstacleTexture, obstacleEdgeTexture));
				pointLock = false;
			}

			// Clear the window off the last frame
			window.clear();

			// Draw the two backgrounds that scroll
			window.draw(background);
			window.draw(background2);
			
			// Make the bird fall
			bird.fall();
			// Draw the bird
			bird.render(window);
			
			// Move the obstacles to produce illusion of motion
			for(Obstacle &obstacle : obstacles)
			{
				obstacle.move(1.0f);
				obstacle.render(window);
			}
			
			// Reset the background positions if first background goes off-screen completely
			if(background.getPosition().x + background.getLocalBounds().width <= 0)
			{
				background.setPosition(sf::Vector2f(0.0f, 0.0f));
				background2.setPosition(sf::Vector2f(window.getSize().x, 0.0f));
			}
			// Scroll the backgrounds
			else
			{
				background.move(-1.0f, 0.0f);
				background2.move(-1.0f, 0.0f);
			}
			
			// Display the score
			window.draw(scoreText);
	
			// Render the display items onto the screen
			window.display();
		}
	}

	return 0;
}
