#include "stdafx.h"
#include "MountainState.h"

#include "Graphics/renderer.h"
MountainState::MountainState()
:
  ITileState(0.f, "../Assets/Samples/Hexagon/hexagon.glb" )
{
}

ITileState* MountainState::Update( HexagonGrid* grid, HexagonTile* tile )
{
  return nullptr;
}

void MountainState::Draw( D3D11Renderer& gfx, const Vec2 & position )
{
   ITileState::Draw( gfx, position );

}

float MountainState::CalculateWaterTransition( ITileState * other )
{
  return 0.0f;
}
