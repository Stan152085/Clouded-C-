#pragma once
#include "ITileState.h"
class MountainState : public ITileState
{
public:
  MountainState( );
  ITileState* Update( HexagonGrid* grid, HexagonTile* tile ) override;
  void Draw( const Vec2& position ) override;
  float CalculateWaterTransition( ITileState* other ) override;
};