#pragma once
#include "ITileState.h"
class DesertState : public ITileState
{
public:
  DesertState(float wetness);
  ITileState* Update(HexagonGrid* grid, HexagonTile* tile ) override;
  void Draw( const Vec2& position ) override;
  float CalculateWaterTransition(ITileState* other) override;

  static float up_threshold_;
};