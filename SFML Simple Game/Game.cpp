#include "Game.h"

//Private Functions
void Game::initVariables()
{
	this->window = nullptr;
	this->points = 0;
	this->enemySpawnTimerMax = 10.0f;
	this->enemySpawnTimer = this->enemySpawnTimerMax;
	this->maxEnemies = 10;
}

void Game::initWindow()
{
	this->videoMode.height = 600;
	this->videoMode.width = 800;

	this->window = new sf::RenderWindow(this->videoMode, "Game 1", sf::Style::Titlebar | sf::Style::Close);
	
	this->window->setFramerateLimit(60);
}

//Constructor and Destructor
Game::Game()
{
	this->initVariables();
	this->initWindow();
	this->initEnemies();
}

Game::~Game()
{
	delete this->window;
}

void Game::initEnemies()
{
	this->enemy.setPosition(10.0f, 10.0f);
	this->enemy.setSize(sf::Vector2f(50.0f, 50.0f));
	this->enemy.setFillColor(sf::Color::Red);
}

//Accessors
const bool Game::running() const
{
	return this->window->isOpen();
}

//Functions
void Game::spawnEnemy()
{
	/*
		Spawn enemies and set their properties
	*/
	
	this->enemy.setPosition(
		static_cast<float>(rand() % static_cast<int>(this->window->getSize().x - this->enemy.getSize().x)),
		0.0f
	);

	this->enemy.setFillColor(sf::Color::Red);

	//spawn enemy
	this->enemies.push_back(this->enemy);

	//remove enemies at the end of screen
}

void Game::pollEvents()
{
	while (this->window->pollEvent(this->ev))
	{
		switch (this->ev.type)
		{
		case sf::Event::Closed:
			this->window->close();
			break;
		case sf::Event::KeyPressed:
			if (this->ev.key.code == sf::Keyboard::Escape)
				this->window->close();
			break;
		}
	}
}

void Game::updateMousePositions()
{
	/*
		@return void

		Updates the mouse positions relative to the window
	*/

	this->mousePosWindow = sf::Mouse::getPosition(*this->window);
	this->mousePosView = this->window->mapPixelToCoords(this->mousePosWindow);
}

void Game::updateEnemies()
{
	/*
		Handles enemy spawn timer and enemy movement
	*/

	//Updating enemy spawn timer
	if (this->enemies.size() < this->maxEnemies)
	{
		if (this->enemySpawnTimer >= this->enemySpawnTimerMax)
		{
			//spawn enemy and reset timer
			this->spawnEnemy();
			this->enemySpawnTimer = 0.0f;
		}
		else
		{
			this->enemySpawnTimer += 1.0f;
		}
	}

	//Move the enemies
	for (int i = 0; i < this->enemies.size(); ++i)
	{
		bool deleted = false;

		this->enemies[i].move(0.0f, 5.0f);

		//check and remove if player clicks on enemy
		if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
		{
			if (this->enemies[i].getGlobalBounds().contains(this->mousePosView))
			{
				deleted = true;

				//Gain points
				this->points += 10.0f;
			}
		}

		//Check and remove the enemy if it moves out of screen
		if (this->enemies[i].getPosition().y > this->window->getSize().y)
		{
			deleted = true;
		}

		//final delete
		if(deleted)
		{
			this->enemies.erase(this->enemies.begin() + i);
		}
	}
}

void Game::update()
{
	this->pollEvents();
	this->updateMousePositions();
	this->updateEnemies();
}

void Game::renderEnemies()
{
	for (auto& e : this->enemies)
	{
		this->window->draw(e);
	}
}

void Game::render()
{
	/*
		Renders game objects
	*/

	this->window->clear();

	//draw game objects
	this->renderEnemies();

	this->window->display();
}
