#include "stdafx.h"
#include "ITileState.h"

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
