#include "stdafx.h"
#include "DesertState.h"
#include "SavannahState.h"

#include "Resources/AssetManager.h"

#include "Graphics/renderer.h"

float DesertState::up_threshold_;


DesertState::DesertState(float wetness)
  :
  ITileState(wetness, "../Assets/Samples/Hexagon/hexagon.glb" )
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

void DesertState::Draw( D3D11Renderer& gfx, const Vec2& position )
{
   gfx.AddToDrawQueue( model_ );
}

float DesertState::CalculateWaterTransition(ITileState* other)
{
  return  (other->wetness() - wetness_) * 0.5f;
}
