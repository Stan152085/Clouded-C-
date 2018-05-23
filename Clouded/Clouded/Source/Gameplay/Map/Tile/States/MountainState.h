#pragma once
#include "ITileState.h"
class MountainState : public ITileState
{
public:
  MountainState( );
  ITileState* Update( HexagonGrid* grid, HexagonTile* tile ) override;
  void Draw( D3D11Renderer& gfx, const Vec2& position ) override;
  float CalculateWaterTransition( ITileState* other ) override;
};