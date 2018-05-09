#include "stdafx.h"
#include "GrassState.h"
#include "SwampState.h"
#include "SavannahState.h"

#include "Gameplay/Map/Tile/Objects/Tree.h"
#include "Gameplay/Map/Tile/HexagonTile.h"

float GrassState::up_threshold_;
float GrassState::down_threshold_;

GrassState::GrassState(float wetness)
  :
  ITileState(wetness)
{
}

ITileState* GrassState::Update( HexagonGrid* grid, HexagonTile* tile )
{
  if (wetness_ >= up_threshold_)
  {
    return new SwampState(wetness_);
  }
  else if (wetness_ < down_threshold_)
  {
    return new SavannahState(wetness_);
  }
  if ( tile->objects.size() == 0 )
  {
    tile->objects.push_back(new Tree());
  }
  return nullptr;
}

float GrassState::CalculateWaterTransition(ITileState* other)
{
  return (other->wetness() - wetness_) * 0.5f;
}
