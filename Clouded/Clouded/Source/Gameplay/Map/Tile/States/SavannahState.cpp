#include "stdafx.h"
#include "SavannahState.h"
#include "GrassState.h"
#include "DesertState.h"

#include "Graphics/renderer.h"
float SavannahState::up_threshold_;
float SavannahState::down_threshold_;


SavannahState::SavannahState(float wetness)
  :
  ITileState(wetness, "../Assets/Samples/Hexagon/hexagon.glb" )
{
}

ITileState* SavannahState::Update( HexagonGrid* grid, HexagonTile* tile )
{
  if (wetness_ >= up_threshold_)
  {
    return new GrassState(wetness_);
  }
  else if (wetness_ < down_threshold_)
  {
    return new DesertState(wetness_);
  }
  return nullptr;
}

void SavannahState::Draw( D3D11Renderer& gfx, const Vec2 & position )
{
   Mat44 world = glm::translate( Vec3( position.x, 0, position.y ) );
   gfx.AddToDrawQueue( model_, world );
}

float SavannahState::CalculateWaterTransition(ITileState * other)
{
  return  (other->wetness() - wetness_) * 0.5f;
}
