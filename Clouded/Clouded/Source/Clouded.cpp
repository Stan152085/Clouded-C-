// Clouded.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "SFML/Main.hpp"
#include "SFML/System.hpp"
#include "SFML/Window.hpp"
#include "Graphics/renderer.h"
#include "Input/Input.h"

#include "Resources/GLTFLoader.h"

int main()
{
  math::Vec2u resolution(700, 500);
	sf::Window window;
	sf::VideoMode mode(resolution.x, resolution.y);
  
  vr::EVRInitError init_error;
  vr::EVRApplicationType type = vr::EVRApplicationType::VRApplication_Scene;
  vr::IVRSystem* vr_system = vr::VR_Init(&init_error, type);
  Input input(vr_system);
	// window.han
	window.create(mode, "Clouded");
	sf::WindowHandle handle = window.getSystemHandle();
	// game initialization
  D3D11Renderer renderer;
  renderer.Intialize(handle, resolution);

  resources::GLTFLoader loader;
  loader.Run();

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
      else if (event.type == sf::Event::EventType::KeyPressed)
      {
        renderer.SetClearColor(0, 0, 0, 0);
      }

		}
    renderer.Draw();
	}
    return 0;
}

