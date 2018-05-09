#include "stdafx.h"
#include "SwampState.h"
#include "WaterState.h"
#include "GrassState.h"

float SwampState::up_threshold_;
float SwampState::down_threshold_;

SwampState::SwampState(float wetness)
  :
  ITileState(wetness)
{
}

ITileState* SwampState::Update( HexagonGrid* grid, HexagonTile* tile )
{
  if (wetness_ >= up_threshold_)
  {
    return new WaterState(wetness_);
  }
  else if (wetness_ < down_threshold_)
  {
    return new GrassState(wetness_);
  }
  return nullptr;
}

float SwampState::CalculateWaterTransition(ITileState * other)
{
  return  (other->wetness() - wetness_) * 0.5f;
}
