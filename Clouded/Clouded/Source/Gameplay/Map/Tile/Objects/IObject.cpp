#include "stdafx.h"
#include "IObject.h"

#include "Building.h"
#include "City.h"
#include "Forest.h"
#include "Tree.h"
#include "Fire.h"

#include "Resources/AssetManager.h"

resources::AssetManager* IObject::asset_manager_;

void IObject::Initialize( resources::AssetManager& asset_manager )
{
   asset_manager_ = &asset_manager;
}

IObject* IObject::Create( Type type )
{
  switch ( type )
  {
  case IObject::kBuilding:
    return new Building("");
  case IObject::kCity:
    return new City("");
  case IObject::kFire:
    return new Fire("");
  case IObject::kForest:
    return new Forest("");
  case IObject::kTree:
    return new Tree("../Assets/Samples/Cube/2cube.glb");
  default:
    return nullptr;
  }
}

IObject::IObject( const char * model )
{
  model_ = asset_manager_->GetModel( std::string(model), std::string( "" ) );
}
