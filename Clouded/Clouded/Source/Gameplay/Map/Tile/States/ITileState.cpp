#include "stdafx.h"
#include "ITileState.h"
#include "DesertState.h"
#include "SavannahState.h"
#include "GrassState.h"
#include "SwampState.h"
#include "WaterState.h"
#include "Resources/AssetManager.h"
#include "Graphics/Renderer.h"
#include "Gameplay/Map/Tile/Objects/IObject.h"


resources::AssetManager* ITileState::asset_manager_;

ITileState::ITileState( float wetness, const char* model )
  :
  wetness_( wetness ),
  model_( asset_manager_->GetModel( std::string( model ), std::string() ) )
{
}

void ITileState::Draw( D3D11Renderer& gfx, const Vec2 & position )
{
  Mat44 world = glm::transpose( glm::translate( Vec3( position.x, position.y, 0 ) ) );
  gfx.AddToDrawQueue( model_, world );
}

ITileState* ITileState::Create( float wetness )
{
  assert( wetness >= 0 && wetness <= 1.0f );
  if ( wetness < DesertState::up_threshold_ )
  {
    return new DesertState( wetness );
  }
  else if ( wetness < SavannahState::up_threshold_ )
  {
    return new SavannahState( wetness );
  }
  else if ( wetness < GrassState::up_threshold_ )
  {
    return new GrassState( wetness );
  }
  else if ( wetness < SwampState::up_threshold_ )
  {
    return new SwampState( wetness );
  }
  // Water does not have anything above it
  else //if (wetness >= WaterState::up_threshold_)
  {
    return new WaterState( wetness );
  }
}

void ITileState::Initialize( resources::AssetManager& asset_manager )
{
  IObject::Initialize( asset_manager );
  asset_manager_ = &asset_manager;
  DesertState::up_threshold_ = 0.2f;
  SavannahState::up_threshold_ = 0.4f;
  GrassState::up_threshold_ = 0.6f;
  SwampState::up_threshold_ = 0.8f;

  SavannahState::down_threshold_ = DesertState::up_threshold_;
  GrassState::down_threshold_ = SavannahState::up_threshold_;
  SwampState::down_threshold_ = GrassState::up_threshold_;
  WaterState::down_threshold_ = SwampState::up_threshold_;
}

float ITileState::wetness()
{
  return wetness_;
}

void ITileState::set_wetness( float wetness )
{
  wetness_ = wetness;
}

ITileState::State ITileState::Type()
{
  assert( wetness_ >= 0 && wetness_ <= 1.0f );
  if ( wetness_ < DesertState::up_threshold_ )
  {
    return Desert;
  }
  else if ( wetness_ < SavannahState::up_threshold_ )
  {
    return Savannah;
  }
  else if ( wetness_ < GrassState::up_threshold_ )
  {
    return Grass;
  }
  else if ( wetness_ < SwampState::up_threshold_ )
  {
    return Swamp;
  }
  // Water does not have anything above it
  else //if (wetness_ >= WaterState::up_threshold_)
  {
    return Water;
  }
}
