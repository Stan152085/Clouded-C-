#include "stdafx.h"
#include "MountainState.h"

MountainState::MountainState()
:
  ITileState(0.f, "../Assets/Samples/Hexagon/hexagon.glb" )
{
}

ITileState* MountainState::Update( HexagonGrid* grid, HexagonTile* tile )
{
  return nullptr;
}

void MountainState::Draw( const Vec2 & position )
{
}

float MountainState::CalculateWaterTransition( ITileState * other )
{
  return 0.0f;
}
