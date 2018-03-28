#pragma once
#include "ITileState.h"
class DesertState : public ITileState
{
public:
  DesertState(float wetness);
  ITileState* Update() override;
  float CalculateWaterTransition(ITileState* other) override;

  static float up_threshold_;
};