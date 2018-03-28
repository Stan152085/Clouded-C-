#include "stdafx.h"
#include "StateConstructor.h"
#include "DesertState.h"
#include "SavannahState.h"
#include "GrassState.h"
#include "SwampState.h"
#include "WaterState.h"

void StateConstructor::Initialize()
{
  DesertState::up_threshold_ = 0.2f;
  SavannahState::up_threshold_ = 0.4f;
  GrassState::up_threshold_ = 0.6f;
  SwampState::up_threshold_ = 0.8f;

  SavannahState::down_threshold_ = DesertState::up_threshold_;
  GrassState::down_threshold_ = SavannahState::up_threshold_;
  SwampState::down_threshold_ = GrassState::up_threshold_;
  WaterState::down_threshold_ = SwampState::up_threshold_;
}

ITileState* StateConstructor::Create(float wetness)
{
  assert(wetness >= 0 && wetness <= 1.0f);
  if (wetness < DesertState::up_threshold_)
  {
    return new DesertState(wetness);
  }
  else if (wetness < SavannahState::up_threshold_)
  {
    return new SavannahState(wetness);
  }
  else if (wetness < GrassState::up_threshold_)
  {
    return new GrassState(wetness);
  }
  else if (wetness < SwampState::up_threshold_)
  {
    return new SwampState(wetness);
  }
  // Water does not have anything above it
  else //if (wetness >= WaterState::up_threshold_)
  {
    return new WaterState(wetness);
  }
}
