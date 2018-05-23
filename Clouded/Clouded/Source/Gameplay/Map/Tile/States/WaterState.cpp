#include "stdafx.h"
#include "WaterState.h"
#include "SwampState.h"
#include "Resources/AssetManager.h"

float WaterState::down_threshold_;

WaterState::WaterState(float wetness)
  :
  ITileState(wetness, "../Assets/Samples/Hexagon/hexagon.glb" )
{
}

ITileState* WaterState::Update( HexagonGrid* grid, HexagonTile* tile )
{
  if (wetness_ < down_threshold_)
  {
    return new SwampState(wetness_);
  }
  return nullptr;
}

void WaterState::Draw( const Vec2 & position )
{
}

float WaterState::CalculateWaterTransition(ITileState* other)
{
  return  (other->wetness() - wetness_) * 0.5f;
}
