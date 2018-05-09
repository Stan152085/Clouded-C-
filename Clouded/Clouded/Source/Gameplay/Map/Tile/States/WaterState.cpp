#include "stdafx.h"
#include "WaterState.h"
#include "SwampState.h"

float WaterState::down_threshold_;

WaterState::WaterState(float wetness)
  :
  ITileState(wetness)
{
}

ITileState* WaterState::Update( HexagonGrid* grid )
{
  if (wetness_ < down_threshold_)
  {
    return new SwampState(wetness_);
  }
  return nullptr;
}

float WaterState::CalculateWaterTransition(ITileState* other)
{
  return  (other->wetness() - wetness_) * 0.5f;
}
