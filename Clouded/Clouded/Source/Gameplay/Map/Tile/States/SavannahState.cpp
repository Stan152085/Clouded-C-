#include "stdafx.h"
#include "SavannahState.h"
#include "GrassState.h"
#include "DesertState.h"

float SavannahState::up_threshold_;
float SavannahState::down_threshold_;


SavannahState::SavannahState(float wetness)
  :
  ITileState(wetness)
{
}

ITileState* SavannahState::Update()
{
  if (wetness_ >= up_threshold_)
  {
    return new GrassState(wetness_);
  }
  else if (wetness_ < down_threshold_)
  {
    return new DesertState(wetness_);
  }
  return nullptr;
}

float SavannahState::CalculateWaterTransition(ITileState * other)
{
  return  (other->wetness() - wetness_) * 0.5f;
}
