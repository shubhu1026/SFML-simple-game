#include<iostream>

#include<SFML/Graphics.hpp>
#include<SFML/System.hpp>
#include<SFML/Window.hpp>
#include<SFML/Audio.hpp>
#include<SFML/Network.hpp>

int main()
{
	sf::RenderWindow window(sf::VideoMode(640, 480), "Simple SFML Game", sf::Style::Titlebar | sf::Style::Close);
	sf::Event e;

	//Game Loop
	while (window.isOpen())
	{
		while (window.pollEvent(e))
		{
			switch (e.type)
			{
			case sf::Event::Closed:
				window.close();
				break;
			case sf::Event::KeyPressed:
				if (e.key.code == sf::Keyboard::Escape)
				{
					window.close();
				}
				break;
			}
		}
	}

	//Update


	//Render
	window.clear(sf::Color::Blue); //clears old frame

	//draw game

	window.display(); //draw current frame

	return 0;
}