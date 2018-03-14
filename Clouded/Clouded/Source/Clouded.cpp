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

  resources::GLTFLoader loader;
  loader.Run();

  Transform trans;
  Mat44 test0 = trans.GetInversedMatrix();
  trans.Translate(Vec3(10, 0, 0));
  test0 = trans.GetMatrix();
  test0 = trans.GetInversedMatrix();
  test0 *= trans.GetMatrix();
  trans.SetRotation(glm::normalize(Quat(0.5f,0.7f,0.7f,0.1f)));
  trans.SetScale(5.0f, 2.0f, 3.0f);
  test0 = trans.GetMatrix();
  test0 = trans.GetInversedMatrix();
  test0 *= trans.GetMatrix();

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
        renderer.SetClearColor(0, 0, 0, 0);
      }

		}
    renderer.Draw();
	}
    return 0;
}

