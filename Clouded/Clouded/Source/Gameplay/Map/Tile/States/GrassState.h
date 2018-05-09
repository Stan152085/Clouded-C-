#pragma once
#include "ITileState.h"
class GrassState : public ITileState
{
public:
  GrassState(float wetness);
  ITileState* Update( HexagonGrid* grid ) override;
  virtual float CalculateWaterTransition(ITileState* other) override;

  static float up_threshold_;
  static float down_threshold_;
};