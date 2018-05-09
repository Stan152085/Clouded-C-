#pragma once
#include "ITileState.h"
class MountainState : public ITileState
{
public:
  MountainState( );
  ITileState* Update( HexagonGrid* grid ) override;
  float CalculateWaterTransition( ITileState* other ) override;
};