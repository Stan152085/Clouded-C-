#include "stdafx.h"
#include "MountainState.h"

MountainState::MountainState()
:
  ITileState(0.f)
{
}

ITileState* MountainState::Update( HexagonGrid* grid, HexagonTile* tile )
{
  return nullptr;
}

float MountainState::CalculateWaterTransition( ITileState * other )
{
  return 0.0f;
}
