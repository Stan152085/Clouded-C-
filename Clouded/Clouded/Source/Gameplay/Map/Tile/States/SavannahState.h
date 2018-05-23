#pragma once
#include "ITileState.h"
class SavannahState : public ITileState
{
public:
  SavannahState(float wetness);
  ITileState* Update( HexagonGrid* grid, HexagonTile* tile ) override;
  void Draw( const Vec2& position ) override;
  virtual float CalculateWaterTransition(ITileState * other) override;
  
  static float up_threshold_;
  static float down_threshold_;
};