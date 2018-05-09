#include "stdafx.h"
#include "MountainState.h"

MountainState::MountainState()
:
  ITileState(0.f)
{
}

ITileState* MountainState::Update()
{
  return this;
}

float MountainState::CalculateWaterTransition( ITileState * other )
{
  return 0.0f;
}
