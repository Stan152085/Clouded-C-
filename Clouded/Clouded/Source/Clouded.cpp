// Clouded.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "SFML/Main.hpp"
#include "SFML/System.hpp"
#include "SFML/Window.hpp"
#include "Graphics/renderer.h"
#include "Input/Input.h"
#include "Core/Transform.h"
#include "Resources/GLTFLoader.h"
#include "Core/Camera.h"
#include "Math/Bounds.h"
#include "Gameplay/Map/HexagonGrid.h"

int main()
{
  Vec2u resolution(700, 500);
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
  Camera cam((float)resolution.x, (float)resolution.y, 60.0f);
  renderer.SetCamera(&cam);
  GridBounds bounds = GridBounds(5,10);
  HexagonGrid grid = HexagonGrid(bounds, 01.0f);
  resources::Run();

	while (window.isOpen())
	{
		// put main gameloop here
		sf::Event event;
    input.Poll();
    if (input.IsButtonReleased(Input::kLeftHand, vr::EVRButtonId::k_EButton_SteamVR_Touchpad))
    {
      window.close();
    }
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::EventType::Closed)
			{
				window.close();
				break;
			}
      else if (event.type == sf::Event::EventType::KeyPressed)
      {
        switch (event.key.code)
        {
          case sf::Keyboard::Up:
          {
            cam.Move(Vec3(0,-1,0), 0.1f);
            break;
          }
          case sf::Keyboard::Left:
          {
            cam.Move(Vec3(1, 0, 0), 0.1f);
            break;
          }
          case sf::Keyboard::Right:
          {
            cam.Move(Vec3(-1, 0, 0), 0.1f);
            break;
          }
          case sf::Keyboard::Down:
          {
            cam.Move(Vec3(0, 1, 0), 0.1f);
            break;
          }
        default:
          break;
        }
        renderer.SetClearColor(0, 0, 0, 0);
      }

		}
    grid.DebugDraw(renderer);
    renderer.Draw();
	}
    return 0;
}

