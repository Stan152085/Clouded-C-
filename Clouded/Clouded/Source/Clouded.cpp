// Clouded.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "SFML/Main.hpp"
#include "SFML/System.hpp"
#include "SFML/Window.hpp"
#include "Input/Input.h"

int main()
{
	sf::Window window;
	sf::VideoMode mode(700, 500);
  vr::HmdError test;
  vr::EVRApplicationType type = vr::EVRApplicationType::VRApplication_Scene;
  vr::IVRSystem* vr_system = vr::VR_Init( &test, type );
  Input input( vr_system );
	window.create(mode, "Clouded");
	// game initialization

	while (window.isOpen())
	{
		// put main gameloop here
		sf::Event event;
    input.Poll();
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

