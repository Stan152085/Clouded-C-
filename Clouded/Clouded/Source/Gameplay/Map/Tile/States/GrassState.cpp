#include "stdafx.h"
#include "GrassState.h"
#include "SwampState.h"
#include "SavannahState.h"

float GrassState::up_threshold_;
float GrassState::down_threshold_;

GrassState::GrassState(float wetness)
  :
  ITileState(wetness)
{
}

ITileState* GrassState::Update( HexagonGrid* grid )
{
  if (wetness_ >= up_threshold_)
  {
    return new SwampState(wetness_);
  }
  else if (wetness_ < down_threshold_)
  {
    return new SavannahState(wetness_);
  }
  
  return nullptr;
}

float GrassState::CalculateWaterTransition(ITileState* other)
{
  return (other->wetness() - wetness_) * 0.5f;
}
