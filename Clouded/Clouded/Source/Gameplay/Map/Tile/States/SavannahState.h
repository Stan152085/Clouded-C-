#pragma once
#include "ITileState.h"
class SavannahState : public ITileState
{
public:
  SavannahState(float wetness);
  ITileState* Update() override;
  virtual float CalculateWaterTransition(ITileState * other) override;
  
  static float up_threshold_;
  static float down_threshold_;
};