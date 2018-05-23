#include "stdafx.h"
#include "WaterState.h"
#include "SwampState.h"
#include "Resources/AssetManager.h"

#include "Graphics/renderer.h"
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

void WaterState::Draw( D3D11Renderer& gfx, const Vec2 & position )
{
   gfx.AddToDrawQueue( model_ );
}

float WaterState::CalculateWaterTransition(ITileState* other)
{
  return  (other->wetness() - wetness_) * 0.5f;
}
