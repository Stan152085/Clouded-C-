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
   Mat44 world = glm::translate( Vec3( position.x, 0, position.y ) );
   gfx.AddToDrawQueue( model_, world );
}

float MountainState::CalculateWaterTransition( ITileState * other )
{
  return 0.0f;
}
