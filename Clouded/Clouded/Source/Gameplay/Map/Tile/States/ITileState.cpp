#include "stdafx.h"
#include "ITileState.h"
#include "DesertState.h"
#include "SavannahState.h"
#include "GrassState.h"
#include "SwampState.h"
#include "WaterState.h"
#include "Resources/AssetManager.h"

#include <string>

resources::AssetManager* ITileState::asset_manager_;

ITileState::ITileState(float wetness, const char* model)
  :
  wetness_(wetness),
  model_( asset_manager_->GetModel( std::string( model),std::string()))
{
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
   asset_manager_ = &asset_manager;
}

float ITileState::wetness()
{
  return wetness_;
}

void ITileState::set_wetness(float wetness)
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
