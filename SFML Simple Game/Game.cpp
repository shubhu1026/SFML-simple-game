#include "Game.h"

//Private Functions
void Game::initVariables()
{
	this->window = nullptr;
	this->endGame = false;
	this->health = 20;
	this->points = 0;
	this->enemySpawnTimerMax = 20.0f;
	this->enemySpawnTimer = this->enemySpawnTimerMax;
	this->maxEnemies = 10;
	this->mouseHeld = false;
}

void Game::initWindow()
{
	this->videoMode.height = 600;
	this->videoMode.width = 800;

	this->window = new sf::RenderWindow(this->videoMode, "Game 1", sf::Style::Titlebar | sf::Style::Close);
	
	this->window->setFramerateLimit(60);
}


void Game::initFonts()
{
	if (!this->font.loadFromFile("Fonts/BrunoAceSC-Regular.ttf"))
	{
		std::cout << "ERROR: GAME::INITFONTS:: Failed to load font!" << "\n";
	}
}

void Game::initText()
{
	this->uiText.setFont(this->font);
	this->uiText.setCharacterSize(12);
	this->uiText.setFillColor(sf::Color::White);
	this->uiText.setString("NONE");
}

void Game::initEnemies()
{
	this->enemy.setPosition(10.0f, 10.0f);
	this->enemy.setSize(sf::Vector2f(100.0f, 100.0f));
	this->enemy.setFillColor(sf::Color::Red);
}

//Constructor and Destructor
Game::Game()
{
	this->initVariables();
	this->initWindow();
	this->initFonts();
	this->initText();
	this->initEnemies();
}

Game::~Game()
{
	delete this->window;
}

//Accessors
const bool Game::running() const
{
	return this->window->isOpen();
}

const bool Game::getEndGame() const
{
	return this->endGame;
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

	//Randomize enemy type
	int type = rand() % 5;

	switch (type)
	{
	case 0:
		this->enemy.setSize(sf::Vector2f(20.0f, 20.0f));
		this->enemy.setFillColor(sf::Color::Red);
		break;
	case 1:
		this->enemy.setSize(sf::Vector2f(40.0f, 40.0f));
		this->enemy.setFillColor(sf::Color::Magenta);
		break;
	case 2:
		this->enemy.setSize(sf::Vector2f(60.0f, 60.0f));
		this->enemy.setFillColor(sf::Color::Blue);
		break;
	case 3:
		this->enemy.setSize(sf::Vector2f(80.0f, 80.0f));
		this->enemy.setFillColor(sf::Color::Cyan);
		break;
	case 4:
		this->enemy.setSize(sf::Vector2f(100.0f, 100.0f));
		this->enemy.setFillColor(sf::Color::Green);
		break;
	default:
		this->enemy.setSize(sf::Vector2f(100.0f, 100.0f));
		this->enemy.setFillColor(sf::Color::Yellow);
		break;
	}

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

void Game::updateText()
{
	std::stringstream ss;
	ss << "Score: " << this->points <<"\n"
		<<"Health: "<< this->health <<"\n";
	this->uiText.setString(ss.str());
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

		//Check and remove the enemy if it moves out of screen
		if (this->enemies[i].getPosition().y > this->window->getSize().y)
		{
			this->enemies.erase(this->enemies.begin() + i);
			this->health -= 1;
			std::cout << "Health: " << this->health << std::endl;
		}
	}

	//check and remove if player clicks on enemy
	if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
	{
		if (!this->mouseHeld)
		{
			this->mouseHeld = true;
			bool deleted = false;
			for (size_t i = 0; i < this->enemies.size() && !deleted; i++)
			{
				if (this->enemies[i].getGlobalBounds().contains(this->mousePosView))
				{
					//Gain points
					if(this->enemies[i].getFillColor() == sf::Color::Red)
						this->points += 10.0f;
					else if (this->enemies[i].getFillColor() == sf::Color::Magenta)
						this->points += 8.0f;
					else if (this->enemies[i].getFillColor() == sf::Color::Blue)
						this->points += 6.0f;
					else if (this->enemies[i].getFillColor() == sf::Color::Cyan)
						this->points += 4.0f;
					else if (this->enemies[i].getFillColor() == sf::Color::Green)
						this->points += 2.0f;

					std::cout << "Points: " << this->points<<std::endl;

					//delete the enemy
					deleted = true;
					this->enemies.erase(this->enemies.begin() + i);
				}
			}
		}
		else
		{
			this->mouseHeld = false;
		}
	}
}

void Game::update()
{
	this->pollEvents();

	if (!this->endGame)
	{
		this->updateMousePositions();
		this->updateEnemies();
		this->updateText();
	}

	//end game condition
	if (this->health <= 0)
	{
		this->endGame = true;
	}
}

void Game::renderText(sf::RenderTarget& target)
{
	target.draw(this->uiText);
}

void Game::renderEnemies(sf::RenderTarget& target)
{
	for (auto& e : this->enemies)
	{
		target.draw(e);
	}
}

void Game::render()
{
	/*
		Renders game objects
	*/

	this->window->clear();

	//draw game objects
	this->renderEnemies(*this->window);
	this->renderText(*this->window);

	this->window->display();
}
