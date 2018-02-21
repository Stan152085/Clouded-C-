// Clouded.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "SFML/Main.hpp"
#include "SFML/System.hpp"
#include "SFML/Window.hpp"

int main()
{
	sf::Window window;
	sf::VideoMode mode(700, 500);

	window.create(mode, "Clouded");
	// game initialization

	while (window.isOpen())
	{
		// put main gameloop here
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::EventType::Closed)
			{
				window.close();
				break;
			}
		}
	}
    return 0;
}

