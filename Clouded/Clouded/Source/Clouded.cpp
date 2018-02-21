// Clouded.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "SFML/Main.hpp"
#include "SFML/System.hpp"
#include "SFML/Window.hpp"
#include "Graphics/renderer.h"

#include "Resources/GLTFLoader.h"

int main()
{
  math::Vec2u resolution(700, 500);
	sf::Window window;
	sf::VideoMode mode(resolution.x, resolution.y);
  
	sf::WindowHandle handle = window.getSystemHandle();
	
  D3D11Renderer renderer;
  renderer.Intialize(handle, resolution);
	// window.han
	window.create(mode, "Clouded");
	// game initialization

  resources::GLTFLoader loader;
  loader.Run();

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

