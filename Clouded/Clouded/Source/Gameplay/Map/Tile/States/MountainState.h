#pragma once
#include "ITileState.h"
class MountainState : public ITileState
{
public:
  MountainState( );
  ITileState* Update() override;
  float CalculateWaterTransition( ITileState* other ) override;
};