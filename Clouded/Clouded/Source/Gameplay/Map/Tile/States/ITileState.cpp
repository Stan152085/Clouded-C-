#include "stdafx.h"
#include "ITileState.h"
#include "DesertState.h"
#include "SavannahState.h"
#include "GrassState.h"
#include "SwampState.h"
#include "WaterState.h"

ITileState::ITileState(float wetness)
  :
  wetness_(wetness)
{
}

float ITileState::wetness()
{
  return wetness_;
}

void ITileState::set_wetness(float wetness)
{
  wetness_ = wetness;
}

ITileState::State ITileState::Type()
{
  assert( wetness_ >= 0 && wetness_ <= 1.0f );
  if ( wetness_ < DesertState::up_threshold_ )
  {
    return Desert;
  }
  else if ( wetness_ < SavannahState::up_threshold_ )
  {
    return Savannah;
  }
  else if ( wetness_ < GrassState::up_threshold_ )
  {
    return Grass;
  }
  else if ( wetness_ < SwampState::up_threshold_ )
  {
    return Swamp;
  }
  // Water does not have anything above it
  else //if (wetness_ >= WaterState::up_threshold_)
  {
    return Water;
  }
}
