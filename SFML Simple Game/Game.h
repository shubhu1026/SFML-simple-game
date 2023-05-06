#pragma once

#include<iostream>
#include<vector>
#include<ctime>

#include<SFML/Graphics.hpp>
#include<SFML/System.hpp>
#include<SFML/Window.hpp>
#include<SFML/Audio.hpp>
#include<SFML/Network.hpp>

class Game
{
private:
	//Variables
	sf::RenderWindow* window;
	sf::VideoMode videoMode;
	sf::Event ev;

	//Mouse Positions
	sf::Vector2i mousePosWindow;
	sf::Vector2f mousePosView;

	//Game Logic
	int points;
	float enemySpawnTimer;
	float enemySpawnTimerMax;
	int maxEnemies;

	//Game Objects
	std::vector<sf::RectangleShape> enemies;
	sf::RectangleShape enemy;

	//private functions
	void initVariables();
	void initWindow();
	void initEnemies();

public:
	//Constructors and Destructors
	Game();
	~Game();

	//Accessors
	const bool running() const;

	//Functions
	void spawnEnemy();

	void pollEvents();
	void updateMousePositions();

	void updateEnemies();
	void update();

	void renderEnemies();
	void render();
};
