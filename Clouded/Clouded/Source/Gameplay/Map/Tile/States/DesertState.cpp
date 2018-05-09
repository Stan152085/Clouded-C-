#include "stdafx.h"
#include "DesertState.h"
#include "SavannahState.h"

float DesertState::up_threshold_;


DesertState::DesertState(float wetness)
  :
  ITileState(wetness)
{
}

ITileState* DesertState::Update( HexagonGrid* grid, HexagonTile* tile )
{
  if (wetness_ >= up_threshold_)
  {
    return new SavannahState(wetness_);
  }
  return nullptr;
}

float DesertState::CalculateWaterTransition(ITileState* other)
{
  return  (other->wetness() - wetness_) * 0.5f;
}
