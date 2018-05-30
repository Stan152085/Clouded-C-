// Clouded.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "SFML/Main.hpp"
#include "SFML/System.hpp"
#include "SFML/Window.hpp"
#include "Graphics/renderer.h"
#include "Input/Input.h"
#include "Core/Transform.h"
#include "Resources/AssetManager.h"
#include "Core/Camera.h"
#include "Math/Bounds.h"
#include "Gameplay/Map/HexagonGrid.h"
#include "Graphics\DebugRenderer.h"
#include "Core/Window.h"

int main()
{
   vr::EVRInitError init_error;
  vr::EVRApplicationType type = vr::EVRApplicationType::VRApplication_Scene;
  vr::IVRSystem* vr_system = vr::VR_Init(&init_error, type);
  Input input(vr_system);
  Vec2u resolution(1280, 720);
  Window window;
  window.Create();



	// sf::Window window;
	// sf::VideoMode mode(resolution.x, resolution.y);
  // 
  // 
	// window.create(mode, "Clouded");
	// sf::WindowHandle handle = window.getSystemHandle();
	// game initialization
  D3D11Renderer renderer;
  renderer.Intialize((HWND)window.window_handle(), resolution, vr_system);
  renderer.SetRenderState(RenderModes::kSolid);

  DebugRenderer dbg_renderer;
  dbg_renderer.set_renderer(&renderer);

  std::string err;
  std::string file = "../Assets/Samples/Hexagon/hexagon.glb";
  resources::AssetManager asset_manager( renderer );
  auto model = asset_manager.GetModel( file, err );

  Camera cam((float)resolution.x, (float)resolution.y, 60.0f);
  cam.transform.SetPosition(Vec3(0, 0, 5));
  cam.transform.SetRotationDegrees(Vec3(0,180,0));
  renderer.SetCamera(&cam);
  GridBounds bounds = GridBounds(5,5);
  HexagonGrid grid = HexagonGrid( bounds, 0.5f, asset_manager );
  resources::Run(renderer);


	while (window.is_open())
	{
    renderer.Clear();
		// put main gameloop here
		// sf::Event event;
    window.ProcessMessages(input);
    input.Poll();
    //if (input.IsButtonReleased(Input::kLeftHand, vr::EVRButtonId::k_EButton_SteamVR_Touchpad))
    //{
    //  window.Close();
    //}
    if(input.IsKeyPressed(Input::UP))
    {
      cam.transform.Translate( cam.transform.Forward()* 0.01f );
    }
    if ( input.IsKeyPressed( Input::DOWN ) )
    {
      cam.transform.Translate( cam.transform.Forward()* -0.01f );
    }
    if ( input.IsKeyPressed( Input::LEFT ) )
    {
      cam.transform.Translate( cam.transform.Right()* -0.01f );
    }
    if ( input.IsKeyPressed( Input::RIGHT ) )
    {
      cam.transform.Translate( cam.transform.Right()* 0.01f );
    }
    if ( input.IsKeyPressed( Input::A ) )
    {
      cam.transform.Rotate( glm::rotate( -0.001f , Vec3( 0, 1.f, 0 ) ));
    }
    if ( input.IsKeyPressed( Input::D) )
    {
      cam.transform.Rotate( glm::rotate( 0.001f, Vec3( 0, 1.f, 0 ) ) );
    }
    if ( input.IsKeyPressed( Input::W ) )
    {
      cam.transform.Rotate( glm::rotate( 0.001f, Vec3( 1.f, 0, 0 ) ) );
    }
    if ( input.IsKeyPressed( Input::S ) )
    {
      cam.transform.Rotate( glm::rotate( -0.001f, Vec3( 1.f, 0, 0 ) ) );

    }
    renderer.SetClearColor(0, 0, 0, 0);
    grid.Update();
    grid.WetnessUpdate();
    grid.Draw( renderer );
    grid.DebugDraw( renderer );
    // grid.DebugDraw(renderer);
    renderer.Present(&input);
	}
    return 0;
}

